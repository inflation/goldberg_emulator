/* Copyright (C) 2019 Mr Goldberg
   This file is part of the Goldberg Emulator

   The Goldberg Emulator is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   The Goldberg Emulator is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the Goldberg Emulator; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "base.h"

struct Listen_Socket {
    HSteamListenSocket socket_id;

    int virtual_port;

    uint32 ip;
    uint16 port;
};


class Steam_Networking_Sockets :
public ISteamNetworkingSockets001,
public ISteamNetworkingSockets
{
    class Settings *settings;
    class Networking *network;
    class SteamCallResults *callback_results;
    class SteamCallBacks *callbacks;
    class RunEveryRunCB *run_every_runcb;

    std::vector<struct Listen_Socket> listen_sockets;
public:
static void steam_callback(void *object, Common_Message *msg)
{
    PRINT_DEBUG("steam_networkingsockets_callback\n");

    Steam_Networking_Sockets *steam_networkingsockets = (Steam_Networking_Sockets *)object;
    steam_networkingsockets->Callback(msg);
}

static void steam_run_every_runcb(void *object)
{
    PRINT_DEBUG("steam_networkingsockets_run_every_runcb\n");

    Steam_Networking_Sockets *steam_networkingsockets = (Steam_Networking_Sockets *)object;
    steam_networkingsockets->RunCallbacks();
}

Steam_Networking_Sockets(class Settings *settings, class Networking *network, class SteamCallResults *callback_results, class SteamCallBacks *callbacks, class RunEveryRunCB *run_every_runcb)
{
    this->settings = settings;
    this->network = network;
    this->run_every_runcb = run_every_runcb;
    this->network->setCallback(CALLBACK_ID_USER_STATUS, settings->get_local_steam_id(), &Steam_Networking_Sockets::steam_callback, this);
    this->network->setCallback(CALLBACK_ID_NETWORKING_SOCKETS, settings->get_local_steam_id(), &Steam_Networking_Sockets::steam_callback, this);
    this->run_every_runcb->add(&Steam_Networking_Sockets::steam_run_every_runcb, this);

    this->callback_results = callback_results;
    this->callbacks = callbacks;
}

~Steam_Networking_Sockets()
{
    //TODO rm network callbacks
    this->run_every_runcb->remove(&Steam_Networking_Sockets::steam_run_every_runcb, this);
}


HSteamListenSocket new_listen_socket(int nSteamConnectVirtualPort, uint32 nIP, uint16 nPort)
{
    static HSteamListenSocket socket_id;
    ++socket_id;
    struct Listen_Socket listen_socket;
    listen_socket.socket_id = socket_id;
    listen_socket.virtual_port = nSteamConnectVirtualPort;
    listen_socket.ip = nIP;
    listen_socket.port = nPort;
    listen_sockets.push_back(listen_socket);
    return socket_id;
}

struct Listen_Socket *get_connection_socket(HSteamListenSocket id)
{
    auto conn = std::find_if(listen_sockets.begin(), listen_sockets.end(), [&id](struct Listen_Socket const& conn) { return conn.socket_id == id;});
    if (conn == listen_sockets.end()) return NULL;
    return &(*conn);
}

/// Creates a "server" socket that listens for clients to connect to, either by calling
/// ConnectSocketBySteamID or ConnectSocketByIPv4Address.
///
/// nSteamConnectVirtualPort specifies how clients can connect to this socket using
/// ConnectBySteamID.  A negative value indicates that this functionality is
/// disabled and clients must connect by IP address.  It's very common for applications
/// to only have one listening socket; in that case, use zero.  If you need to open
/// multiple listen sockets and have clients be able to connect to one or the other, then
/// nSteamConnectVirtualPort should be a small integer constant unique to each listen socket
/// you create.
///
/// In the open-source version of this API, you must pass -1 for nSteamConnectVirtualPort
///
/// If you want clients to connect to you by your IPv4 addresses using
/// ConnectByIPv4Address, then you must set nPort to be nonzero.  Steam will
/// bind a UDP socket to the specified local port, and clients will send packets using
/// ordinary IP routing.  It's up to you to take care of NAT, protecting your server
/// from DoS, etc.  If you don't need clients to connect to you by IP, then set nPort=0.
/// Use nIP if you wish to bind to a particular local interface.  Typically you will use 0,
/// which means to listen on all interfaces, and accept the default outbound IP address.
/// If nPort is zero, then nIP must also be zero.
///
/// A SocketStatusCallback_t callback when another client attempts a connection.
HSteamListenSocket CreateListenSocket( int nSteamConnectVirtualPort, uint32 nIP, uint16 nPort )
{
    PRINT_DEBUG("Steam_Networking_Sockets::CreateListenSocket %i %u %u\n", nSteamConnectVirtualPort, nIP, nPort);
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    return new_listen_socket(nSteamConnectVirtualPort, nIP, nPort);
}

/// Creates a "server" socket that listens for clients to connect to by 
/// calling ConnectByIPAddress, over ordinary UDP (IPv4 or IPv6)
///
/// You must select a specific local port to listen on and set it
/// the port field of the local address.
///
/// Usually you wil set the IP portion of the address to zero, (SteamNetworkingIPAddr::Clear()).
/// This means that you will not bind to any particular local interface.  In addition,
/// if possible the socket will be bound in "dual stack" mode, which means that it can
/// accept both IPv4 and IPv6 clients.  If you wish to bind a particular interface, then
/// set the local address to the appropriate IPv4 or IPv6 IP.
///
/// When a client attempts to connect, a SteamNetConnectionStatusChangedCallback_t
/// will be posted.  The connection will be in the connecting state.
HSteamListenSocket CreateListenSocketIP( const SteamNetworkingIPAddr &localAddress )
{
    PRINT_DEBUG("Steam_Networking_Sockets::CreateListenSocketIP\n");
}

/// Creates a connection and begins talking to a "server" over UDP at the
/// given IPv4 or IPv6 address.  The remote host must be listening with a
/// matching call to CreateListenSocketIP on the specified port.
///
/// A SteamNetConnectionStatusChangedCallback_t callback will be triggered when we start
/// connecting, and then another one on either timeout or successful connection.
///
/// If the server does not have any identity configured, then their network address
/// will be the only identity in use.  Or, the network host may provide a platform-specific
/// identity with or without a valid certificate to authenticate that identity.  (These
/// details will be contained in the SteamNetConnectionStatusChangedCallback_t.)  It's
/// up to your application to decide whether to allow the connection.
///
/// By default, all connections will get basic encryption sufficient to prevent
/// casual eavesdropping.  But note that without certificates (or a shared secret
/// distributed through some other out-of-band mechanism), you don't have any
/// way of knowing who is actually on the other end, and thus are vulnerable to
/// man-in-the-middle attacks.
HSteamNetConnection ConnectByIPAddress( const SteamNetworkingIPAddr &address )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ConnectByIPAddress\n");
}

/// Like CreateListenSocketIP, but clients will connect using ConnectP2P
///
/// nVirtualPort specifies how clients can connect to this socket using
/// ConnectP2P.  It's very common for applications to only have one listening socket;
/// in that case, use zero.  If you need to open multiple listen sockets and have clients
/// be able to connect to one or the other, then nVirtualPort should be a small integer (<1000)
/// unique to each listen socket you create.
///
/// If you use this, you probably want to call ISteamNetworkingUtils::InitializeRelayNetworkAccess()
/// when your app initializes
HSteamListenSocket CreateListenSocketP2P( int nVirtualPort )
{
    PRINT_DEBUG("Steam_Networking_Sockets::CreateListenSocketP2P\n");
}

/// Begin connecting to a server that is identified using a platform-specific identifier.
/// This requires some sort of third party rendezvous service, and will depend on the
/// platform and what other libraries and services you are integrating with.
///
/// At the time of this writing, there is only one supported rendezvous service: Steam.
/// Set the SteamID (whether "user" or "gameserver") and Steam will determine if the
/// client is online and facilitate a relay connection.  Note that all P2P connections on
/// Steam are currently relayed.
///
/// If you use this, you probably want to call ISteamNetworkingUtils::InitializeRelayNetworkAccess()
/// when your app initializes
HSteamNetConnection ConnectP2P( const SteamNetworkingIdentity &identityRemote, int nVirtualPort )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ConnectP2P\n");
}

/// Creates a connection and begins talking to a remote destination.  The remote host
/// must be listening with a matching call to CreateListenSocket.
///
/// Use ConnectBySteamID to connect using the SteamID (client or game server) as the network address.
/// Use ConnectByIPv4Address to connect by IP address.
///
/// A SteamNetConnectionStatusChangedCallback_t callback will be triggered when we start connecting,
/// and then another one on timeout or successful connection
//#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
HSteamNetConnection ConnectBySteamID( CSteamID steamIDTarget, int nVirtualPort )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ConnectBySteamID\n");
}

//#endif
HSteamNetConnection ConnectByIPv4Address( uint32 nIP, uint16 nPort )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ConnectByIPv4Address\n");
}


/// Accept an incoming connection that has been received on a listen socket.
///
/// When a connection attempt is received (perhaps after a few basic handshake
/// packets have been exchanged to prevent trivial spoofing), a connection interface
/// object is created in the k_ESteamNetworkingConnectionState_Connecting state
/// and a SteamNetConnectionStatusChangedCallback_t is posted.  At this point, your
/// application MUST either accept or close the connection.  (It may not ignore it.)
/// Accepting the connection will transition it either into the connected state,
/// of the finding route state, depending on the connection type.
///
/// You should take action within a second or two, because accepting the connection is
/// what actually sends the reply notifying the client that they are connected.  If you
/// delay taking action, from the client's perspective it is the same as the network
/// being unresponsive, and the client may timeout the connection attempt.  In other
/// words, the client cannot distinguish between a delay caused by network problems
/// and a delay caused by the application.
///
/// This means that if your application goes for more than a few seconds without
/// processing callbacks (for example, while loading a map), then there is a chance
/// that a client may attempt to connect in that interval and fail due to timeout.
///
/// If the application does not respond to the connection attempt in a timely manner,
/// and we stop receiving communication from the client, the connection attempt will
/// be timed out locally, transitioning the connection to the
/// k_ESteamNetworkingConnectionState_ProblemDetectedLocally state.  The client may also
/// close the connection before it is accepted, and a transition to the
/// k_ESteamNetworkingConnectionState_ClosedByPeer is also possible depending the exact
/// sequence of events.
///
/// Returns k_EResultInvalidParam if the handle is invalid.
/// Returns k_EResultInvalidState if the connection is not in the appropriate state.
/// (Remember that the connection state could change in between the time that the
/// notification being posted to the queue and when it is received by the application.)
EResult AcceptConnection( HSteamNetConnection hConn )
{
    PRINT_DEBUG("Steam_Networking_Sockets::AcceptConnection\n");
}


/// Disconnects from the remote host and invalidates the connection handle.
/// Any unread data on the connection is discarded.
///
/// nReason is an application defined code that will be received on the other
/// end and recorded (when possible) in backend analytics.  The value should
/// come from a restricted range.  (See ESteamNetConnectionEnd.)  If you don't need
/// to communicate any information to the remote host, and do not want analytics to
/// be able to distinguish "normal" connection terminations from "exceptional" ones,
/// You may pass zero, in which case the generic value of
/// k_ESteamNetConnectionEnd_App_Generic will be used.
///
/// pszDebug is an optional human-readable diagnostic string that will be received
/// by the remote host and recorded (when possible) in backend analytics.
///
/// If you wish to put the socket into a "linger" state, where an attempt is made to
/// flush any remaining sent data, use bEnableLinger=true.  Otherwise reliable data
/// is not flushed.
///
/// If the connection has already ended and you are just freeing up the
/// connection interface, the reason code, debug string, and linger flag are
/// ignored.
bool CloseConnection( HSteamNetConnection hPeer, int nReason, const char *pszDebug, bool bEnableLinger )
{
    PRINT_DEBUG("Steam_Networking_Sockets::CloseConnection\n");
}


/// Destroy a listen socket, and all the client sockets generated by accepting connections
/// on the listen socket.
///
/// pszNotifyRemoteReason determines what cleanup actions are performed on the client
/// sockets being destroyed.  (See DestroySocket for more details.)
///
/// Note that if cleanup is requested and you have requested the listen socket bound to a
/// particular local port to facilitate direct UDP/IPv4 connections, then the underlying UDP
/// socket must remain open until all clients have been cleaned up.
bool CloseListenSocket( HSteamListenSocket hSocket, const char *pszNotifyRemoteReason )
{
    PRINT_DEBUG("Steam_Networking_Sockets::CloseListenSocket old\n");
}

/// Destroy a listen socket.  All the connections that were accepting on the listen
/// socket are closed ungracefully.
bool CloseListenSocket( HSteamListenSocket hSocket )
{
    PRINT_DEBUG("Steam_Networking_Sockets::CloseListenSocket\n");
}

/// Set connection user data.  Returns false if the handle is invalid.
bool SetConnectionUserData( HSteamNetConnection hPeer, int64 nUserData )
{
    PRINT_DEBUG("Steam_Networking_Sockets::SetConnectionUserData\n");
}


/// Fetch connection user data.  Returns -1 if handle is invalid
/// or if you haven't set any userdata on the connection.
int64 GetConnectionUserData( HSteamNetConnection hPeer )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetConnectionUserData\n");
}


/// Set a name for the connection, used mostly for debugging
void SetConnectionName( HSteamNetConnection hPeer, const char *pszName )
{
    PRINT_DEBUG("Steam_Networking_Sockets::SetConnectionName\n");
}


/// Fetch connection name.  Returns false if handle is invalid
bool GetConnectionName( HSteamNetConnection hPeer, char *pszName, int nMaxLen )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetConnectionName\n");
}


/// Send a message to the remote host on the connected socket.
///
/// eSendType determines the delivery guarantees that will be provided,
/// when data should be buffered, etc.
///
/// Note that the semantics we use for messages are not precisely
/// the same as the semantics of a standard "stream" socket.
/// (SOCK_STREAM)  For an ordinary stream socket, the boundaries
/// between chunks are not considered relevant, and the sizes of
/// the chunks of data written will not necessarily match up to
/// the sizes of the chunks that are returned by the reads on
/// the other end.  The remote host might read a partial chunk,
/// or chunks might be coalesced.  For the message semantics 
/// used here, however, the sizes WILL match.  Each send call 
/// will match a successful read call on the remote host 
/// one-for-one.  If you are porting existing stream-oriented 
/// code to the semantics of reliable messages, your code should 
/// work the same, since reliable message semantics are more 
/// strict than stream semantics.  The only caveat is related to 
/// performance: there is per-message overhead to retain the 
/// messages sizes, and so if your code sends many small chunks 
/// of data, performance will suffer. Any code based on stream 
/// sockets that does not write excessively small chunks will 
/// work without any changes. 
EResult SendMessageToConnection( HSteamNetConnection hConn, const void *pData, uint32 cbData, ESteamNetworkingSendType eSendType )
{
    PRINT_DEBUG("Steam_Networking_Sockets::SendMessageToConnection old\n");
}

/// Send a message to the remote host on the specified connection.
///
/// nSendFlags determines the delivery guarantees that will be provided,
/// when data should be buffered, etc.  E.g. k_nSteamNetworkingSend_Unreliable
///
/// Note that the semantics we use for messages are not precisely
/// the same as the semantics of a standard "stream" socket.
/// (SOCK_STREAM)  For an ordinary stream socket, the boundaries
/// between chunks are not considered relevant, and the sizes of
/// the chunks of data written will not necessarily match up to
/// the sizes of the chunks that are returned by the reads on
/// the other end.  The remote host might read a partial chunk,
/// or chunks might be coalesced.  For the message semantics 
/// used here, however, the sizes WILL match.  Each send call 
/// will match a successful read call on the remote host 
/// one-for-one.  If you are porting existing stream-oriented 
/// code to the semantics of reliable messages, your code should 
/// work the same, since reliable message semantics are more 
/// strict than stream semantics.  The only caveat is related to 
/// performance: there is per-message overhead to retain the 
/// message sizes, and so if your code sends many small chunks 
/// of data, performance will suffer. Any code based on stream 
/// sockets that does not write excessively small chunks will 
/// work without any changes. 
///
/// Returns:
/// - k_EResultInvalidParam: invalid connection handle, or the individual message is too big.
///   (See k_cbMaxSteamNetworkingSocketsMessageSizeSend)
/// - k_EResultInvalidState: connection is in an invalid state
/// - k_EResultNoConnection: connection has ended
/// - k_EResultIgnored: You used k_nSteamNetworkingSend_NoDelay, and the message was dropped because
///   we were not ready to send it.
/// - k_EResultLimitExceeded: there was already too much data queued to be sent.
///   (See k_ESteamNetworkingConfig_SendBufferSize)
virtual EResult SendMessageToConnection( HSteamNetConnection hConn, const void *pData, uint32 cbData, int nSendFlags )
{
    PRINT_DEBUG("Steam_Networking_Sockets::SendMessageToConnection\n");
}

/// If Nagle is enabled (its on by default) then when calling 
/// SendMessageToConnection the message will be queued up the Nagle time
/// before being sent to merge small messages into the same packet.
///
/// Call this function to flush any queued messages and send them immediately
/// on the next transmission time (often that means right now).
EResult FlushMessagesOnConnection( HSteamNetConnection hConn )
{
    PRINT_DEBUG("Steam_Networking_Sockets::FlushMessagesOnConnection\n");
}

/// Fetch the next available message(s) from the connection, if any.
/// Returns the number of messages returned into your array, up to nMaxMessages.
/// If the connection handle is invalid, -1 is returned.
///
/// The order of the messages returned in the array is relevant.
/// Reliable messages will be received in the order they were sent (and with the
/// same sizes --- see SendMessageToConnection for on this subtle difference from a stream socket).
///
/// Unreliable messages may be dropped, or delivered out of order withrespect to
/// each other or with respect to reliable messages.  The same unreliable message
/// may be received multiple times.
///
/// If any messages are returned, you MUST call SteamNetworkingMessage_t::Release() on each
/// of them free up resources after you are done.  It is safe to keep the object alive for
/// a little while (put it into some queue, etc), and you may call Release() from any thread.
int ReceiveMessagesOnConnection( HSteamNetConnection hConn, SteamNetworkingMessage_t **ppOutMessages, int nMaxMessages )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ReceiveMessagesOnConnection\n");
}

/// Same as ReceiveMessagesOnConnection, but will return the next message available
/// on any connection that was accepted through the specified listen socket.  Examine
/// SteamNetworkingMessage_t::m_conn to know which client connection.
///
/// Delivery order of messages among different clients is not defined.  They may
/// be returned in an order different from what they were actually received.  (Delivery
/// order of messages from the same client is well defined, and thus the order of the
/// messages is relevant!)
int ReceiveMessagesOnListenSocket( HSteamListenSocket hSocket, SteamNetworkingMessage_t **ppOutMessages, int nMaxMessages )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ReceiveMessagesOnListenSocket\n");
}

/// Returns basic information about the high-level state of the connection.
bool GetConnectionInfo( HSteamNetConnection hConn, SteamNetConnectionInfo_t *pInfo )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetConnectionInfo\n");
}


/// Fetch the next available message(s) from the socket, if any.
/// Returns the number of messages returned into your array, up to nMaxMessages.
/// If the connection handle is invalid, -1 is returned.
///
/// The order of the messages returned in the array is relevant.
/// Reliable messages will be received in the order they were sent (and with the
/// same sizes --- see SendMessageToConnection for on this subtle difference from a stream socket).
///
/// FIXME - We're still debating the exact set of guarantees for unreliable, so this might change.
/// Unreliable messages may not be received.  The order of delivery of unreliable messages
/// is NOT specified.  They may be received out of order with respect to each other or
/// reliable messages.  They may be received multiple times!
///
/// If any messages are returned, you MUST call Release() to each of them free up resources
/// after you are done.  It is safe to keep the object alive for a little while (put it
/// into some queue, etc), and you may call Release() from any thread.
int ReceiveMessagesOnConnection( HSteamNetConnection hConn, SteamNetworkingMessage001_t **ppOutMessages, int nMaxMessages )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ReceiveMessagesOnConnection\n");
}
 

/// Same as ReceiveMessagesOnConnection, but will return the next message available
/// on any client socket that was accepted through the specified listen socket.  Examine
/// SteamNetworkingMessage_t::m_conn to know which client connection.
///
/// Delivery order of messages among different clients is not defined.  They may
/// be returned in an order different from what they were actually received.  (Delivery
/// order of messages from the same client is well defined, and thus the order of the
/// messages is relevant!)
int ReceiveMessagesOnListenSocket( HSteamListenSocket hSocket, SteamNetworkingMessage001_t **ppOutMessages, int nMaxMessages )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ReceiveMessagesOnListenSocket\n");
}
 

/// Returns information about the specified connection.
bool GetConnectionInfo( HSteamNetConnection hConn, SteamNetConnectionInfo001_t *pInfo )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetConnectionInfo\n");
}


/// Returns brief set of connection status that you might want to display
/// to the user in game.
bool GetQuickConnectionStatus( HSteamNetConnection hConn, SteamNetworkingQuickConnectionStatus *pStats )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetQuickConnectionStatus\n");
}


/// Returns detailed connection stats in text format.  Useful
/// for dumping to a log, etc.
///
/// Returns:
/// -1 failure (bad connection handle)
/// 0 OK, your buffer was filled in and '\0'-terminated
/// >0 Your buffer was either nullptr, or it was too small and the text got truncated.  Try again with a buffer of at least N bytes.
int GetDetailedConnectionStatus( HSteamNetConnection hConn, char *pszBuf, int cbBuf )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetDetailedConnectionStatus\n");
}

/// Returns local IP and port that a listen socket created using CreateListenSocketIP is bound to.
///
/// An IPv6 address of ::0 means "any IPv4 or IPv6"
/// An IPv6 address of ::ffff:0000:0000 means "any IPv4"
bool GetListenSocketAddress( HSteamListenSocket hSocket, SteamNetworkingIPAddr *address )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetListenSocketAddress\n");
}

/// Returns information about the listen socket.
///
/// *pnIP and *pnPort will be 0 if the socket is set to listen for connections based
/// on SteamID only.  If your listen socket accepts connections on IPv4, then both
/// fields will return nonzero, even if you originally passed a zero IP.  However,
/// note that the address returned may be a private address (e.g. 10.0.0.x or 192.168.x.x),
/// and may not be reachable by a general host on the Internet.
bool GetListenSocketInfo( HSteamListenSocket hSocket, uint32 *pnIP, uint16 *pnPort )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetListenSocketInfo\n");
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    struct Listen_Socket *socket = get_connection_socket(hSocket);
    if (!socket) return false;
    if (pnIP) *pnIP = socket->ip;
    if (pnPort) *pnPort = socket->port;
    return true;
}


/// Create a pair of connections that are talking to each other, e.g. a loopback connection.
/// This is very useful for testing, or so that your client/server code can work the same
/// even when you are running a local "server".
///
/// The two connections will immediately be placed into the connected state, and no callbacks
/// will be posted immediately.  After this, if you close either connection, the other connection
/// will receive a callback, exactly as if they were communicating over the network.  You must
/// close *both* sides in order to fully clean up the resources!
///
/// By default, internal buffers are used, completely bypassing the network, the chopping up of
/// messages into packets, encryption, copying the payload, etc.  This means that loopback
/// packets, by default, will not simulate lag or loss.  Passing true for bUseNetworkLoopback will
/// cause the socket pair to send packets through the local network loopback device (127.0.0.1)
/// on ephemeral ports.  Fake lag and loss are supported in this case, and CPU time is expended
/// to encrypt and decrypt.
///
/// The SteamID assigned to both ends of the connection will be the SteamID of this interface.
bool CreateSocketPair( HSteamNetConnection *pOutConnection1, HSteamNetConnection *pOutConnection2, bool bUseNetworkLoopback )
{
    PRINT_DEBUG("Steam_Networking_Sockets::CreateSocketPair old\n");
}

/// Create a pair of connections that are talking to each other, e.g. a loopback connection.
/// This is very useful for testing, or so that your client/server code can work the same
/// even when you are running a local "server".
///
/// The two connections will immediately be placed into the connected state, and no callbacks
/// will be posted immediately.  After this, if you close either connection, the other connection
/// will receive a callback, exactly as if they were communicating over the network.  You must
/// close *both* sides in order to fully clean up the resources!
///
/// By default, internal buffers are used, completely bypassing the network, the chopping up of
/// messages into packets, encryption, copying the payload, etc.  This means that loopback
/// packets, by default, will not simulate lag or loss.  Passing true for bUseNetworkLoopback will
/// cause the socket pair to send packets through the local network loopback device (127.0.0.1)
/// on ephemeral ports.  Fake lag and loss are supported in this case, and CPU time is expended
/// to encrypt and decrypt.
///
/// If you wish to assign a specific identity to either connection, you may pass a particular
/// identity.  Otherwise, if you pass nullptr, the respective connection will assume a generic
/// "localhost" identity.  If you use real network loopback, this might be translated to the
/// actual bound loopback port.  Otherwise, the port will be zero.
bool CreateSocketPair( HSteamNetConnection *pOutConnection1, HSteamNetConnection *pOutConnection2, bool bUseNetworkLoopback, const SteamNetworkingIdentity *pIdentity1, const SteamNetworkingIdentity *pIdentity2 )
{
    PRINT_DEBUG("Steam_Networking_Sockets::CreateSocketPair\n");
}

/// Get the identity assigned to this interface.
/// E.g. on Steam, this is the user's SteamID, or for the gameserver interface, the SteamID assigned
/// to the gameserver.  Returns false and sets the result to an invalid identity if we don't know
/// our identity yet.  (E.g. GameServer has not logged in.  On Steam, the user will know their SteamID
/// even if they are not signed into Steam.)
bool GetIdentity( SteamNetworkingIdentity *pIdentity )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetIdentity\n");
}


//#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE

//
// Clients connecting to dedicated servers hosted in a data center,
// using central-authority-granted tickets.
//

/// Called when we receive a ticket from our central matchmaking system.  Puts the
/// ticket into a persistent cache, and optionally returns the parsed ticket.
///
/// See stamdatagram_ticketgen.h for more details.
bool ReceivedRelayAuthTicket( const void *pvTicket, int cbTicket, SteamDatagramRelayAuthTicket *pOutParsedTicket )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ReceivedRelayAuthTicket\n");
}


/// Search cache for a ticket to talk to the server on the specified virtual port.
/// If found, returns the number of second until the ticket expires, and optionally
/// the complete cracked ticket.  Returns 0 if we don't have a ticket.
///
/// Typically this is useful just to confirm that you have a ticket, before you
/// call ConnectToHostedDedicatedServer to connect to the server.
int FindRelayAuthTicketForServer( CSteamID steamID, int nVirtualPort, SteamDatagramRelayAuthTicket *pOutParsedTicket )
{
    PRINT_DEBUG("Steam_Networking_Sockets::FindRelayAuthTicketForServer old\n");
}

/// Search cache for a ticket to talk to the server on the specified virtual port.
/// If found, returns the number of seconds until the ticket expires, and optionally
/// the complete cracked ticket.  Returns 0 if we don't have a ticket.
///
/// Typically this is useful just to confirm that you have a ticket, before you
/// call ConnectToHostedDedicatedServer to connect to the server.
int FindRelayAuthTicketForServer( const SteamNetworkingIdentity &identityGameServer, int nVirtualPort, SteamDatagramRelayAuthTicket *pOutParsedTicket )
{
    PRINT_DEBUG("Steam_Networking_Sockets::FindRelayAuthTicketForServer\n");
}

/// Client call to connect to a server hosted in a Valve data center, on the specified virtual
/// port.  You must have placed a ticket for this server into the cache, or else this connect attempt will fail!
///
/// You may wonder why tickets are stored in a cache, instead of simply being passed as an argument
/// here.  The reason is to make reconnection to a gameserver robust, even if the client computer loses
/// connection to Steam or the central backend, or the app is restarted or crashes, etc.
///
/// If you use this, you probably want to call ISteamNetworkingUtils::InitializeRelayNetworkAccess()
/// when your app initializes
HSteamNetConnection ConnectToHostedDedicatedServer( const SteamNetworkingIdentity &identityTarget, int nVirtualPort )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ConnectToHostedDedicatedServer\n");
}

/// Client call to connect to a server hosted in a Valve data center, on the specified virtual
/// port.  You should have received a ticket for this server, or else this connect call will fail!
///
/// You may wonder why tickets are stored in a cache, instead of simply being passed as an argument
/// here.  The reason is to make reconnection to a gameserver robust, even if the client computer loses
/// connection to Steam or the central backend, or the app is restarted or crashes, etc.
HSteamNetConnection ConnectToHostedDedicatedServer( CSteamID steamIDTarget, int nVirtualPort )
{
    PRINT_DEBUG("Steam_Networking_Sockets::ConnectToHostedDedicatedServer old\n");
}


//
// Servers hosted in Valve data centers
//

/// Returns the value of the SDR_LISTEN_PORT environment variable.
uint16 GetHostedDedicatedServerPort()
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetHostedDedicatedServerPort\n");
    //TODO?
    return 27054;
}


/// If you are running in a production data center, this will return the data
/// center code.  Returns 0 otherwise.
SteamNetworkingPOPID GetHostedDedicatedServerPOPID()
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetHostedDedicatedServerPOPID\n");
}


/// Return info about the hosted server.  You will need to send this information to your
/// backend, and put it in tickets, so that the relays will know how to forward traffic from
/// clients to your server.  See SteamDatagramRelayAuthTicket for more info.
///
/// NOTE ABOUT DEVELOPMENT ENVIRONMENTS:
/// In production in our data centers, these parameters are configured via environment variables.
/// In development, the only one you need to set is SDR_LISTEN_PORT, which is the local port you
/// want to listen on.  Furthermore, if you are running your server behind a corporate firewall,
/// you probably will not be able to put the routing information returned by this function into
/// tickets.   Instead, it should be a public internet address that the relays can use to send
/// data to your server.  So you might just end up hardcoding a public address and setup port
/// forwarding on your corporate firewall.  In that case, the port you put into the ticket
/// needs to be the public-facing port opened on your firewall, if it is different from the
/// actual server port.
///
/// This function will fail if SteamDatagramServer_Init has not been called.
///
/// Returns false if the SDR_LISTEN_PORT environment variable is not set.
bool GetHostedDedicatedServerAddress( SteamDatagramHostedAddress *pRouting )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetHostedDedicatedServerAddress %p\n", pRouting);
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    pRouting->SetDevAddress(network->getOwnIP(), 27054);
    return true;
}


/// Create a listen socket on the specified virtual port.  The physical UDP port to use
/// will be determined by the SDR_LISTEN_PORT environment variable.  If a UDP port is not
/// configured, this call will fail.
///
/// Note that this call MUST be made through the SteamNetworkingSocketsGameServer() interface
HSteamListenSocket CreateHostedDedicatedServerListenSocket( int nVirtualPort )
{
    PRINT_DEBUG("Steam_Networking_Sockets::CreateHostedDedicatedServerListenSocket %i\n", nVirtualPort);
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    return new_listen_socket(nVirtualPort, 0, 0);
}


//#endif // #ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE

//
// Gets some debug text from the connection
//
bool GetConnectionDebugText( HSteamNetConnection hConn, char *pOut, int nOutCCH )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetConnectionDebugText\n");
}


//
// Set and get configuration values, see ESteamNetworkingConfigurationValue for individual descriptions.
//
// Returns the value or -1 is eConfigValue is invalid
int32 GetConfigurationValue( ESteamNetworkingConfigurationValue eConfigValue )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetConfigurationValue\n");
}

// Returns true if successfully set
bool SetConfigurationValue( ESteamNetworkingConfigurationValue eConfigValue, int32 nValue )
{
    PRINT_DEBUG("Steam_Networking_Sockets::SetConfigurationValue %i: %i\n", eConfigValue, nValue);
    return true;
}


// Return the name of an int configuration value, or NULL if config value isn't known
const char *GetConfigurationValueName( ESteamNetworkingConfigurationValue eConfigValue )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetConfigurationValueName\n");
}


//
// Set and get configuration strings, see ESteamNetworkingConfigurationString for individual descriptions.
//
// Get the configuration string, returns length of string needed if pDest is nullpr or destSize is 0
// returns -1 if the eConfigValue is invalid
int32 GetConfigurationString( ESteamNetworkingConfigurationString eConfigString, char *pDest, int32 destSize )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetConfigurationString\n");
}

bool SetConfigurationString( ESteamNetworkingConfigurationString eConfigString, const char *pString )
{
    PRINT_DEBUG("Steam_Networking_Sockets::SetConfigurationString\n");
}


// Return the name of a string configuration value, or NULL if config value isn't known
const char *GetConfigurationStringName( ESteamNetworkingConfigurationString eConfigString )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetConfigurationStringName\n");
}


//
// Set and get configuration values, see ESteamNetworkingConnectionConfigurationValue for individual descriptions.
//
// Returns the value or -1 is eConfigValue is invalid
int32 GetConnectionConfigurationValue( HSteamNetConnection hConn, ESteamNetworkingConnectionConfigurationValue eConfigValue )
{
    PRINT_DEBUG("Steam_Networking_Sockets::GetConnectionConfigurationValue\n");
}

// Returns true if successfully set
bool SetConnectionConfigurationValue( HSteamNetConnection hConn, ESteamNetworkingConnectionConfigurationValue eConfigValue, int32 nValue )
{
    PRINT_DEBUG("Steam_Networking_Sockets::SetConnectionConfigurationValue\n");
}


// TEMP KLUDGE Call to invoke all queued callbacks.
// Eventually this function will go away, and callwacks will be ordinary Steamworks callbacks.
// You should call this at the same time you call SteamAPI_RunCallbacks and SteamGameServer_RunCallbacks
// to minimize potential changes in timing when that change happens.
void RunCallbacks( ISteamNetworkingSocketsCallbacks *pCallbacks )
{
    PRINT_DEBUG("Steam_Networking_Sockets:RunCallbacks\n");
}


void RunCallbacks()
{
}

void Callback(Common_Message *msg)
{
    if (msg->has_low_level()) {
        if (msg->low_level().type() == Low_Level::CONNECT) {
            
        }

        if (msg->low_level().type() == Low_Level::DISCONNECT) {

        }
    }

    if (msg->has_networking_sockets()) {

    }
}

};

$fi = $args[0]

if( ! $fi )
{
  $fi = "steam_api.dll"
}

function findinterface

($api)
{
  $str = Select-String "$api[0-9][0-9][0-9]" $fi
  if( $str -match "$api[0-9]{3}" )
  {
    Write-Output "$($matches[0])"
  }  
}

findinterface SteamClient                            >steam_interfaces.txt
findinterface SteamGameServer                        >>steam_interfaces.txt
findinterface SteamGameServerStats                   >>steam_interfaces.txt
findinterface SteamUser                              >>steam_interfaces.txt
findinterface SteamFriends                           >>steam_interfaces.txt
findinterface SteamUtils                             >>steam_interfaces.txt
findinterface SteamMatchMaking                       >>steam_interfaces.txt
findinterface SteamMatchMakingServers                >>steam_interfaces.txt
findinterface STEAMUSERSTATS_INTERFACE_VERSION       >>steam_interfaces.txt
findinterface STEAMAPPS_INTERFACE_VERSION            >>steam_interfaces.txt
findinterface SteamNetworking                        >>steam_interfaces.txt
findinterface STEAMREMOTESTORAGE_INTERFACE_VERSION   >>steam_interfaces.txt
findinterface STEAMSCREENSHOTS_INTERFACE_VERSION     >>steam_interfaces.txt
findinterface STEAMHTTP_INTERFACE_VERSION            >>steam_interfaces.txt
findinterface STEAMUNIFIEDMESSAGES_INTERFACE_VERSION >>steam_interfaces.txt
findinterface STEAMCONTROLLER_INTERFACE_VERSION      >>steam_interfaces.txt
findinterface STEAMUGC_INTERFACE_VERSION             >>steam_interfaces.txt
findinterface STEAMAPPLIST_INTERFACE_VERSION         >>steam_interfaces.txt
findinterface STEAMMUSIC_INTERFACE_VERSION           >>steam_interfaces.txt
findinterface STEAMMUSICREMOTE_INTERFACE_VERSION     >>steam_interfaces.txt
findinterface STEAMHTMLSURFACE_INTERFACE_VERSION_    >>steam_interfaces.txt
findinterface STEAMINVENTORY_INTERFACE_V             >>steam_interfaces.txt
findinterface SteamController                        >>steam_interfaces.txt

USERNAME = ""
PASSWORD = ""

#steam ids with public profiles that own a lot of games
TOP_OWNER_IDS = [76561198028121353, 76561198001237877, 76561198001678750, 76561198237402290]

from stats_schema_achievement_gen import achievements_gen
from steam.client import SteamClient
from steam.client.cdn import CDNClient
from steam.enums import common
from steam.enums.emsg import EMsg
from steam.core.msg import MsgProto
import os
import sys

if len(sys.argv) < 2:
    print("\nUsage: {} appid\n\nExample: {} 480\n".format(sys.argv[0], sys.argv[0]))
    exit(1)

appid = int(sys.argv[1])

client = SteamClient()
if not os.path.exists("login_temp"):
    os.makedirs("login_temp")
client.set_credential_location("login_temp")

if (len(USERNAME) == 0 or len(PASSWORD) == 0):
    client.cli_login()
else:
    while True:
        ret = client.login(USERNAME, password=PASSWORD)
        if ret != common.EResult.OK:
            if ret == common.EResult.TryAnotherCM:
                continue
            print("error logging in with set username and password, trying cli login. error was:", ret)
            client.cli_login()
            break
        else:
            break

def get_stats_schema(client, game_id, owner_id):
    message = MsgProto(EMsg.ClientGetUserStats)
    message.body.game_id = game_id
    message.body.schema_local_version = -1
    message.body.crc_stats = 0
    message.body.steam_id_for_user = owner_id

    client.send(message)
    return client.wait_msg(EMsg.ClientGetUserStatsResponse, timeout=5)

def generate_achievement_stats(client, game_id, output_directory):
    steam_id_list = TOP_OWNER_IDS + [client.steam_id]
    for x in steam_id_list:
        out = get_stats_schema(client, game_id, x)
        if out is not None:
            if len(out.body.schema) > 0:
                with open('UserGameStatsSchema_{}.bin'.format(appid), 'wb') as f:
                    f.write(out.body.schema)
                achievements, stats = achievements_gen.generate_stats_achievements(out.body.schema, output_directory)
                return
            else:
                print("no schema", out)

out_dir = os.path.join("{}".format( "{}_output".format(appid)), "steam_settings")

if not os.path.exists(out_dir):
    os.makedirs(out_dir)

print("outputting config to", out_dir)

generate_achievement_stats(client, appid, out_dir)
with open(os.path.join(out_dir, "steam_appid.txt"), 'w') as f:
    f.write(str(appid))


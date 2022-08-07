
USERNAME = ""
PASSWORD = ""

#steam ids with public profiles that own a lot of games
TOP_OWNER_IDS = [76561198028121353, 76561198001237877, 76561198355625888, 76561198001678750, 76561198237402290]

from stats_schema_achievement_gen import achievements_gen
from steam.client import SteamClient
from steam.client.cdn import CDNClient
from steam.enums import common
from steam.enums.common import EResult
from steam.enums.emsg import EMsg
from steam.core.msg import MsgProto
import os
import sys
import json

prompt_for_unavailable = True

if len(sys.argv) < 2:
    print("\nUsage: {} appid appid appid etc..\n\nExample: {} 480\n".format(sys.argv[0], sys.argv[0]))
    exit(1)

appids = []
for id in sys.argv[1:]:
    appids +=  [int(id)]

client = SteamClient()
if not os.path.exists("login_temp"):
    os.makedirs("login_temp")
client.set_credential_location("login_temp")

if (len(USERNAME) == 0 or len(PASSWORD) == 0):
    client.cli_login()
else:
    result = client.login(USERNAME, password=PASSWORD)
    while result in (EResult.AccountLogonDenied, EResult.InvalidLoginAuthCode,
                        EResult.AccountLoginDeniedNeedTwoFactor, EResult.TwoFactorCodeMismatch,
                        EResult.TryAnotherCM, EResult.ServiceUnavailable,
                        EResult.InvalidPassword,
                        ):

        if result == EResult.InvalidPassword:
            print("invalid password, the password you set is wrong.")
            exit(1)

        elif result in (EResult.AccountLogonDenied, EResult.InvalidLoginAuthCode):
            prompt = ("Enter email code: " if result == EResult.AccountLogonDenied else
                        "Incorrect code. Enter email code: ")
            auth_code, two_factor_code = input(prompt), None

        elif result in (EResult.AccountLoginDeniedNeedTwoFactor, EResult.TwoFactorCodeMismatch):
            prompt = ("Enter 2FA code: " if result == EResult.AccountLoginDeniedNeedTwoFactor else
                        "Incorrect code. Enter 2FA code: ")
            auth_code, two_factor_code = None, input(prompt)

        elif result in (EResult.TryAnotherCM, EResult.ServiceUnavailable):
            if prompt_for_unavailable and result == EResult.ServiceUnavailable:
                while True:
                    answer = input("Steam is down. Keep retrying? [y/n]: ").lower()
                    if answer in 'yn': break

                prompt_for_unavailable = False
                if answer == 'n': break

            client.reconnect(maxdelay=15)

        result = client.login(USERNAME, PASSWORD, None, auth_code, two_factor_code)


def get_stats_schema(client, game_id, owner_id):
    message = MsgProto(EMsg.ClientGetUserStats)
    message.body.game_id = game_id
    message.body.schema_local_version = -1
    message.body.crc_stats = 0
    message.body.steam_id_for_user = owner_id

    client.send(message)
    return client.wait_msg(EMsg.ClientGetUserStatsResponse, timeout=5)

def generate_achievement_stats(client, game_id, output_directory, backup_directory):
    steam_id_list = TOP_OWNER_IDS + [client.steam_id]
    for x in steam_id_list:
        out = get_stats_schema(client, game_id, x)
        if out is not None:
            if len(out.body.schema) > 0:
                with open(os.path.join(backup_directory, 'UserGameStatsSchema_{}.bin'.format(appid)), 'wb') as f:
                    f.write(out.body.schema)
                achievements, stats = achievements_gen.generate_stats_achievements(out.body.schema, output_directory)
                return
            else:
                print("no schema", out)

for appid in appids:
    backup_dir = os.path.join("backup","{}".format(appid))
    out_dir = os.path.join("{}".format( "{}_output".format(appid)), "steam_settings")

    if not os.path.exists(backup_dir):
        os.makedirs(backup_dir)

    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    print("outputting config to", out_dir)

    raw = client.get_product_info(apps=[appid])
    game_info = raw["apps"][appid]

    if "common" in game_info:
        game_info_common = game_info["common"]
        if "community_visible_stats" in game_info_common:
            generate_achievement_stats(client, appid, out_dir, backup_dir)

    with open(os.path.join(out_dir, "steam_appid.txt"), 'w') as f:
        f.write(str(appid))

    if "depots" in game_info:
        if "branches" in game_info["depots"]:
            if "public" in game_info["depots"]["branches"]:
                if "buildid" in game_info["depots"]["branches"]["public"]:
                    buildid = game_info["depots"]["branches"]["public"]["buildid"]
                    with open(os.path.join(out_dir, "build_id.txt"), 'w') as f:
                        f.write(str(buildid))

    game_info_backup = json.dumps(game_info, indent=4)
    with open(os.path.join(backup_dir, "product_info.json"), "w") as f:
        f.write(game_info_backup)

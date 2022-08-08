
USERNAME = ""
PASSWORD = ""

#steam ids with public profiles that own a lot of games
TOP_OWNER_IDS = [76561198028121353, 76561198001237877, 76561198355625888, 76561198001678750, 76561198237402290, 76561197979911851, 76561198152618007, 76561197969050296, 76561198213148949, 76561198037867621, 76561198108581917]

from stats_schema_achievement_gen import achievements_gen
from controller_config_generator import parse_controller_vdf
from steam.client import SteamClient
from steam.client.cdn import CDNClient
from steam.enums import common
from steam.enums.common import EResult
from steam.enums.emsg import EMsg
from steam.core.msg import MsgProto
import os
import sys
import json
import urllib.request

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
    auth_code, two_factor_code = None, None
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

def get_ugc_info(client, published_file_id):
    return client.send_um_and_wait('PublishedFile.GetDetails#1', {
            'publishedfileids': [published_file_id],
            'includetags': False,
            'includeadditionalpreviews': False,
            'includechildren': False,
            'includekvtags': False,
            'includevotes': False,
            'short_description': True,
            'includeforsaledata': False,
            'includemetadata': False,
            'language': 0
        })

def download_published_file(client, published_file_id, backup_directory):
    ugc_info = get_ugc_info(client, published_file_id)

    if (ugc_info is None):
        print("failed getting published file", published_file_id)
        return None

    file_details = ugc_info.body.publishedfiledetails[0]
    if (file_details.result != EResult.OK):
        print("failed getting published file", published_file_id, file_details.result)
        return None

    if not os.path.exists(backup_directory):
        os.makedirs(backup_directory)

    with open(os.path.join(backup_directory, "info.txt"), "w") as f:
        f.write(str(ugc_info.body))

    with urllib.request.urlopen(file_details.file_url) as response:
        data = response.read()
        with open(os.path.join(backup_directory, file_details.filename.replace("/", "_").replace("\\", "_")), "wb") as f:
            f.write(data)
        return data
    return None

def get_dlc(raw_infos):
    try:
        try:
            dlc_list = set(map(lambda a: int(a), raw_infos["extended"]["listofdlc"].split(",")))
        except:
            dlc_list = set()
        depot_app_list = set()
        if "depots" in raw_infos:
            depots = raw_infos["depots"]
            for dep in depots:
                depot_info = depots[dep]
                if "dlcappid" in depot_info:
                    dlc_list.add(int(depot_info["dlcappid"]))
                if "depotfromapp" in depot_info:
                    depot_app_list.add(int(depot_info["depotfromapp"]))
        return (dlc_list, depot_app_list)
    except:
        print("could not get dlc infos, are there any dlcs ?")
        return (set(), set())

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

    dlc_config_list = []
    dlc_list, depot_app_list = get_dlc(game_info)
    if (len(dlc_list) > 0):
        dlc_raw = client.get_product_info(apps=dlc_list)["apps"]
        for dlc in dlc_raw:
            try:
                dlc_config_list.append((dlc, dlc_raw[dlc]["common"]["name"]))
            except:
                dlc_config_list.append((dlc, None))

    with open(os.path.join(out_dir, "DLC.txt"), 'w') as f:
        for x in dlc_config_list:
            if (x[1] is not None):
                f.write("{}={}\n".format(x[0], x[1]))

    config_generated = False
    if "config" in game_info:
        if "steamcontrollerconfigdetails" in game_info["config"]:
            controller_details = game_info["config"]["steamcontrollerconfigdetails"]
            for id in controller_details:
                details = controller_details[id]
                controller_type = ""
                enabled_branches = ""
                if "controller_type" in details:
                    controller_type = details["controller_type"]
                if "enabled_branches" in details:
                    enabled_branches = details["enabled_branches"]
                print(id, controller_type)
                out_vdf = download_published_file(client, int(id), os.path.join(backup_dir, controller_type + str(id)))
                if out_vdf is not None and not config_generated:
                    if (controller_type in ["controller_xbox360", "controller_xboxone"] and (("default" in enabled_branches) or ("public" in enabled_branches))):
                        parse_controller_vdf.generate_controller_config(out_vdf.decode('utf-8'), os.path.join(out_dir, "controller"))
                        config_generated = True

    game_info_backup = json.dumps(game_info, indent=4)
    with open(os.path.join(backup_dir, "product_info.json"), "w") as f:
        f.write(game_info_backup)

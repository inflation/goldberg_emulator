Install python then run this in a terminal/powershell to install the dependencies: pip install -U steam[client]

This script depends on python files that are in subfolders so make sure to download the whole folder not just the script.

Using the script: python generate_emu_config.py appid

The appid is the number in the steam url.

The first time you run the script it will ask you for your steam username, password and email code. 
The email code will only be asked the first time and the sentry will be saved to the login_temp folder.

This script will not save your username/password anywhere. If you don't want to always type it you must
open up the script in a text editor and change:

USERNAME = ""
PASSWORD = ""

To:

USERNAME = "your_username"
PASSWORD = "your_password"


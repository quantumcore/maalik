import os
from colorama import Style, Fore
import colorama
from PIL import Image
from io import BytesIO



banner = Style.BRIGHT + r"""

        • ▌ ▄ ·.  ▄▄▄·  ▄▄▄· ▄▄▌  ▪  ▄ •▄ 
        ·██ ▐███▪▐█ ▀█ ▐█ ▀█ ██•  ██ █▌▄▌▪
        ▐█ ▌▐▌▐█·▄█▀▀█ ▄█▀▀█ ██▪  ▐█·▐▀▀▄·
        ██ ██▌▐█▌▐█ ▪▐▌▐█ ▪▐▌▐█▌▐▌▐█▌▐█.█▌
        ▀▀  █▪▀▀▀ ▀  ▀  ▀  ▀ .▀▀▀ ▀▀▀·▀  ▀
                Defying All odds
               [ Version : {ver} ]
""" + Style.RESET_ALL


procmonitor = Style.BRIGHT + r"""
 ▄▄▄·▄▄▄         ▄▄· ▄▄▄ ..▄▄ · .▄▄ ·     • ▌ ▄ ·.        ▐ ▄ ▪  ▄▄▄▄▄      ▄▄▄  
▐█ ▄█▀▄ █·▪     ▐█ ▌▪▀▄.▀·▐█ ▀. ▐█ ▀.     ·██ ▐███▪▪     •█▌▐███ •██  ▪     ▀▄ █·
 ██▀·▐▀▀▄  ▄█▀▄ ██ ▄▄▐▀▀▪▄▄▀▀▀█▄▄▀▀▀█▄    ▐█ ▌▐▌▐█· ▄█▀▄ ▐█▐▐▌▐█· ▐█.▪ ▄█▀▄ ▐▀▀▄ 
▐█▪·•▐█•█▌▐█▌.▐▌▐███▌▐█▄▄▌▐█▄▪▐█▐█▄▪▐█    ██ ██▌▐█▌▐█▌.▐▌██▐█▌▐█▌ ▐█▌·▐█▌.▐▌▐█•█▌
.▀   .▀  ▀ ▀█▄▀▪·▀▀▀  ▀▀▀  ▀▀▀▀  ▀▀▀▀     ▀▀  █▪▀▀▀ ▀█▄▀▪▀▀ █▪▀▀▀ ▀▀▀  ▀█▄▀▪.▀  ▀
            
            Process Monitor for Host : {ip}
"""

help = Style.BRIGHT + r"""

HELP
----------------------------------
-> Session Core Commands :
----------------------------------
-. show options - View Maalik Attack Options.
-. show targets - View Scanned Targets.
-. set target < target > - Set a Target to Attack.*
-. set attackport < port > - Set Target Port to Attack.*
-. set exploitport < port > - Set Port to send Exploits on (Default is random).*
-. show info - Show information on the Selected Target.
-. clear_hosts - Clear All Hosts.
-. clear_ports - Clear all Ports.
-. run - Start Pivoting Tunnel.
-. run autoblue - Automatically runs Eternal Blue on the set Target.

----------------------------------
-> Session Commands :
----------------------------------
-. help - Print this help message.
-. ls - List files in current directory.
-. cd < dir > - Go in another directory.
-. osinfo - systeminfo output.
-. delete - delete file.
-. process_monitor - Monitor a process.
-. shell - Reverse shell.
-. execute - Execute a file.
-. execargs - Execute a file with command line arguments.
-. clientinfo - View basic information of FHDAWN.
-. port_scan - Scan for open ports on a Host.
-. tasklist - View Running Processes.
-. taskkill - Kill Running Process.
-. host_sweep - Get all hostnames of scanned targets or specific IP (use -h to specify ip).
-. upload - Upload file.
-. download - Download file.
-. dllinject - Reflective DLL Injection. Load your own DLL.
-. network_scan - Scan the network.
-. (UAC)* windefender_exclude - Add Windows Defender Exclusions.
    - To Upload Malware, add Exclusion and upload malware to that directory.
-. screenshot - Take Screenshot.

----------------------------------
POST Exploitation
----------------------------------
-. {cmd} netuser - List users.
-. {cmd} systeminfo - View full System Information.
-. {cmd} driverquery - View all Drivers.
-. {cmd} tasklist - Get list of running processes.
-. {cmd} drives - Get Available Drive Letters.
-. {cmd} set - Get all envoironment variables. 
-. {cmd} qwinsta - Displays information about sessions on a Remote Desktop Session Host server.
-. {cmd} netshall - Acronym for 'netsh wlan show profiles'.
-. {cmd} rdp_enable - Enable Remote Desktop.
-. {cmd} rdp_disable - Disable Remote Desktop.
-. {cmd} firewall_off - Disable Firewall.
-. {cmd} firewall_on - Enable firewall.
-. {cmd} portfwd - Forward a PORT on the Remote PC.
-. {cmd} portfwd_reset - Reset all forwarded Ports.
-. {dps} rshell - Netcat Reverse Shell.
-. {dps} bshell - (UAC)* Netcat Bind Shell.
-. {dps} dropmsf - In Memory Meterpreter.
-. {dps} elevate - Administrator Prompt Trigger.
-. {dps} chromedump - Dump Google Chrome Passwords.
""". format(
    cmd = Style.BRIGHT + Fore.RED + "(CMD)" + Style.RESET_ALL,
    dps = Style.BRIGHT + Fore.GREEN + "(DPS)" + Style.RESET_ALL
)

def clear_screen():
    if(os.name == "nt"):
        os.system("cls")
    else:
        os.system("clear")


def uniquify(path):
    """
    Credits : https://stackoverflow.com/questions/13852700/create-file-but-if-name-exists-add-number/57896232#57896232
    """
    filename, extension = os.path.splitext(path)
    counter = 1

    while os.path.exists(path):
        path = filename + " (" + str(counter) + ")" + extension
        counter += 1

    return path

def xor(data, key):
    output = []
    for i in range(len(data)):
        xor_num = ord(data[i]) ^ ord(key[i % len(key)])
        output.append(chr(xor_num))
    return ''.join(output)


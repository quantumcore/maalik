<p align = "center">
  <img src="https://github.com/quantumcored/maalik/raw/master/maaliklogo.png" height = "290" width = "250">
  </p>

---

# Maalik
Maalik is a Network Pivoting and Post Exploitation Framework.

Post-exploitation refers to any actions taken after a Computer is compromised.
So Maalik assumes that you've already taken over a computer. After which, Maalik can be used to Pivot deeper into the network (Exploit other computers on the same Network). **Keep in mind that Maalik is not made for malicious or blackhat purposes but can be used for so. So You are responsible for anything that you do with it**. 

## Features
---

Feature | Description
--------|-------------
Stealth | Runs in background, Only writes hidden plaintext file to disk.
Execute / list / delete files and Browse | Full access to all files. Browse the system remotley.
Windows Defender Exclusions | Add Windows Defender Exclusions.
Network Pivoting | Forwards a Port to another Host on the network to forward exploit traffic onto it.
Enable / Disable Firewall | Enable or Disable Windows Firewall, Use full for pivoting scenarios. Firewall is automatically turned off during Pivot attack.
Network Scanner | Discover Hosts in the subnet.
Port Scanner | Scans discovered hosts for [common ports](https://github.com/quantumcored/maalik/blob/master/common_ports).
Automatic Eternal Blue | Automatically runs metasploit using rc file to potentially exploit Port 445.
Reverse Shell | Stable Reverse Shell, Commands executed as ``cmd.exe /c <your input>``.
File upload / download | Upload or Download Files.
Reflective DLL Injection | Reflective DLL Injection into any process.
Screenshot | Take screenshot (``bmp=>png``).
GeoLocation | Geolocate Fhdawn.
SAM Dump | Dumps SAM and SYSTEM files to disk, Downloads and dumps them using ``samdump2``.


##### Dynamic Payload System (DPS). 
Executes 'Payloads' in Memory using Reflective DLL Injection.
The Payload is a 32 bit Reflective DLL, That carries out tasks after successful Injection.
DLL output is written to a TEXT file named ``output.png`` which is used to smuggle output back to server, And also give the DLL Payload commands.

Payload | Description
--------|------------
(DPS) Reverse Shell | Netcat Reverse shell.
(DPS) Administrator Prompt Trigger | Forcefully attempt to Execute Fhdawn as Administrator.
(DPS) Chrome Password Recovery | Dumps Saved Google Chrome passwords. (Does not work on latest version) :shipit:
(DPS) In Memory Meterpreter | Execute Metasploit C Shellcode.
(DPS) Keystroke logging | Log keystrokes.
(DPS) Capture Mic Input | Record Mic.

##### CMD Post Exploitation Commands (CMD)
These are cmd commands that are useful in a post exploit situation. Not listed here.

---

#### Usage :
- netsec link
- netsec link2

This Project is active in developement. There may be Errors and bugs that I may have missed, Or an Idea or suggestion. Please submit [here](https://github.com/quantumcored/maalik/issues).

#### TODO 
- Add Route pivoting.
- Add Reflective Exploits.

#### Developer
Hi my name's [Fahad](https://github.com/quantumcore).
You may contact me, on [Discord](https://discordapp.com/invite/8snh7nx) or [My Website](https://quantumcored.com/)

#### LICENSE
[VIEW LICENSE](https://github.com/quantumcored/maalik/blob/master/LICENSE) 

The Developer is not responsible for any misuse of Damage caused by the program. This is created only to innovate NetSec and **YOU**. :point_left:

#### Donate
Help me with my future projects. Thank you.
[Donate with Crypto](https://commerce.coinbase.com/checkout/cebcb394-f73e-4990-98b9-b3fdd852358f)

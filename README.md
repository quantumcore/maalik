<html>
  <p align="center">
  <img src="https://github.com/quantumcored/maalik/raw/master/maalik.png">
  </p>
</html>

---

# Maalik
Maalik is a Network Pivoting and Post Exploitation Framework.

Post-exploitation refers to any actions taken after a Computer is compromised.
So Maalik assumes that you've already taken over a computer. After which, Maalik can be used to Pivot deeper into the network (Exploit other computers on the same Network). **Keep in mind that Maalik is not made for malicious or blackhat purposes but can be used for so. So You are responsible for anything that you do with it**. 

<html>
  <p align="center">
  <img src="https://github.com/quantumcored/maalik/raw/master/maalikp.png">
  </p>
</html>


## Features
---
#### V.1
- Stealth.
- Trigger UAC Prompt to run as Administrator.
- Full Administrator Stable Reverse Shell.
- Execute / list / delete files.
- Add Windows Defender Exclusions.
- Upload any Malware without getting it Blocked.
- Process Monitor.
- Directory Monitor.
- Post Exploitation Attack Methods. 
- Enable / Disable Firewall.
- Enable / Disable Remote Desktop.
- Network Scanner - Remote End.
- Port Scanner - Remote End.
- Automatic Eternal Blue Pivot.
- Network Pivoting (made Easy for you).

---

Built especially for Windows 7 and above. 


### Screenshots
![network_scanner](https://blog.quantumcored.com/wp-content/uploads/2020/08/mlk2.png)

![showinfo](https://blog.quantumcored.com/wp-content/uploads/2020/08/mlk6.png)

![shell](https://blog.quantumcored.com/wp-content/uploads/2020/08/mlk1.png)

![attack](https://blog.quantumcored.com/wp-content/uploads/2020/08/mlk5.png)

---

### Getting Started
- [(Blog post) Introduction to Maalik](https://blog.quantumcored.com/?p=64)
- [(Blog post) Installation and Usage](https://blog.quantumcored.com/?p=77)
- [(Blog post) Pivoting without any Tools](https://blog.quantumcored.com/?p=52)

##### Requirements
- Linux Based PC.
- Python 3.

```
$ git clone https://github.com/quantumcored/maalik.git
$ cd maalik
$ sudo chmod +x install.sh
$ sudo ./install.sh
```
**Run Maalik**.
```
$ sudo python3 main.py
```
**Generate Maalik Client (Fhdawn)**.
```
$ cd fhdawn
$ make linux ; make build-linux
```
(Remember to use a Strong Key when asked for during build)

---

### Demonstration (old version)
[![Demo](https://i.imgur.com/110jcqH.png)](https://youtu.be/4y1lYAkQSF4)


This Project is active in developement. There may be Errors and bugs that I may have missed, Or an Idea or suggestion. Please submit [here](https://github.com/quantumcored/maalik/issues).

#### Developer
Hi my name's [Fahad](https://github.com/quantumcore).
You may contact me, on [Discord](https://discordapp.com/invite/8snh7nx) or [My Website](https://quantumcored.com/)

#### LICENSE
[VIEW LICENSE](https://github.com/quantumcored/maalik/blob/master/LICENSE) 

The Developer is not responsible for any misuse of Damage caused by the program. This is created only to innovate NetSec and **YOU**. :point_left:

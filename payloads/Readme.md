## Payloads
These are 'Payloads' that are executed in Memory using Reflective DLL Injection.

**output.png** is the file used to get output of these payloads. The payloads write output in this Image.
Fhdawn reads the output and sends back to server.

- ncshell.c - Netcat Basic Reverse shell.
- ReflectiveDll.c - Standard Reflective DLL, shows messagebox.
- ChromePassDecryptor.c - Dumps Chrome passwords to 'password.txt'.
- elevate.c - Used to execute Fhdawn as adminstrator.

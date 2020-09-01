## Payloads
These are 'Payloads' that are executed in Memory using Reflective DLL Injection.

**output.png** is the image used to get output of these payloads. The payloads write output in this Image.
Fhdawn reads the output and sends back to server. Note, Only the output is read and sent, Not the entire image.

- ncshell.c - Netcat Basic Reverse shell.
- ReflectiveDll.c - Standard Reflective DLL, shows messagebox.

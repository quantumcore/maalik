clear
if [[ $EUID -ne 0 ]]; then
   echo "[^] Run as root." 
   exit 1
fi
apt-get install mingw-w64 
apt-get install python3-pip 
pip3 install colorama
sleep 1
echo "Done."
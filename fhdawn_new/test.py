import socket
import _thread
import sys

def _recv(sock):
    while(True):
        data = sock.recv(1024).decode()
        print("---------------\n\n" + data)

def main():

    def message(sock):
        while(True):
            data = input("--> ")
            if(len(data) > 0):
                client.send(data.encode())
            
            elif(data == "exit"):
                sys.exit()

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
    server.setsockopt(socket.SOL_TCP, socket.TCP_KEEPIDLE, 1)
    server.setsockopt(socket.SOL_TCP, socket.TCP_KEEPINTVL, 1)
    server.setsockopt(socket.SOL_TCP, socket.TCP_KEEPCNT, 5)

    host = "0.0.0.0"
    port = 421

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

    while(True):
        client, addr = server.accept()
        _thread.start_new_thread(_recv, (client,))
        message(client)

main()
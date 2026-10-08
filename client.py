import socket

STM32_IP = "192.168.1.10"
PORT = 7

print("Connecting to STM32 TCP server...")

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((STM32_IP, PORT))

    print("Connected!")

    data = s.recv(1024)

    print("Received from STM32:", data.decode(errors="replace"))
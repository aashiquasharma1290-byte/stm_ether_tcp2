
import socket

STM32_IP = "192.168.1.10"
PORT = 7

def main():
    print(f"Connecting to STM32 at {STM32_IP}:{PORT}...")

    try:
        with socket.create_connection((STM32_IP, PORT), timeout=5) as s:
            s.settimeout(1)
            print("STM32 connected with Ethernet!")

            # Receive the STM32 greeting
            try:
                greeting = s.recv(1024)
                if greeting:
                    print("STM32:", greeting.decode(errors="replace").strip())
            except socket.timeout:
                print("No greeting received yet.")

            print("\nType a message and press Enter.")
            print("Type 'exit' to disconnect.\n")

            while True:
                message = input("Aashiqua (PC) > ")

                if message.lower() == "exit":
                    break

                if not message:
                    continue

                s.sendall((message + "\r\n").encode())

                # Receive STM32 response
                try:
                    response = s.recv(1024)
                    if response:
                        print("STM32 >", response.decode(errors="replace").strip())
                    else:
                        print("STM32 disconnected.")
                        break
                except socket.timeout:
                    print("No response received from STM32.")

    except OSError as e:
        print("Connection failed:", e)
        print("Check the STM32 IP, Ethernet cable, and TCP server.")

if __name__ == "__main__":
    main()

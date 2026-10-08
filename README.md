# STM32H747 Ethernet TCP Server

## Configuration

- **MCU:** STM32H747I-DISCO
- **PHY:** LAN8742
- **Interface:** RMII
- **Protocol Stack:** lwIP
- **Network:** Static IP, DHCP disabled
- **STM32 IP:** 192.168.1.10
- **TCP Server Port:** 7
- **PC:** Python TCP Client

## Project Files

- **`main.c`** — Initializes the system, peripherals, lwIP and TCP server, and continuously processes Ethernet packets.
- **`lwip.c`** — Configures and initializes the lwIP network interface and static IP address.
- **`ethernetif.c`** — Handles the Ethernet interface, RMII communication, LAN8742 PHY and DMA.
- **`tcp_echoserver.h`** — TCP server initialization declaration.
- **`tcp_echoserver.c`** — Implements the TCP server: bind, listen, accept and data transmission.
- **`STM32H747XIXX_FLASH.ld`** — Defines memory sections for Ethernet DMA descriptors and RX buffer pool in RAM_D2.
- **`PC_Client/client.py`** — Python TCP client that connects to the STM32 server and receives the transmitted data.

## TCP Communication Flow

```text
STM32 Static IP
192.168.1.10
      ↓
Create TCP Server
      ↓
Bind → Port 7
      ↓
Listen
      ↓
Accept PC Connection
      ↓
Send "AASHIQUA"
      ↓
Python TCP Client
      ↓
Receive "AASHIQUA"

**Expected Result**
Connecting to STM32 TCP server...
Connected!
Received from STM32: AASHIQUA

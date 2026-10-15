## Objective

To implement Ethernet-based TCP communication using the STM32H747I-DISCO as a TCP server and a PC as a Python TCP client.# STM32H747 Ethernet TCP Server

## Configuration

- **MCU:** STM32H747I-DISCO
- **PHY:** LAN8742
- **Interface:** RMII
- **Protocol Stack:** lwIP
- **Network:** Static IP, DHCP disabled
- **STM32 IP:** 192.168.1.10
- **TCP Server Port:** 7
- **PC:** Python TCP Client(IP,192.168.001.20)

## Project Files

- **`main.c`** — Initializes the system, peripherals, lwIP and TCP server, and continuously processes Ethernet packets.
- **`lwip.c`** — Configures and initializes the lwIP network interface and static IP address.
- **`ethernetif.c`** — Handles the Ethernet interface, RMII communication, LAN8742 PHY and DMA.
- **`tcp_echoserver.h`** — TCP server initialization declaration.
- **`tcp_echoserver.c`** — Implements the TCP server: bind, listen, accept and data transmission.
- **`STM32H747XIX_FLASH.ld`** — Defines memory sections for Ethernet DMA descriptors and RX buffer pool in RAM_D2.
- **`client.py`** — Python TCP client that connects to the STM32 server and receives the transmitted data.

## TCP Communication Flow
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
Python TCP Client (PC)
      ↓
Receive "AASHIQUA"
      ↓
Send Message from PC
      ↓
STM32 Processes the Message
      ↓
Send Reply to PC


**Expected Result**
Connecting to STM32 at 192.168.1.10:7...
STM32 connected with Ethernet!
STM32: AASHIQUA

Type a message and press Enter.
Type 'exit' to disconnect.

Aashiqua (PC) > Hello STM32
STM32 > Hello PC

Aashiqua (PC) > How are you?
STM32 > I am fine

Aashiqua (PC) > Test message
STM32 > Test message


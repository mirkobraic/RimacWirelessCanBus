# Wi-Fi CAN Diagnostics App

## Overview

This is a cross-platform diagnostics app that connects wirelessly to a vehicle's CAN network using a Wi-Fi to CAN module. It enables basic vehicle diagnostics, UDS communication, and raw CAN message monitoring—all from a desktop or mobile device.

## Features

- **Wireless Vehicle Diagnostics**
  - Compatible with the Kvaser BlackBird v2 Wi-Fi CAN module.
  - Easily extensible to work with any other Wi-Fi CAN module.
  - Connects via mobile hotspot or local Wi-Fi.

- **Cross-Platform Support**
  - Built with Qt Quick (QML + C++).
  - Runs on Windows, macOS, Linux, Android, and iOS.

- **Simple and Modular UI**
  - **Connection Screen:** Enter CAN provider, baud rate, RX-TX IDs, IP address, and port to establish connection.
  - **Home Screen:** Main navigation with quick access to UDS client, raw CAN viewer, server info, and disconnect. 
  - **Raw CAN Screen:** View received messages in hexadecimal format along with message IDs and timestamps.
  - **UDS Client Screen:** View used for reading and clearing DTC messages.

<img height="350" alt="Screenshot 2025-08-15 at 15 25 37" src="https://github.com/user-attachments/assets/a1515c3e-5541-43a8-8ebe-2ae84765f157" alt="Connection Screen"/>
<img height="350" alt="Screenshot 2025-08-15 at 15 26 00" src="https://github.com/user-attachments/assets/d603891f-0e1a-479d-8aeb-9a50c02b3610" alt="Main Menu UI"/>
<img height="350" alt="Screenshot 2025-08-15 at 15 26 30" src="https://github.com/user-attachments/assets/f49bbf50-4892-44ee-82cd-47abc743963b" alt="Raw CAN View"/>
<img height="350" alt="Screenshot 2025-08-15 at 15 30 45" src="https://github.com/user-attachments/assets/0d0ffeeb-9a52-4f99-a540-ad3d4d4ffe34" alt="UDS Client Screen"/>

- **UDS Client Integration**
  - Uses `RimacUdsClient` C++ library.
  - Supports reading/clearing DTCs and querying ECU server info.

- **Modular Architecture**
  - Uses abstract `CanBusInterface` to support different Wi-Fi CAN modules with minimal changes.

## Use Cases

- Mobile diagnostics during track testing
- Lightweight fault-code inspection
- In-field ECU communication without heavy equipment

## Getting Started

1. Power your CAN Wi-Fi device (e.g. Kvaser BlackBird v2).
2. Connect your device and the app to the same Wi-Fi (e.g. mobile hotspot).
3. Fill in connection details and press "Connect".
4. Use the app to read DTCs, send CAN messages, or view live data.

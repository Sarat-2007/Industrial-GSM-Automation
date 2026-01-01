# Industrial GSM Automation (A7670C + Arduino) 📡

![Status](https://img.shields.io/badge/Status-Operational-success)
![Hardware](https://img.shields.io/badge/Hardware-A7670C_GSM-red)
![Range](https://img.shields.io/badge/Range-Global_(Cellular)-blue)

## 💡 Overview
This project is an Industrial Automation System that allows users to control 4 electrical appliances from anywhere in the world using SMS commands. It uses the **A7670C GSM Module** for 4G/LTE connectivity, ensuring reliability even where Wi-Fi is unavailable.

## 🛠️ Hardware Stack
* **Microcontroller:** Arduino Uno
* **Connectivity:** A7670C GSM Module (4G LTE)
* **Actuation:** 4-Channel Relay Module (5V)
* **Power:** 12V/2A Adapter (Buck converter to 5V/4V for GSM)

## ⚙️ How It Works
1.  **Security:** The system verifies the sender's phone number against a hardcoded "Owner Number." Unknown numbers are ignored.
2.  **Parsing:** It listens for specific codes (e.g., `s1on`) in the SMS body.
3.  **Action:** It triggers the corresponding relay to switch the appliance.
4.  **Feedback:** It sends an SMS back to the owner confirming the status (e.g., "Appliance 1 is ON").

## 🎮 SMS Command List
Send these exact codes to the GSM SIM number:

| Device | Turn ON | Turn OFF |
|--------|---------|----------|
| **Device 1** | `s1on` | `s1off` |
| **Device 2** | `s2on` | `s2off` |
| **Device 3** | `s3on` | `s3off` |
| **Device 4** | `s4on` | `s4off` |
| **System** | `status`| (Check Health) |

## 🚀 Mobile App (Coming Soon)
A custom Android APK is being developed to replace manual typing with a simple GUI.

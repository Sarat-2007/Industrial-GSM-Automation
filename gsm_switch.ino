/*
 * Project: GSM Based Smart Switch (A7670C)
 * Author: Sarat Chandra
 * Hardware: Arduino Uno, A7670C GSM, 4-Channel Relay
 */

#include <SoftwareSerial.h>

// --- Configuration ---
// A7670C TX connects to Arduino Pin 10
// A7670C RX connects to Arduino Pin 11
const int gsmTx = 10; 
const int gsmRx = 11;

// Relay Pins (Active LOW for most 4-channel modules)
const int RELAY_1 = 2;
const int RELAY_2 = 3;
const int RELAY_3 = 4;
const int RELAY_4 = 5;

// SECURITY: Allow commands ONLY from this number
// Format: +91 followed by your 10 digit number
const String OWNER_NUMBER = "+91**********"; 

// Initialize Software Serial
SoftwareSerial gsm(gsmTx, gsmRx);

void setup() {
    // 1. Debug Serial (PC)
    Serial.begin(9600);
    
    // 2. GSM Serial (A7670C)
    // Note: If A7670C default is 115200, you might need to change this or configure module first.
    gsm.begin(9600); 

    // 3. Initialize Relays (Start OFF)
    // Most relays are Active LOW, so HIGH = OFF
    pinMode(RELAY_1, OUTPUT); digitalWrite(RELAY_1, HIGH);
    pinMode(RELAY_2, OUTPUT); digitalWrite(RELAY_2, HIGH);
    pinMode(RELAY_3, OUTPUT); digitalWrite(RELAY_3, HIGH);
    pinMode(RELAY_4, OUTPUT); digitalWrite(RELAY_4, HIGH);

    Serial.println("System Initializing...");
    delay(1000);
    
    // 4. Config GSM Module
    initGSM();
}

void loop() {
    // Listen for incoming data
    if (gsm.available()) {
        String data = gsm.readString();
        Serial.println("Received: " + data); // Print to Serial Monitor for debugging

        // Check for incoming SMS tag
        if (data.indexOf("+CMT:") > 0) {
            
            // Security Check: Does the SMS contain the owner's number?
            if (data.indexOf(OWNER_NUMBER) > 0) {
                Serial.println("Authorized Number Detected.");
                executeCommand(data);
            } else {
                Serial.println("WARNING: Unauthorized SMS blocked.");
            }
        }
    }
}

// --- Helper Functions ---

void initGSM() {
    gsm.println("AT"); // Check connection
    delay(1000);
    gsm.println("AT+CMGF=1"); // Set Text Mode
    delay(1000);
    // Route SMS to serial immediately
    gsm.println("AT+CNMI=2,2,0,0,0"); 
    delay(1000);
    Serial.println("GSM Ready. Waiting for SMS...");
}

void executeCommand(String msg) {
    msg.toLowerCase(); // Convert to lowercase to handle S1ON, s1on, S1on etc.

    // --- Relay 1 ---
    if (msg.indexOf("s1on") > 0) {
        digitalWrite(RELAY_1, LOW); // ON
        Serial.println("Relay 1 turned ON");
        sendSMS("Appliance 1 is ON");
    }
    else if (msg.indexOf("s1off") > 0) {
        digitalWrite(RELAY_1, HIGH); // OFF
        Serial.println("Relay 1 turned OFF");
        sendSMS("Appliance 1 is OFF");
    }

    // --- Relay 2 ---
    else if (msg.indexOf("s2on") > 0) {
        digitalWrite(RELAY_2, LOW);
        Serial.println("Relay 2 turned ON");
        sendSMS("Appliance 2 is ON");
    }
    else if (msg.indexOf("s2off") > 0) {
        digitalWrite(RELAY_2, HIGH);
        sendSMS("Appliance 2 is OFF");
    }

    // --- Relay 3 ---
    else if (msg.indexOf("s3on") > 0) {
        digitalWrite(RELAY_3, LOW);
        sendSMS("Appliance 3 is ON");
    }
    else if (msg.indexOf("s3off") > 0) {
        digitalWrite(RELAY_3, HIGH);
        sendSMS("Appliance 3 is OFF");
    }

    // --- Relay 4 ---
    else if (msg.indexOf("s4on") > 0) {
        digitalWrite(RELAY_4, LOW);
        sendSMS("Appliance 4 is ON");
    }
    else if (msg.indexOf("s4off") > 0) {
        digitalWrite(RELAY_4, HIGH);
        sendSMS("Appliance 4 is OFF");
    }
    
    // --- Status Check ---
    else if (msg.indexOf("status") > 0) {
        sendSMS("System is Online. Waiting for commands.");
    }
}

void sendSMS(String text) {
    gsm.println("AT+CMGS=\"" + OWNER_NUMBER + "\""); 
    delay(500);
    gsm.print(text);
    delay(500);
    gsm.write(26); // CTRL+Z ascii code
    delay(2000);
    Serial.println("Feedback SMS Sent.");
}

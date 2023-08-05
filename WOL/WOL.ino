#define BLYNK_TEMPLATE_ID "YOUR TEMPLATE ID" //replace with your blynk template ID
#define BLYNK_TEMPLATE_NAME "YOUR TEMPLATE NAME" // replace with the name of your blynk program
#define BLYNK_AUTH_TOKEN "YOUR AUTH TOKEN" // replace with your blynk auth token
#define W5100_CS 10
#define SDCARD_CS 4
#define BLYNK_PRINT Serial

#include <BlynkSimpleEthernet.h>
#include <EthernetUdp.h>
#include <Ethernet.h>
#include <avr/wdt.h>

//MAC Addresses
byte arduinoMAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // MAC address of your arduino
byte pcMAC[] = {0x39, 0x19, 0x59, 0x46, 0x02, 0x45};      // MAC address of your target pc

//IP Addresses
IPAddress arduinoIP(11, 1, 1, 111); // IP address of your arduino
IPAddress pcIP(0, 0, 0, 000);      // IP address of your target pc

//constants
const int BLYNK_TERMINAL_PIN = V7;
const int MAX_PACKET_BUFFER_SIZE = 255;
const unsigned int SOURCE_PORT = 49152;      // can be any number you'd like. Should be between 49152 to 65535. Must match Destination port in python code
const unsigned int DESTINATION_PORT = 49152; // can be any number you'd like. Should be between 49152 to 65535. Must match Source port in python code
const unsigned long WAIT_TIME_MS = 2000;

// strings used to help handle incoming packets - these will also show up in the python code
char expectedIdentifier[] = "SOME_PC_IDENTIFIER"; 
char loginSecret[] = "SuperSecretLoginSecret"; 
char WOLOnSecret[] = "SuperSecretWOLOnSecret";
char ShuttingDownPCSecret[] = "SuperSecretShuttingDownPCSecret";
char CheckPCPowerStateSecret[] = "SuperSecretCheckPCPowerStateSecret";

enum State {
  IDLE,
  CHECKING_POWER_STATE,
  CHECKING_SHUTTING_DOWN
};

State currentState = IDLE;

unsigned long lastStateChangeTime = 0;

EthernetUDP Udp;

void setup()
{
  // Debug console
  Serial.begin(9600);
  Serial.println("Program Start");

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  // Connect to Blynk server
  Blynk.begin(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);
  Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "clr");
  Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Connected to Blynk Cloud!\n");

  Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Starting Ethernet and UDP\n");
  Ethernet.begin(arduinoMAC, arduinoIP);
  Udp.begin(SOURCE_PORT);
  Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Connected to Ethernet and UDP!\n");
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    processPacket(packetSize);
  }
  handleState();
  Blynk.run();
}

//WOL / Power Button
BLYNK_WRITE(V0)
{
  int value = param.asInt();

  if (value == HIGH)
  {
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "----------\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Sending WOL Packet\n");
    sendWakeOnLANPacket();
    sendPacket(WOLOnSecret, sizeof(WOLOnSecret));
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "WOL Packet Sent!\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "It usually takes about 60 sec for pc to power on\n");
  }
}

// Clear Terminal
BLYNK_WRITE(V1)
{
  int value = param.asInt();
  if (value == HIGH)
  {
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Clearing Terminal\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "clr");
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Terminal Successfully cleared!\n");
  }
}

// Shutdown 
BLYNK_WRITE(V2)
{
  int value = param.asInt();
  if (value == HIGH)
  {
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Sending Shutdown Packet\n");
    sendPacket(ShuttingDownPCSecret, sizeof(ShuttingDownPCSecret));
    setState(CHECKING_SHUTTING_DOWN);
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Waiting for a response...\n");
  }
}

// Check Power State
BLYNK_WRITE(V3)
{
  int value = param.asInt();
  if (value == HIGH)
  {
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Checking Power State\n");
    sendPacket(CheckPCPowerStateSecret, sizeof(CheckPCPowerStateSecret));
    setState(CHECKING_POWER_STATE);
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Waiting for a response...\n");
  }
}

// Reset Arduino
BLYNK_WRITE(V4)
{
  int value = param.asInt();
  if (value == HIGH)
  {
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "Resetting Arduino\n");
    wdt_enable(WDTO_15MS);
    while (1);
  }
}

// Send WOL Packet
void sendWakeOnLANPacket()
{
  byte magicPacket[102];
  for (int i = 0; i < 6; i++)
  {
    magicPacket[i] = 0xFF;
  }

  for (int j = 0; j < 16; j++)
  {
    for (int k = 0; k < 6; k++)
    {
      magicPacket[6 + j * 6 + k] = pcMAC[k];
    }
  }
  Udp.beginPacket(pcIP, 9); //WOL packets are usually sent on port 9
  Udp.write(magicPacket, sizeof(magicPacket));
  Udp.endPacket();
}

// Send any other packet
void sendPacket(char* contents, size_t size) {
  Udp.beginPacket(pcIP, DESTINATION_PORT);
  Udp.write(contents, size);
  Udp.endPacket();
}

// determine if the packet is from the correct pc
void processPacket(int packetSize) {
  char packetBuffer[MAX_PACKET_BUFFER_SIZE];
  int packetLen = Udp.read(packetBuffer, sizeof(packetBuffer));
  if (packetLen > 0) {
    packetBuffer[packetLen] = '\0';
    if (strstr(packetBuffer, expectedIdentifier) != NULL) {
      handleIncomingPacket(packetBuffer);
    }
  }
}

// read the packet and determine what to do
void handleIncomingPacket(char* packetBuffer) {
  if (strstr(packetBuffer, loginSecret) != NULL) {
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "PC is now powered on and logged in!\n");
  } else if (strstr(packetBuffer, WOLOnSecret) != NULL) {
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "PC is already powered on!\n");
  } else if (strstr(packetBuffer, ShuttingDownPCSecret) != NULL) {
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "PC is now shutting down!\n");
    currentState = IDLE;
  } else if (strstr(packetBuffer, CheckPCPowerStateSecret) != NULL) {
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "PC is powered on!\n");
    currentState = IDLE;
  }
  else {
    Blynk.virtualWrite(BLYNK_TERMINAL_PIN, packetBuffer);
    currentState = IDLE;
  }
}

// set the state and reset the last time the state was changed
void setState(State newState) {
  currentState = newState;
  lastStateChangeTime = millis();
}

// wait for packet to respond
void handleState() {
  switch (currentState) {
    case IDLE:
      break;
    case CHECKING_POWER_STATE:
      if (millis() - lastStateChangeTime > WAIT_TIME_MS) {
        Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "PC is powered off!\n");
        currentState = IDLE;
      }
      break;
    case CHECKING_SHUTTING_DOWN:
      if (millis() - lastStateChangeTime > WAIT_TIME_MS) {
        Blynk.virtualWrite(BLYNK_TERMINAL_PIN, "PC is already powered off or not connected to the internet!\n");
        currentState = IDLE;
      }
      break;
  }
}
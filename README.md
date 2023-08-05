# Required materials:
PC with windows 10/11 (earlier versions might work I haven't tested them)  
An Arduino MEGA 2560 (I belive an Arduino Uno would work as well - also not tested)  
An Ethernet Shield for the arduino  
A way to power the arduino   
An Ethernet cord for the arduino  
An Ethernet cord for your pc  
Python  
the Arduino IDE  
# Download the files
click the green code button on the top right of this page  
click download zip  
in file explorer navigate to the zip file and click extract all  
save the python file wherever you want  
save the arduino folder in the folder with all the other arduino folders (I believe the default path is "D:\Documents\Arduino").   
# Configure your PC for WOL
## Configure your BIOS
  These steps might differ depending on your BIOS  
  Press F2 during boot to enter BIOS Setup. Go to the Power menu  
  Set Wake-on-LAN to Power On. Press F10 to save and exit the BIOS Setup  
  The option could also be named Power On By PCI Devices or similar  
    
  You can always google how to enable WOL on the **[YOUR MOTHERBOARD HERE]** 
  
## Configure your Ethernet Controller/Network card in Windows
  Open Device Manager > Network Adapters > Right click your Network card > Properties > Advanced > Look for the following (usually found at the bottom of the alphabetically sorted list):  
  Wake on Magic Packet -> ensure value is enabled  
  Wake on pattern Match -> ensure value is enabled  
  WOL & Shutdown Link Speed **(you may not have this setting - that's okay)** -> 10 Mbps First  
  Then click Power Managment > Allow this device to wake the computer

## Configure Windows power mangament settings
  Press the windows key and search `choose a power plan` > Click High performance    
  Press the windows key and type cmd > click run as administrator  
  type `powercfg -h off` to disable fast startup  
&nbsp;&nbsp;&nbsp;&nbsp;*I found this setting resets when I unplug my pc from the wall (while it's shutdown of course). Not sure if this is a universal quality but it is something to note. I spent HOURS trouble shooting my WOL setup on for this to be the issue  
# Configure windows to run the python file everytime you login
## Firewall settings  
press the windows key and type firewall to open the firewall settings page  
click `Allow an app through firewall` 
click `Change settings` in the top right of the new window  
scroll down until you see Python and ensure that for all the apps named python you have both the private and public checkboxes ticked  
then click `ok`  
## Task Scheduler  
press the windows key again and type `task scheduler`  
click `create basic task` on the right  
give it a name (i'll call mine `ArduinoWOLProgram`) and and optional description  
click next and select when you want the program to run (`When I log on`)  
click next and select Start a program
click next and then click browse. Navigate to your python folder (Mine was located at `"C:\Python311\"`) and in that folder find the program with the name `pythonw.exe` not `python.exe` but `pythonw.exe` this pythonw.exe will run python programs without opening up a command prompt window. which is ideal for this use case as the program will run in the background without affecting the user experience of the OS. (the file consumes minimal resources and shouldn't make any noticeable change to the performace of your computer)  
click `pythonw.exe` and click open
then open file explorer and find the folder where you saved the python file. I saved mine to my desktop. Right click `WOL.py` > show more options > Copy as path  
go back to the Task Scheduler Window and in the Add argumetns (optional): text field paste the path to `WOL.py`  
click next and then click finish  
Find it in the list of scheduled tasks and double click it to open it's properties  
at the bottom of the first tab select run with highest privlages and then click okay  
**The python program is now configured to run everytime you log in**

# Disable password on your computer
if you're going to be using your computer as a remote desktop and won't have a way to control your pc until you're logged in you can disable the password and the login screen so your pc boots directly to your desktop  
press the windows key and type `change your password`  
click password > change and enter your password  
click next > next and then click finish
# I belive you also will have to disable the login screen. I'm not sure the steps on how to do that. So I recommend you google it for your system. 
# Setup Blynk to communicate with the Arduino
go to `https://blynk.cloud/dashboard/register` and enter your email.  
Click I accept the terms and conditions and then click sign up.  
Open your email and find the email from BLYNK. Click Create Password.  
enter a password and click next  
enter your name and click done.  
click skip to skip the blynk tour and click cancel.  
click the square on the left navigation bar to the templates page.  
click new template and give it a name Ill use the name `WOL App`  
select your hardware type `arduino` and how it's connected `ethernet`  
Then click done. 
Click the datastreams tab and in the top right click New Datastream. 
Select Virtual Pin and keep all the default settings (Name: `Integer V0`, Alias: `Integer V0`, Pin: `V0`, Data type: `Integer`, Units: `none`, Min: `0`, Max: `0`, Default Value: `0`) and click create
repeat 5 more times until you have 5 Integer Data Streams (`V0, V1, V2, V3, V4, V5`) 
click new Datastream one more time and select Virtual Pin. except this time select Data Type: String and keep the default values of everything else (Name: `String V5`, Alias: `String V5`, PIN: `V5`)  
click add device > give it a name (this will be what shows up in the App on your phone) and then click `copy to clipboard` to copy the Configuration for the program. 
Go to your phone and download the Blynk app from the appstore. Right now it's the one with the Ukrain flag in the logo.  
Open the app and click Log in. Enter your Email and Password and click login.  
click the x in the top right to exit the tutorial and click the x again.  
click Go to Developer Zone and you should see the template we just created. (mine says `WOLa`)
click it and click the x. Click the + in the top right and select Button. Repeat this step 5 times so you have 5 buttons on the page.  
click the + again and this time scroll down until you see terminal and select that.  
click the first button to open the button editing page.  
click the plus by the `Choose datastream` and select Integer V0  
click design in the bottom right corner and for the labels give it the label: (Off: `Power on`, On: `Power On`)  
click the x to save your work and go back to the edit page.  
repeat this process for the other 4 buttons but instead labeling them with and having a unique interger datastream for each button.(`Clear Terminal`, `Shutdown`, `check power state`, `reset arduino`)  
to make your life easier make sure you label the buttons like the following:  
`Power On -> v0`  
`Clear Terminal -> V1`  
`Shutdown -> V2`  
`Check Power State -> V3`  
`Reset Arduino -> V4`  

whe you've edited all the buttons click the terminal window, select choose datastream and click `String V5` untoggle the input line and then click x. 
# Configure the Python code and the Arduino Code
now that blynk has been successfully setup we can go and add the authorization tokens to the WOL.ino file.  
open the arduino IDE 
## Arduino.INO
follow the steps from this website to install the blynk library `https://docs.blynk.io/en/blynk-library-firmware-api/installation/install-blynk-library-in-arduino-ide`  
open the WOL.ino file and replace the following with your BLYNK information:
```
#define BLYNK_TEMPLATE_ID "YOUR BLYNK TEMPLATE ID"
#define BLYNK_TEMPLATE_NAME "YOUR BLYNK NAME"
#define BLYNK_AUTH_TOKEN "YOUR BLYNK AUTH TOKEN"
```
## Finding your Arduino's IP/MAC address
  open the Arduino IDE if it's not open already  
  click file > new sketch  
  paste the following
```
#include <Ethernet.h>
#include <SPI.h>

// MAC address for the Ethernet shield (replace with your desired MAC address if needed)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  
  // Initialize Ethernet shield with the MAC address
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true); // Loop forever if DHCP configuration fails
  }
  
  Serial.println("Ethernet Shield Rev2 IP Address:");
  Serial.println(Ethernet.localIP());
  Serial.println();

  Serial.println("Ethernet Shield Rev2 MAC Address:");
  printMacAddress(mac);
}

void loop() {}

// Function to print the MAC address
void printMacAddress(byte mac[]) {
  for (int i = 0; i < 6; i++) {
    // Convert each byte to a hexadecimal string representation
    char buf[3];
    sprintf(buf, "%02X", mac[i]);
    Serial.print(buf);
    if (i < 5) {
      Serial.print(":");
    }
  }
  Serial.println();
}
```
then click upload  
Open your serial monitor by going tools > Serial Monitor and ensure the Baud Rate is 9600  
take note of the Arduino's IP address and MAC address  

## find your pc's IP address and MAC address  
press the windows key and type CMD to open the command promt  
type `ipconfig /all` and press enter  
find your ethernet adapter. It should say something like `Ethernet Adapter Ethernet:`  
scroll down until you see IPv4 Address. This is your internal IP address to use for this program  
also find Physical Address. This is your MAC address.  
take note of both of these.  

## finish configuring the code  
open up the python file and on the line that says `ARDUINO_IP = ` set that equal to your IP (for example: `ARDUINO_IP = 39.0.0.100`)  
and then on the line that says `PC_IP = ` do the same. For example: `PC_IP = 39.0.0.111`  
press file > save  

open up WOL.INO and find the line `byte arduinoMAC[] = {0xDE 0xAD 0xBE 0xEF 0xFE 0xED}` and replace the DEADBEEFFEED with your arduino MAC address found earlier.  
do the same for the line that says `byte pcMAC` but but your pc mac address (Physical address) that you found earlier.  
replace the line `IPAddress arduinoIP(39, 0, 0, 100 )` with your arduino's IP address found earlier  
finally replace the line that says `IPAddress pcIP(39, 0, 0, 111)` with your PC's IP address found earlier.  
press upload to upload the arduino's code to the arduino and you should be good to go!  
# You're Done!   
Well done on setting this up! If you have any questions feel free to message me on discord at `@_3va_` and I'll do my best to answer any questions you have!
  

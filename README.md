# What is this guide?
This is a comprehensive guide that will walk you through step by step how to configure your phone, along with an arduino and PC, to send Wake-On-LAN (WOL) magic packets to your pc from anywhere in the world!  
WOL allows you to remotely power on a computer over a network connection. This is useful if you'd like to remote desktop into your PC but don't want to leave it on 24/7. You may also want to use your PC as a server but again don't want your PC to be on 24/7. Let's get started!
# Required materials:
PC with Windows 10/11 (earlier versions might work however I haven't tested them)  
An Arduino MEGA 2560 (I belive an Arduino Uno would work as well however I also haven't tested with it)  
An Ethernet Shield for the Arduino  
A cord to power the Arduino   
An Ethernet cord for the arduino  
An Ethernet cord for your pc  
Python (I used v3.11.4)  
The Arduino IDE  
# Download the files
Click the green code button on the top right of this page  
Click download zip  
In File Explorer navigate to the zip file and click extract all  
Move the python file wherever you want that you can easily access (the desktop is a convient place)  
Move the Arduino folder to the folder with all the other Arduino folders (I believe the default path is "D:\Documents\Arduino")   
# Configure your PC for WOL
## Configure your BIOS
These steps might differ depending on your BIOS  
Turn off your PC  
Once it's fully powered off, power it back on and press F2 during boot to enter BIOS Setup  
Go to the Power menu  
Set Wake-on-LAN to Power On. Press F10 to save and exit the BIOS Setup  
The option could also be named Power On By PCI Devices, or something similar  

This step is always really annoying because it requires you to Turn off your PC and turn it back on  
You can always google how to enable WOL on the **[YOUR MOTHERBOARD HERE]**  
## Configure your Ethernet Controller/Network card in Windows
`Open Device Manager` > `Network Adapters` > `Right click your Network card` > `Properties` > `Advanced` > Look for the following:  
`Wake on Magic Packet` -> ensure value is set to `enabled`  
`Wake on Pattern Match` -> ensure value is `enabled`  
`WOL & Shutdown Link Speed` **(you may not have this setting - that's okay)** ensure it's value is set to `10 Mbps First`  
Then click `Power Managment` > `Allow this device to wake the computer`  

## Configure Windows power mangament settings
Press the windows key and search `choose a power plan` > Click `High performance`  
Press the windows key and type `cmd` > click `run as administrator`  
Type `powercfg -h off` to disable fast startup  
&nbsp;&nbsp;&nbsp;&nbsp;*I found this setting resets when I unplug my pc from the wall (while it's shutdown of course). Not sure if this is a universal quality but it is something to note. I spent HOURS trouble shooting my WOL setup on for this to be the issue  
# Configure windows to run the python file everytime you login
## Firewall settings  
Press the windows key and type firewall to open the firewall settings page  
Click `Allow an app through firewall`  
Click `Change settings` in the top right of the new window  
Scroll down until you see Python and ensure that for all the apps named python you have both the private and public checkboxes ticked  
Then click `ok`  
## Task Scheduler  
Press the windows key again and type `task scheduler`  
Click `create basic task` on the right  
Give it a name (I'll call mine `ArduinoWOLProgram`) and an optional description  
Click next and select `When the computer starts`  
Click next and select Start a program
Click next and then click browse. Navigate to your python folder (Mine was located at `"C:\Python311\"`) and in that folder find the program with the name `pythonw.exe` not `python.exe` but `pythonw.exe` this `pythonw.exe` will run python programs without opening up a command prompt window. This is ideal for this use case as the program will run in the background without affecting the user experience of the OS. (the program consumes minimal resources and shouldn't make any noticeable change to the performace of your computer)  
Click `pythonw.exe` and click open
Then open File Explorer and find the folder where you saved `WOL.py`. I saved mine to my desktop. Right click `WOL.py` > `show more options` > `Copy as path`  
Go back to the Task Scheduler Window and in the `Add argumetns (optional):` text field paste the path to `WOL.py`  
Click next and then click finish  
Find it in the list of scheduled tasks and double click it to open it's properties  
At the bottom of the first tab select run with highest privlages  
In that same menu, click run whether user is logged in or not and then click okay  
Enter your password for your user in the new window that opened up and click okay  
**The python program is now configured to run everytime the pc powers on!**
# Setup Blynk to communicate with the Arduino
Go to `https://blynk.cloud/dashboard/register` and enter your email  
Click I accept the terms and conditions and then click sign up  
Open your email and find the email from BLYNK. Click Create Password  
Enter a password and click next  
Enter your name and click done  
Click skip to skip the blynk tour and click cancel  
Click the square on the left navigation bar to the templates page  
Click new template and give it a name Ill use the name `WOL App`  
Select your hardware type `arduino` and how it's connected `ethernet`  
Then click done  
Click the datastreams tab and in the top right click New Datastream  
Select Virtual Pin and keep all the default settings (Name: `Integer V0`, Alias: `Integer V0`, Pin: `V0`, Data type: `Integer`, Units: `none`, Min: `0`, Max: `0`, Default Value: `0`) and click create  
Repeat 5 more times until you have 5 Integer Data Streams (`V0, V1, V2, V3, V4, V5`)  
Click new Datastream one more time and select Virtual Pin. except this time select Data Type: String and keep the default values of everything else (Name: `String V5`, Alias: `String V5`, PIN: `V5`)  
Click add device > give it a name (this will be what shows up in the App on your phone) and then click `copy to clipboard` to copy the Configuration for the program  
Go to your phone and download the Blynk app from the appstore. Right now it's the one with the Ukrain flag in the logo  
Open the app and click Log in. Enter your Email and Password and click login  
Click the x in the top right to exit the tutorial and click the x again  
Click Go to Developer Zone and you should see the template we just created. (mine says `WOLa`)
Click it and click the x. Click the + in the top right and select Button. Repeat this step 5 times so you have 5 buttons on the page.  
Click the + again and this time scroll down until you see terminal and select that.  
Click the first button to open the button editing page.  
Click the plus by the `Choose datastream` and select Integer V0  
Click design in the bottom right corner and for the labels give it the label: (Off: `Power on`, On: `Power On`)  
Click the x to save your work and go back to the edit page.  
Repeat this process for the other 4 buttons but instead labeling them with and having a unique interger datastream for each button.(`Clear Terminal`, `Shutdown`, `check power state`, `reset arduino`)  
To make your life easier make sure you label the buttons like the following:  
`Power On -> v0`  
`Clear Terminal -> V1`  
`Shutdown -> V2`  
`Check Power State -> V3`  
`Reset Arduino -> V4`  
  
When you've edited all the buttons click the terminal window, select choose datastream and click `String V5` untoggle the input line and then click x  
# Configure the Python code and the Arduino Code
Now that blynk has been successfully setup we can go and add the authorization tokens to the WOL.ino file  
Open the arduino IDE  
## Arduino.INO
Follow the steps from this website to install the blynk library `https://docs.blynk.io/en/blynk-library-firmware-api/installation/install-blynk-library-in-arduino-ide`  
Open the WOL.ino file and replace the following with your BLYNK information:  
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
Then click upload  
Open your serial monitor by going tools > Serial Monitor and ensure the Baud Rate is 9600  
Take note of the Arduino's IP address and MAC address  

## find your pc's IP address and MAC address  
Press the windows key and type CMD to open the command promt  
Type `ipconfig /all` and press enter  
Find your ethernet adapter. It should say something like `Ethernet Adapter Ethernet:`  
Scroll down until you see IPv4 Address. This is your internal IP address to use for this program  
Also find Physical Address. This is your MAC address  
Take note of both of these  
  
## finish configuring the code  
Open up the python file and on the line that says `ARDUINO_IP = ` set that equal to your IP (for example: `ARDUINO_IP = 39.0.0.100`)  
And then on the line that says `PC_IP = ` do the same. For example: `PC_IP = 39.0.0.111`  
Press file > save  

Open up WOL.INO and find the line `byte arduinoMAC[] = {0xDE 0xAD 0xBE 0xEF 0xFE 0xED}` and replace the DEADBEEFFEED with your arduino MAC address found earlier  
Do the same for the line that says `byte pcMAC` but but your pc mac address (Physical address) that you found earlier  
Replace the line `IPAddress arduinoIP(39, 0, 0, 100 )` with your arduino's IP address found earlier  
Finally replace the line that says `IPAddress pcIP(39, 0, 0, 111)` with your PC's IP address found earlier  
Press upload to upload the arduino's code to the arduino and you should be good to go!  
# You're Done!   
Well done on setting this up! If you have any questions feel free to message me on discord at `@_3va_` and I'll do my best to answer any questions you have!  
## Moonlight / Sunshine
I set this up so I could power my pc remotely when I leave for college and then remote desktop with Moonlight / Sunshine  
If you use Sunshine you should be able to remote into your PC when it's logged out  
However if you use GFE as the host then you won't be able to connect to the pc until you log in  
A solution I found was to disable the login screen and not have a password.  Not ideal for obvious security reasons   

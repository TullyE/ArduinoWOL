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
# I belive you also will have to disable the login screen. I'm not sure the steps on how to do that. So I reccomend you google it for your system. 
# Setup Blynk to communicate with the Arduino
# Configure the Python code and the Arduino Code
# Setup the Blynk APP
# You're Done!   
Well done on setting this up! If you have any questions feel free to message me on discord at `@_3va_` and I'll do my best to answer any questions you have!
  

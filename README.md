# Configure your PC for WOL
## Configure your BIOS
  These steps might differ depending on your BIOS  
  Press F2 during boot to enter BIOS Setup. Go to the Power menu  
  Set Wake-on-LAN to Power On. Press F10 to save and exit the BIOS Setup  
  The option could also be named Power On By PCI Devices or similar  
    
  You can always google how to enable WOL on the **[YOUR MOTHERBOARD HERE]** 
  
## Configure your Ethernet Controller/Network card in Windows
  Open Device Manager > Network Adapters > Right click your Network card > Properties > Advanced > Look for (usually found at the bottom of the alphabetically sorted list):  
  Wake on Magic Packet -> ensure value is enabled  
  Wake on pattern Match -> ensure value is enabled  
  WOL & Shutdown Link Speed **(you may not have this setting - that's okay)** -> 10 Mbps First  
  Then click Power Managment > Allow this device to wake the computer

# Configure your PC for WOL
## BIOS
  These steps might differ depending on your BIOS  
  Press F2 during boot to enter BIOS Setup. Go to the Power menu  
  Set Wake-on-LAN to Power On. Press F10 to save and exit the BIOS Setup  
  The option could also be named Power On By PCI Devices or similar  
    
  You can always google how to enable WOL on the **[YOUR MOTHERBOARD HERE]** 
  
## Hardware
  
  Configure your Ethernet Controller/Network card in Windows:
  Start Device Manager>Network Adapters>Right click your Network card>Properties>Advanced>Look for:
  WOL/Wake-on-Lan/Wake from shutdown/Magic Packet or similar settings, and make sure're they're configured
  While still in the properties of your Network Card. Go to the Power Management tab, and make sure this box is ticked:
  Allow this device to wake the computer. More advanced settings can be found in advanced power settings in the Control Panel

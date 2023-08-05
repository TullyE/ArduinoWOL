import socket
import asyncio
import subprocess

# constants
#IP Addresses
ARDUINO_IP = "11.1.1.111"#replace with your arduino's IP
PC_IP = "00.0.0.000" #replace with your PC's IP

DESTINATION_PORT = 49152 #can be any number you'd like. should be between 49152 to 65535. Must match Source port in arduino code
SOURCE_PORT = 49152 # can be any number you'd like. should be between 49152 to 65535. Must match Destination port in arduino code
IDENTIFIER = "SOME_PC_IDENTIFIER" # match the identifier in the WOL.ino
LOGIN_SECRET = "SuperSecretLoginSecret" # match the login secret in the WOL.ino
WOL_ON_SECRET = "SuperSecretWOLOnSecret" # match the WOL on secret in the WOL.ino
SHUTTING_DOWN_PC_SECRET = "SuperSecretShuttingDownPCSecret" # match the shutdown secret in the WOL.ino
CHECK_PC_POWER_STATE_SECRET = "SuperSecretCheckPCPowerStateSecret" # match the check pc power secret in the WOL.ino

# Shutdown PC
def shutdown_windows():
    try:
        subprocess.run(["shutdown", "/s", "/f], check=True, capture_output=True)
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
    else:
        send_packet(SHUTTING_DOWN_PC_SECRET)
        print("Windows shutdown command executed successfully.")
        # Close the socket properly using the `with` statement
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sender_socket:
        # No need to call sender_socket.close() here
        pass

# Send a packet to arduino
def send_packet(secret):
    message = f"{IDENTIFIER}:{secret}"
    sender_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sender_socket.sendto(message.encode(), (ARDUINO_IP, DESTINATION_PORT))
    sender_socket.close()

# Handle incoming packets
class MyUDPServerProtocol(asyncio.DatagramProtocol):
    def datagram_received(self, data, addr):
        try:
            message = data.decode('utf-8')  # Specify the correct encoding
            if(CHECK_PC_POWER_STATE_SECRET == message):
                send_packet(CHECK_PC_POWER_STATE_SECRET)
            elif(WOL_ON_SECRET == message):
                send_packet(WOL_ON_SECRET)
            elif(SHUTTING_DOWN_PC_SECRET == message):
                shutdown_windows()
            else:
                send_packet(f"Packet: {message} not recognized!")
        except UnicodeDecodeError:
            pass          

async def main():
    # Create a UDP server
    loop = asyncio.get_event_loop()
    transport, protocol = await loop.create_datagram_endpoint(
        MyUDPServerProtocol, local_addr=(PC_IP, SOURCE_PORT)
    )

    try:
        await asyncio.Future()
    except KeyboardInterrupt:
        print("Server shutting down.")
    finally:
        # Close the socket when done
        transport.close()

if __name__ == "__main__":
    send_packet(LOGIN_SECRET)
    asyncio.run(main())

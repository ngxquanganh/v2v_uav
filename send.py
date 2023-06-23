import socket
import struct
def send_broadcast_message(message):
    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Enable broadcasting mode
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    
    # Set the destination address and port for broadcasting
    broadcast_address = '192.168.1.255'
    broadcast_port = 1111
    destination = (broadcast_address, broadcast_port)
    
    # Send the message
    sock.sendto(message.encode(), destination)
    
    # Close the socket
    sock.close()

# Example usage

# Pack the integer into 4 bytes
packed_data = struct.pack("I", 15)

# Convert the packed data to a string
packed_string = packed_data.decode("latin-1")

message = ""
message += struct.pack("I",0).decode("latin-1")
message += "05\0"
message += struct.pack("I",6).decode("latin-1")
message += "helllo"

print(message)
send_broadcast_message(message)

import socket

HOST = "127.0.0.1"
PORT = 8080

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((HOST, PORT))

print(f"Listening on {HOST}:{PORT}")

packet_count = 0
byte_count = 0

while True:
    data, addr = sock.recvfrom(65535)

    packet_count += 1
    byte_count += len(data)

    print(
        f"Packet #{packet_count:<6} "
        f"From {addr[0]}:{addr[1]} "
        f"Size: {len(data)} bytes "
        f"Total: {byte_count} bytes"
    )
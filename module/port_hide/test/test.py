import socket

host = "0.0.0.0"
port = 2600

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((host, port))

# max 5 queued connections
server_socket.listen(5)

print(f"Server listening on {host}:{port}...")

while True:
    client_socket, client_address = server_socket.accept()
    print(f"Connection established with {client_address}")

    client_socket.send(b"Hello from server on port 2600!")

    client_socket.close()

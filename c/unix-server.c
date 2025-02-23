#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/my_unix_socket"

int main() {
	int server_socket, client_socket;
	struct sockaddr_un server_addr;
	struct ucred peer_cred;
	socklen_t len = sizeof(peer_cred);

	// Create a Unix domain socket
	server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if (server_socket == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Set up the server address
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sun_family = AF_UNIX;
	strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

	// Bind the socket
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		perror("bind");
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections
	if (listen(server_socket, 1) == -1) {
		perror("listen");
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	printf("Server is listening on %s\n", SOCKET_PATH);

	// Accept a client connection
	client_socket = accept(server_socket, NULL, NULL);
	if (client_socket == -1) {
		perror("accept");
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	printf("Client connected\n");

	// Get the client's PID using SO_PEERCRED
	if (getsockopt(client_socket, SOL_SOCKET, SO_PEERCRED, &peer_cred, &len) == -1) {
		perror("getsockopt");
		close(client_socket);
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	// Print the PID of the client
	printf("Client PID: %d\n", peer_cred.pid);

	// Close sockets
	close(client_socket);
	close(server_socket);

	// Optionally, remove the socket file
	unlink(SOCKET_PATH);

	return 0;
}

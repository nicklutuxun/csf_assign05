/*
 * Nick Lu
 * tlu32@jhu.edu
 * 
 * Jiarui Chen
 * jchen324@jhu.edu
 */

#include <stdio.h>      /* for snprintf */
#include "csapp.h"
#include "calc.h"

#define LINEBUF_SIZE 1024

int chat_with_client(struct Calc *calc, int client_fd);

int main(int argc, char **argv) {
	if (argc != 2) {
		// incorrect number of command line argument
		exit(0); 	
	}

	// create the calculator
	struct Calc *calc = calc_create();

	// read port number
	const char *port = argv[1];

	// create server socket
	int server_fd = open_listenfd((char*) port);
	if (server_fd < 0) { return 0; } // fatal error

	int keep_going = 1;
	while (keep_going) {
		// wait fo client to connect
		int client_fd = Accept(server_fd, NULL, NULL);
		if (client_fd > 0) {
			// interacte with client; chat_with_client returns 0 for server to shut down
			keep_going = chat_with_client(calc, client_fd);
			close(client_fd); // close the connection
		}
	}
	close(server_fd); // close server socket

	calc_destroy(calc);
	return 0;
}

int chat_with_client(struct Calc *calc, int client_fd) {
	rio_t in;
	char linebuf[LINEBUF_SIZE];

	rio_readinitb(&in, client_fd); // associate client file descriptor with buffer in

	/*
	 * Read lines of input, evaluate them as calculator expressions,
	 * and (if evaluation was successful) print the result of each
	 * expression.  Quit when "quit" command is received. Shutdown when 
	 * "shutdown" command is received.
	 */
	int done = 0;
	while (!done) {
		// read line from client
		ssize_t n = rio_readlineb(&in, linebuf, LINEBUF_SIZE);
		if (n <= 0) {
			/* error or end of input */
			done = 1;
		} else if (strcmp(linebuf, "quit\n") == 0 || strcmp(linebuf, "quit\r\n") == 0) {
			/* quit command */
			done = 1;
		} else if (strcmp(linebuf, "shutdown\n") == 0 || strcmp(linebuf, "shutdown\r\n") == 0) {
			/* quit command and shut down server */
			done = 1;
			return 0;
		}
		else {
			/* process input line */
			int result;
			if (calc_eval(calc, linebuf, &result) == 0) {
				/* expression couldn't be evaluated */
				rio_writen(client_fd, "Error\n", 6);
			} else {
				/* output result */
				int len = snprintf(linebuf, LINEBUF_SIZE, "%d\n", result);
				if (len < LINEBUF_SIZE) {
					// send response to client
					rio_writen(client_fd, linebuf, len);
				}
			}
		}
	}
	return 1;
}

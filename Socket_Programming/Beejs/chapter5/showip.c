/*
 ** showip.c -- Shows IP addressess for a given hostname
 */

#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main(int argc, char **argv) {

	struct addrinfo hints;	// hints structure
	struct addrinfo *res;	// addresses to be stored
	struct addrinfo *p;	// temp pointer

	int status;

	char ipstr[INET6_ADDRSTRLEN];

	if (argc != 2) {
		fprintf(stderr, "Use: ./showip hostname\n");
		return 1;
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;		// Can be AF_INET or AF_INET6, can be specified when need to force IPv4/v6
	hints.ai_socktype = SOCK_STREAM;	// TCP Stream
	
	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {		// getaddrinfo returns 0 if it runs fine otherwise an other number. it takes four inputs, 1st -> ip address/website to check, 2nd -> port to connect, 3rd -> hints on certain values, 4th -> where you want to store the result
		fprintf(stderr, "getaddrinfo: %d: %s\n", status, gai_strerror(status));	// standard error 
		return 2;
	}

	printf("IP addresses for %s:\n\n", argv[1]);

	for (p = res; p != NULL; p = p -> ai_next) {
		void *addr;	// to store address
		char *ipver;	// tp store whether IPv4 or IPv6
		
		// Have to put this statement here as we are not sure whether it is going to be IPv4 or IPv6 and then fetch info
		
		// IPv4
		if (p -> ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p -> ai_addr;
			addr = &(ipv4 -> sin_addr);
			ipver = "IPv4";
		}
		else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p -> ai_addr;
                        addr = &(ipv6 -> sin6_addr);
                        ipver = "IPv6";
		}
	
		// convert IP to a string and print it
		inet_ntop(p -> ai_family, addr, ipstr, sizeof ipstr);
		printf("  %s: %s\n", ipver, ipstr);
	}

	freeaddrinfo(res);

	return 0;
}

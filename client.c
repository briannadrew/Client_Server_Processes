/* ===================================================================================== *
*                                Client Server Processes                                 *
*                                                                                        *
*  Name: client.c                                                                        *
*                                                                                        *
*  Written by: Brianna Drew - March/April 2020                                           *
*                                                                                        *
*  Purpose: To create a client-server environment to copy the contents of a file from    *
*           the server to the client. Ran as a single process on computer.               *
*                                                                                        *
*  Description of Parameters:                                                            *
*  argv[1] - path to file to be copied from server to client                             *
*             (e.g. /home/COIS/3380/lab4_sourcefile)                                     *
*                                                                                        *
*  Subroutines/Libraries Required:                                                       *
*      See include statements.                                                           *
*                                                                                        *
*  References: https://www.geeksforgeeks.org/c-program-for-file-transfer-using-udp/      *
*                                                                                        *
* ====================================================================================== */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_PROTOCOL 0
#define IP_ADDRESS "127.0.0.1" // localhost
#define PORT_NO 52446
#define NET_BUF_SIZE 32
#define sendrecvflag 0
FILE* fp;


// function receive file
int recvFile(char* buf, int s)
{
	int i;
	char ch;
	for (i = 0; i < s; i++) {
		ch = buf[i];
		if (ch == EOF)
                   {
		       return 1;
                   }
		else
                   {
			printf("%c", ch);
                        putc(ch, fp);
                   }
	}
	return 0;
}

// function clear buffer
void clearBuf(char* b)
{
	int i;
	for (i = 0; i < NET_BUF_SIZE; i++)
		b[i] = '\0';
}



int main(int argc, char** argv)
{
    system("clear");
        if (argc != 2)
          {
             printf("Improper Command Line Entry\n");
             printf("Format should be: EXECUTABLE FILEPATH\n");
             printf("Exiting Client Program\n");
             exit(0);
          }

	int sockfd, nBytes;
	struct sockaddr_in addr_con;
	int addrlen = sizeof(addr_con);
	addr_con.sin_family = AF_INET;
	addr_con.sin_port = htons(PORT_NO);
	addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	char net_buf[NET_BUF_SIZE];


        fp = fopen("lab_sourcefile_local_clone", "w");

	// socket()
	sockfd = socket(AF_INET, SOCK_DGRAM,
					IP_PROTOCOL);

	if (sockfd < 0)
		printf("\nfile descriptor not received!!\n");
	else
		printf("\nfile descriptor %d received\n", sockfd);

	while (1) {
	        strncpy(net_buf, argv[1],32);
               sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr*)&addr_con, addrlen);

		printf("\n---------Data Received---------\n");

		while (1) {
			// receive
			clearBuf(net_buf);
			nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE,
							sendrecvflag, (struct sockaddr*)&addr_con,
							&addrlen);

			// process
			if (recvFile(net_buf, NET_BUF_SIZE)) {
				break;
			}
		}
		printf("\n-------------------------------\n\n\n");
                fclose(fp);
		sendto(sockfd, "Terminate", NET_BUF_SIZE, sendrecvflag, (struct sockaddr*)&addr_con, addrlen);

/* Display md5sum
		printf("\nmd5sum");
		printf("\n-------------------------------\n");
                system("./md5.sh");
		printf("\n\n");
*/
                break;
	}

	return 0;
}

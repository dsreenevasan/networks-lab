#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#define FSIZE 4
int main(int argc, char *argv[]){
	printf("Serving in port %d\n", atoi(argv[1]));
	int server_sock, client_sock, c, read_size, i, j, p;
	char buffer[20];
	struct sockaddr_in server, client;
	char server_message[1000], client_message[1000], buf[1000];
	
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sock == -1){
		printf("Socket couldnt be created");
		return 1;
	}
	
	puts("Socket created");
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(argv[1]));
	
	if(bind(server_sock, (struct sockaddr*)&server, sizeof(server)) < 0){
		puts("Bind Error");
		return 1;
	}
	puts("Bind Done");
	
	listen(server_sock, 10);
	puts("Server ready to take incoming connections");
	
	c = sizeof(struct sockaddr_in);
	client_sock = accept(server_sock, (struct sockaddr*)&client, (socklen_t*)&c);
	if(client_sock < 0){
		puts("Accept failed");
		return 1;
	}
	puts("Connection accepted");
	
/*	while(1){
		bzero(client_message, 1000);
		bzero(server_message, 1000);	
		if(read_size = recv(client_sock, client_message, 1000, 0) > 0){
			puts("Client: ");
			puts(client_message);
			for(i=0; client_message[i]; i++)
	                        client_message[i] = tolower(client_message[i]);
        		if(strcmp(client_message, "bye") == 0)
				strcat(server_message, "Bye!");
			else{
				puts("Server:");
				gets(server_message);
			}
			write(client_sock, server_message, strlen(server_message));
		}
		if(read_size == 0){
			puts("Client disconnected");
			break;
		}
		if(read_size < 0){
			puts("Recv failed");
			break;
		}
	}*/

//	bzero(client_message, 1000);
	puts("Enter any text:");
	gets(server_message);
//	while(1){
		for(i=0; server_message[i];i+=4){
			bzero(client_message, 1000);
		//	for(j=0; j<FSIZE && server_message[j]; j++)
		//		strcat(client_message, server_message[i+j]);
			strncpy(client_message, server_message+i, FSIZE);
			for(j=0; j<FSIZE && server_message[j]; j++){
				sprintf(buffer, "%d", i+j+1);
				strcat(client_message, buffer);
			}
			if(send(client_sock, client_message, 1000, 0) < 0){
				puts("Send error");
				return 1;
			}
			printf("\nSuccessfully transmitted %d frames", FSIZE);
			if(recv(client_sock, buf, 1000, 0) < 0){
				perror("recv failed");
				return 1;
			}
			bzero(client_message, 1000);
//			int p =(int) buf[0] - 48;
			sscanf(buf, "%d", &p);
			printf("\np - %d", p);
			if(p > -1){
				printf("\nRetransmitting from frame %d\n", p);
				for(j=p-1; j%FSIZE != 0; j++)
					client_message[j-p+1] = server_message[j];
//					strcat(client_message, server_message[i]);
				for(j=p-1; j%FSIZE != 0; j++){
					sprintf(buffer, "%d", j+1);
					strcat(client_message, buffer);
				}
				if(send(client_sock, client_message, 1000, 0) < 0){
					puts("send error");
					return 1;
				}
			}
			
			
		}

	strcpy(client_message, "exit");
	send(client_sock, client_message, 1000, 0);
	close(server_sock);
	return 0;
}


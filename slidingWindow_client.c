#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]){
	printf("Client running in port %d\n", atoi(argv[1]));
	
	int client_sock, server_sock, c, read_size, i, choice;
	struct sockaddr_in server, client;
	char server_message[1000], client_message[1000];
	
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sock == -1){
		puts("Couldnt create socket");
		return 1;
	}
	puts("Socket created");
	
//	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	
	if(connect(server_sock, (struct sockaddr*)&server, sizeof(server)) < 0){
		perror("Error in connecting to server");
		return 1;
	}
	
/*	while(1){
		bzero(server_message, 1000);
		puts("Client:");
	//	scanf("%s", client_message);
		gets(client_message);
		
		if(send(server_sock, client_message, strlen(client_message), 0) < 0){
			puts("Error in sending the message");
			return 1;
		}
		
		if(recv(server_sock, server_message, 1000, 0) < 0){
			puts("Recv failed");
			return 1;
		}
		puts("Server:");
		puts(server_message);
		for(i=0; client_message[i]; i++)
			client_message[i] = tolower(client_message[i]);
		if(strcmp(client_message, "bye") == 0)
			break;
	}*/


	while(1){
		if(recv(server_sock, client_message, 1000, 0) < 0){
			perror("recv failed");
			return 1;
		}
		if(strcmp(client_message, "exit") == 0){
			puts("exiting");
			return 1;
		}
		puts("Transmitted frames.........");
		puts(client_message);
		puts("Is there any error in any of the frames?\n 1.Yes\t0.No");
		scanf("%d",&choice);
		if(choice){
			puts("Enter the frame no that contains error");
			scanf("%d", &choice);
		}
		else
			choice = -1;
		bzero(server_message, 1000);
		sprintf(server_message, "%d", choice);
		puts(server_message);
		if(send(server_sock, server_message, 1000, 0) < 0){
			puts("Send failed");
			return 1;
		}
		if(strcmp(server_message, "-1") != 0){
			puts("Retransmitted frames.........");
			if(recv(server_sock, client_message, 1000, 0) < 0){
	                        perror("recv failed");          
        	                return 1;
               		 }
			puts(client_message);
		}
	}
	
	close(server_sock);
	return 0;
}

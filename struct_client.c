#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
struct student{
	char name[20];
	int age;
}stud;

int main(int argc, char *argv[]){
	printf("Client running in port %d\n", atoi(argv[1]));
	
	int client_sock, server_sock, c, read_size, i;
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

	puts("Enter your name and age");
	gets(stud.name);
	scanf("%d", &stud.age);

	send(server_sock, (void *)&stud, sizeof(struct student), 0);
	close(server_sock);
	return 0;
}

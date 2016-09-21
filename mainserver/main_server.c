#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
int main(int argc, char *argv[]){
	printf("Serving in port %d\n", atoi(argv[1]));
	int server_sock, client_sock, c, read_size, i, buffer;
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

	bzero(buf, 1000);
	strcat(buf, "HTTP/1.1 ");
	if(recv(client_sock, client_message, 2000, 0) > 0){
		printf("File requested is %s", client_message);
		FILE *fp = fopen("index.html", "r");
		if(fp != NULL){
			strcat(buf, "GET OK");
			buffer = fread(buf, 1, 1000, fp);
//			fscanf(fp, "%[^EOF]", buffer);
		}
		if(fp == NULL){
//			bzero(buf, 1000);
			strcat(buf, "404 FILE NOT FOUND");
		}
		if(send(client_sock, buf, 1000, 0) < 0){
                                perror("couldnt send file");
                                return 1;
                }
		if(fp != NULL && feof(fp)){
			puts("File sent");
		}
	}
	else{
		puts("recv failed");
	}		
	close(server_sock);
	return 0;
}


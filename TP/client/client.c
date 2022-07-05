//Lívia Stéffanny de Sousa         - 20.1.4029
//Pedro Henrique Oliveira da Silva - 20.1.4005

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024
 
void send_file(FILE *fp, char* filename, int sockfd){

    char data[SIZE] = {0};
    send(sockfd, filename, SIZE, 0);
 
    while(fread(data, sizeof(char), SIZE, fp)!= 0){
        if(send(sockfd, data, sizeof(data), 0) == -1) {
            perror("[-]Erro no envio do arquivo.❌");
            exit(1);
        }
        bzero(data, SIZE);
    }
}
 
int main(){

    char ip[SIZE];
    int port = 0;
    int e, sockfd;
    struct sockaddr_in server_addr;
    
    printf("Digite o número do IP do servidor:\n");
    scanf("%s", ip);

    printf("Digite o número da porta do servidor:\n");
    scanf("%d", &port);
   
    FILE *fp;
    char filename[SIZE];
    //Criando um socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Erro no socket.❌");
        exit(1);
    }

    printf("[+]Socket do servidor criado com sucesso. ✔️\n");
 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
 
    //Solicitando conexão com o servidor
    e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e == -1) {
        perror("[-]Erro no socket. ❌");
        exit(1);
    }

    printf("[+]Conectado ao servidor.✔️\n");
    printf("Digite o nome do arquivo a ser enviado: \n");
    scanf("%s", filename);

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("[-]Erro na leitura do arquivo. ❌");
        exit(1);
    }
 
    send_file(fp, filename, sockfd);
    printf("[+]Dados de arquivo enviados com sucesso.✔️\n");
 
    printf("[+]Fechando a conexão.👋\n");
    close(sockfd);
 
    return 0;
}
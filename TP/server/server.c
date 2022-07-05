//Lívia Stéffanny de Sousa         - 20.1.4029
//Pedro Henrique Oliveira da Silva - 20.1.4005

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024
 
void write_file(int sockfd){

  int n;
  FILE *fp;
  char filename[SIZE] = {0};
  char buffer[SIZE] = {0};
  
  n = recv(sockfd, filename, SIZE, 0);

  fp = fopen(filename, "wb");
  bzero(filename, SIZE);
  while (1){
    //Recebendo bytes do cliente
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    fwrite(buffer, sizeof(buffer), 1, fp);
    bzero(buffer, SIZE);
  }
  fclose(fp);
  return;
}
  
int main(){

    char *ip = "127.0.0.1";
    int port = 8080;
    int e, sockfd, new_sock;

    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    //Criando um socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Erro no socket. ❌");
        exit(1);
    }

    printf("[+]Socket do servidor criado com sucesso. ✔️\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    //Escolhendo a porta para escutar a conexão TCP
    e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
   
    if(e < 0) {
        perror("[-]Erro na ligação. ❌");
        exit(1);
    }

    printf("[+]Ligação bem sucedida. ✔️\n");
    //Colocando o socket para aguardar a conexão
    if(listen(sockfd, 10) == 0){
        printf("[+]Escutando...🎧\n");
    }
    else {
        perror("[-]Erro na escuta. ❌");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    //Aceitando uma nova conexão com o cliente
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
    write_file(new_sock);
    
    printf("[+]Dados escritos no arquivo com sucesso. ✔️\n");
    
    return 0;
}
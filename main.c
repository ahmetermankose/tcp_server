#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/aes.h>
 /* AES key for Encryption and Decryption */
const static unsigned char aes_key[]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
/* Print Encrypted and Decrypted data packets */
void print_data(const char *tittle, const void* data, int len);

int main(){
 
  char *ip = "127.0.0.1";
  int port = 8080;
 
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[129];
  int n;
  AES_KEY dec_key;
  /* Init vector */
  unsigned char iv[AES_BLOCK_SIZE];
  memset(iv, 0x00, AES_BLOCK_SIZE);
 
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");
 
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);
 
  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("[-]Bind error");
    exit(1);
  }
  printf("[+]Bind to the port number: %d\n", port);
 
  listen(server_sock, 5);
  printf("Listening...\n");
 
  while(1){
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("[+]Client connected.\n");
 
    bzero(buffer, 128);
    recv(client_sock, buffer, sizeof(buffer), 0);
   // printf("Client: %s\n size %ld", buffer,sizeof(buffer));
 print_data("\n buffer",buffer, sizeof(buffer));
    unsigned char dec_out[sizeof(buffer)];
    AES_set_decrypt_key(aes_key, sizeof(aes_key)*8, &dec_key); // Size of key is in bits
    AES_cbc_encrypt(buffer, dec_out, sizeof(buffer), &dec_key, iv, AES_DECRYPT);
    print_data("\n Decrypted",dec_out, sizeof(dec_out)); //You can not print data as a string, because after Encryption its not ASCII
   
    /*bzero(buffer, 115);
    strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
    printf("Server: %s\n", buffer);
    send(client_sock, buffer, strlen(buffer), 0);*/
 
    close(client_sock);
    printf("[+]Client disconnected.\n\n");
 
  }
  return 0;
}

void print_data(const char *tittle, const void* data, int len)
{
    printf("%s : ",tittle);
    const unsigned char * p = (const unsigned char*)data;
    int i = 0;
    printf("pdata len %d\n",len);
    for (; i<len; ++i)
        printf("%02X ", *p++);
    
    printf("\n");
}
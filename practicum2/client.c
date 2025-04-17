/*
 * client.c -- TCP Socket Client
 * CS5600 / Practicum 2
 * Agnibha Chatterjee
 * Tarun Mohan 
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
  if (argc != 3 && argc != 4) {
    printf("Usage: %s WRITE <local-file-path> [remote-file-path]\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "WRITE") != 0) {
    printf("Only WRITE command is supported in this version.\n");
    return 1;
  }

  const char *local_path = argv[2];
  const char *remote_path = (argc == 4) ? argv[3] : NULL;

  // Open local file
  FILE *fp = fopen(local_path, "rb");
  if (!fp) {
    perror("Failed to open local file");
    return 1;
  }

  // Get file size
  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  if (file_size < 0) {
    perror("ftell failed");
    fclose(fp);
    return 1;
  }

  char *file_buf = malloc(file_size);
  if (!file_buf) {
    perror("malloc failed");
    fclose(fp);
    return 1;
  }
  if (fread(file_buf, 1, file_size, fp) != file_size) {
    perror("fread failed");
    free(file_buf);
    fclose(fp);
    return 1;
  }
  fclose(fp);

  int socket_desc;
  struct sockaddr_in server_addr;
  char server_message[2000];
  
  // Clean buffers:
  memset(server_message,'\0',sizeof(server_message));
  
  // Create socket:
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  
  if(socket_desc < 0){
    printf("Unable to create socket\n");
    free(file_buf);
    return -1;
  }
  
  printf("Socket created successfully\n");
  
  // Set port and IP the same as server-side:
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2000);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  // Send connection request to server:
  if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
    printf("Unable to connect\n");
    close(socket_desc);
    free(file_buf);
    return -1;
  }
  printf("Connected with server successfully\n");
  
  // Send protocol: WRITE\n<remote-path>\n[<client-path>\n]<file-size>\n<file-bytes>
  char header[4096];
  int header_len = 0;
  if (remote_path) {
    // Normal case: remote path provided
    header_len = snprintf(header, sizeof(header), "WRITE\n%s\n%ld\n", remote_path, file_size);
    if (send(socket_desc, header, header_len, 0) < 0) {
      printf("Unable to send header\n");
      close(socket_desc);
      free(file_buf);
      return -1;
    }
  } else {
    // Remote path missing: send empty line, then local path
    header_len = snprintf(header, sizeof(header), "WRITE\n\n%s\n%ld\n", local_path, file_size);
    if (send(socket_desc, header, header_len, 0) < 0) {
      printf("Unable to send header\n");
      close(socket_desc);
      free(file_buf);
      return -1;
    }
  }
  // Send file data
  if (send(socket_desc, file_buf, file_size, 0) < 0) {
    printf("Unable to send file data\n");
    close(socket_desc);
    free(file_buf);
    return -1;
  }
  free(file_buf);
  
  // Receive the server's response:
  memset(server_message, '\0', sizeof(server_message));
  int recv_len = recv(socket_desc, server_message, sizeof(server_message)-1, 0);
  if(recv_len < 0){
    printf("Error while receiving server's msg\n");
    close(socket_desc);
    return -1;
  }
  server_message[recv_len] = '\0';
  printf("Server's response: %s\n",server_message);
  
  // Close the socket:
  close(socket_desc);
  
  return 0;
}

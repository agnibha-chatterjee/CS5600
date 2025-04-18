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
  if (argc < 2) {
    printf("Usage:\n");
    printf("  %s WRITE <local-path> [remote-path] <ro|rw>\n", argv[0]);
    printf("  %s GET <remote-path> [local-path]\n", argv[0]);
    printf("  %s RM <remote-path>\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "WRITE") == 0) {
    const char *local_path = argv[2];
    const char *remote_path = NULL;
    const char *permission = NULL;
    if (argc == 5) {
      remote_path = argv[3];
      permission = argv[4];
    } else if (argc == 4) {
      permission = argv[3];
    } else {
      printf("Usage: %s WRITE <local-path> [remote-path] <ro|rw>\n", argv[0]);
      return 1;
    }
    if (strcmp(permission, "ro") != 0 && strcmp(permission, "rw") != 0) {
      fprintf(stderr, "Invalid permission: %s (must be ro or rw)\n", permission);
      return 1;
    }

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
      header_len = snprintf(header, sizeof(header), "WRITE\n%s\n%s\n%ld\n", remote_path, permission, file_size);
      if (send(socket_desc, header, header_len, 0) < 0) {
        printf("Unable to send header\n");
        close(socket_desc);
        free(file_buf);
        return -1;
      }
    } else {
      header_len = snprintf(header, sizeof(header), "WRITE\n\n%s\n%s\n%ld\n", local_path, permission, file_size);
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
  } else if (strcmp(argv[1], "GET") == 0) {
    const char *remote_path = argv[2];
    const char *local_path = (argc == 4) ? argv[3] : argv[2];

    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[4096];
    memset(server_message,'\0',sizeof(server_message));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc < 0){
      printf("Unable to create socket\n");
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
      return -1;
    }
    printf("Connected with server successfully\n");

    // Send protocol: GET\n<remote-path>\n
    char header[4096];
    int header_len = 0;
    if (argc == 4) {
      // Both remote and local path provided
      header_len = snprintf(header, sizeof(header), "GET\n%s\n", remote_path);
    } else {
      // Only remote path provided, use as both
      header_len = snprintf(header, sizeof(header), "GET\n%s\n", remote_path);
    }
    if (send(socket_desc, header, header_len, 0) < 0) {
      printf("Unable to send header\n");
      close(socket_desc);
      return -1;
    }

    // Receive file size or error
    int recv_len = 0;
    int idx = 0;
    char c = 0;
    char sizebuf[64] = {0};
    // Read until newline (file size or error message)
    while (idx < sizeof(sizebuf) - 1) {
      recv_len = recv(socket_desc, &c, 1, 0);
      if (recv_len <= 0) break;
      if (c == '\n') break;
      sizebuf[idx++] = c;
    }
    sizebuf[idx] = '\0';
    if (strncmp(sizebuf, "ERROR", 5) == 0) {
      printf("Server's response: %s\n", sizebuf);
      close(socket_desc);
      return 1;
    }
    long file_size = atol(sizebuf);
    if (file_size <= 0) {
      printf("Invalid file size received: %s\n", sizebuf);
      close(socket_desc);
      return 1;
    }
    // Open local file for writing
    FILE *fp = fopen(local_path, "wb");
    if (!fp) {
      perror("Failed to open local file for writing");
      close(socket_desc);
      return 1;
    }
    long received = 0;
    while (received < file_size) {
      int to_read = (file_size - received) > sizeof(server_message) ? sizeof(server_message) : (file_size - received);
      recv_len = recv(socket_desc, server_message, to_read, 0);
      if (recv_len <= 0) break;
      fwrite(server_message, 1, recv_len, fp);
      received += recv_len;
    }
    fclose(fp);
    if (received == file_size) {
      printf("File received and saved to %s\n", local_path);
    } else {
      printf("File transfer incomplete (%ld/%ld bytes)\n", received, file_size);
    }
    close(socket_desc);
    return 0;
  } else if (strcmp(argv[1], "RM") == 0) {
    const char *remote_path = argv[2];
    // For symmetry, allow a third argument, but only use remote_path for now

    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[4096];
    memset(server_message,'\0',sizeof(server_message));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc < 0){
      printf("Unable to create socket\n");
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
      return -1;
    }
    printf("Connected with server successfully\n");

    // Send protocol: RM\n<remote-path>\n
    char header[4096];
    int header_len = snprintf(header, sizeof(header), "RM\n%s\n", remote_path);
    if (send(socket_desc, header, header_len, 0) < 0) {
      printf("Unable to send header\n");
      close(socket_desc);
      return -1;
    }

    // Receive server's response
    int recv_len = recv(socket_desc, server_message, sizeof(server_message)-1, 0);
    if(recv_len < 0){
      printf("Error while receiving server's msg\n");
      close(socket_desc);
      return -1;
    }
    server_message[recv_len] = '\0';
    printf("Server's response: %s\n", server_message);
    close(socket_desc);
    return 0;
  } else {
    printf("Unknown command: %s\n", argv[1]);
    return 1;
  }
}

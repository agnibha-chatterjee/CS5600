/*
 * server.c -- TCP Socket Server
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
#define ROOT_DIR "server_root"

// Helper to read a line from a socket
int read_line(int sock, char *buf, int maxlen) {
  int i = 0;
  char c = 0;
  while (i < maxlen - 1) {
    int r = recv(sock, &c, 1, 0);
    if (r <= 0) return -1;
    if (c == '\n') break;
    buf[i++] = c;
  }
  buf[i] = '\0';
  return i;
}

int main(void)
{
  int socket_desc, client_sock;
  socklen_t client_size;
  struct sockaddr_in server_addr, client_addr;
  char server_message[8196], client_message[8196];
  
  // Create root directory if it doesn't exist
  mkdir(ROOT_DIR, 0777);
  
  // Clean buffers:
  memset(server_message, '\0', sizeof(server_message));
  memset(client_message, '\0', sizeof(client_message));
  
  // Create socket:
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  
  if(socket_desc < 0){
    printf("Error while creating socket\n");
    return -1;
  }
  printf("Socket created successfully\n");
  
  // Set port and IP:
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2000);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  // Bind to the set port and IP:
  if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
    printf("Couldn't bind to the port\n");
    return -1;
  }
  printf("Done with binding\n");
  
  // Listen for clients:
  if(listen(socket_desc, 5) < 0){
    printf("Error while listening\n");
    close(socket_desc);
    return -1;
  }
  printf("\nListening for incoming connections.....\n");
  
  while (1) {
    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    
    if (client_sock < 0){
      printf("Can't accept\n");
      continue;
    }
    printf("Client connected at IP: %s and port: %i\n", 
           inet_ntoa(client_addr.sin_addr), 
           ntohs(client_addr.sin_port));
    
    // Read command, remote path, file size
    char linebuf[4096];
    int pos = 0, n;
    // Read command
    if (read_line(client_sock, linebuf, sizeof(linebuf)) <= 0) {
      strcpy(server_message, "Failed to read command\n");
      send(client_sock, server_message, strlen(server_message), 0);
      close(client_sock);
      continue;
    }
    if (strcmp(linebuf, "WRITE") != 0) {
      strcpy(server_message, "Unknown command\n");
      send(client_sock, server_message, strlen(server_message), 0);
      close(client_sock);
      continue;
    }
    // Read remote path (may be empty)
    char remote_path[4096] = {0};
    if (read_line(client_sock, remote_path, sizeof(remote_path)) < 0) {
      strcpy(server_message, "Failed to read remote path\n");
      send(client_sock, server_message, strlen(server_message), 0);
      close(client_sock);
      continue;
    }
    // If remote_path is empty, read client path and use it
    if (remote_path[0] == '\0') {
      char client_path[4096] = {0};
      if (read_line(client_sock, client_path, sizeof(client_path)) < 0) {
        strcpy(server_message, "Failed to read client path\n");
        send(client_sock, server_message, strlen(server_message), 0);
        close(client_sock);
        continue;
      }
      strncpy(remote_path, client_path, sizeof(remote_path)-1);
      remote_path[sizeof(remote_path)-1] = '\0';
    }
    // Read file size
    char sizebuf[64] = {0};
    if (read_line(client_sock, sizebuf, sizeof(sizebuf)) < 0) {
      strcpy(server_message, "Failed to read file size\n");
      send(client_sock, server_message, strlen(server_message), 0);
      close(client_sock);
      continue;
    }
    long filesize = atol(sizebuf);
    if (filesize <= 0) {
      strcpy(server_message, "Invalid file size\n");
      send(client_sock, server_message, strlen(server_message), 0);
      close(client_sock);
      continue;
    }
    // Prepare full path
    char fullpath[8192];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", ROOT_DIR, remote_path);
    // Create directories as needed
    char *p = fullpath + strlen(ROOT_DIR) + 1;
    for (; *p; ++p) {
      if (*p == '/') {
        *p = 0;
        mkdir(fullpath, 0777);
        *p = '/';
      }
    }
    // Receive file data
    FILE *fp = fopen(fullpath, "wb");
    if (!fp) {
      snprintf(server_message, sizeof(server_message), "Failed to open file for writing: %s\n", strerror(errno));
      send(client_sock, server_message, strlen(server_message), 0);
      close(client_sock);
      continue;
    }
    long received = 0;
    while (received < filesize) {
      int to_read = (filesize - received) > sizeof(client_message) ? sizeof(client_message) : (filesize - received);
      n = recv(client_sock, client_message, to_read, 0);
      if (n <= 0) break;
      fwrite(client_message, 1, n, fp);
      received += n;
    }
    fclose(fp);
    if (received == filesize) {
      strcpy(server_message, "File written successfully\n");
    } else {
      snprintf(server_message, sizeof(server_message), "File transfer incomplete (%ld/%ld bytes)\n", received, filesize);
    }
    send(client_sock, server_message, strlen(server_message), 0);
    close(client_sock);
  }
  close(socket_desc);
  return 0;
}

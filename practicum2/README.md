# Remote File System (RFS)

This project implements a simple remote file system using a TCP client-server model in C. The server provides file storage and management services, and the client can connect to the server to write, read, or delete files remotely.

## Features

- **WRITE**: Upload a file from the client to the server.
- **GET**: Download a file from the server to the client.
- **RM**: Delete a file or folder on the server.

## Directory Structure

- All files on the server are stored under a root directory called `server_root`.

## Compilation

Compile both the client and server using gcc:

```
gcc -o client client.c
gcc -o server server.c
```

## Running the Server

Start the server (it will keep running and handle multiple client requests):

```
./server
```

## Running the Client

The client supports three commands: `WRITE`, `GET`, and `RM`.

### WRITE (Upload a file)

```
./client WRITE <local-file-path> [remote-file-path]
```

- If `remote-file-path` is omitted, the file will be saved on the server with the same name as the local file.
- Example:
  - `./client WRITE data/foo.txt folder/bar.txt` (uploads `data/foo.txt` to `server_root/folder/bar.txt`)
  - `./client WRITE foo.txt` (uploads `foo.txt` to `server_root/foo.txt`)

### GET (Download a file)

```
./client GET <remote-file-path> [local-file-path]
```

- If `local-file-path` is omitted, the file will be saved locally with the same name as the remote file.
- Example:
  - `./client GET folder/bar.txt mycopy.txt` (downloads `server_root/folder/bar.txt` to `mycopy.txt`)
  - `./client GET folder/bar.txt` (downloads `server_root/folder/bar.txt` to `bar.txt`)

### RM (Delete a file or folder)

```
./client RM <remote-file-path>
```

- Example:
  - `./client RM folder/bar.txt` (deletes `server_root/folder/bar.txt`)

## Notes

- The server must be running before you use the client.
- All file paths for the server are relative to the `server_root` directory.
- The server will create directories as needed for uploads.
- The `RM` command uses the standard C `remove()` function, so it will only delete empty directories or files.

## Authors

- Agnibha Chatterjee
- Tarun Mohan

# Remote File System (RFS)

This project implements a simple remote file system using a TCP client-server model in C. The server provides file storage and management services, and the client can connect to the server to write, read, or delete files remotely.

## Authors

- Agnibha Chatterjee
- Tarun M

## Features

- **WRITE**: Upload a file from the client to the server.
- **GET**: Download a file from the server to the client.
- **RM**: Delete a file or folder on the server.
- **LS**: List files and directories on the server with permissions in symbolic form.
- **Permissions**: When creating a file via `WRITE`, specify `ro` (read-only) or `rw` (read-write).

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

The client supports four commands: `WRITE`, `GET`, `RM`, and `LS`.

### WRITE (Upload a file)

```
./client WRITE <local-file-path> [remote-file-path] <ro|rw>
```

- If `remote-file-path` is omitted, the file will be saved on the server with the same name as the local file.
- `<ro|rw>` sets the initial permission: `ro` for read-only; `rw` for read-write.
- Example:
  - `./client WRITE sample/a.txt documents/b.txt rw` (uploads `sample/a.txt` to `server_root/documents/b.txt` with read-write permission)
  - `./client WRITE c.txt ro` (uploads `c.txt` to `server_root/c.txt` with read-only permission)

### GET (Download a file)

```
./client GET <remote-file-path> [local-file-path]
```

- If `local-file-path` is omitted, the file will be saved locally with the same name as the remote file.
- Example:
  - `./client GET documents/b.txt local_copy.txt` (downloads `server_root/documents/b.txt` to `local_copy.txt`)
  - `./client GET documents/b.txt` (downloads `server_root/documents/b.txt` to `b.txt`)

### RM (Delete a file or folder)

```
./client RM <remote-file-path>
```

- Example:
  - `./client RM documents/b.txt` (deletes `server_root/documents/b.txt`)

### LS (List files and directories)

```
./client LS [remote-path]
```

- If `[remote-path]` is omitted, lists the contents of the root directory `server_root`.
- If `remote-path` is a directory, lists its contents.
- If `remote-path` is a file, shows that file and its permissions.
- Format: `<permissions> <name>` where `<permissions>` is in symbolic form (e.g. `drwxr-xr-x` or `-rw-r--r--`) [we piggy back of the OS here]
- Example:
  - `./client LS` (lists `server_root`)
  - `./client LS sample` (lists `server_root/sample`)
  - `./client LS sample/a.txt` (shows `sample/a.txt` permissions)

## Notes

- The server must be running before you use the client.
- All file paths for the server are relative to the `server_root` directory.
- The server will create directories as needed for uploads.
- The `RM` command uses the standard C `remove()` function, so it will only delete empty directories or files.
- Permissions are set at initial upload and enforced by the server; read-only files cannot be modified or deleted.

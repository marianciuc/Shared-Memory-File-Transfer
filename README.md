# Shared-Memory-File-Transfer
This project contains two C language programs demonstrating file transfer between processes using shared memory in UNIX/Linux systems. The programs are capable of transferring both text and binary files.

## Programs

### sender

The `sender` program is designed to send a file to shared memory. It opens a specified file, reads its contents, and copies it into the shared memory, where it can be read by the `receiver` program.

#### Usage

```bash
./sender <file_path>
```
* `<file_path>`: Path to the file to be sent.

### receiver

The `receiver` program reads data from shared memory, sent by the `sender`, and writes it to a specified file. It waits for the start of data transmission and continues reading until it receives a signal that the transmission is complete.

#### Usage

```bash
./receiver <shared_memory_key> <dest_file>
```
* `<shared_memory_key>`: The key of the shared memory used for data transfer.
* `<dest_file>`: Path to the file where the received data will be written.

## How It Works

Data is transferred through shared memory, where `sender` writes data to memory and `receiver` reads from it. This method allows for efficient data transfer between processes without the need for intermediate files.

## Build and Run

To build each of the programs, use `gcc`:

```bash
gcc -o sender sender.c
gcc -o receiver receiver.c 
```

After compiling, first start `receiver`, specifying the shared memory key and the file path for writing, then start `sender`, specifying the file path of the file to be transferred.

## License
This project is distributed under the MIT License.

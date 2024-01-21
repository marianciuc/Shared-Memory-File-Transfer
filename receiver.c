// SO IS1 222A LAB12
// Volodymyr Marianchuk
// mv53868@zut.edu.pl

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>

#define PENDING_STATUS "_[STATUS:PENDING]_"
#define END_STATUS "_[STATUS:END]_"
#define STARTED_STATUS "_[STATUS:STARTED]_"

#define SHSIZE 100

void error(char *msg, int err_code) {
    perror(msg);
    exit(err_code);
}

int main(int argc, char *argv[]) {
    if (argc != 3) error("Incorrect program arguments: receiver <shared_memory_key> <dest_file>", -1);

    char * FILE_NAME = argv[2];
    key_t  shm_key   = atoi(argv[1]);
    int    shm_id;
    int    fd;
    void   * mem;

    if (( shm_id = shmget(shm_key, SHSIZE,  0777)) < 0 ) error("shmget error", shm_id);
    if (( mem    = shmat(shm_id, NULL, 0)) == (void *) -1) error("shmat error", -1);
    if (( fd     = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) error("File not opened", fd);

    printf("The receiver is running and waiting for a waiting sender\n");
    memcpy(mem, STARTED_STATUS, strlen(STARTED_STATUS) + 1);
    while (strcmp(mem, STARTED_STATUS) == 0) sleep(1);

    printf("Data transmission has started\n");

    size_t bytes_to_read;
    while (strcmp(mem, END_STATUS) != 0) {
        memcpy(&bytes_to_read, mem, sizeof(bytes_to_read));
        write(fd, mem + sizeof(bytes_to_read), bytes_to_read);

        memcpy(mem, PENDING_STATUS, strlen(PENDING_STATUS) + 1);
        while (strcmp(mem, PENDING_STATUS) == 0) sleep(1);
    }


    if (close(fd) < 0) error("File not closed", -1);
    if (shmdt(mem) == -1) error("shmdt error", -1);
    printf("The file was transferred successfully\n");
    return 0;
}

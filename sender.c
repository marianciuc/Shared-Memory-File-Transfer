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
    if (argc != 2) error("Incorrect program arguments: sender <file_path>", -1);

    char    *FILE_NAME = argv[1];
    key_t   shm_key;
    ssize_t read_bytes;
    void    * mem;
    int     shmid;
    char    BUFFER[SHSIZE];
    int     fd;

    if (( shm_key = ftok(FILE_NAME, 1))  == -1) error("Ftok error", shm_key);
    if (( shmid   = shmget(shm_key, SHSIZE, 0777 | IPC_CREAT | IPC_EXCL)) < 0) error("Shared memory error", shmid);
    if (( mem     = shmat(shmid, NULL, 0)) == (void *) -1) error("Shared memory segment error", -1);
    if (( fd      = open(FILE_NAME, O_RDONLY)) < 0) error("File not opened", fd);

    printf("The sender program was successfully started\n");
    printf("Segment key %d\n", shm_key);
    printf("Attached file %s\n", FILE_NAME);

    while (strcmp(mem, STARTED_STATUS) != 0) sleep(1);

    printf("Starting data transfer\n");
    while ((read_bytes = read(fd, BUFFER, sizeof(BUFFER))) > 0) {
        memcpy(mem, &read_bytes, sizeof(read_bytes));
        memcpy(mem + sizeof(read_bytes), BUFFER, read_bytes);

        while (strcmp(mem, PENDING_STATUS) != 0) sleep(1);
    }

    memcpy(mem, END_STATUS, strlen(END_STATUS) + 1);
    printf("Data successfully transferred\n");

    if (close(fd) < 0) error("File not closed", -1);
    if (shmdt(mem) == -1) error("shmdt error", -1);
    if (shmctl(shmid, IPC_RMID, NULL) == -1) error("shmctl error", -1);
    return 0;
}

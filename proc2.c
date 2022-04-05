#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int flag = 0;
char name1[] = "/readsem";
char name2[] = "/writesem";
uint N = 100;
char buf[100];
void* addr;

void* func(void* args) {
    sem_t* sem_read = sem_open(name1, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);
    sem_t* sem_write = sem_open(name2, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);
    if (sem_read == SEM_FAILED || sem_write == SEM_FAILED) {
        printf("Sem error: %s\n", strerror(errno));
        exit(errno);
    }
    while (flag == 0) {
        sem_wait(sem_write);
        memmove(buf, addr, N);
        printf("Чтение: %s\n", buf);
        sem_post(sem_read);
    }
    sem_close(sem_read);
    sem_close(sem_write);
    pthread_exit(NULL);
}

int main() {
    printf("Программа чтения начала работу.\n");

    key_t key = ftok(NULL, 1);
    int shm_id = shmget(key, N, IPC_CREAT | 0660);
    if (shm_id == -1) {
        printf("Shm error: %s\n", strerror(errno));
        exit(errno);
    }
    addr = shmat(shm_id, NULL, 0);

    pthread_t id;
    pthread_create(&id, NULL, func, NULL);
    printf("Программа чтения ожидает нажатия клавиши.\n");
    getchar();
    printf("Клавиша нажата.\n");

    flag = 1;
    pthread_join(id, NULL);

    shmdt(addr);

    printf("\nПрограмма чтения закончила работу.\n");
    return 0;
}

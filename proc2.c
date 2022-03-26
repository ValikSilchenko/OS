#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int flag = 0;
char name[11] = "/semaphore";

void* func(void* args) {
    sem_t* id = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);
    if (id == SEM_FAILED) {
        printf("Error: %s\n", strerror(errno));
        exit(errno);
    }
    FILE* fp = fopen("lab5.txt", "a");
    while (flag == 0) {
        sem_wait(id);
        for (int i = 0; i < 10; i++) {
            fputc('2', fp);
            printf("2");
            fflush(stdout);
            sleep(1);
        }
        sem_post(id);
        sleep(1);
    }
    sem_close(id);
    fclose(fp);
    pthread_exit(NULL);
}

int main() {
    printf("Программа 2 начала работу.\n");

    pthread_t id;
    pthread_create(&id, NULL, func, NULL);
    printf("Программа 2 ожидает нажатия клавиши.\n");
    getchar();
    printf("Клавиша нажата.\n");

    flag = 1;
    pthread_join(id, NULL);

    printf("\nПрограмма 2 закончила работу.\n");
    return 0;
}

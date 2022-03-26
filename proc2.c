#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <bits/fcntl.h>
#include <unistd.h>

int flag = 0;
char name[10] = "/synchron";

void* func(void* args) {
    sem_t* id = sem_open(name, O_CREAT);
    FILE* fp = fopen("lab5.txt", "w");
    while (flag == 0) {
        sem_wait(id);
        for (int i = 0; i < 10; i++) {
            fputc('2', fp);
            printf("2");
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
    pthread_join(id, NULL);

    printf("Программа 2 закончила работу.\n");
    return 0;
}

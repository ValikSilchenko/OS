#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int flag1 = 0, flag2 = 0;
int fields[2];

void* func1(void* args) {
    printf("Поток 1 начал работу.\n");

    char buf[255];
    while (flag2 == 0) {
        getdomainname(buf, 255);
        write(fields[1], buf, 255);
        printf("Поток 1 записал данные в канал.\n");
        sleep(1);
    }

    printf("Поток 1 завершил работу.\n");
    pthread_exit(NULL);
}

void* func2(void* args) {
    printf("Поток 2 начал работу.\n");

    char buf[255];
    while (flag2 == 0) {
        read(fields[0], buf, 255);
        printf("Поток 2 считал данные с канала: %s\n", buf);
        sleep(1);
    }

    printf("Поток 2 завершил работу.\n");
    pthread_exit(NULL);
}

int main() {
    printf("Программа начала работу.\n");
    pthread_t id1, id2;
    pipe(fields);
    pthread_create(&id1, NULL, func1, NULL);
    pthread_create(&id2, NULL, func2, NULL);

    printf("Программа ждёт нажатия клавиши.\n");
    getchar();
    printf("Клавиша нажата.\n");

    flag1 = 1;
    flag2 = 1;
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    close(fields[0]);
    close(fields[1]);

    printf("Программа завершила работу.\n");
}

#include <pthread.h>
#include <stdio.h>

int flag = 0;

void* func(void* args) {

    while (flag == 0) {

    }
    pthread_exit(NULL);
}

int main() {
    printf("Процесс записи начал работу.\n");
    pthread_t id;
    pthread_create(&id, NULL, func, NULL);

    printf("Ожидание нажатия клавиши...\n");
    getchar();
    printf("Клавиша нажата.\n");

    flag = 1;
    pthread_join(id, NULL);

    printf("Процесс записи закончил работу.\n");
    return 0;
}

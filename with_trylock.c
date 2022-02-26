#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int flag1 = 0, flag2 = 0;
pthread_mutex_t mutex;

static void* func1(void* args) {
    printf("Поток 1 начал работу.\n");
    sleep(1);
    int error;

    while (flag1 == 0) {
        error = pthread_mutex_trylock(&mutex);
        while (error != 0) {
            printf("Ошибка в потоке 1: %s\n", strerror(error));
            sleep(1);
            error = pthread_mutex_trylock(&mutex);
        }
        if (flag1 == 0)
            for (int i = 0; i < 10; i++) {
                printf("1\n");
                sleep(1);
            }
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }

    printf("\nПоток 1 закончил работу.\n");
    pthread_exit(NULL);
}

static void* func2(void* args) {
    printf("Поток 2 начал работу.\n");
    int error;
    sleep(1);

    while (flag2 == 0) {
        error = pthread_mutex_trylock(&mutex);
        while (error != 0) {
            printf("Ошибка в потоке 2: %s\n", strerror(error));
            sleep(1);
            error = pthread_mutex_trylock(&mutex);
        }
        if (flag2 == 0)
            for (int i = 0; i < 10; i++) {
                printf("2\n");
                sleep(1);
            }
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }

    printf("\nПоток 2 закончил работу.\n");
    pthread_exit(NULL);
}


int main() {
    printf("Программа начала работу.\n");

    pthread_mutex_init(&mutex, NULL);
    pthread_t id1, id2;
    pthread_create(&id1, NULL, func1, NULL);
    pthread_create(&id2, NULL, func2, NULL);

    printf("Программа ждёт нажатия клавишию\n");
    getchar();
    printf("Клавиша нажата.\n");

    flag1 = 1;
    flag2 = 1;

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    pthread_mutex_destroy(&mutex);

    printf("Программа закончила работу\n");
    return 0;
}

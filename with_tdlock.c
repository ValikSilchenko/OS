#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int flag1 = 0, flag2 = 0;
pthread_mutex_t mutex;

static void* func1(void* args) {
    printf("Поток 1 начал работу.\n");
    sleep(1);
    struct timespec clock;

    while (flag1 == 0) {
        clock_gettime(CLOCK_REALTIME, &clock);
        clock.tv_sec += 5;
        while (pthread_mutex_timedlock(&mutex, &clock) != 0) {}
        if (flag1 == 0)
            for (int i = 0; i < 10; i++) {
                printf("1");
                fflush(stdout);
                sleep(1);
            }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    printf("\nПоток 1 закончил работу.\n");
    pthread_exit(NULL);
}

static void* func2(void* args) {
    printf("Поток 2 начал работу.\n");
    sleep(1);
    struct timespec clock;

    while (flag2 == 0) {
        clock_gettime(CLOCK_REALTIME, &clock);
        clock.tv_sec += 5;
        while (pthread_mutex_timedlock(&mutex, &clock) != 0) {}
        if (flag2 == 0)
            for (int i = 0; i < 10; i++) {
                printf("2");
                fflush(stdout);
                sleep(1);
            }
        pthread_mutex_unlock(&mutex);
        sleep(1);
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

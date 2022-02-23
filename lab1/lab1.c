#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int flag;
    pthread_t id;
}T;

static void* func1(void* arg) {
    printf("\nПоток 1 начал работу.\n");
    T* args = (T*) arg;
    printf("ID потока 1, полученный через pthread_self:   %lu.\n", pthread_self());
    printf("ID потока 1, полученный через pthread_create: %lu.\n", args->id);
    sleep(1);  // для красивого вывода

    while (args->flag == 0) {
        printf("1");
        fflush(stdout);
        sleep(1);
    }

    printf("Поток 1 закончил работу.\n");
    pthread_exit((void*)1);
}

static void* func2(void* arg) {
    printf("\nПоток 2 начал работу.\n");
    T* args = (T*) arg;
    printf("ID потока 2, полученный через pthread_self:   %lu.\n", pthread_self());
    printf("ID потока 2, полученный через pthread_create: %lu.\n", args->id);
    sleep(1);

    while (args->flag == 0) {
        printf("2");
        fflush(stdout);
        sleep(1);
    }

    printf("Поток 2 закончил работу.\n");
    pthread_exit((void*)2);
}

int main() {
    printf("Программа начала работу.\n");

    T args1;
    args1.flag = 0;
    T args2;
    args2.flag = 0;
    pthread_create(&args1.id, NULL, func1, &args1);
    printf("ID потока 1, полученный через pthread_create: %lu.\n", args1.id);
    pthread_create(&args2.id, NULL, func2, &args2);

    printf("\nПрограмма ждёт нажатия клавиши.\n");
    getchar();
    printf("\nКлавиша нажата.\n\n");

    args1.flag = 1;
    args2.flag = 1;
    int* exitcode;
    pthread_join(args1.id, (void**)&exitcode);
    printf("Результат завершения потока 1: %p.\n", exitcode);
    pthread_join(args2.id, (void**)&exitcode);
    printf("Результат завершения потока 2: %p.\n", exitcode);

    printf("Программа завершила работу.\n");
    return 0;
}

#include <pthread.h>
#include <stdio.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

int flag = 0;
char name[] = "/queue";
int N = 100;

void* func(void* args) {
    mqd_t q_id = mq_open(name, O_CREAT | O_WRONLY | O_NONBLOCK, S_IWUSR | S_IRUSR, NULL);
    if (q_id == -1) {
        perror("mq_open error");
        exit(errno);
    }

    char buf[N];
    struct timespec time;

    while (flag == 0) {
        getdomainname(buf, N);
        N = strlen(buf);
        clock_gettime(CLOCK_REALTIME, &time);
        time.tv_sec += 7;
        if (mq_timedsend(q_id, buf, N, NULL, &time) == -1)
            perror("mq_send error");
        else
            printf("Отправлено сообщение: %s\n", buf);
        sleep(1);
    }

    mq_close(q_id);
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
    mq_unlink(name);

    printf("Процесс записи закончил работу.\n");
    return 0;
}

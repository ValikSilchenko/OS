#include <pthread.h>
#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

int flag = 0;
char name[] = "/queue";

void* func(void* args) {
    mqd_t q_id = mq_open(name, O_CREAT | O_RDONLY | O_NONBLOCK, S_IRUSR | S_IWUSR, NULL);
    if (q_id == -1) {
        perror("mq_open error");
        exit(errno);
    }

    struct mq_attr attr;
    mq_getattr(q_id, &attr);
    long int N = attr.mq_msgsize;
    char buf[N];
    struct timespec time;

    while (flag == 0) {
        memset(buf, 0, N);
        clock_gettime(CLOCK_REALTIME, &time);
        time.tv_sec += 7;
        if (mq_timedreceive(q_id, buf, N, NULL, &time) == -1) {
            perror("mq_receive error");
            sleep(1);
        } else
            printf("Получено сообщение: %s\n", buf);
    }

    mq_close(q_id);
    pthread_exit(NULL);
}

int main() {
    printf("Процесс чтения начал работу.\n");
    pthread_t id;
    pthread_create(&id, NULL, func, NULL);

    printf("Ожидание нажатия клавиши...\n");
    getchar();
    printf("Клавиша нажата.\n");

    flag = 1;
    pthread_join(id, NULL);

    printf("Процесс чтения закончил работу.\n");
    return 0;
}

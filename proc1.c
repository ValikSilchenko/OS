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
int N = 200;

void* func(void* args) {
    struct mq_attr attr;
    int oflag = O_CREAT | O_WRONLY;
    attr.mq_maxmsg = 15;
    attr.mq_msgsize = N;
    mqd_t q_id = mq_open(name, oflag, S_IWUSR | S_IRUSR, &attr);
    if (q_id == -1) {
        perror("mq_open error");
        exit(errno);
    }
    attr.mq_flags = O_NONBLOCK;
    mq_setattr(q_id, &attr, NULL);
    attr.mq_msgsize = 0;
    attr.mq_maxmsg = 0;
    mq_getattr(q_id, &attr);
    printf("Изменённые атрибуты: maxmsg - %ld, msgsize - %ld\n", attr.mq_maxmsg, attr.mq_msgsize);

    char buf[N];
    struct timespec time;

    while (flag == 0) {
        getdomainname(buf, N);
        N = strlen(buf);
        clock_gettime(CLOCK_REALTIME, &time);
        time.tv_sec += 5;
        mq_getattr(q_id, &attr);
        printf("Количество сообщений в очереди: %ld\n", attr.mq_curmsgs);
        if (mq_timedsend(q_id, buf, N, NULL, &time) == -1)
            perror("mq_timedsend error");
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

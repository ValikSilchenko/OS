#include <pthread.h>
#include <stdio.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


int flag = 0;
char name[] = "/queue";
int N = 100;

void* func(void* args) {
    struct mq_attr attr;
    attr.mq_msgsize = 8200;
    mqd_t q_id = mq_open(name, O_CREAT | O_RDWR, 0666, NULL);
    if (q_id == -1) {
        perror("mq_open error");
        exit(errno);
    }
    char buf[N];
    while (flag == 0) {
        getdomainname(buf, N);
        N = strlen(buf);
        printf("%s\n", buf);
        if (mq_send(q_id, buf, N, 1) == -1)
            perror("mq_send error");
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

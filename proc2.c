#include <pthread.h>
#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int flag = 0;
char name[] = "/queue";

void* func(void* args) {
    struct mq_attr attr;
    mqd_t q_id = mq_open(name, O_CREAT | O_RDONLY, S_IWUSR | S_IRUSR, NULL);
    mq_getattr(q_id, &attr);
    long int N = attr.mq_msgsize;
    if (q_id == -1) {
        perror("mq_open error");
        exit(errno);
    }
    char buf[N];
    uint c = 1;
    while (flag == 0) {
        mq_getattr(q_id, &attr);
        N = attr.mq_msgsize;
        printf("%ld - ", N);
        if (mq_receive(q_id, buf, N, NULL) == -1)
            perror("mq_receive error");
        else
            printf("%s\n", buf);
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

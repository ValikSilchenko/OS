#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main() {
    printf("Программа начала работу.\n");

    char buf[50], old[50];
    gethostname(old, 50);

    pid_t id = fork();
    if (id == -1)
        perror("fork");
    else if (id == 0) {
        unshare(CLONE_NEWUTS);
        printf("\nДочерний процесс создан в изолированном пространстве имён UTS.\n");
        gethostname(buf, 50);
        printf("Hostname до изменения внутри изолированного пространства: %s\n", buf);

        if (sethostname("Test", 4) == -1)
            perror("sethostname");
        gethostname(buf, 50);

        printf("Hostname после изменения внутри изолированного пространства: %s\n", buf);
        printf("Дочерний процесс закончил работу.\n\n");
    } else {
        int status;
        waitpid(id, &status, 0);
        gethostname(buf, 50);
        printf("Hostname в родительском процессе: %s\n", buf);

        id = fork();
        if (id == -1)
            perror("fork");
        else if (id == 0) {
            printf("\nДочерний процесс создан в разделяемом пространстве имён UTS.\n");
            gethostname(buf, 50);
            printf("Hostname до изменения внутри разделяемого пространства: %s\n", buf);

            if (sethostname("Test", 4) == -1)
                perror("sethostname");
            gethostname(buf, 50);

            printf("Hostname после изменения внутри разделяемого пространства: %s\n", buf);
            printf("Дочерний процесс закончил работу.\n\n");
        } else {
            waitpid(id, &status, 0);
            gethostname(buf, 50);
            printf("Hostname в родительском процессе: %s\n", buf);
            sethostname(old, strlen(old));
            printf("Родительский процесс закончил работу.\n");
        }
    }
    return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main() {
    printf("Программа начала работу.\n");

    printf("Id процесса программы: %d\n", getpid());
    printf("Id процесса-родителя программы: %d\n", getppid());
    pid_t pid = fork();
    int res;
    if (pid == 0) {
        char buf[50];
        realpath("proc1", buf);
        if (execlp(buf, "Program", "children", NULL) == -1)
            printf("Ошибка exec: %s\n", strerror(errno));
    } else {
        int c = waitpid(pid, &res, WNOHANG);
        while (c == 0) {
            printf("Ожидание завершения дочернего процесса.\n");
            usleep(5 * 100000);
            c = waitpid(pid, &res, WNOHANG);
        }
        if (c == -1)
            printf("Error: %s\n", strerror(errno));
        else if (WIFEXITED(res))
            printf("Код завершения дочернего процесса: %d\n", WEXITSTATUS(res));
        else if (WIFSIGNALED(res))
            printf("Дочерний процесс был прерван сигналом %d\n", WTERMSIG(res));
        else
            printf("Ошибка.\n");
    }

    printf("Программа закончила работу.\n");
    return 3;
}
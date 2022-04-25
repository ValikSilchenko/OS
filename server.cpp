#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Программа начала работу.\n");

    pid_t id = fork();
    if (id == -1)
        perror("fork");
    else if (id == 0) {
    // child
    } else {
        waitpid(id, )
    }

    printf("Программа закончила работу.\n");
    return 0;
}
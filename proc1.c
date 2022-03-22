#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    printf("Дочерний процесс начал работу.\n");

    printf("Id дочернего процесса: %d\n", getpid());
    printf("Id процесса-родителя дочернего процесса: %d\n", getppid());
    for (int i = 0; i < argc; i++) {
        printf("Аргумент %d: %s\n", i, argv[i]);
        sleep(1);
    }

    printf("Дочерний процесс закончил работу.\n");
    return 0;
}
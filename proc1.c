#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    printf("Программа 1 начала работу.\n");
    for (int i = 0; i < argc; i++) {
        printf("Аргумент %d: %s\n", i, argv[i]);
        sleep(1);
    }
    printf("Id родителя программы 1: %d\n", getppid());
    printf("Id программы 1: %d\n", getpid());
    printf("Программа 1 закончила работу.\n");
    return 1;
}
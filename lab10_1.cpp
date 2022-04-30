#include <cstdio>
#include <ucontext.h>
#include <unistd.h>
#include <fcntl.h>

ucontext_t *utc1, *utc2, *utcM;

void func1() {
    while (1) {
        printf("Coroutine 1.\n");
        sleep(1);
        swapcontext(utc1, utc2);
    }
}

void func2() {
    while (1) {
        printf("Coroutine 2.\n");
        sleep(1);
        if (getchar() == 10)
            swapcontext(utc2, utcM);
        else
            swapcontext(utc2, utc1);
    }
}

void createCoroutine(ucontext_t *utc, void (*func) ()) {
    if (getcontext(utc) == -1) {
        perror("getcontext");
        return;
    }
    char* stack = new char[16 * 1024];
    utc->uc_stack.ss_sp = stack;
    utc->uc_stack.ss_size = 16 * 1024;
    makecontext(utc, func, 0);
}

void destroyCoroutine(ucontext_t* utc) {
    delete [] utc->uc_stack.ss_sp;
    delete utc;
}

int main() {
    printf("Программа начала работу.\n");

    utc1 = new ucontext_t;
    createCoroutine(utc1, func1);
    utc2 = new ucontext_t;
    createCoroutine(utc2, func2);

    int flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    utcM = new ucontext_t;
    if (getcontext(utcM) == -1)
        perror("getcontext");
    if (swapcontext(utcM, utc1) == -1)
        perror("swapcontext");

    destroyCoroutine(utc1);
    destroyCoroutine(utc2);
    delete utcM;
    fcntl(STDIN_FILENO, F_SETFL, flags);

    printf("Программа закончила работу.\n");
    return 0;
}
#include <cstdio>
#include <ucontext.h>
#include <unistd.h>
#include <fcntl.h>
#include <queue>

ucontext_t *utc1, *utc2, *utcM, *utcD, *active = nullptr;
std::queue<ucontext_t*> qu;

void func1() {
    while (1) {
        printf("Coroutine 1.\n");
        sleep(1);
        swapcontext(utc1, utcD);
    }
}

void func2() {
    while (1) {
        printf("Coroutine 2.\n");
        sleep(1);
        if (getchar() == 10)
            swapcontext(utc2, utcM);
        else
            swapcontext(utc2, utcD);
    }
}

void dispatcher() {
    while (1) {
        if (active != nullptr)
            qu.push(active);
        active = qu.front();
        qu.pop();
        swapcontext(utcD, active);
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
    qu.push(utc);
}

void destroyCoroutine(ucontext_t* utc) {
    delete [] utc->uc_stack.ss_sp;
    delete utc;
}

int main() {
    printf("Программа начала работу.\n");

    utcD = new ucontext_t;
    createCoroutine(utcD, dispatcher);
    qu.pop();
    utc1 = new ucontext_t;
    createCoroutine(utc1, func1);
    utc2 = new ucontext_t;
    createCoroutine(utc2, func2);

    int flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    utcM = new ucontext_t;
    if (getcontext(utcM) == -1)
        perror("getcontext");
    if (swapcontext(utcM, utcD) == -1)
        perror("swapcontext");

    destroyCoroutine(utc1);
    destroyCoroutine(utc2);
    destroyCoroutine(utcD);
    delete utcM;
    fcntl(STDIN_FILENO, F_SETFL, flags);

    printf("Программа закончила работу.\n");
    return 0;
}
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <fcntl.h>
#include <queue>
#include <string>
#include <strings.h>

int servSoc;
char name[] = "mysock";
std::queue<std::string> msgs;
bool receivingFl = true, handlingFl = true, awaitingFl = true;

void* receiving(void* args) {

    pthread_exit(nullptr);
}

void* handling(void* args) {

    pthread_exit(nullptr);
}

void* awaiting(void* args) {
    printf("Ожидание соединения.\n");
    int s;
    while (awaitingFl) {
        s = accept(servSoc, )
    }
}

int main() {
    printf("Серверная программа начала работу.\n");

    struct sockaddr_un addr;
    bzero(&addr, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, name);
    if ((servSoc = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("server socket");
        exit(errno);
    }
    fcntl(servSoc, F_SETFL, O_NONBLOCK);
    int optVal = 1;
    setsockopt(servSoc, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    bind(servSoc, (struct sockaddr*) &addr, sizeof(struct sockaddr));
    listen(servSoc, 1);

    pthread_t id;
    pthread_create(&id, nullptr, awaiting, nullptr);

    printf("Ожидание нажатия клавиши.\n");
    getchar();
    printf("Клавиша нажата.\n");

    awaitingFl = false;
    pthread_join(id, nullptr);

    printf("Серверная программа закончила работу.\n");
    return 0;
}
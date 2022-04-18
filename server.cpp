#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <fcntl.h>
#include <vector>
#include <strings.h>
#include <unistd.h>

int servSoc, clientSoc, bufSize = 10;
char name[] = "mycock";
bool receivingFl = true, handlingFl = true, awaitingFl = true;
struct sockaddr_un clAddr;
socklen_t len = sizeof(clAddr);
pthread_t idAwt, idHndl, idRcv;
pthread_mutex_t mutex;
std::vector<int> queue(bufSize);

void* receiving(void* args) {
    long received;
    uint recvbuf;
    while (receivingFl) {
        if ((received = recv(clientSoc, &recvbuf, sizeof(recvbuf), 0)) == -1) {
            perror("receive");
            sleep(1);
        } else if (received == 0) {
            printf("Разъединение.\n");
            sleep(1);
        } else {
            pthread_mutex_lock(&mutex);
            queue.insert(queue.begin(), recvbuf);
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(nullptr);
}

void* handling(void* args) {
    char temp[50];
    uint N;
    while (handlingFl) {
        pthread_mutex_lock(&mutex);
        if (!queue.empty()) {
            printf("Запрос %d принят.\n", queue.back());
            queue.pop_back();
            pthread_mutex_unlock(&mutex);
            getdomainname(temp, 50);
            N = strlen(temp);
            if ((send(clientSoc, temp, N, 0)) == -1) {
                perror("send");
                sleep(1);
            } else
                printf("Отправлено: %s\n", temp);
        } else {
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
    }
    pthread_exit(nullptr);
}

void* awaiting(void* args) {
    printf("Ожидание соединения.\n");
    while (awaitingFl) {
        if ((clientSoc = accept(servSoc, (sockaddr*)(&clAddr), &len)) == -1) {
            perror("accept");
            sleep(1);
        } else {
            printf("Соединение установлено.\n");
            pthread_create(&idRcv, nullptr, receiving, nullptr);
            pthread_create(&idHndl, nullptr, handling, nullptr);
            pthread_exit(nullptr);
        }
    }
    pthread_exit(nullptr);
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
    if (setsockopt(servSoc, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)))
        perror("setsockopt");
    if (bind(servSoc, (struct sockaddr*) &addr, sizeof(struct sockaddr)))
        perror("bind");
    if (!listen(servSoc, 1)) {

        pthread_mutex_init(&mutex, nullptr);
        pthread_create(&idAwt, nullptr, awaiting, nullptr);

        printf("Ожидание нажатия клавиши.\n");
        getchar();
        printf("Клавиша нажата.\n");

        awaitingFl = false;
        receivingFl = false;
        handlingFl = false;
        pthread_join(idAwt, nullptr);
        pthread_join(idRcv, nullptr);
        pthread_join(idHndl, nullptr);
        pthread_mutex_destroy(&mutex);

        shutdown(clientSoc, SHUT_RDWR);
    } else
        perror("listen");

    shutdown(servSoc, SHUT_RDWR);
    close(servSoc);
    close(clientSoc);
    if (remove(name))
        perror("unlink");

    printf("Серверная программа закончила работу.\n");
    return 0;
}
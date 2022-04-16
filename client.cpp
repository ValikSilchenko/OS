#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

int soc, serverSoc, bufSize = 10, sendCntr = 0, receiveCntr = 0;
char name[] = "mycock";
bool receivingFl = true, sendingFl = true, connectionFl = true;
socklen_t len;
pthread_t idCon, idSnd, idRcv;

void* receiving(void* args) {
    uint N = 50;
    char buf[N];
    long received;
    while (receivingFl) {
        if ((received = recv(serverSoc, buf, N, 0)) == -1) {
            perror("receive");
            sleep(1);
        } else if (received == 0) {
            printf("Разъединение.\n");
            sleep(1);
        } else {
            receiveCntr++;
            printf("Ответ %d получен: %s", receiveCntr, buf);
        }
    }
    pthread_exit(nullptr);
}

void* sending(void* args) {
    while (sendingFl) {
        sendCntr++;
        if ((send(serverSoc, &sendCntr, sizeof(sendCntr), 0)) == -1)
            perror("send");
        else
            printf("Запрос %d отправлен.\n", sendCntr);
        sleep(1);
    }
    pthread_exit(nullptr);
}

void* connection(void* args) {
    printf("Ожидание соединения.\n");
    struct sockaddr_un serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, name);
    while (connectionFl) {
        if ((serverSoc = connect(soc, (sockaddr*)(&serverAddr), sizeof(serverAddr))) == -1) {
            perror("connect");
            sleep(1);
        } else {
            printf("Соединение установлено.\n");
            pthread_create(&idSnd, nullptr, sending, nullptr);
            pthread_create(&idRcv, nullptr, receiving, nullptr);
            pthread_exit(nullptr);
        }
    }
    pthread_exit(nullptr);
}

int main() {
    printf("Клиентская программа начала работу.\n");

    if ((soc = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("client socket");
        exit(errno);
    }
    fcntl(soc, F_SETFL, O_NONBLOCK);
    int optVal = 1;
    setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));

    pthread_create(&idCon, nullptr, connection, nullptr);

    printf("Ожидание нажатия клавиши.\n");
    getchar();
    printf("Клавиша нажата.\n");

    connectionFl = false;
    receivingFl = false;
    sendingFl = false;
    pthread_join(idCon, nullptr);
    pthread_join(idRcv, nullptr);
    pthread_join(idSnd, nullptr);

    shutdown(soc, 2);
    close(soc);

    printf("Клиентская программа закончила работу.\n");
    return 0;
}
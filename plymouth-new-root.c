#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

void timeout_handler(int signum) {
    fprintf(stderr, "timeout\n");
    exit(124);
}

int main() {
    int sockfd;
    struct sockaddr_un addr;

    /* Create Unix domain socket */
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';
    strncpy(addr.sun_path + 1, "/org/freedesktop/plymouthd", sizeof(addr.sun_path) - 2);

    signal(SIGALRM, timeout_handler);
    alarm(3);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(sa_family_t) + 27) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    if (write(sockfd, "R\x02\t/sysroot", 12) < 0) {
        perror("write");
        close(sockfd);
        return 1;
    }

    close(sockfd);
    return 0;
}

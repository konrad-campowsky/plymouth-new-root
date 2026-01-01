#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int sockfd;
    struct sockaddr_un addr;
    ssize_t bytes_written;

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

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(sa_family_t) + 1 + strlen("/org/freedesktop/plymouthd")) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    char message[] = "R\x02\t/sysroot";
    bytes_written = write(sockfd, message, 12);

    if (write(sockfd, message, 12) < 0) {
        perror("write");
        close(sockfd);
        return 1;
    }

    close(sockfd);
    return 0;
}

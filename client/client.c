#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 256

int main(int argc, char* argv[])
{
    int sock_fd;
    struct sockaddr_in serv_name;
    int status;
    char buf[MAX_BUF];
    int nbytes;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s ip_address port_number\n", argv[0]);
        exit(1);
    }

    // create a socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Socket creation");
        exit(1);
    }

    // server address
    serv_name.sin_family = AF_INET;
    inet_aton(argv[1], &serv_name.sin_addr);
    serv_name.sin_port = htons(atoi(argv[2]));

    // connect to the server
    status = connect(sock_fd, (struct sockaddr *)&serv_name, sizeof(serv_name));
    if (status == -1) {
        perror("Connection error");
        exit(1);
    }

    while(1) {
        strcpy(buf, "hello");
        write(sock_fd, buf, strlen(buf));
        printf("[write] %s\n", buf);

        memset(buf, 0, sizeof(buf));
        nbytes = read(sock_fd, buf, sizeof(buf));
        if (nbytes == 0)
            break;
        else
            printf("[read] %s\n", buf);

        usleep(500000);
    }
    close(sock_fd);

    return 0;
}

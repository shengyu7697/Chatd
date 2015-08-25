#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 256

int main(int argc, char* argv[])
{
    int sock_fd, new_fd;
    socklen_t addrlen;
    struct sockaddr_in my_name, peer_name;
    int status;
    char buf[MAX_BUF];
    int nbytes;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s port_number\n", argv[0]);
        exit(1);
    }

    // create a socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // server address
    my_name.sin_family = AF_INET;
    my_name.sin_addr.s_addr = INADDR_ANY;
    my_name.sin_port = htons(atoi(argv[1]));

    status = bind(sock_fd, (struct sockaddr *)&my_name, sizeof(my_name));
    if (status == -1) {
        perror("Binding error");
        exit(1);
    }

    status = listen(sock_fd, 5);
    if (status == -1) {
        perror("Listening error");
        exit(1);
    }

    addrlen = sizeof(peer_name);

    while(1) {
        // wait for a connection
        new_fd = accept(sock_fd, (struct sockaddr *)&peer_name, &addrlen);
        printf("accecpt fd%d\n", new_fd);

        if (new_fd == -1) {
            perror("Wrong connection");
            exit(1);
        }

        while(1) {
            memset(buf, 0, sizeof(buf));
            nbytes = read(new_fd, buf, sizeof(buf));
            if (nbytes == 0)
                break;
            else
                printf("[read] %s\n", buf);

            sprintf(buf, "fd%d", new_fd);
            write(new_fd, buf, strlen(buf));
            printf("[write] %s\n", buf);
        }
        close(new_fd);
    }

    return 0;
}

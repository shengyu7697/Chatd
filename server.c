#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 256

int main(int argc, char* argv[])
{
    int sock_fd, new_fd, max_fd;
    socklen_t addrlen;
    struct sockaddr_in my_name, peer_name;
    int status;
    char recv_buf[MAX_BUF], send_buf[MAX_BUF];
    int nbytes;
    int i, ret;
    fd_set read_fds_master, read_fds;

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
    // set up variables for select
    max_fd = sock_fd;
    FD_ZERO(&read_fds_master);
    FD_SET(sock_fd, &read_fds_master);

    while(1)
    {
        // copy fd set
        read_fds = read_fds_master;
        ret = select(max_fd+1, &read_fds, NULL, NULL, NULL);
        if (ret == -1) {
            perror("Select");
            return -1;
        } else if (ret == 0) {
            printf("Select timeout\n");
            continue;
        }

        for (i = 0; i < max_fd+1; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == sock_fd) {
                    // wait for a connection
                    new_fd = accept(sock_fd, (struct sockaddr *)&peer_name, &addrlen);
                    printf("accecpt fd%d\n", new_fd);

                    if (new_fd == -1) {
                        perror("Wrong connection");
                        exit(1);
                    }

                    // add to fd set
                    FD_SET(new_fd, &read_fds_master);
                    if (new_fd > max_fd) {
                        max_fd = new_fd;
                    }
                } else {
                    // already connected socket
                    memset(recv_buf, 0, sizeof(recv_buf));
                    nbytes = read(i, recv_buf, sizeof(recv_buf));
                    if (nbytes == 0) {
                        // connection closed
                        close(i);
                        printf("client fd%d disconnect\n", i);

                        // remove from fd set
                        FD_CLR(i, &read_fds_master);
                        break;
                    } else {
                        printf("[read from fd%d] %s\n", i, recv_buf);
                    }

                    sprintf(send_buf, "[fd%d] %s", i, recv_buf);
                    write(i, send_buf, strlen(send_buf));
                    printf("[write to fd%d] %s\n", i, send_buf);
                }
            }
        }
    }

    return 0;
}
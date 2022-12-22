/*
This file is to check if the cycles are counted in during sleep() and epoll_wait()
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_PORT 2345
#define MAX_CONN 16
#define MAX_EVENTS 32
#define BUF_SIZE 16
#define MAX_LINE 256

static void set_sockaddr(struct sockaddr_in *addr)
{
    bzero((char *)addr, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(DEFAULT_PORT);
}

static void epoll_ctl_add(int epfd, int fd, uint32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl()\n");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    int epoll_fd;
    struct epoll_event *events;
    clock_t start, end;
    double cpu_time_used;
    int listen_sock;
    struct sockaddr_in srv_addr;

    start = clock();
    sleep(1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("It takes %f seconds calculated by cycles when sleep(1).\n",
           cpu_time_used);

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    set_sockaddr(&srv_addr);
    bind(listen_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    listen(listen_sock, MAX_CONN);
    epoll_fd = epoll_create(1);
    epoll_ctl_add(epoll_fd, listen_sock, EPOLLIN | EPOLLOUT | EPOLLET);
    start = clock();
    epoll_wait(epoll_fd, events, 1, 1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf(
        "It takes %f seconds calculated by cycles when epoll_wait(..., timeout=1).\n",
        cpu_time_used);
}

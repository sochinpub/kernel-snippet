#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/signalfd.h>


#define handle_error(msg) do {  \
    perror(msg);                \
    exit(EXIT_FAILURE);         \
} while(0)

int main(int argc, char *argv[])
{
    sigset_t mask;
    int sfd;
    struct signalfd_siginfo sfdi;
    ssize_t s;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        handle_error("sigprocmask failed");
    }

    sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) {
        handle_error("signalfd failed");
    }

    for(;;) {
        s = read(sfd, &sfdi, sizeof(struct signalfd_siginfo));
        if (s != sizeof(struct signalfd_siginfo)) {
            handle_error("read failed");
        } else if (sfdi.ssi_signo == SIGINT) {
            printf("Got SIGINT\n");
        } else if (sfdi.ssi_signo == SIGQUIT) {
            printf("Got SIGQUIT\n");
        } else {
            handle_error("Read unexpected signal\n");
        }

    }

    return 0;
}

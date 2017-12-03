#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

#include <sys/eventfd.h>

#define handle_error(msg) do {  \
    perror(msg);                \
    exit(1);                    \
} while(0)

int main(int argc, char *argv[])
{
    uint64_t u;
    ssize_t s;

    int efd, j;
    if (argc < 2) {
        fprintf(stderr, "input in the command argument\n");
        exit(1);
    }

    if ((efd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)) == -1) {
        handle_error("eventfd failed");
    }

    switch(fork()) {
    case 0:
            for (j = 1; j < argc; j++) {
                printf("child writing %s to efd\n", argv[j]);
                u = strtoull(argv[j], NULL, 0);
                s = write(efd, &u, sizeof(uint64_t));
                if (s != sizeof(uint64_t)) {
                    handle_error("write efd failed");
                }
            }
        exit(0);
    case -1:
        handle_error("fork failed");
    default:
        sleep(2);
        printf("parent ready to read\n");
        s = read(efd, &u, sizeof(uint64_t));
        if (s != sizeof(uint64_t)) {
            if (errno == EAGAIN) {
                printf("Parent read value %d\n", s);
                return 1;
            }
            handle_error("parent read failed");
        }
        printf("parent read %d, %llu (0x%llx) from efd\n", s, (unsigned long long)u, (unsigned long long)u);
        exit(0);
    }

    return 0;
}

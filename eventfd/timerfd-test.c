#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/timerfd.h>

#define handle_err(msg) do {    \
    perror(msg);                \
    exit(EXIT_FAILURE);         \
} while(0)

void print_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("print_time: current time: %ld.%ld\n", tv.tv_sec, tv.tv_usec);
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "input in initial_interval, interval and interval_iterations\n");
        exit(-1);
    }
    struct timespec now;
    if (clock_gettime(CLOCK_REALTIME, &now) == -1) {
        handle_err("clock_gettime failed");
    }

    struct itimerspec new_value;
    new_value.it_value.tv_sec = now.tv_sec + atoi(argv[1]);
    new_value.it_value.tv_nsec = now.tv_nsec;
    new_value.it_interval.tv_sec = atoi(argv[2]);
    new_value.it_interval.tv_nsec = 0;

    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1) {
        handle_err("timerfd_create failed");
    }

    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1) {
        handle_err("timerfd_settime failed");
    }
    print_time();
    printf("timer started\n");

    uint64_t tot_exp;
    for (tot_exp = 0; tot_exp < atoi(argv[3]);) {
        uint64_t exp;
        ssize_t s = read(fd, &exp, sizeof(uint64_t));
        if (s != sizeof(uint64_t)) {
            handle_err("read failed");
        }
        tot_exp += exp;
        print_time();
        printf("read: %llu, total = %llu\n", exp, tot_exp);
    }
    return 0;
}

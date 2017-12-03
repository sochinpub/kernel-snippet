#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
const char *program_name;


static void print_usage(FILE *stream, int exit_code)
{

}

int main(int argc, char *argv[])
{
    int next_option;

    const char * const short_options = "hl:";
    const struct option long_options[] = {
        {"help", 0, NULL, 'h'},
        {"load", 1, NULL, 'l'},
        {NULL, 0, NULL, 0}
    };
    program_name = argv[0];

    do {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);
        switch (next_option) {
        case 'h': /* -h or --help */
            print_usage(stdout, 0);
        case 'i': /* -i or --interval */
            // interval operation
            break;
        case 'd':
            // duration operation
            break;
        case '?': /* undefine options */
            //
            print_usage(stderr, 1);
        case -1:
            break;
        default: /* unexpected */
            abort();
        }
    }while(next_option != -1);

    return 0;
}

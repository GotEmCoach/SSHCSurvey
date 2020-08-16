#include <stdlib.h>
#include <stdio.h>
#include <libssh/libssh.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

void usage (FILE *fp, const char *path)
{

}


static int VERBOSE;
static const char *SSH_PASSWORD;
static FILE *logfile;

int main(int argc, char **argv) 
{
    int c;     
    ssh_session main_session = ssh_new();
    while(1)
    {
        int option_index = 0;
        static struct option long_options[] = 
        {
            {"username",    required_argument, NULL, 'U' },
            {"password",    required_argument, NULL, 'P' },
            {"port",        required_argument, NULL, 'p' },
            {"log",         required_argument, NULL, 'L' },
            {"host",        required_argument, NULL, 'H' },
            {"survey-file", optional_argument, NULL, 'f' },
            {"verbose",     no_argument,       &VERBOSE, 'v' },
            {NULL,          0,                 NULL,   0}
        };
        c = getopt_long(argc, argv, "U:P:p:L:H:f:", long_options, &option_index);
        if (c == -1)
            break;
        switch (c)
        {
            case 0:
                break;
            case 1:
                break;
            case 'U':
                ssh_options_set(main_session, SSH_OPTIONS_HOST, optarg); /*libssh API used here for setting up host to connect to */
                break;
            case 'P':
                SSH_PASSWORD = optarg;      /* saving password for later after I connect */
                break;
            case 'p':
                ssh_options_set(main_session, SSH_OPTIONS_PORT, optarg);
                break;
            case 'L':
                logfile = fopen(optarg, "w");
            default:
                break;
        }
    }




}






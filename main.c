#include <stdlib.h>
#include <stdio.h>
#include <libssh/libssh.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

void usage (FILE *fp, const char *path)
{

}


static int *VERBOSE;
static const char *SSH_PASSWORD;
static FILE *logfile;
static FILE *surveyfile;

int main(int argc, char **argv) 
{
    int option_index = 0;
    int c;
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
    ssh_session main_session = ssh_new();
    if (main_session == NULL)
        printf('Session could not be opened\n');
        exit(-1);
    c = getopt_long(argc, argv, "U:P:p:L:H:f:", long_options, &option_index);
    while(c != -1)
    {
        switch (c)
        {
            case 0:
                break;
            case 'U':
                ssh_options_set(main_session, SSH_OPTIONS_USER, optarg); /*libssh API used here for setting up user to use */
                break;
            case 'P':
                SSH_PASSWORD = malloc(sizeof(optarg) + 1);
                SSH_PASSWORD = optarg;                                   /* saving password for later after I connect */
                break;
            case 'p':
                ssh_options_set(main_session, SSH_OPTIONS_PORT, optarg); /* libssh API used here for setting up port to connect to */
                break;
            case 'L':
                logfile = fopen(optarg, "w"); /* open the logfile specified in getopts */
                break;
            case 'H':
                ssh_options_set(main_session, SSH_OPTIONS_HOST, optarg);
                break;
            case 'f':
                surveyfile = fopen(optarg, "r");
                break;
            case 'v':
                VERBOSE = 1;
                break;
            case ':':
                printf('Value missing for command.\n');
                break;
            default:
                printf('Missing options, Please refer to the help menu for guidance.\n');
                break;
        }
    }
    if (ssh_options_parse_config(main_session, NULL) < 0) /* if the ssh.conf is not good to go, exit */
    {
        printf('SSH config could not be read\n');
        exit(-1);
    }
    if (ssh_connect(main_session) != SSH_OK) /*Actual network connection here, will still have to authenticate. */
    {
        printf('Connection was not able to be made\n');
        exit(-1);
    }

}






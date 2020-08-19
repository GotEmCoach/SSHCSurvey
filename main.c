#include <stdlib.h>
#include <stdio.h>
#define LIBSSH_STATIC 1
#include <libssh/libssh.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

static int verbose_flag;

int main(int argc, char **argv) 
{
    ssh_session my_ssh_session;
    int c;
    int rc;
    char *host;
    char *password;
    int port;
    char *username;
    char *logfile;
    /*change to file later*/
    char *surveyfile;
    /*change to file later */

    struct option longopts[] = 
    {
        { "user",   required_argument,      NULL,   'U'},
        { "pass",   required_argument,      NULL,   'P'},
        { "port",   required_argument,      NULL,   'p'},
        { "logfile",required_argument,      NULL,   'L'},
        { "survey", required_argument,      NULL,   's'},
        { "verbose",no_argument,            &verbose_flag,   1},
        { "host",   required_argument,      NULL,   'H'},
        {0, 0, 0, 0}
    };
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "U:P:p:L:s:vH:", longopts, NULL)) != -1)
    {
        switch(c)
        {
            case 0:
            /* If this option set a flag, do nothing else now. */
                if (longopts[option_index].flag != 0)
                    break;
                printf ("option %s", longopts[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;
            case 'U':
                username = optarg;
                break;
            case 'P':
                password = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'L':
                logfile = optarg;
                break;
            case 's':
                surveyfile = optarg;
                break;
            case 'v':
                verbose_flag = 1;
                break;
            case 'H':
                host = optarg;
                break;
            case '?':
                break;
            default:
                abort();
        }
    }

    my_ssh_session = ssh_new();
    if (my_ssh_session == NULL) 
    {
        exit(-1);
    }

    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, username);
    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK)
    {
        printf("connect failed\n");
        exit(-1);
    }
    else
    {
        printf("good connect\n");
    }
    
    rc = ssh_userauth_password(my_ssh_session, NULL, password);
    if (rc != SSH_OK)
    {
        printf("auth failed\n");
        exit(-1);
    }
    else
    {
        printf("good auth\n");
    }
    

    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
}

#include <stdlib.h>
#include <stdio.h>
#define LIBSSH_STATIC 1
#include <libssh/libssh.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

static int verbose_flag;

struct winsize get_size()
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size;
}

int keyboard_hit()
{
    struct timeval tv = { 0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int interactive_shell_session(ssh_session main_session, ssh_channel main_channel)
{
    int rc;
    char error;
    void *err;
    char buffer[256];
    int nbytes, nwritten;
    rc = ssh_channel_open_session(main_channel);
    if (rc != SSH_OK)
    {
        ssh_channel_free(main_channel);
        printf("open session no workie");
        exit(-1);
    }
    rc = ssh_channel_request_pty(main_channel);
    if (rc != SSH_OK)
    {
        error = ssh_get_error(err);
        printf("Error: %c", error);
        exit(-1);
    }
    struct winsize size = get_size();
    rc = ssh_channel_change_pty_size(main_channel, size.ws_col, size.ws_row);
    if (rc != SSH_OK)
    {
        printf("size change no workie");
        exit(-1);
    }
    rc = ssh_channel_request_shell(main_channel);
    if (rc != SSH_OK)
    {
        printf("shell request no workie");
        exit(-1);
    }
    while (ssh_channel_is_open(main_channel) && !ssh_channel_is_eof(main_channel))
    {
        struct timeval timeout;
        ssh_channel in_channels[2], out_channels[2];
        fd_set file_descriptors;
        int max_file_descriptor;

        timeout.tv_sec = 30;
        timeout.tv_usec = 0;
        in_channels[0] = main_channel;
        in_channels[1] = NULL;
        FD_ZERO(&file_descriptors);
        FD_SET(0, &file_descriptors);
        FD_SET(ssh_get_fd(main_session), &file_descriptors);
        max_file_descriptor = ssh_get_fd(main_session) + 1;

        ssh_select(in_channels, out_channels, max_file_descriptor, &file_descriptors, &timeout);

        if (out_channels[0] != NULL)
        {
            nbytes = ssh_channel_read(main_channel, buffer, sizeof(buffer), 0);
            if (nbytes < 0) return SSH_ERROR;
            if (nbytes > 0)
            {
                nwritten = write(1, buffer, nbytes);
                if (nwritten != nbytes) return SSH_ERROR;
            }
        }
        if (FD_ISSET(0, &file_descriptors))
        {
            nbytes = read(0, buffer, sizeof(buffer));
            if (nbytes < 0) return SSH_ERROR;
            if (nbytes > 0)
            {
                nwritten = ssh_channel_write(main_channel, buffer, nbytes);
                if (nbytes != nwritten) return SSH_ERROR;
            }
        }
    }
    return rc;
}

int main(int argc, char **argv) 
{
    ssh_session main_session;
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

    main_session = ssh_new();
    if (main_session == NULL) 
    {
        exit(-1);
    }

    ssh_options_set(main_session, SSH_OPTIONS_HOST, host);
    ssh_options_set(main_session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(main_session, SSH_OPTIONS_USER, username);
    rc = ssh_connect(main_session);
    if (rc != SSH_OK)
    {
        printf("connect failed\n");
        exit(-1);
    }
    else
    {
        printf("good connect\n");
    }
    
    rc = ssh_userauth_password(main_session, NULL, password);
    if (rc != SSH_OK)
    {
        printf("auth failed\n");
        exit(-1);
    }
    else
    {
        printf("good auth\n");
    }
    ssh_channel main_channel;
    main_channel = ssh_channel_new(main_session);
    if (main_channel == NULL)
    {
        printf("channel no good");
        exit(-1);
    }

    rc = interactive_shell_session(main_session, main_channel);

    ssh_disconnect(main_session);
    ssh_free(main_session);
}



#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>
#include <getopt.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

/* *
 *  The deamon boilerplate used is based on:
 *  https://github.com/jirihnidek/daemon
 * */

#define DELAY 1

static char* pid_file;
static char* log_file;
static int running = 0;

int daemonize(char* pid_file) {
    int fd;
    pid_t pid;
    // Attempt to fork from current process
    if ((pid = fork()) < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // Promote child to session leader
    if (setsid < 0)
        exit(EXIT_FAILURE);

    // Ignore if child exits or hangs
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // fork again
    if ((pid = fork()) < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // set to complete file permissions and change working directory
    umask(0);
    chdir("/");

    // close all fds
    for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
		close(fd);
	}

    // redirect pipes
	stdin = fopen("/dev/null", "r");
	stdout = fopen("/dev/null", "w+");
	stderr = fopen("/dev/null", "w+");

    // open pid file
    if (pid_file != NULL) {
        char str[256];
        int fd = -1;
        if ((fd = open(pid_file, O_RDWR|O_CREAT, 0640)) < 0)
            exit(EXIT_FAILURE);
	    if (lockf(fd, F_TLOCK, 0) < 0)
	        exit(EXIT_FAILURE);
        sprintf(str, "%d\n", getpid());
        write(fd, str, strlen(str));
    }

    return 0;
}

void usage(char* name) {
    printf("\nUsage: %s [OPTIONS]\n\n", name);
    printf("'Power to the People' PTP² Server\n");
    printf("Options:\n");
    printf("  -h  --help                Print help info\n");
    printf("  -d  --daemonize           Run this application as a daemon\n");
    printf("  -p  --pid       filename  PID file used to daemonize app\n");
    printf("\n");
}

int main(int argc, char** argv) {
    
    FILE* log;

    static struct option options[] = {
        { "p",   required_argument,  0, 'p' },
        { "d",   no_argument,        0, 'b' },
        { NULL,  0,                  0, 0 }
    };

    int daemonized, value, option_index = 0, ret;
    while ((value = getopt_long(argc, argv, "c:l:t:p:dh", options, &option_index)) != -1) {
        switch (value) {
            case 'p':
                pid_file = strdup(optarg);
                break;
            case 'l':
                log_file = strdup(optarg);
                break;
            case 'd':
                daemonized = 1;
                break;
            case 'h':
                usage(argv[0]);
                return EXIT_SUCCESS;
            default:
                break;
        }
    }

    if (daemonized == 1) {
        daemonize(pid_file);
    }
    
    openlog(argv[0], LOG_PID|LOG_CONS, LOG_DAEMON);
    syslog(LOG_INFO, "Started %s", argv[0]);
    
    signal(SIGINT, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    if (log_file != NULL) {
        log = fopen(log_file, "a+");
        if (log == NULL) {
            syslog(LOG_ERR, "Unable to open log file: %s! Error: %s", log_file, strerror(errno));
            log = stdout;
        }
    } else {
        log = stdout;
    }
    
    running = 1;

    while (running == 1) {
        ret = fprintf(log, "Debug\n");
		if (ret < 0) {
			syslog(LOG_ERR, "Can not write to log stream: %s, error: %s", (log == stdout) ? "stdout" : log_file, strerror(errno));
			break;
		}
		ret = fflush(log);
		if (ret != 0) {
			syslog(LOG_ERR, "Can not fflush() log stream: %s, error: %s", (log == stdout) ? "stdout" : log_file, strerror(errno));
			break;
		}

        // This is where the logic goes
        
        

        sleep(DELAY);
    }
    
    // Cleanup Routines
    // close log file
    if (log != stdout)
        fclose(log);
    
    syslog(LOG_INFO, "Stopped %s", argv[0]);
    closelog();
    
    // free memory used to store filenames
    if (log_file != NULL) free(log_file);
    if (pid_file != NULL) free(pid_file);

    return EXIT_SUCCESS;
}

//
// Created by nilfer on 11/18/25.
//
#include "logger.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

static int pipe_fd[2]; //because pipe_fd and logger_pid is only needed in this file we use static so that it can prevent bugs
static pid_t logger_pid = -1;

int create_log_process()
{
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return -1;
    }

    logger_pid = fork();

    if (logger_pid < 0) {
        perror("fork");
        return -1;
    }

    // child (logger) process
    if (logger_pid == 0) {
        close(pipe_fd[1]);   // logger only reads

        FILE *log = fopen("gateway.log", "a");
        if (!log) { //if the opening file fail log points to NULL and we say if not NULL do this
            perror("log file");
            return -1;
        }

        char buffer[256];
        int seq = 0;

        while (1) { //so basically infinite loop till we get end
            int index = 0;


            while (read(pipe_fd[0], &buffer[index], 1) == 1) { // read characters until newline so till \n// read characters until newline so till \n
                if (buffer[index] == '\n') {
                    buffer[index] = '\0'; //when we are done we place a \0. so we have a complete line
                    break;
                }
                index++;
            }


            if (strcmp(buffer, "END") == 0) // this is where we break out the infitie while lope above
                break;


            time_t now = time(NULL);
            char *ts = ctime(&now); //to convert the number into a human-readable string
            ts[strlen(ts) - 1] = '\0'; //apparently ctime() always ends with a newline, so i replace it with \0.


            fprintf(log, "%d - %s - %s\n", seq++, ts, buffer);
            fflush(log);
        }

        fclose(log);
        close(pipe_fd[0]);
        _exit(0); //we use _exit() instead exit() cuz it doesn't flush parent buffers. exit() is dangerous after fork()
    }

    // parent process
    close(pipe_fd[0]); // parent only writes
    return 0;
}

int write_to_log_process(char *msg)
{
    write(pipe_fd[1], msg, strlen(msg));
    write(pipe_fd[1], "\n", 1);
    return 0;
}

int end_log_process()
{
    write(pipe_fd[1], "END\n", 4);
    close(pipe_fd[1]);
    waitpid(logger_pid, NULL, 0);
    return 0;
}

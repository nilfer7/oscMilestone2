//
// Created by nilfer on 11/18/25.
//
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main() {
    int myPipe[2];                  // an array of integers myPipe[0] = read end, myPipe[1] = write end
    char message[] = "Hi There";

    // 1. Create pipe
    if (pipe(myPipe) == -1) {
        perror("pipe failed");
        return -1;
    }

    // 2. Fork (create child)
    pid_t pid = fork();  //pid_t is a data type (an integer type) used to hold process IDs.

    if (pid < 0) {
        perror("fork failed");
        return -1;
    }


    // PARENT PROCESS
    if (pid > 0) {
        close(myPipe[0]);  // parent closes read end

        write(myPipe[1], message, strlen(message) + 1); // we copy our string (message) in the write end of the myPipe and at the end we declare how long it should be and we add +1 for '\0'
        close(myPipe[1]);  // done writing
    }

    // CHILD PROCESS
    else { //cuz in the child fork returns 0
        close(myPipe[1]);  // child closes write end

        char buffer[100];
        read(myPipe[0], buffer, sizeof(buffer));
        close(myPipe[0]);

        /*
        // Reverse case of each letter
        for (int i = 0; buffer[i] != '\0'; i++) { //as long as the charachter is not the last one which is \0
            if (islower(buffer[i])) //if the selected char small char we switch to big
                buffer[i] = toupper(buffer[i]);
            else if (isupper(buffer[i]))
                buffer[i] = tolower(buffer[i]);
        }
        */

        // i wanted to use pointers
        char *p = buffer;        // p points at the first character of buffer

        // Loop until *p (the char pointed to) is the string terminator '\0'
        while (*p != '\0') {
            if (islower(*p))
                *p = toupper(*p);
            else if (isupper(*p))
                *p = tolower(*p);

            p++;  // we set the pointer to the next character
        }



        printf("Child received: %s\n", buffer);
    }

    return 0;
}

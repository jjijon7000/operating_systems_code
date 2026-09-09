#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        printf("child: output before close\n");
        fflush(stdout);
        close(STDOUT_FILENO);
        errno = 0;
        if (printf("child: output after close\n") < 0) {
            dprintf(STDERR_FILENO, "printf failed after close: %s\n", strerror(errno));
        }
        _exit(EXIT_SUCCESS);
    }

    if (waitpid(pid, NULL, 0) < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    printf("parent: the child could no longer write to standard output.\n");
    return 0;
}

/*********************************************
    Your name: julio
    Question 7: after the child closes STDOUT_FILENO, printf() cannot write
    to standard output and fails when the output is flushed. the parent still
    has its own standard output descriptor.
*********************************************/
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
        errno = 0;
        if (wait(NULL) < 0) {
            printf("child: wait failed with errno %d (%s)\n",
                   errno, strerror(errno));
        }
        printf("child: exiting with status 42\n");
        return 42;
    }

    int status;
    pid_t waited = wait(&status);
    if (waited < 0) {
        perror("wait");
        return EXIT_FAILURE;
    }
    printf("parent: wait returned child PID %ld\n", (long)waited);
    if (WIFEXITED(status)) {
        printf("parent: child exit status was %d\n", WEXITSTATUS(status));
    }

    errno = 0;
    if (wait(NULL) < 0) {
        printf("a second wait in the parent fails with errno %d (%s)\n",
               errno, strerror(errno));
    }
    return 0;
}

/*********************************************
    Your name: julio
    Question 5: in the parent, wait() returns the child's PID and its exit
    status. in the child, wait() fails with ECHILD because it has no children.
*********************************************/
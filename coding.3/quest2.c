#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    const char *filename = "quest2.output";
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, 0644);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(fd);
        return EXIT_FAILURE;
    }

    const char *message = pid == 0 ? "child wrote using the inherited fd\n"
                                   : "parent wrote using the inherited fd\n";
    if (dprintf(fd, "%s", message) < 0) {
        perror("dprintf");
    }

    if (pid != 0) {
        if (waitpid(pid, NULL, 0) < 0) {
            perror("waitpid");
            close(fd);
            return EXIT_FAILURE;
        }
    }
    close(fd);
    printf("Both processes could access the inherited descriptor; see %s.\n", filename);
    return 0;
}

/*********************************************
    Your name: julio
    Question 2: both processes can use the file descriptor because fork()
    copies it to the child. they share the same open file, and O_APPEND makes
    each write go to the end of the file.
*********************************************/
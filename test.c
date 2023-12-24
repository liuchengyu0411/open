#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s file1 file2 target_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int source_fd1, source_fd2, target_fd;
    ssize_t nread;
    char buffer[BUFFER_SIZE];

    source_fd1 = open(argv[1], O_RDONLY);
    if (source_fd1 == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    target_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (target_fd == -1) {
        perror("open");
        close(source_fd1);
        exit(EXIT_FAILURE);
    }

    while ((nread = read(source_fd1, buffer, sizeof(buffer))) > 0) {
        if (write(target_fd, buffer, nread) != nread) {
            perror("write");
            close(source_fd1);
            close(target_fd);
            exit(EXIT_FAILURE);
        }
    }

    source_fd2 = open(argv[2], O_RDONLY);
    if (source_fd2 == -1) {
        perror("open");
        close(source_fd1);
        close(target_fd);
        exit(EXIT_FAILURE);
    }

    while ((nread = read(source_fd2, buffer, sizeof(buffer))) > 0) {
        if (write(target_fd, buffer, nread) != nread) {
            perror("write");
            close(source_fd1);
            close(source_fd2);
            close(target_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (nread == -1) {
        perror("read");
        close(source_fd1);
        close(source_fd2);
        close(target_fd);
        exit(EXIT_FAILURE);
    }

    close(source_fd1);
    close(source_fd2);
    close(target_fd);

    target_fd = open(argv[3], O_RDONLY);
    if (target_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while ((nread = read(target_fd, buffer, sizeof(buffer))) > 0) {
        if (write(STDOUT_FILENO, buffer, nread) != nread) {
            perror("write");
            close(target_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (nread == -1) {
        perror("read");
        close(target_fd);
        exit(EXIT_FAILURE);
    }

    close(target_fd);
    return 0;
}

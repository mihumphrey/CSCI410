#include <iostream>
#include <stdio.h>
#include <fcntl.h>

int main() {
    char buf[100];
    int fd0 = open("foo.txt", O_RDONLY);
    int fd1 = open("bar.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    printf("Hola!\n");
    dup2(fd1, STDOUT_FILENO);
    printf("Como estas?\n");
    dup2(fd0, STDIN_FILENO);
    for (;;) {
        ssize_t bytes_read = read(STDIN_FILENO, buf, sizeof(buf));
        if (bytes_read == 0)
            return 0;
        ssize_t bytes_to_write = bytes_read;
        while (bytes_to_write > 0)
            bytes_to_write -= write(STDOUT_FILENO, buf + bytes_read - bytes_to_write, bytes_to_write);
    }
    return 0;
}
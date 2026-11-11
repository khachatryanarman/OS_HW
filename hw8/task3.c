#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define PATH_MAX_LEN 4096

static int read_line(const char *prompt, char *buf, size_t size)
{
    if (write(STDOUT_FILENO, prompt, strlen(prompt)) < 0) {
        perror("write (prompt)");
        return -1;
    }

    size_t len = 0;
    while (len < size - 1) {
        char c;
        ssize_t n = read(STDIN_FILENO, &c, 1);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("read (stdin)");
            return -1;
        }
        if (n == 0 || c == '\n')
            break;
        buf[len++] = c;
    }
    buf[len] = '\0';

    if (len == 0) {
        fprintf(stderr, "Error: empty path\n");
        return -1;
    }
    return 0;
}

static int write_byte(int fd, char c)
{
    for (;;) {
        ssize_t w = write(fd, &c, 1);
        if (w == 1) return 0;
        if (w < 0 && errno == EINTR) continue;
        return -1;
    }
}

int main(void)
{
    char path[PATH_MAX_LEN];

    if (read_line("Enter path to a text file: ", path, sizeof path) < 0)
        return EXIT_FAILURE;

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    if (size == (off_t)-1) {
        perror("lseek (SEEK_END)");
        close(fd);
        return EXIT_FAILURE;
    }

    for (off_t pos = size - 1; pos >= 0; pos--) {
        if (lseek(fd, pos, SEEK_SET) == (off_t)-1) {
            perror("lseek (SEEK_SET)");
            close(fd);
            return EXIT_FAILURE;
        }

        char c;
        ssize_t n;
        do {
            n = read(fd, &c, 1);
        } while (n < 0 && errno == EINTR);

        if (n < 0) {
            perror("read");
            close(fd);
            return EXIT_FAILURE;
        }
        if (n == 0) {
            fprintf(stderr, "Unexpected EOF at offset %lld\n", (long long)pos);
            close(fd);
            return EXIT_FAILURE;
        }

        if (write_byte(STDOUT_FILENO, c) < 0) {
            perror("write (stdout)");
            close(fd);
            return EXIT_FAILURE;
        }
    }

    if (write_byte(STDOUT_FILENO, '\n') < 0) {
        perror("write (newline)");
        close(fd);
        return EXIT_FAILURE;
    }

    if (close(fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

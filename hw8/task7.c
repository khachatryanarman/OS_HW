#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define PATH_MAX_LEN 4096
#define CHUNK_SIZE 4096

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

static ssize_t read_full(int fd, char *buf, size_t count)
{
    size_t total = 0;
    while (total < count) {
        ssize_t n = read(fd, buf + total, count - total);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0)
            break;
        total += (size_t)n;
    }
    return (ssize_t)total;
}

int main(void)
{
    char path1[PATH_MAX_LEN];
    char path2[PATH_MAX_LEN];

    if (read_line("Enter first file path: ", path1, sizeof path1) < 0)
        return 2;
    if (read_line("Enter second file path: ", path2, sizeof path2) < 0)
        return 2;

    int fd1 = open(path1, O_RDONLY);
    if (fd1 < 0) {
        perror("open (first file)");
        return 2;
    }

    int fd2 = open(path2, O_RDONLY);
    if (fd2 < 0) {
        perror("open (second file)");
        close(fd1);
        return 2;
    }

    char buf1[CHUNK_SIZE];
    char buf2[CHUNK_SIZE];
    long long offset = 0;
    int result = 0;

    for (;;) {
        ssize_t n1 = read_full(fd1, buf1, sizeof buf1);
        if (n1 < 0) {
            perror("read (first file)");
            result = 2;
            break;
        }

        ssize_t n2 = read_full(fd2, buf2, sizeof buf2);
        if (n2 < 0) {
            perror("read (second file)");
            result = 2;
            break;
        }

        ssize_t common = (n1 < n2) ? n1 : n2;
        ssize_t i;
        for (i = 0; i < common; i++) {
            if (buf1[i] != buf2[i])
                break;
        }

        if (i < common) {
            printf("Files differ at byte %lld\n", offset + i);
            result = 1;
            break;
        }

        if (n1 != n2) {
            printf("Files differ at byte %lld\n", offset + common);
            result = 1;
            break;
        }

        if (n1 == 0) {
            printf("Files are identical\n");
            break;
        }

        offset += n1;
    }

    if (close(fd1) < 0) {
        perror("close (first file)");
        result = 2;
    }
    if (close(fd2) < 0) {
        perror("close (second file)");
        result = 2;
    }

    return result;
}

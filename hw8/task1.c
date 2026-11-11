#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUF_SIZE 4096
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

static ssize_t write_all(int fd, const char *buf, size_t count)
{
    size_t total = 0;
    while (total < count) {
        ssize_t w = write(fd, buf + total, count - total);
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        total += (size_t)w;
    }
    return (ssize_t)total;
}

int main(void)
{
    char src_path[PATH_MAX_LEN];
    char dst_path[PATH_MAX_LEN];

    if (read_line("Enter source file path: ", src_path, sizeof src_path) < 0)
        return EXIT_FAILURE;
    if (read_line("Enter destination file path: ", dst_path, sizeof dst_path) < 0)
        return EXIT_FAILURE;

    int src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) {
        perror("open (source)");
        return EXIT_FAILURE;
    }

    int dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        perror("open (destination)");
        close(src_fd);
        return EXIT_FAILURE;
    }

    char buf[BUF_SIZE];
    long long total = 0;
    int status = EXIT_SUCCESS;

    for (;;) {
        ssize_t n = read(src_fd, buf, sizeof buf);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("read (source)");
            status = EXIT_FAILURE;
            break;
        }
        if (n == 0)
            break;

        if (write_all(dst_fd, buf, (size_t)n) < 0) {
            perror("write (destination)");
            status = EXIT_FAILURE;
            break;
        }
        total += n;
    }

    if (close(src_fd) < 0) {
        perror("close (source)");
        status = EXIT_FAILURE;
    }
    if (close(dst_fd) < 0) {
        perror("close (destination)");
        status = EXIT_FAILURE;
    }

    if (status == EXIT_SUCCESS)
        printf("Copied %lld bytes from '%s' to '%s'\n", total, src_path, dst_path);
    else
        fprintf(stderr, "Copy failed after %lld bytes\n", total);

    return status;
}

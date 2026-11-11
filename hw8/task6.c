#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FILENAME "numbers.txt"
#define TARGET_LINE 4
#define NEW_TEXT "100\n"

static int write_all(int fd, const char *buf, size_t count)
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
    return 0;
}

static int read_all(int fd, char *buf, size_t count)
{
    size_t total = 0;
    while (total < count) {
        ssize_t n = read(fd, buf + total, count - total);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0) {
            errno = EIO;
            return -1;
        }
        total += (size_t)n;
    }
    return 0;
}

static int fail(const char *msg, int fd, char *mem)
{
    perror(msg);
    free(mem);
    if (fd >= 0) close(fd);
    return EXIT_FAILURE;
}

int main(void)
{
    int fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return fail("open (create)", -1, NULL);

    for (int i = 1; i <= 10; i++) {
        char line[16];
        int len = snprintf(line, sizeof line, "%d\n", i);
        if (write_all(fd, line, (size_t)len) < 0)
            return fail("write (initial)", fd, NULL);
    }

    if (close(fd) < 0)
        return fail("close (create)", -1, NULL);

    fd = open(FILENAME, O_RDWR);
    if (fd < 0)
        return fail("open (read/write)", -1, NULL);

    off_t line_start = (TARGET_LINE == 1) ? 0 : -1;
    off_t line_end = -1;
    off_t offset = 0;
    int newlines = 0;
    char chunk[256];

    while (line_end < 0) {
        ssize_t n = read(fd, chunk, sizeof chunk);
        if (n < 0) {
            if (errno == EINTR) continue;
            return fail("read (scan)", fd, NULL);
        }
        if (n == 0)
            break;
        for (ssize_t i = 0; i < n; i++) {
            if (chunk[i] != '\n')
                continue;
            newlines++;
            if (newlines == TARGET_LINE - 1)
                line_start = offset + i + 1;
            if (newlines == TARGET_LINE) {
                line_end = offset + i + 1;
                break;
            }
        }
        offset += n;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    if (size == (off_t)-1)
        return fail("lseek (SEEK_END)", fd, NULL);

    if (line_start < 0 || line_start >= size) {
        fprintf(stderr, "File has fewer than %d lines\n", TARGET_LINE);
        close(fd);
        return EXIT_FAILURE;
    }
    if (line_end < 0)
        line_end = size;

    size_t rem_len = (size_t)(size - line_end);
    char *rem = malloc(rem_len > 0 ? rem_len : 1);
    if (!rem)
        return fail("malloc", fd, NULL);

    if (lseek(fd, line_end, SEEK_SET) == (off_t)-1)
        return fail("lseek (remainder)", fd, rem);
    if (read_all(fd, rem, rem_len) < 0)
        return fail("read (remainder)", fd, rem);

    if (lseek(fd, line_start, SEEK_SET) == (off_t)-1)
        return fail("lseek (line start)", fd, rem);
    if (write_all(fd, NEW_TEXT, strlen(NEW_TEXT)) < 0)
        return fail("write (new line)", fd, rem);
    if (write_all(fd, rem, rem_len) < 0)
        return fail("write (remainder)", fd, rem);

    off_t new_size = line_start + (off_t)strlen(NEW_TEXT) + (off_t)rem_len;
    if (ftruncate(fd, new_size) < 0)
        return fail("ftruncate", fd, rem);

    free(rem);
    rem = NULL;

    if (lseek(fd, 0, SEEK_SET) == (off_t)-1)
        return fail("lseek (rewind)", fd, NULL);

    for (;;) {
        ssize_t n = read(fd, chunk, sizeof chunk);
        if (n < 0) {
            if (errno == EINTR) continue;
            return fail("read (print)", fd, NULL);
        }
        if (n == 0)
            break;
        if (write_all(STDOUT_FILENO, chunk, (size_t)n) < 0)
            return fail("write (stdout)", fd, NULL);
    }

    if (close(fd) < 0)
        return fail("close", -1, NULL);
    return EXIT_SUCCESS;
}

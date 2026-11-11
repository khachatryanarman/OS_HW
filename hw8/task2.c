#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FILENAME "data.txt"
#define TRUNC_LEN 10

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

int main(void)
{
    const char *text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open (write)");
        return EXIT_FAILURE;
    }
    if (write_all(fd, text, strlen(text)) < 0) {
        perror("write");
        close(fd);
        return EXIT_FAILURE;
    }
    if (close(fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }

    
    fd = open(FILENAME, O_RDWR);
    if (fd < 0) {
        perror("open (read/write)");
        return EXIT_FAILURE;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    if (size == (off_t)-1) {
        perror("lseek (SEEK_END)");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Size before truncation: %lld bytes\n", (long long)size);

    if (ftruncate(fd, TRUNC_LEN) < 0) {
        perror("ftruncate");
        close(fd);
        return EXIT_FAILURE;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size == (off_t)-1) {
        perror("lseek (SEEK_END)");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Size after truncation:  %lld bytes\n", (long long)size);

    if (lseek(fd, 0, SEEK_SET) == (off_t)-1) {
        perror("lseek (SEEK_SET)");
        close(fd);
        return EXIT_FAILURE;
    }

    char buf[64];
    ssize_t total = 0;
    for (;;) {
        ssize_t n = read(fd, buf + total, sizeof buf - 1 - (size_t)total);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("read");
            close(fd);
            return EXIT_FAILURE;
        }
        if (n == 0)
            break;
        total += n;
        if ((size_t)total >= sizeof buf - 1)
            break;
    }
    buf[total] = '\0';

    printf("Content after truncation: \"%s\" (%zd bytes read)\n", buf, total);

    if (close(fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

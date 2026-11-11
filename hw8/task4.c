#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FILENAME "log.txt"
#define LINE_MAX_LEN 1024
#define OUT_MAX_LEN (LINE_MAX_LEN + 64)

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
    int fd = open(FILENAME, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    const char *prompt = "Enter a log line: ";
    if (write_all(STDOUT_FILENO, prompt, strlen(prompt)) < 0) {
        perror("write (prompt)");
        close(fd);
        return EXIT_FAILURE;
    }

    char line[LINE_MAX_LEN];
    size_t len = 0;
    while (len < sizeof line) {
        char c;
        ssize_t n = read(0, &c, 1);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("read (stdin)");
            close(fd);
            return EXIT_FAILURE;
        }
        if (n == 0)
            break;
        line[len++] = c;
        if (c == '\n')
            break;
    }

    char out[OUT_MAX_LEN];
    int prefix = snprintf(out, sizeof out, "PID=%ld: ", (long)getpid());
    if (prefix < 0) {
        fprintf(stderr, "snprintf failed\n");
        close(fd);
        return EXIT_FAILURE;
    }

    size_t out_len = (size_t)prefix;
    memcpy(out + out_len, line, len);
    out_len += len;
    if (len == 0 || line[len - 1] != '\n')
        out[out_len++] = '\n';

    if (write_all(fd, out, out_len) < 0) {
        perror("write (log)");
        close(fd);
        return EXIT_FAILURE;
    }

    /*
     * With O_APPEN the kernel moves the file offset to the current end of
     * the file right before every write, and the move and the write happen as
     * one atomic step. After the write, the offset sits just past the bytes
     * that were written, which is now the end of the file. SEEK_CUR reports
     * that offset, so it equals the file's new size. Each new entry makes the
     * file longer, so the value keeps growing from run to run, even though
     * the program never seeks forward itself.
     */
    off_t pos = lseek(fd, 0, SEEK_CUR);
    if (pos == (off_t)-1) {
        perror("lseek (SEEK_CUR)");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("New file offset: %lld\n", (long long)pos);

    if (close(fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FILENAME "sparse.bin"
#define SKIP_BYTES (1024 * 1024)

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
    int fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open (write)");
        return EXIT_FAILURE;
    }

    if (write_all(fd, "START", 5) < 0) {
        perror("write (START)");
        close(fd);
        return EXIT_FAILURE;
    }

    if (lseek(fd, SKIP_BYTES, SEEK_CUR) == (off_t)-1) {
        perror("lseek (SEEK_CUR)");
        close(fd);
        return EXIT_FAILURE;
    }

    if (write_all(fd, "END", 3) < 0) {
        perror("write (END)");
        close(fd);
        return EXIT_FAILURE;
    }

    if (close(fd) < 0) {
        perror("close (write)");
        return EXIT_FAILURE;
    }

    fd = open(FILENAME, O_RDONLY);
    if (fd < 0) {
        perror("open (read)");
        return EXIT_FAILURE;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    if (size == (off_t)-1) {
        perror("lseek (SEEK_END)");
        close(fd);
        return EXIT_FAILURE;
    }

    /*
     * The apparent size is 5 + 1048576 + 3 = 1048584 bytes, but du reports
     * far less. Seeking past the end and writing there leaves a "hole" a
     * range of the file that was never written. The filesystem does not
     * allocate disk blocks for a hole. It only records that the range exists,
     * and any read from it returns zeros produced on the fly. Real blocks are
     * allocated only for the data that was actually written ("START" and
     * "END"), usually one block each, so du shows only a few kilobytes while
     * ls -l and lseek report the full logical size.
     */
    printf("Apparent file size: %lld bytes\n", (long long)size);

    if (close(fd) < 0) {
        perror("close (read)");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

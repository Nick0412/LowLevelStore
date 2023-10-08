#include "Helper.h"
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    printf("STARTING PROGRAM WITH PID %d\n", getpid());
    Socket listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    Utility_CheckAndPrintForError(listening_socket, -1);
    struct timeval socket_timeout = {
        .tv_sec = 5,
        .tv_usec = 0
    };
    U8 should_reuse_address = 1;
    setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &should_reuse_address, sizeof(listening_socket));
    setsockopt(listening_socket, SOL_SOCKET, SO_RCVTIMEO, &socket_timeout, sizeof(listening_socket));
    setsockopt(listening_socket, SOL_SOCKET, SO_SNDTIMEO, &socket_timeout, sizeof(listening_socket));

    SocketAddress socket_address = {
        .sin_family = AF_INET,
        .sin_port = htons(20000)
    };
    inet_pton(AF_INET, "127.0.0.1", &socket_address.sin_addr);
    
    int result = bind(listening_socket, (GenericSocketAddress*)&socket_address, sizeof(socket_address));
    Utility_CheckAndPrintForError(result, -1);

    result = listen(listening_socket, 100);
    Utility_CheckAndPrintForError(result, -1);
    
    int result;
    printf("Making fifo\n");
    result = mkfifo("/tmp/server-fifo", S_IRWXO | S_IRWXU | S_IRWXG);
    Utility_CheckAndPrintForError(result, -1);

    printf("Opening fifo\n");
    int fifo_fd = open("/tmp/server-fifo", O_RDWR | O_NONBLOCK);
    Utility_CheckAndPrintForError(fifo_fd, -1);

    Utility_CheckAndPrintForError(result, -1);
    PollFileDescriptor poll_fd = {
        .events = POLLIN,
        .fd = fifo_fd
    };

    printf("Starting main loop\n");
    S8 should_stop = 0;
    while (should_stop != 'y')
    {
        printf("Polling for 3s\n");
        int result = poll(&poll_fd, 1, 3000);
        printf("Got results: %d\n", result);
        printf("Events %d\n", poll_fd.revents);
        if (result > 0 && (poll_fd.revents & POLLIN))
        {
            printf("Attempting to read from stdin\n");
            int read_results = read(fifo_fd, &should_stop, 1);
            printf("Got read_results %d\n", read_results);
            printf("Got: %c\n", should_stop);
            printf("Got: %d\n", should_stop);
            poll_fd.revents = 0;
        }
        printf("\n\n");
        sleep(4);
    }

    close(fifo_fd);
    unlink("/tmp/server-fifo");
    return 0;
}

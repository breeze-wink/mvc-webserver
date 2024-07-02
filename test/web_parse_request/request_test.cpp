#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Logger.h"
#include "request.h"

using namespace breeze::web;
using namespace breeze::utility;

int main()
{
    Singleton<Logger>::Instance() -> open("/home/breeze1/mvc-webserver/bin/test/test.log");

    int fd = open("/home/breeze1/mvc-webserver/test/web_parse_request/http_request.txt", O_RDONLY, 0644);
    char msg[1024] = {0};

    struct stat st;
    fstat(fd, &st);

    read(fd, msg, st.st_size);

    printf("%s\n", msg);

    Request req;
    req.parse_header(msg, sizeof(msg));

    req.show();

    close(fd);
    return 0;
}
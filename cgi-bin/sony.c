#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cgic.h>

#define SONY_HOST   "192.168.1.204"
#define SONY_PORT   20060
#define RCV_TIMEOUT 5 

static char sony_host[16] = {0};

void send_code(char *code, char *res)
{
    if (strlen(code) != 23)
    {
        strcpy(res, "CODE FAIL");
        return;
    }

    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        strcpy(res, "SOCKET FAIL");
        return;
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, sony_host, &(serv_addr.sin_addr));
    serv_addr.sin_port = htons(SONY_PORT);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        strcpy(res, "CONNECT FAIL");
        close(sockfd);
        return;
    }

    struct timeval tv;
    tv.tv_sec = RCV_TIMEOUT;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    send(sockfd, code, 23, MSG_MORE);
    send(sockfd, "\n", 1, 0);
    read(sockfd, res, 24);
    res[24] = 0;
    close(sockfd);
}

int cgiMain()
{
    char code[24];
    cgiFormString("code", code, sizeof(code));
    cgiFormString("host", sony_host, sizeof(sony_host));

    if (strlen(sony_host) == 0)
    {
        strcpy(sony_host, SONY_HOST);
    }

    char res[25];
    send_code(code, res);

    cgiHeaderContentType("text/html");
    fprintf(cgiOut, res);
    return 0;
}


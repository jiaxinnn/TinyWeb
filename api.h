#ifndef API_H
#define API_H
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>

pid_t gettid();

void handle_error(const char *msg);
void handle_error_s(const std::string &msg);
void setnonblocking(int sock);

std::string cstr2string(const char *str);
void splitString(const std::string &, const std::string &, std::vector<std::string> &);
std::map<char, std::string> getOption(int argc, char *argv[]);

int createSocket(int domain, int type, int protocol);
int createSocket();
void Close(int fd);

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Bind(int sockfd, const struct sockaddr *addr);
int Listen(int sockfd, int backlog);

void shutdownWrite(int sockfd);

int Open(const char *pathname, int flags, mode_t mode);
int Stat(const char *pathname, struct stat *buf);
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
int Munmap(void *start, size_t length);

ssize_t writeString(int sockfd, const std::string &str);
ssize_t Rio_writen(int fd, void *usrbuf, size_t n);
ssize_t writeHtml(int sockfd, const std::string &filename);

int get_line(int sock, char *buf, int size);

in_addr_t Inet_addr(const std::string &host);
std::string Inet_ntop(int af, const void *src,
                      char *dst, socklen_t size);

void IpPortToSockAddr(const char *, int, struct sockaddr_in *res);
void SockAddrToIpPort(char *, int &, const struct sockaddr_in *src);

int setNoBlock(int fd);
int setCLOEXEC(int fd);

void epoll_addfd(int epfd, int fd, int events);
void epoll_modfd(int epfd, int fd, int events);
void epoll_removefd(int epfd, int fd);

typedef void (*sighandler_t)(int);
void add_signal(int sign, sighandler_t handler);
void remove_signal(int sign);

////////////////////// socket api////////////////////

uint16_t hostToNet16(uint16_t a)
{
    return htons(a);
}
uint32_t hostToNet32(uint32_t a)
{
    return htonl(a);
}

uint16_t netToHost16(uint16_t a)
{
    return ntohs(a);
}
uint32_t netToHost32(uint32_t a)
{
    return ntohl(a);
}

#endif
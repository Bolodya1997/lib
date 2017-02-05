#include <netdb.h>
#include <cstring>
#include <logging.h>
#include "datagram_socket.h"

using namespace std;
using namespace net;

#define sock_len sizeof(sockaddr_in)

datagram_socket::datagram_socket() {
    filed = ::socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    if (filed < 0)
        throw (net_exception("socket"));
}

datagram_socket::datagram_socket(uint16_t port) : datagram_socket() {
    in_addr inet_addr = {
            .s_addr = INADDR_ANY
    };
    sockaddr_in sock_addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr = inet_addr
    };
    if (bind(filed, (const sockaddr *) &sock_addr, sock_len) < 0)
        throw (net_exception("bind"));
}

void datagram_socket::set_receiver(string hostname, uint16_t port) {
    addrinfo hints, *res;
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(hostname.data(), NULL, &hints, &res) < 0)
        throw (net_exception("getaddrinfo"));

    sockaddr_in sock_addr = *(sockaddr_in *) res->ai_addr;
    sock_addr.sin_port = htons(port);

    set_receiver(sock_addr);
    freeaddrinfo(res);
}

ssize_t datagram_socket::read(void *buff, size_t n) {
    pollable::read(buff, n);
    sender_empty = false;

    socklen_t tmp = sizeof(sender);
    return recvfrom(filed, buff, n, MSG_DONTWAIT | MSG_NOSIGNAL,
                    (sockaddr *) &sender, &tmp);
}

ssize_t datagram_socket::write(const void *buff, size_t n) {
    pollable::write(buff, n);

    if (receiver_empty)
        throw (net_exception("sendto"));

    return sendto(filed, buff, n, MSG_DONTWAIT | MSG_NOSIGNAL,
                  (const sockaddr *) &receiver, sock_len);
}
#ifndef TCP_OVER_UDP_DATAGRAM_SOCKET_H
#define TCP_OVER_UDP_DATAGRAM_SOCKET_H

#include <string>
#include <netinet/ip.h>
#include "../poll/pollable.h"
#include "../net/net_exception.h"

namespace net {

    class datagram_socket : public pollable {

        sockaddr_in receiver;
        sockaddr_in sender;

        bool receiver_empty = true;
        bool sender_empty = true;

    public:
        datagram_socket();
        datagram_socket(uint16_t port);

        void set_receiver(std::string hostname, uint16_t port);

        void set_receiver(sockaddr_in receiver) {
            this->receiver = receiver;
            receiver_empty = false;
        }

        sockaddr_in get_sender() {
            if (sender_empty)
                throw (net_exception("no sender"));

            return sender;
        }

        ssize_t read(void *buff, size_t n) override;
        ssize_t write(const void *buff, size_t n) override;

    private:
        void __abstract_guard() override { };
    };

}

#endif //TCP_OVER_UDP_DATAGRAM_SOCKET_H

#ifndef PROXY_SERVER_SOCKET_H
#define PROXY_SERVER_SOCKET_H

#include <stdint.h>
#include "../poll/pollable.h"
#include "fd_exception.h"
#include "net_exception.h"

namespace net {

    class server_socket : public pollable {

        const static int QUEUE_SIZE = 100;

    public:
        server_socket(uint16_t port);

        pollable *accept() override;

    private:
        void __abstract_guard() override { };
    };

}

#endif //PROXY_SERVER_SOCKET_H

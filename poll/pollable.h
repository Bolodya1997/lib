#ifndef PROXY_POLLABLE_H
#define PROXY_POLLABLE_H

#include <sys/poll.h>
#include <sys/types.h>
#include <unistd.h>
#include "../templates/observer.h"
#include "fd_watcher.h"
#include "../templates/synchronisable.h"

enum {
    POLL_NO = 0x00,
    POLL_AC = 0x01,
    POLL_CO = 0x02,
    POLL_RE = 0x04,
    POLL_WR = 0x08
};

/*
 *      #######################################
 *      #                                     #
 *      #         MUST BE THREAD SAFE         #
 *      #                                     #
 *      #######################################
 */
class pollable : public synchronisable {

    static fd_watcher *watcher;

    observer *owner = NULL;

    pollfd _pollfd;
    bool acceptable;
    bool connectable;

    bool closed = false;

protected:
    int filed = -1;

    virtual int &get_filed(pollable &obj) {
        return obj.filed;
    }

public:
    pollable();
    virtual ~pollable() {
        if (filed < 0)
            return;

        ::close(filed);

        if (watcher)
            watcher->update();
    }

    static void set_watcher(fd_watcher *watcher) {
        pollable::watcher = watcher;
    }

    observer *get_owner() {
        return owner;
    }

    void set_owner(observer *_owner) {
        this->owner = _owner;
    }

    virtual void close();

    virtual bool is_closed() {
        return closed;
    }

    virtual pollable *set_actions(short actions);
    virtual short get_actions();

    bool is_acceptable();
    virtual pollable *accept();

    bool is_connectable();
    virtual void connect();

    bool is_readable();
    virtual ssize_t read(void *buff, size_t n);

    bool is_writable();
    virtual ssize_t write(const void *buff, size_t n);

    bool is_disconnected() {
        return _pollfd.revents == POLLHUP;
    }

//  ----------------------------------------
    int get_fd() {
        return filed;
    }
//  ----------------------------------------

private:
    pollfd get_pollfd();
    void set_revents(short revents) {
        _pollfd.revents = revents;
    }

    virtual void __abstract_guard() = 0;

    friend class poller;
};

#endif //PROXY_POLLABLE_H

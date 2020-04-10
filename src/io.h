#pragma once

namespace io {
    // a threaded version of sending buffer to every client
    void dispatch_all(const char *buffer);
}

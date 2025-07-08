#ifndef SEPIA_COMM2_SCOPEDMESSAGESENDER_H
#define SEPIA_COMM2_SCOPEDMESSAGESENDER_H
#include <sepia/comm2/messagesender.h>

namespace sepia::comm2
{
    class ScopedMessageSender
    {
    public:
        ScopedMessageSender()
        {
            sepia::comm2::MessageSender::init();
        }

        ~ScopedMessageSender()
        {
            sepia::comm2::MessageSender::destroy();
        }
    };
}

#endif // SEPIA_COMM2_SCOPEDMESSAGESENDER_H
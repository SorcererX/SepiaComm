#ifndef SEPIA_COMM2_MESSAGESENDER_H
#define SEPIA_COMM2_MESSAGESENDER_H
#include <vector>
#include <string>
#include <zmq.hpp>

namespace sepia
{
    namespace comm2
    {
        class Header;

        class MessageSender
        {
        public:
            static void init();
            static void destroy();
            static void rawSend( const std::string& a_name, const unsigned char* a_msg, size_t a_msgSize );

        private:
            MessageSender() = delete;
            // static zmq::context_t sm_context;
            static thread_local zmq::socket_t stm_externalSocket;
        };

    }
}

#endif // HYSPEX_COMM_MESSAGESENDER_H

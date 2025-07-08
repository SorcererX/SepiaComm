#ifndef SEPIA_COMM2_GLOBAL_H
#define SEPIA_COMM2_GLOBAL_H
#include <memory>
#include <zmq.hpp>

namespace sepia
{
    namespace comm2
    {
        class Global
        {
        public:
            ~Global();
            static std::shared_ptr< Global > instance();

            zmq::context_t& getContext();
            std::string getPublisherUrl();
            std::string getSubscriberUrl();

        private:
            Global();
            zmq::context_t m_context;
        };
    }
}

#endif // SEPIA_COMM2_GLOBAL_H

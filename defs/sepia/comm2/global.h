#ifndef SEPIA_COMM2_GLOBAL_H
#define SEPIA_COMM2_GLOBAL_H
#include <memory>
#include <string>
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

            struct Config
            {
                std::string publisherUrl = "tcp://127.0.0.1:31339";
                std::string subscriberUrl = "tcp://127.0.0.1:31340";
                int contextThreads = 4;
            };

            static Config loadConfiguration();

            zmq::context_t& getContext();
            std::string getPublisherUrl();
            std::string getSubscriberUrl();

        private:
            Global();

            Config m_config;
            zmq::context_t m_context;
        };
    }
}

#endif // SEPIA_COMM2_GLOBAL_H

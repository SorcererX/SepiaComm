#include <sepia/comm2/global.h>

namespace sepia
{
    namespace comm2
    {
        std::shared_ptr< Global > Global::instance()
        {
            static std::shared_ptr< Global > shared = std::shared_ptr< Global >( new Global() ); // cannot use make_shared due to private constructor
            return shared;
        }

        Global::Global()
            : m_context( 4 )
        {
        }

        Global::~Global()
        {
        }

        zmq::context_t& Global::getContext()
        {
            return m_context;
        }

        std::string Global::getSubscriberUrl()
        {
            return "tcp://127.0.0.1:31340";
        }

        std::string Global::getPublisherUrl()
        {
            return "tcp://127.0.0.1:31339";
        }
    }
}

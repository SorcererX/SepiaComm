#ifndef SEPIA_COMM2_RECEIVER_H
#define SEPIA_COMM2_RECEIVER_H
#include <memory>
#include <map>
#include <string>
#include <unordered_set>

namespace zmq
{
    class socket_t;
}

namespace sepia::comm2
{
    class ObserverAll;
    class ObserverBase;

    class Receiver
    {
    public:
        Receiver();
        Receiver( ObserverAll* a_observer, int a_timeoutMs = -1 );
        ~Receiver();
        bool init( int a_timeoutMs = -1 );
        bool destroy();
        bool exec();
        bool addObserver( ObserverAll* a_observer );
        bool removeObserver( ObserverAll* a_observer );

    protected:
        bool distribute( const std::string& a_name, const char* a_data, size_t a_size );
        bool addObserver( const std::string& a_name, ObserverBase* a_observer );
        bool removeObserver( const std::string& a_name, ObserverBase* a_observer );

    private:
        Receiver( const Receiver& ) = delete;
        typedef std::map< const std::string, std::unordered_set< ObserverBase* > > ObserverMap;
        ObserverMap m_observers;
        std::unique_ptr< zmq::socket_t > m_socket{ nullptr };
        bool m_blocking{ false };
        bool m_initialized{ false };
    };
}

#endif // SEPIA_COMM2_RECEIVER_H
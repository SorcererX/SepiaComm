#ifndef SEPIA_UTIL_THREADOBJECT_H
#define SEPIA_UTIL_THREADOBJECT_H
#include <atomic>
#include <thread>

namespace sepia
{
    namespace util
    {
        class ThreadObject
        {
        public:
            ThreadObject();
            virtual ~ThreadObject();
            void join();
            void stop();
            void start();

        protected:
            virtual void own_thread() = 0;

        protected:
            std::atomic_bool m_terminate{ true };
            std::unique_ptr< std::thread > m_thread{ nullptr };
        };
    }
}
#endif // SEPIA_UTIL_THREADOBJECT_H

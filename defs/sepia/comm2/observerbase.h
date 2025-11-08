#ifndef SEPIA_COMM2_OBSERVERBASE_H
#define SEPIA_COMM2_OBSERVERBASE_H
#include "observerall.h"

namespace sepia::comm2
{
    class ObserverBase : public virtual ObserverAll
    {
    public:
        virtual ~ObserverBase() = default;
        virtual void process( const char* a_buffer, std::size_t a_size ) = 0;
        ObserverBase( const ObserverBase& ) = delete;
        ObserverBase( const ObserverBase&& ) = delete;
        ObserverBase& operator=( const ObserverBase& ) = delete;
        ObserverBase&& operator=( const ObserverBase&& ) = delete;

    protected:
        ObserverBase() = default;
    };

}

#endif // SEPIA_COMM2_OBSERVERBASE_H

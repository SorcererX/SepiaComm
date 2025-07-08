#ifndef SEPIA_COMM2_OBSERVERBASE_H
#define SEPIA_COMM2_OBSERVERBASE_H
#include "observerall.h"

namespace sepia
{
    namespace comm2
    {
        class ObserverBase : public virtual ObserverAll
        {
        public:
            virtual ~ObserverBase() {}
            virtual void process( const char* a_buffer, std::size_t a_size ) = 0;

        protected:
            ObserverBase()
            {
            }
        };

    }
}
#endif // SEPIA_COMM2_OBSERVERBASE_H

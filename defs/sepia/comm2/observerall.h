#ifndef SEPIA_COMM2_OBSERVERALL_H
#define SEPIA_COMM2_OBSERVERALL_H
#include <cstdint>
#include <utility>
#include <list>
#include <string>

namespace sepia
{
    namespace comm2
    {
        class ObserverBase;
        class ObserverAll
        {
        public:
            virtual ~ObserverAll() {}

            std::list< std::pair< std::string, ObserverBase* > > getObservers()
            {
                return m_observers;
            }

        protected:
            void addObserver( const std::string& a_name, ObserverBase* a_observer )
            {
                m_observers.push_back( std::make_pair( a_name, a_observer ) );
            }

            void removeObserver( const std::string& a_name, ObserverBase* a_observer )
            {
                std::erase( m_observers, std::make_pair( a_name, a_observer ) );
            }

        private:
            std::list< std::pair< std::string, ObserverBase* > > m_observers;
        };

    }
}
#endif // SEPIA_COMM2_OBSERVERALL_H

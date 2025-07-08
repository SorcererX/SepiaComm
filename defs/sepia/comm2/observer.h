#ifndef SEPIA_COMM2_OBSERVER_H
#define SEPIA_COMM2_OBSERVER_H
#include "observerbase.h"
#include <flatbuffers/flatbuffers.h>
#include <google/protobuf/message_lite.h>

namespace sepia
{
    namespace comm2
    {
        template< class MessageName, typename Enable = void >
        class Observer : public ObserverBase
        {
        public:
            ~Observer();

        protected:
            Observer();
            virtual void receive( const std::unique_ptr< MessageName >& msg ) = 0;

        private:
        };

        template< typename MessageName >
        class Observer< MessageName, typename std::enable_if< std::is_base_of< google::protobuf::MessageLite, MessageName >::value >::type > : public ObserverBase
        {
        public:
            virtual ~Observer()
            {
                removeObserver( MessageName::default_instance().GetTypeName(), this );
            }

            void process( const char* a_buffer, size_t a_size ) override
            {
                m_msg->ParseFromArray( a_buffer, static_cast< int >( a_size ) );
                receive( *m_msg );
            }

        protected:
            Observer()
                : m_msg( std::unique_ptr< MessageName >( new MessageName ) )
            {
                addObserver( MessageName::default_instance().GetTypeName(), this );
            }

            virtual void receive( const MessageName& ) {}

        private:
            std::unique_ptr< MessageName > m_msg;
        };

        template< typename MessageName >
        class Observer< MessageName, typename std::enable_if< std::is_base_of< flatbuffers::NativeTable, MessageName >::value >::type > : public ObserverBase
        {
        public:
            ~Observer()
            {
                removeObserver( MessageName::GetFullyQualifiedName(), this );
            }

            void process( const char* a_buffer, size_t a_size ) override
            {
                std::unique_ptr< MessageName > message = std::unique_ptr< MessageName >( flatbuffers::GetRoot< typename MessageName::TableType >( a_buffer )->UnPack( nullptr ) );
                receive( *message );
            }

        protected:
            Observer()
            {
                addObserver( MessageName::GetFullyQualifiedName(), this );
            }

            virtual void receive( const MessageName& msg ) {};

        private:
        };
    }
}
#endif // SEPIA_COMM2_OBSERVER_H

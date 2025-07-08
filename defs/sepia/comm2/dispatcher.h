#ifndef SEPIA_COMM2_DISPATCHER_H
#define SEPIA_COMM2_DISPATCHER_H
#include <sepia/comm2/messagesender.h>
#include <iostream>
#include <flatbuffers/flatbuffers.h>
#include <google/protobuf/message_lite.h>

namespace sepia
{
    namespace comm2
    {
        template< typename MessageName, typename Enable = void >
        class Dispatcher : protected MessageSender
        {
        public:
            static void send( const MessageName* a_message, unsigned int a_subscriberId = 0 );
        };

        template< typename MessageName >
        class Dispatcher< MessageName, typename std::enable_if< std::is_base_of< google::protobuf::MessageLite, MessageName >::value >::type > : protected MessageSender
        {
        public:
            static void send( const MessageName* a_message, unsigned int a_subscriberId = 0 )
            {
                const int bytesize = a_message->ByteSizeLong();

                if( bytesize > stm_buffer.size() )
                {
                    stm_buffer.resize( bytesize * 3 / 2 );
                }
                a_message->SerializeToArray( stm_buffer.data(), bytesize );

                if( a_subscriberId == 0 )
                {
                    rawSend( MessageName::default_instance().GetTypeName(), reinterpret_cast< const unsigned char* >( stm_buffer.data() ), bytesize );
                }
                else
                {
                    std::string filter = MessageName::default_instance().GetTypeName();
                    filter.append( "," );
                    filter.append( std::to_string( a_subscriberId ) );
                    rawSend( filter, reinterpret_cast< const unsigned char* >( stm_buffer.data() ), bytesize );
                }
            }

        private:
            static thread_local std::vector< char > stm_buffer;
        };

        template< typename MessageName >
        thread_local std::vector< char > Dispatcher< MessageName, typename std::enable_if< std::is_base_of< google::protobuf::MessageLite, MessageName >::value >::type >::stm_buffer;

        template< typename MessageName >
        class Dispatcher< MessageName, typename std::enable_if< std::is_base_of< flatbuffers::NativeTable, MessageName >::value >::type > : protected MessageSender
        {
        public:
            static void send( const MessageName* a_message, unsigned int a_subscriberId = 0 )
            {
                flatbuffers::FlatBufferBuilder fbb;
                auto offset = MessageName::TableType::Pack( fbb, a_message );
                fbb.Finish( offset );
                if( a_subscriberId == 0 )
                {
                    rawSend( MessageName::GetFullyQualifiedName(), fbb.GetBufferPointer(), fbb.GetSize() );
                }
                else
                {
                    std::string filter = MessageName::GetFullyQualifiedName();
                    filter.append( "," );
                    filter.append( std::to_string( a_subscriberId ) );
                    rawSend( filter, fbb.GetBufferPointer(), fbb.GetSize() );
                }
            }
        };
    }
}

#endif // SEPIA_COMM2_DISPATCHER_H

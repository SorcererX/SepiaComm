#include "receivetester.hpp"
#include "receiver.h"

ReceiveTester::ReceiveTester()
    : m_flatbufferMessageCount( 0 )
    , m_protobufMessageCount( 0 )
    , m_lastFlatbufferSeqNo( -1 )
    , m_lastProtobufSeqNo( -1 )
{
}

ReceiveTester::~ReceiveTester()
{
}

void ReceiveTester::own_thread()
{
    sepia::comm2::Receiver handler;
    handler.addObserver( this );
    handler.init( 100 );

    while( !m_terminate && handler.exec() )
    {
    }
}

void ReceiveTester::stop()
{
    sepia::util::ThreadObject::stop();
}

void ReceiveTester::receive( const comm2tester_msgs::TestT& a_msg )
{
    m_flatbufferMessageCount++;

    if( a_msg.seq_no != m_lastFlatbufferSeqNo + 1 )
    {
        std::cout << "Lost Flatbuffer message: " << a_msg.seq_no << " count: " << a_msg.seq_no - m_lastFlatbufferSeqNo << std::endl;
    }
    m_lastFlatbufferSeqNo = a_msg.seq_no;

    if( m_flatbufferMessageCount % 10000 == 0 )
    {
        std::cout << "Got " << m_flatbufferMessageCount << " flatbuffer based message(s)." << " ( id: " << a_msg.seq_no << " )" << std::endl;
    }
}

void ReceiveTester::receive( const comm2tester_msgs::ProtoTest& a_msg )
{
    m_protobufMessageCount++;

    if( a_msg.seq_no() != m_lastProtobufSeqNo + 1 )
    {
        std::cout << "Lost Protobuf message: " << a_msg.seq_no() << " count: " << a_msg.seq_no() - m_lastProtobufSeqNo << std::endl;
    }
    m_lastProtobufSeqNo = a_msg.seq_no();

    if( m_protobufMessageCount % 10000 == 0 )
    {
        std::cout << "Got " << m_protobufMessageCount << " protobuf based message(s)." << " ( id: " << a_msg.seq_no() << " )" << std::endl;
    }
}

int64_t ReceiveTester::getMessageCount()
{
    return m_flatbufferMessageCount + m_protobufMessageCount;
}

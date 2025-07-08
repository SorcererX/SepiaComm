#include <sepia/comm2/observer.h>
#include <sepia/util/threadobject.h>
#include "comm2tester_msgs_generated.h"
#include "comm2tester_msgs.pb.h"
#include <sepia/comm2/receiver.h>

class ReceiveTester : public sepia::comm2::Observer< comm2tester_msgs::TestT >, public sepia::comm2::Observer< comm2tester_msgs::ProtoTest >, public sepia::util::ThreadObject
{
public:
    ReceiveTester();
    ~ReceiveTester();
    void stop();
    int64_t getMessageCount();

protected:
    void receive( const comm2tester_msgs::TestT& a_msg ) override;
    void receive( const comm2tester_msgs::ProtoTest& a_msg ) override;
    void own_thread();
    int64_t m_flatbufferMessageCount;
    int64_t m_protobufMessageCount;
    int64_t m_lastFlatbufferSeqNo;
    int64_t m_lastProtobufSeqNo;
};

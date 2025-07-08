#include <iostream>
#include "messagelogger.h"
#include <zmq.hpp>

int main( int argc, char* argv[] )
{
    zmq::context_t context( 3 );
    zmq::socket_t publisher( context, ZMQ_XPUB );
    zmq::socket_t subscriber( context, ZMQ_XSUB );
    zmq::socket_t capture( context, ZMQ_PUSH );

    capture.bind( "inproc://capture" );
    publisher.bind( "tcp://*:31339" );
    subscriber.bind( "tcp://*:31340" );

    MessageLogger logger( &context );
    logger.start();

    zmq::proxy( static_cast< zmq::socket_ref >( publisher ), static_cast< zmq::socket_ref >( subscriber ), static_cast< zmq::socket_ref >( capture ) );
    logger.stop();
    logger.join();

    return 0;
}

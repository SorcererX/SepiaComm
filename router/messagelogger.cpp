#include "messagelogger.h"
#include <iostream>
#include <zmq.hpp>

MessageLogger::MessageLogger( zmq::context_t* a_context )
    : m_context( a_context )
{
}

MessageLogger::~MessageLogger()
{
}

void MessageLogger::own_thread()
{
    std::cout << "MessageLogger - ready" << std::endl;
    zmq::socket_t logger( *m_context, ZMQ_PULL );
    logger.connect( "inproc://capture" );

    std::string last_msg = "";
    uint64_t counter = 0;

    while( !m_terminate )
    {
        int more = 0;

        zmq::message_t msg;
        try
        {
            zmq::recv_result_t result = logger.recv( msg );
        }
        catch( const zmq::error_t& ex )
        {
            if( ex.num() != ETERM )
            {
                throw;
            }
            break;
        }

        int parts = 1;

        std::string name = msg.str();

        while( true )
        {
            auto more = logger.get( zmq::sockopt::rcvmore );

            if( !more )
            {
                break;
            }
            parts++;

            try
            {
                zmq::recv_result_t result = logger.recv( msg );
            }
            catch( const zmq::error_t& ex )
            {
                if( ex.num() != ETERM )
                {
                    throw;
                }
                break;
            }
        }

        if( name != last_msg )
        {
            if( counter > 1 )
            {
                std::cout << "\nNAME: " << last_msg << " total: " << counter << std::endl;
            }
            std::cout << "NAME: " << name << " parts: " << parts << std::endl;
            counter = 1;
            last_msg = name;
        }
        else
        {
            std::cout << "." << std::flush;
            if( counter % 100 == 0 )
            {
                std::cout << " " << counter << std::endl;
            }
            counter++;
        }
    }
    std::cout << "MessageLogger - closed." << std::endl;
    logger.close();
}

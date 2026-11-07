#include "messagelogger.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>
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

    // The names in the current run, so two messages that alternate, like a request and its reply, are shown as one run.
    struct Seen
    {
        std::string name;
        uint64_t count;
        char symbol;
    };
    std::vector< Seen > run;
    // Each name in a run has its own symbol for the repeats.
    constexpr std::array symbols = { '.', '+' };
    constexpr size_t maxRunNames = symbols.size();
    uint64_t dots = 0;

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

        std::string name = msg.to_string();

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

        // Single frames starting with 1 or 0 are subscriptions from the subscribers, the rest of the frame is the topic.
        if( parts == 1 && !name.empty() && ( name[ 0 ] == 1 || name[ 0 ] == 0 ) )
        {
            const std::string topic = name.substr( 1 );
            name = ( name[ 0 ] == 1 ? "SUBSCRIBE " : "UNSUBSCRIBE " ) + ( topic.empty() ? std::string( "(all)" ) : topic );
        }

        const auto seen = std::ranges::find( run, name, &Seen::name );
        if( seen != run.end() )
        {
            seen->count++;
            dots++;
            std::cout << seen->symbol << std::flush;
            if( dots % 100 == 0 )
            {
                std::cout << " " << dots << std::endl;
            }
            continue;
        }

        if( dots % 100 != 0 )
        {
            std::cout << std::endl;
        }
        // A new name joins the run while there is room, otherwise the run ends.
        if( run.size() == maxRunNames )
        {
            for( const auto& [ runName, count, symbol ] : run )
            {
                if( count > 1 )
                {
                    std::cout << "NAME: " << runName << " total: " << count << std::endl;
                }
            }
            run.clear();
            dots = 0;
        }
        const char symbol = symbols[ run.size() ];
        std::cout << "NAME: " << name << " parts: " << parts << " repeats: " << symbol << std::endl;
        run.push_back( { .name = name, .count = 1, .symbol = symbol } );
    }
    std::cout << "MessageLogger - closed." << std::endl;
    logger.close();
}

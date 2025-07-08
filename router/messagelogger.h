#ifndef MESSAGELOGGER_H
#define MESSAGELOGGER_H
#include "threadobject.h"
#include <zmq.hpp>

class MessageLogger : public sepia::util::ThreadObject
{
public:
    MessageLogger( zmq::context_t* a_context );
    virtual ~MessageLogger();

protected:
    void own_thread();

private:
    zmq::context_t* m_context;
};

#endif // MESSAGELOGGER_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <sepia/comm2/global.h>
#include <sepia/comm2/messagesender.h>

namespace py = pybind11;

PYBIND11_MODULE( sepia_comm, m )
{
    m.doc() = "Python bindings for the SepiaComm messaging utilities.";

    py::class_< sepia::comm2::Global::Config >( m, "Config" )
        .def( py::init<>() )
        .def_readwrite( "publisher_url", &sepia::comm2::Global::Config::publisherUrl )
        .def_readwrite( "subscriber_url", &sepia::comm2::Global::Config::subscriberUrl )
        .def_readwrite( "context_threads", &sepia::comm2::Global::Config::contextThreads );

    m.def(
        "load_configuration",
        []()
        {
            return sepia::comm2::Global::loadConfiguration();
        },
        "Load SepiaComm configuration using SEPIA_CONFIG or the default sepia.yaml file." );

    m.def(
        "publisher_url",
        []()
        {
            return sepia::comm2::Global::instance()->getPublisherUrl();
        },
        "Return the publisher URL from the active SepiaComm configuration." );

    m.def(
        "subscriber_url",
        []()
        {
            return sepia::comm2::Global::instance()->getSubscriberUrl();
        },
        "Return the subscriber URL from the active SepiaComm configuration." );

    m.def(
        "init_sender",
        []()
        {
            sepia::comm2::MessageSender::init();
        },
        "Connect the shared MessageSender socket to the configured subscriber endpoint." );

    m.def(
        "destroy_sender",
        []()
        {
            sepia::comm2::MessageSender::destroy();
        },
        "Close the shared MessageSender socket." );

    m.def(
        "raw_send",
        []( const std::string& topic, py::bytes payload )
        {
            const std::string buffer = payload;
            sepia::comm2::MessageSender::rawSend(
                topic,
                reinterpret_cast< const unsigned char* >( buffer.data() ),
                buffer.size() );
        },
        py::arg( "topic" ),
        py::arg( "payload" ),
        "Publish a raw multipart message with the provided topic and payload bytes." );
}

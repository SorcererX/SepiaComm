#ifndef QT_MESSAGE_RECIEVER_H
#define QT_MESSAGE_RECEIVER_H
#include <QObject>
#include <QSocketNotifier>
#include <qsocketnotifier.h>
#include <sepia/comm2/receiver.h>
#include <sepia/comm2/observerall.h>
#include <iostream>
namespace sepia::comm2
{
    class QtMessageReceiver : public QObject
    {
    public:
        QtMessageReceiver( ObserverAll* a_observer, QObject* a_object )
            : QObject( a_object )
            , m_receiver( a_observer )
        {
            m_receiver.init( 0 ); // non-blocking

            int fd = m_receiver.getSocketFileDescriptor();

            m_notifier = new QSocketNotifier( fd, QSocketNotifier::Read, this );

            connect( m_notifier, &QSocketNotifier::activated, this, [ this ]( int )
            {
                while( m_receiver.getHaveMessagesAvailable() )
                {
                    m_receiver.exec();
                }
            } );
        }
        QtMessageReceiver() = delete;
        ~QtMessageReceiver() = default;

    protected:
    private:
        sepia::comm2::Receiver m_receiver;
        QSocketNotifier* m_notifier;
        Q_DISABLE_COPY_MOVE( QtMessageReceiver );
    };
}

#endif // QT_MESSAGE_RECEIVER_H
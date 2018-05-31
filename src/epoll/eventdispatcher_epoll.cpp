#include <QtCore/QPair>
#include <QtCore/QSocketNotifier>
#include <QtCore/QThread>
#include "eventdispatcher_epoll.h"
#include "eventdispatcher_epoll_p.h"

EventDispatcherEPoll::EventDispatcherEPoll(QObject* parent)
	: QAbstractEventDispatcher(parent), d_ptr(new EventDispatcherEPollPrivate(this))
{
}

EventDispatcherEPoll::~EventDispatcherEPoll(void)
{
#if QT_VERSION < 0x040600
	delete this->d_ptr;
	this->d_ptr = 0;
#endif
}

bool EventDispatcherEPoll::processEvents(QEventLoop::ProcessEventsFlags flags)
{
	Q_D(EventDispatcherEPoll);
	return d->processEvents(flags);
}

bool EventDispatcherEPoll::hasPendingEvents(void)
{
	extern uint qGlobalPostedEventsCount();
	return qGlobalPostedEventsCount() > 0;
}

void EventDispatcherEPoll::registerSocketNotifier(QSocketNotifier* notifier)
{
#ifndef QT_NO_DEBUG
	if (notifier->socket() < 0) {
		qWarning("QSocketNotifier: Internal error: sockfd < 0");
		return;
	}

	if (notifier->thread() != thread() || thread() != QThread::currentThread()) {
		qWarning("QSocketNotifier: socket notifiers cannot be enabled from another thread");
		return;
	}
#endif

	Q_D(EventDispatcherEPoll);
	d->registerSocketNotifier(notifier);
}

void EventDispatcherEPoll::unregisterSocketNotifier(QSocketNotifier* notifier)
{
#ifndef QT_NO_DEBUG
	if (notifier->socket() < 0) {
		qWarning("QSocketNotifier: Internal error: sockfd < 0");
		return;
	}

	if (notifier->thread() != thread() || thread() != QThread::currentThread()) {
		qWarning("QSocketNotifier: socket notifiers cannot be disabled from another thread");
		return;
	}
#endif

	Q_D(EventDispatcherEPoll);
	d->unregisterSocketNotifier(notifier);
}

void EventDispatcherEPoll::registerTimer(
	int timerId,
	int interval,
#if QT_VERSION >= 0x050000
	Qt::TimerType timerType,
#endif
	QObject* object
)
{
#ifndef QT_NO_DEBUG
	if (timerId < 1 || interval < 0 || !object) {
		qWarning("%s: invalid arguments", Q_FUNC_INFO);
		return;
	}

	if (object->thread() != this->thread() && this->thread() != QThread::currentThread()) {
		qWarning("%s: timers cannot be started from another thread", Q_FUNC_INFO);
		return;
	}
#endif

	Qt::TimerType type;
#if QT_VERSION >= 0x050000
	type = timerType;
#else
	type = Qt::CoarseTimer;
#endif

	Q_D(EventDispatcherEPoll);
	if (interval) {
		d->registerTimer(timerId, interval, type, object);
	}
	else {
		d->registerZeroTimer(timerId, object);
	}
}

bool EventDispatcherEPoll::unregisterTimer(int timerId)
{
#ifndef QT_NO_DEBUG
	if (timerId < 1) {
		qWarning("%s: invalid arguments", Q_FUNC_INFO);
		return false;
	}

	if (this->thread() != QThread::currentThread()) {
		qWarning("%s: timers cannot be stopped from another thread", Q_FUNC_INFO);
		return false;
	}
#endif

	Q_D(EventDispatcherEPoll);
	return d->unregisterTimer(timerId);
}

bool EventDispatcherEPoll::unregisterTimers(QObject* object)
{
#ifndef QT_NO_DEBUG
	if (!object) {
		qWarning("%s: invalid arguments", Q_FUNC_INFO);
		return false;
	}

	if (object->thread() != this->thread() && this->thread() != QThread::currentThread()) {
		qWarning("%s: timers cannot be stopped from another thread", Q_FUNC_INFO);
		return false;
	}
#endif

	Q_D(EventDispatcherEPoll);
	return d->unregisterTimers(object);
}

QList<QAbstractEventDispatcher::TimerInfo> EventDispatcherEPoll::registeredTimers(QObject* object) const
{
	if (!object) {
		qWarning("%s: invalid argument", Q_FUNC_INFO);
		return QList<QAbstractEventDispatcher::TimerInfo>();
	}

	Q_D(const EventDispatcherEPoll);
	return d->registeredTimers(object);
}

#if QT_VERSION >= 0x050000
int EventDispatcherEPoll::remainingTime(int timerId)
{
	Q_D(const EventDispatcherEPoll);
	return d->remainingTime(timerId);
}
#endif

void EventDispatcherEPoll::wakeUp(void)
{
	Q_D(EventDispatcherEPoll);
	d->wakeup();
}

void EventDispatcherEPoll::interrupt(void)
{
	Q_D(EventDispatcherEPoll);
	d->m_interrupt = true;
	this->wakeUp();
}

void EventDispatcherEPoll::flush(void)
{
}

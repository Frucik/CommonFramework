#pragma once

#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

template<class T>
class CFQueue
{
public:
	CFQueue()
	{
		m_Queue.reserve(10000);
	}

	inline T dequeue()
	{
		QMutexLocker locker(&m_Mutex);
		
		while (m_Queue.size() == 0)
		{
			m_unEmpty.wait(&m_Mutex);
		}

		return m_Queue.dequeue();
	}

	inline enqueue(const T &t)
	{
		QMutexLocker locker(&m_Mutex);

		m_Queue.enqueue(t);

		m_unEmpty.wakeAll();
	}

	inline void clear()
	{
		QMutexLocker locker(&m_Mutex);

		m_Queue.clear();
		m_Queue.reserve(10000);
		m_unEmpty.wakeAll();
	}

	inline int size()
	{
		QMutexLocker locker(&m_Mutex);

		return m_Queue.size();
	}

private:
	QMutex m_Mutex;
	QQueue<T> m_Queue;
	QWaitCondition m_unEmpty;
};

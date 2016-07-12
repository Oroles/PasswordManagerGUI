#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include <QList>
#include <QMutex>
#include <QWaitCondition>

template <typename T>
class SyncQueue
{
public:
    SyncQueue();

    void add(const T& v);
    bool try_pop(T& v);
    bool wait_and_pop(T &v);

private:
    QList<T> m_queue;
    QMutex m_mutex;
    QWaitCondition m_cond;

};

template <typename T>
SyncQueue<T>::SyncQueue()
{

}

template <typename T>
void SyncQueue<T>::add(const T &v)
{
    QMutexLocker locker(&m_mutex);
    m_queue.append(v);
    m_cond.wakeAll();
}

template <typename T>
bool SyncQueue<T>::try_pop(T& v)
{
    QMutexLocker locker(&m_mutex);
    if (m_queue.empty())
    {
        return false;
    }
    v = m_queue.front();
    m_queue.pop_front();
    return true;
}

template <typename T>
bool SyncQueue<T>::wait_and_pop(T& v)
{
    QMutexLocker locker(&m_mutex);
    if (m_queue.empty())
    {
        m_cond.wait(&m_mutex, 500);
    }
    if (m_queue.empty())
    {
        return false;
    }
    v = m_queue.front();
    m_queue.pop_front();
    return true;
}

#endif // SYNCQUEUE_H

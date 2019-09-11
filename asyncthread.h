#ifndef ASYNCTHREAD_H
#define ASYNCTHREAD_H

#include <QThread>
#include "asynctaskabastract.h"

// T = input object
// R = result object

template<class T, class R> class AsyncTaskAbstract;

template <class T, class R>
class AsyncThread : public QThread
{
public:
    AsyncThread(AsyncTaskAbstract<T, R> *caller) : QThread(nullptr)
    {
        assert(caller != nullptr);
        m_caller = caller;
    }
    ~AsyncThread() { m_caller = nullptr; }

    virtual void run() override {
        m_result = m_caller->runInBackground(m_caller->m_param);
        m_caller->onBackgroundTaskFinished();
    }

private:
    AsyncTaskAbstract<T, R> *m_caller;
    R m_result;

    friend class AsyncTaskAbstract<T, R>;
};

#endif // ASYNCTHREAD_H

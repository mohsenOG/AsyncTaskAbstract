#ifndef ASYNCTASKABSTRACT_H
#define ASYNCTASKABSTRACT_H

#include "asyncthread.h"

#define UNUSED_PAR(x) (void)x

enum TaskState {
    TASK_CANCELED,
    TASK_DONE
};

template <typename T, class R>
class AsyncTaskAbstract
{
public:
    AsyncTaskAbstract() {
        init();
    }
    virtual ~AsyncTaskAbstract() = 0;

    virtual void onPreExecute() {} // Will be called on the caller thread
    virtual R runInBackground(T& params) = 0; // Will be run on the Async thread
    virtual void onPostExecute(R &result, TaskState state) {UNUSED_PAR(result); UNUSED_PAR(state);} // Will be run on the caller thread

    void onBackgroundTaskFinished() {
        onPostExecute(m_worker->m_result, TASK_DONE);
        m_worker->deleteLater();
        m_worker = nullptr;
        m_isRunning = false;

    }

    void execute(T & param) {
        if (m_isRunning)
            return;

        init();
        m_param = param;
        m_isRunning = true;
        m_canceled = true;
        onPreExecute();

        m_worker->start();
    }
    void cancel() {
        if (!m_isRunning) return;

        m_canceled = true;
        m_worker->terminate();
        m_worker->wait();
        m_isRunning = false;

        onPostExecute(m_worker->m_result, TASK_CANCELED);
        delete m_worker;
        m_worker = nullptr;
    }

    bool isRunning() { return m_isRunning; }
    bool cancelled() { return m_canceled; }

private:
    AsyncThread<T, R> *m_worker {nullptr};
    bool m_isRunning {false};
    bool m_canceled {false};

    // input - result holders
    T m_param;

    friend class AsyncThread<T, R>;

    void init() {
        if (m_worker == nullptr && !m_isRunning)
        {
            m_worker = new AsyncThread<T, R>(this);
        }
    }
};

template<class T, class R>
AsyncTaskAbstract<T, R>::~AsyncTaskAbstract() {
    m_worker->quit();
    m_worker->wait();
}

#endif // ASYNCTASKABSTRACT_H

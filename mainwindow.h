#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "asynctaskabastract.h"

class MyAsyncTask;

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTaskExecute();
    void onTaskCanceled();
    void onPreExecuteCalled();
    void onPostExecuteCalled(double result, TaskState state);
    void onMohsenClicked();
    QString randomSentence();
    void onResetClicked();


private:
    Ui::MainWindow *ui;
    MyAsyncTask* task;

};


#include <unistd.h>
// -----------------------------
class MyAsyncTask : public QObject, public AsyncTaskAbstract<int, double>
{
    Q_OBJECT
public:
    MyAsyncTask() { qRegisterMetaType<TaskState>("TaskState");}
    ~MyAsyncTask() {}

    virtual void onPreExecute() override
    {
        emit preExecuteCalled();
    }

    virtual double runInBackground(int& params) override
    {
        double ret = 0.;
        for (int i = 0; i < params; ++i)
        {
            ret += i;
            sleep(1);
        }
        return ret;
    }


    virtual void onPostExecute(double &result, TaskState state) override
    {
        emit postExecuteCalled(result, state);
    }

signals:
    void preExecuteCalled();
    void postExecuteCalled(double result, TaskState state);
};

#endif // MAINWINDOW_H

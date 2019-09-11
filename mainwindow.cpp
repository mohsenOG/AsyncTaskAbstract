#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setText("");
    ui->label_num->setText("");

    task = new MyAsyncTask();

    connect(ui->pb_run, &QPushButton::clicked, this, &MainWindow::onTaskExecute);
    connect(ui->pb_cancel, &QPushButton::clicked, this, &MainWindow::onTaskCanceled);

    connect(task, &MyAsyncTask::preExecuteCalled, this, &MainWindow::onPreExecuteCalled);
    connect(task, &MyAsyncTask::postExecuteCalled, this, &MainWindow::onPostExecuteCalled);
    connect(ui->bp_mohsen, &QPushButton::clicked, this, &MainWindow::onMohsenClicked);
    connect(ui->pb_reset, &QPushButton::clicked, this, &MainWindow::onResetClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete task;
}

void MainWindow::onTaskExecute()
{
    int x = 20;
    task->execute(x);
}
void MainWindow::onTaskCanceled()
{
    task->cancel();
}

void MainWindow::onPreExecuteCalled()
{
    QString string = ui->label->text();
    string.append("\n").append("OnPreExecute is called");
    ui->label->setText(string);
}

void MainWindow::onPostExecuteCalled(double result, TaskState state)
{
    if (state == TASK_DONE) ui->label_num->setText(QString::number(result));
    else ui->label_num->setText("Execution canceled!");
    QString string = ui->label->text();
    string.append("\n").append("OnPostExecute is called").append("\n").append(state == TASK_DONE ? "DONE" : "CANCELED");
    ui->label->setText(string);
}

void MainWindow::onMohsenClicked()
{
    QString string = ui->label_mohsen->text();
    string.append("\n").append(randomSentence());
    ui->label_mohsen->setText(string);
}

QString MainWindow::randomSentence()
{
    static constexpr double fraction { 1.0 / (RAND_MAX + 1.0) };
    int num = static_cast<int>((11) * (std::rand() * fraction));
    if (num > 5) return "Leave Mohsen alone!";
    else return "Mohsen needs some money :)";
}
void MainWindow::onResetClicked()
{
    ui->label_mohsen->setText("");
    ui->label_num->setText("");
    ui->label->setText("");
}

#include "dialogmanager.h"
#include <QTimer>
#include "dialogs/dtaskdialog.h"
#include "../app/global.h"
#include "../app/filesignalmanager.h"
#include "../controllers/filejob.h"
#include "dialogs/messagewrongdialog.h"
#include <ddialog.h>

DWIDGET_USE_NAMESPACE

DialogManager::DialogManager(QObject *parent) : QObject(parent)
{
    initTaskDialog();
    initConnect();
}

DialogManager::~DialogManager()
{

}

void DialogManager::initTaskDialog()
{
    m_taskDialog = new DTaskDialog;
}

void DialogManager::initConnect()
{
    connect(fileSignalManager, &FileSignalManager::jobAdded, m_taskDialog, &DTaskDialog::addTask);
    connect(fileSignalManager, &FileSignalManager::jobRemoved, m_taskDialog, &DTaskDialog::removeTask);
    connect(fileSignalManager, &FileSignalManager::jobDataUpdated, m_taskDialog, &DTaskDialog::handleUpdateTaskWidget);
    connect(m_taskDialog, &DTaskDialog::abortTask, fileSignalManager, &FileSignalManager::abortTask);

    connect(fileSignalManager, &FileSignalManager::conflictDialogShowed, m_taskDialog, &DTaskDialog::showConflictDiloagByJob);
    connect(m_taskDialog, &DTaskDialog::conflictShowed, fileSignalManager, &FileSignalManager::conflictTimerStoped);
    connect(m_taskDialog, &DTaskDialog::conflictHided, fileSignalManager, &FileSignalManager::conflictTimerReStarted);
    connect(m_taskDialog, &DTaskDialog::conflictRepsonseConfirmed, fileSignalManager, &FileSignalManager::conflictRepsonseConfirmed);
    connect(m_taskDialog, &DTaskDialog::conflictRepsonseConfirmed, this, &DialogManager::handleConflictRepsonseConfirmed);

    connect(m_taskDialog, &DTaskDialog::abortTask, this, &DialogManager::abortJob);

    connect(fileSignalManager, &FileSignalManager::requestShowUrlWrongDialog, this, &DialogManager::showUrlWrongDialog);

}

void DialogManager::addJob(FileJob *job)
{
    m_jobs.insert(job->getJobId(), job);
}


void DialogManager::removeJob()
{
    FileJob *job = qobject_cast<FileJob *>(sender());
    m_jobs.remove(job->getJobId());
    job->deleteLater();
}


void DialogManager::abortJob(const QMap<QString, QString> &jobDetail)
{
    QString jobId = jobDetail.value("jobId");
    FileJob * job = m_jobs.value(jobId);
    job->setApplyToAll(true);
    job->setStatus(FileJob::Cancelled);
}


void DialogManager::showUrlWrongDialog(const QString &url)
{
    MessageWrongDialog d(url);
    qDebug() << url;
    d.exec();
}


void DialogManager::handleConflictRepsonseConfirmed(const QMap<QString, QString> &jobDetail, const QMap<QString, QVariant> &response)
{
    QString jobId = jobDetail.value("jobId");
    FileJob * job = m_jobs.value(jobId);
    if(job != NULL)
    {
        bool applyToAll = response.value("applyToAll").toBool();
        int code = response.value("code").toInt();
        job->setApplyToAll(applyToAll);
        //0 = coexist, 1 = replace, 2 = skip
        switch(code)
        {
        case 0:job->setStatus(FileJob::Started);break;
        case 1:
            job->setStatus(FileJob::Started);
            job->setReplace(true);
            break;
        case 2:job->setStatus(FileJob::Cancelled);break;
        default:
            qDebug() << "unknown code"<<code;
        }
    }
}

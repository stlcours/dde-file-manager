#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>

class FileOperation;

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = 0);
    ~AppController();

    void initControllers();
    void initConnect();

signals:

public slots:

private:
    FileOperation* m_fileOperationController = NULL;
};

#endif // APPCONTROLLER_H
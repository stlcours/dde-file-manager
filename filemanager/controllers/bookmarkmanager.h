#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include "basemanager.h"
#include "bookmark.h"
#include "abstractfilecontroller.h"

#include <QDir>

class AbstractFileInfo;

class BookMarkManager : public AbstractFileController, public BaseManager
{
    Q_OBJECT
public:
    explicit BookMarkManager(QObject *parent = 0);
    ~BookMarkManager();
    void load();
    void save();
    QList<BookMark *> getBookmarks();
private:
    void loadJson(const QJsonObject &json);
    void writeJson(QJsonObject &json);
    QList<BookMark *> m_bookmarks;

signals:
    void updates(const QString &directory, const QList<AbstractFileInfo*> &infoList);

public slots:
    BookMark *writeIntoBookmark(const QString &name, const QString &url);
    void removeBookmark(const QString &name, const QString &url);
    void fetchFileInformation(const QString &url,
                              int filter = int(QDir::AllEntries | QDir::NoDotAndDotDot));

    // AbstractFileController interface
public:
    const QList<AbstractFileInfo *> getChildren(const QString &fileUrl, QDir::Filters filter, bool &accepted) const;
    AbstractFileInfo *createFileInfo(const QString &fileUrl, bool &accepted) const;
};

#endif // BOOKMARKMANAGER_H

#ifndef ABSTRACTFILEINFO_H
#define ABSTRACTFILEINFO_H

#include <QSharedDataPointer>
#include <QFile>
#include <QFileInfo>

#include "durl.h"

class FileInfoData :  public QSharedData
{
public:
    DUrl url;
    mutable QString mimeTypeName;
    QFileInfo fileInfo;

private:
    friend class AbstractFileInfo;
};

class AbstractFileInfo
{
public:
    enum MenuAction {
        Open,
        OpenInNewWindow,
        OpenWith,
        OpenFileLocation,
        Compress,
        Decompress,
        DecompressHere,
        Cut,
        Copy,
        Paste,
        Rename,
        Remove,
        Delete,
        Property,
        NewFolder,
        NewFile,
        NewWindow,
        SelectAll,
        Separator,
        ClearRecent,
        ClearTrash,
        DisplayAs, /// sub menu
        SortBy, /// sub menu
        NewDocument, /// sub menu
        Restore,
        CompleteDeletion,
        Mount,
        Unmount,
        Name,
        Size,
        Type,
        CreatedDate,
        LastModifiedDate,
        Help,
        About,
        Exit,
        IconView,
        ListView
    };

    enum MenuType {
        Normal,
        SpaceArea
    };

    AbstractFileInfo();
    AbstractFileInfo(const DUrl &url);
    AbstractFileInfo(const QString &url);
    AbstractFileInfo(const AbstractFileInfo &other);

    virtual ~AbstractFileInfo();

    inline AbstractFileInfo &operator =(const AbstractFileInfo &other)
    {data = other.data; return *this;}

    virtual void setUrl(const DUrl &url);
    virtual bool exists() const;

    virtual QString filePath() const;
    virtual QString absoluteFilePath() const;
    virtual QString fileName() const;
    virtual QString displayName() const;

    virtual QString path() const;
    virtual QString absolutePath() const;

    virtual bool isCanRename() const = 0;
    virtual bool isReadable() const;
    virtual bool isWritable() const;
    virtual bool isExecutable() const;
    virtual bool isHidden() const;

    virtual bool isRelative() const;
    virtual bool isAbsolute() const;
    virtual bool makeAbsolute();

    virtual bool isFile() const;
    virtual bool isDir() const;
    virtual bool isSymLink() const;

    virtual QString readLink() const;
    inline QString symLinkTarget() const { return readLink(); }

    virtual QString owner() const;
    virtual uint ownerId() const;
    virtual QString group() const;
    virtual uint groupId() const;

    inline bool permission(QFile::Permissions permissions) const
    {return (permissions & this->permissions()) == permissions;}

    virtual QFile::Permissions permissions() const;

    virtual qint64 size() const;

    virtual QDateTime created() const;
    virtual QDateTime lastModified() const;
    virtual QDateTime lastRead() const;

    virtual QString mimeTypeName() const
    {return data->mimeTypeName;}

    inline DUrl fileUrl() const
    {return data->url;}

    inline QString scheme() const
    {return data->url.scheme();}

    virtual QIcon fileIcon() const = 0;

    virtual DUrl parentUrl() const;
    virtual QVector<MenuAction> menuActionList(MenuType type = Normal) const;

protected:
    QSharedDataPointer<FileInfoData> data;
};

QT_BEGIN_NAMESPACE
QDebug operator<<(QDebug deg, const AbstractFileInfo &info);
QT_END_NAMESPACE

#endif // ABSTRACTFILEINFO_H
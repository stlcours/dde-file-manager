#include "trashfileinfo.h"
#include "fileinfo.h"

#include "../app/global.h"

#include "../shutil/iconprovider.h"

#include <QMimeType>

TrashFileInfo::TrashFileInfo()
    : AbstractFileInfo()
{

}

TrashFileInfo::TrashFileInfo(const DUrl &url)
    : AbstractFileInfo()
{
    TrashFileInfo::setUrl(url);
}

TrashFileInfo::TrashFileInfo(const QString &url)
    : AbstractFileInfo()
{
    TrashFileInfo::setUrl(DUrl(url));
}

bool TrashFileInfo::isCanRename() const
{
    return false;
}

bool TrashFileInfo::isWritable() const
{
    return false;
}

QString TrashFileInfo::displayName() const
{
    return m_displayName;
}

void TrashFileInfo::setUrl(const DUrl &fileUrl)
{
    AbstractFileInfo::setUrl(fileUrl);

    data->fileInfo.setFile(TRASHFILEPATH + fileUrl.path());

    updateInfo();
}

QIcon TrashFileInfo::fileIcon() const
{
    return iconProvider->getFileIcon(absoluteFilePath());
}

QString TrashFileInfo::mimeTypeName() const
{
    if(data->mimeTypeName.isNull()) {
        data->mimeTypeName = FileInfo::mimeType(absoluteFilePath()).name();
    }

    return data->mimeTypeName;
}

QFileDevice::Permissions TrashFileInfo::permissions() const
{
    QFileDevice::Permissions p = AbstractFileInfo::permissions();

    p &= ~QFileDevice::WriteOwner;
    p &= ~QFileDevice::WriteUser;
    p &= ~QFileDevice::WriteGroup;
    p &= ~QFileDevice::WriteOther;

    return p;
}

QVector<AbstractFileInfo::MenuAction> TrashFileInfo::menuActionList(AbstractFileInfo::MenuType type) const
{
    QVector<MenuAction> actionKeys;

    if(type == SpaceArea) {
        actionKeys.reserve(7);

        actionKeys << MenuAction::OpenInNewWindow
                   << MenuAction::Separator
                   << MenuAction::Paste
                   << MenuAction::SelectAll
                   << MenuAction::ClearTrash
                   << MenuAction::Separator
                   << MenuAction::Property;
    } else {
        actionKeys.reserve(12);

        actionKeys << MenuAction::Open << (isDir() ? OpenInNewWindow : OpenWith)
                   << MenuAction::Separator
                   << MenuAction::Compress << MenuAction::Separator
                   << MenuAction::Copy << MenuAction::Cut << MenuAction::Separator
                   << MenuAction::Restore << MenuAction::CompleteDeletion
                   << MenuAction::Separator
                   << MenuAction::Property;
    }

    return actionKeys;
}

bool TrashFileInfo::restore() const
{
    QDir dir(originalFilePath.left(originalFilePath.lastIndexOf('/')));

    if(dir.isAbsolute() && !dir.mkpath(dir.absolutePath())) {
        qDebug() << "mk" << dir.absolutePath() << "failed!";

        return false;
    }

    return fileService->renameFile(DUrl::fromLocalFile(absoluteFilePath()), DUrl::fromLocalFile(originalFilePath));
}

void TrashFileInfo::updateInfo()
{
    const QString &filePath = this->absoluteFilePath();
    const QString &basePath = TRASHFILEPATH;
    const QString &fileBaseName = filePath.mid(basePath.size(), filePath.indexOf('/', basePath.size() + 1) - basePath.size());

    QSettings setting(TRASHINFOPATH + fileBaseName + ".trashinfo", QSettings::NativeFormat);

    setting.beginGroup("Trash Info");
    setting.setIniCodec("utf-8");

    originalFilePath = QByteArray::fromPercentEncoding(setting.value("Path").toByteArray()) + filePath.mid(basePath.size() + fileBaseName.size());
    m_displayName = originalFilePath.mid(originalFilePath.lastIndexOf('/') + 1);

    deletionDate = setting.value("DeletionDate").toString();
}
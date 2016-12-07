#ifndef TRASHDESKTOPFILEINFO_H
#define TRASHDESKTOPFILEINFO_H

#include "desktopfileinfo.h"

class TrashDesktopFileInfo : public DesktopFileInfo
{
public:
    TrashDesktopFileInfo(const DUrl& fileUrl = TrashDesktopFileInfo::trashDesktopFileUrl());

    ~TrashDesktopFileInfo();

    bool isCanRename() const Q_DECL_OVERRIDE;
    bool isCanShare() const Q_DECL_OVERRIDE;
    bool isWritable() const Q_DECL_OVERRIDE;

    QVector<MenuAction> menuActionList(MenuType type = SingleFile) const;
    QList<QIcon> additionalIcon() const;

    static DUrl trashDesktopFileUrl() ;
};

#endif // TRASHDESKTOPFILEINFO_H
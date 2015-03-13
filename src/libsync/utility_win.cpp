/*
 * Copyright (C) by Daniel Molkentin <danimo@owncloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#include <shlobj.h>
#include <winbase.h>
#include <windows.h>
#include <shlguid.h>
#include <string>
#include <QLibrary>

static const char runPathC[] = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";

typedef HRESULT (WINAPI *SHGetKnownFolderPathFun)(
    const GUID &rfid, 
    DWORD dwFlags, 
    HANDLE hToken, 
    PWSTR *ppszPath
);
typedef HRESULT (WINAPI *SHGetFolderPathFun)(
    HWND hwndOwner, 
    int nFolder, 
    HANDLE hToken, 
    DWORD dwFlags, 
    LPWSTR pszPath
);
     
static void setupFavLink_private(const QString &folder)
{
    // Windows Explorer: Place under "Favorites" (Links)
    
    SHGetKnownFolderPathFun SHGetKnownFolderPathPtr = NULL;
    SHGetFolderPathFun SHGetFolderPathPtr = NULL;
    QString linkName; 
    QDir folderDir(QDir::fromNativeSeparators(folder));
    if((!SHGetKnownFolderPathPtr) && (!SHGetFolderPathPtr))
    {
      QLibrary kernel32Lib("shell32.dll");
      if(kernel32Lib.load())
      {
        SHGetKnownFolderPathPtr = (SHGetKnownFolderPathFun) kernel32Lib.resolve("SHGetKnownFolderPath");
        SHGetFolderPathPtr = (SHGetFolderPathFun) kernel32Lib.resolve("SHGetFolderPathW");
      }
    }
     
    if(SHGetKnownFolderPathPtr) {
        /* Use new WINAPI functions */
        wchar_t *path = NULL;
        if(SHGetKnownFolderPathPtr(FOLDERID_Links, 0, NULL, &path) == S_OK) {
            QString Links= QDir::fromNativeSeparators(QString::fromWCharArray(path)); 
            linkName = QDir(Links).filePath(folderDir.dirName() + QLatin1String(".lnk"));
        }
    } else {
        /* Use legacy functions */
        wchar_t path[MAX_PATH];
        SHGetSpecialFolderPath(0, path, CSIDL_PROFILE, FALSE);
        QString profile = QDir::fromNativeSeparators(QString::fromWCharArray(path));
        linkName = QDir(profile).filePath(QDir(QLatin1String("Links")).filePath(folderDir.dirName() + QLatin1String(".lnk")));
    }
    qDebug() << Q_FUNC_INFO << " creating link from " << linkName << " to " << folder;
    if (!QFile::link(folder, linkName))
        qDebug() << Q_FUNC_INFO << "linking" << folder << "to" << linkName << "failed!";

}

bool hasLaunchOnStartup_private(const QString &appName)
{
    QString runPath = QLatin1String(runPathC);
    QSettings settings(runPath, QSettings::NativeFormat);
    return settings.contains(appName);
}

void setLaunchOnStartup_private(const QString &appName, const QString& guiName, bool enable)
{
    Q_UNUSED(guiName);
    QString runPath = QLatin1String(runPathC);
    QSettings settings(runPath, QSettings::NativeFormat);
    if (enable) {
        settings.setValue(appName, QCoreApplication::applicationFilePath().replace('/','\\'));
    } else {
        settings.remove(appName);
    }
}

static inline bool hasDarkSystray_private()
{
    return true;
}

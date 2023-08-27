#ifndef FILESANDDIRS_H
#define FILESANDDIRS_H

#include "fileandsystem_global.h"

#include <QObject>

#include <Windows.h>
#include <ShlObj.h>
#include <QSettings>
#include <Shlwapi.h>
#include <QFileInfo>
#include <QDir>
#include <QIcon>

struct fileList
{
    QString name;    // 文件或目录的名称
    QString type;    // 文件或目录的类型
    QString path;    // 文件或目录的路径
    qint64 size;     // 文件或目录的大小（字节）
    QString s_size;   // 格式化后的文件或目录大小字符串
};

class FILEANDSYSTEMSHARED_EXPORT FileAndSystem
{
public:
    FileAndSystem();

    /**
 * @brief 文件和目录操作：获取文件或目录的大小
 *
 * @param path 文件或目录的路径
 * @return qint64 文件或目录的大小（字节）
 */
    qint64 getFileOrDir_size(QString path);

    /**
 * @brief 文件和目录操作：将以字节为单位的大小转换为人类可读的格式
 *
 * @param size 文件或目录的大小（字节）
 * @return QString 格式化后的文件或目录大小字符串
 */
    QString unitConversion(qint64 size);

    /**
 * @brief 文件和目录操作：获取与指定文件关联的图标
 *
 * @param path 文件的路径
 * @return QIcon 文件关联的图标
 */
    QIcon GetSpecifiedFileIcon(const QString &path);

    /**
 * @brief 文件和目录操作：将指定的文件和目录压缩为zip文件
 *
 * @param paths 待压缩的文件和目录的路径列表
 * @param zipPath 压缩文件的保存路径
 * @param hide 是否隐藏压缩过程窗口，默认为1（隐藏）
 */
    void compressFilesAndDirs(QStringList paths, QString zipPath, bool hide = 1);

    /**
 * @brief 文件和目录操作：将指定的zip文件解压缩到指定的路径
 *
 * @param zipPath 待解压缩的zip文件路径
 * @param path 解压缩的目标路径
 * @param hide 是否隐藏解压缩过程窗口，默认为1（隐藏）
 */
    void decompressFilesAndDirs(QString zipPath, QString path, bool hide = 1);

    /**
 * @brief 文件和目录操作：获取指定磁盘的可用空间
 *
 * @param disk 磁盘的盘符（如：C、D、E等）
 * @return qint64 磁盘的可用空间大小（字节）
 */
    qint64 getDiskAvailableSpace(QString disk);

    /**
 * @brief 文件和目录操作：创建指定路径下的目录
 *
 * @param dirPath 目录的路径
 * @return bool 如果目录创建成功，则返回true；否则返回false
 */
    bool createDirectory(const QString& dirPath);

    /**
 * @brief 文件和目录操作：清空指定目录中的所有文件和子目录
 *
 * @param dirPath 目录的路径
 */
    void clearDirectory(const QString& dirPath);

    /**
 * @brief 文件和目录操作：删除指定目录及其中所有文件和子目录
 *
 * @param dirPath 目录的路径
 */
    void removeDirectory(const QString &dirPath);

    /**
 * @brief 文件和目录操作：将指定文件夹的图标更改为指定的图标名称
 *
 * @param folderPath 文件夹的路径
 * @param iconName 图标的名称
 */
    void ChangeFolderIcon(QString folderPath, QString iconName);

    /**
 * @brief 随机字符串生成：生成指定长度的随机字符串
 *
 * @param length 随机字符串的长度
 * @return QString 生成的随机字符串
 */
    QString GenerateRandomString(const int length);

    /**
 * @brief 资源管理器操作：在Windows资源管理器中打开指定文件夹的路径
 *
 * @param folderPath 文件夹的路径
 */
    void openExplorer(const QString& folderPath);

    /**
 * @brief 进程操作：检查是否有指定名称的进程正在运行
 *
 * @param processName 进程的名称
 * @return bool 如果存在指定名称的进程，则返回true；否则返回false
 */
    bool FindProcess(QString processName);

    /**
 * @brief 启动文件夹操作：获取启动文件夹的路径
 *
 * @return QString 启动文件夹的路径
 */
    QString GetstartupPath();

};

#endif // FILESANDDIRS_H

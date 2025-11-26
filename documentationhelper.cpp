#include "documentationhelper.h"
#include <QCoreApplication>
#include <QDir>

bool DocumentationHelper::openDocumentation(const QString &mdFilePath, QWidget *parent)
{
    // Check if file exists
    if (!QFile::exists(mdFilePath)) {
        QMessageBox::warning(parent, "Documentation Not Found",
                            QString("The Documentation file could not be found:\n%1\n\n"
                                    "Please ensure the docs folder is in the same directory as the application.")
                                 .arg(mdFilePath));
        return false;
    }

    // Convert to absolute path and create file URL
    QFileInfo fileInfo(mdFilePath);
    QString absolutePath = fileInfo.absoluteFilePath();
    QUrl fileUrl = QUrl::fromLocalFile(absolutePath);

    // Open in default browser/md-viewer
    bool success = QDesktopServices::openUrl(fileUrl);

    if (!success) {
        QMessageBox::warning(parent, "Cannot Open Documentation",
                             "Unable to open the documentation file.\n",
                             "Please ensure you have a default application set for .md (markdown) files.");
        return false;
    }

    return true;
}

bool DocumentationHelper::documentationExists(const QString &mdFilePath)
{
    return QFile::exists(mdFilePath);
}

QString DocumentationHelper::getDocsPath()
{
    // Get the directory where the executable is located
    QString appDir = QCoreApplication::applicationDirPath();

    // Return a path to the docs folder
    return QDir(appDir).filePath("docs");
}

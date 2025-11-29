#ifndef DOCUMENTATIONHELPER_H
#define DOCUMENTATIONHELPER_H

#include <QString>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QMessageBox>

class DocumentationHelper
{
public:
    // Opens a md file in the user's default browser
    // Returns true if successful, false otherwise
    static bool openDocumentation(const QString &mdFilePath, QWidget *parent = nullptr);

    // Opens a md file in a built-in viewer dialog
    static void showDocumentationViewer(const QString &mdFilePath, QWidget *parent = nullptr);

    // Helper to check if documentation file exists
    static bool documentationExists(const QString &mdFilePath);

    // Get the path to the docs folder relative to executable
    static QString getDocsPath();

    // Convert markdown to HTML for rendering in-app
    static QString convertMarkdownToHtml(const QString &markdownText);
};

#endif // DOCUMENTATIONHELPER_H

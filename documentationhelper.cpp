#include "documentationhelper.h"

#include <QCoreApplication>
#include <QDir>
#include <QDialog>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextStream>
#include <QRegularExpression>
#include <QHBoxLayout>
#include <QObject>

// Helper function for processing inline markdown formatting
static QString processInlineFormatting(const QString &text);

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
                             "Unable to open the documentation file.\n"
                             "Please ensure you have a default application set for .md (markdown) files.",
                             QMessageBox::Ok);
        return false;
    }

    return true;
}

void DocumentationHelper::showDocumentationViewer(const QString &mdFilePath, QWidget *parent)
{
    // Check if file exists
    if (!QFile::exists(mdFilePath)) {
        QMessageBox::warning(parent, "Documentation Not Found",
                             QString("The documentation file could not be found:\n%1\n\n"
                                     "Please ensure the docs folder is in the same directory as the application.")
                                 .arg(mdFilePath));
        return;
    }

    // Read the markdown file
    QFile file(mdFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(parent, "Error", "Could not open documentation file.");
        return;
    }

    QString markdownContent = QTextStream(&file).readAll();
    file.close();

    // Convert to HTML
    QString htmlContent = convertMarkdownToHtml(markdownContent);

    // Create viewer dialog
    QDialog *viewer = new QDialog(parent);
    viewer->setWindowTitle("DTE Assistant | Tutorials - " + QFileInfo(mdFilePath).fileName());
    viewer->resize(900, 700);
    viewer->setWindowIcon(QIcon("logo.ico"));

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(viewer);

    // Create text browser
    QTextBrowser *browser = new QTextBrowser(viewer);
    browser->setHtml(htmlContent);
    browser->setOpenExternalLinks(true);
    layout->addWidget(browser);

    // Add close button
    QPushButton *closeButton = new QPushButton("Close", viewer);
    closeButton->setMaximumWidth(100);
    QObject::connect(closeButton, &QPushButton::clicked, viewer, &QDialog::close);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    viewer->exec();
    delete viewer;
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

QString DocumentationHelper::convertMarkdownToHtml(const QString &markdownText)
{
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; padding: 20px; }"
                   "h1 { color: #2c3e50; border-bottom: 2px solid #3498db; }"
                   "h2 { color: #34495e; border-bottom: 1px solid #95a5a6; }"
                   "code { background-color: #f4f4f4; padding: 2px 4px; }"
                   "pre { background-color: #f4f4f4; padding: 10px; border-radius: 4px; }"
                   "</style></head><body>";

    QStringList lines = markdownText.split('\n');
    bool inCodeBlock = false;

    for (const QString &line : lines) {
        // Code blocks
        if (line.startsWith("```")) {
            if (inCodeBlock) {
                html += "</pre>";
                inCodeBlock = false;
            } else {
                html += "<pre>";
                inCodeBlock = true;
            }
            continue;
        }

        if (inCodeBlock) {
            html += line.toHtmlEscaped() + "\n";
            continue;
        }

        // Headers
        if (line.startsWith("# ")) {
            html += "<h1>" + line.mid(2) + "</h1>";
        } else if (line.startsWith("## ")) {
            html += "<h2>" + line.mid(3) + "</h2>";
        } else if (line.startsWith("### ")) {
            html += "<h3>" + line.mid(4) + "</h3>";
        }
        // Bold
        else if (line.contains("**")) {
            QString processed = line;
            processed.replace(QRegularExpression("\\*\\*(.+?)\\*\\*"), "<strong>\\1</strong>");
            html += "<p>" + processed + "</p>";
        }
        // Lists
        else if (line.startsWith("- ") || line.startsWith("* ")) {
            html += "<li>" + line.mid(2) + "</li>";
        }
        // Regular text
        else if (!line.trimmed().isEmpty()) {
            html += "<p>" + line + "</p>";
        }
    }

    html += "</body></html>";
    return html;
}

QString processInlineFormatting(const QString &text)
{
    QString result = text;

    // Bold (**text** or __text__)
    result.replace(QRegularExpression("\\*\\*(.+?)\\*\\*"), "<strong>\\1</strong>");
    result.replace(QRegularExpression("__(.+?)__"), "<strong>\\1</strong>");

    // Italic (*text* or _text_)
    result.replace(QRegularExpression("\\*(.+?)\\*"), "<em>\\1</em>");
    result.replace(QRegularExpression("_(.+?)_"), "<em>\\1</em>");

    // Inline code (`code`)
    result.replace(QRegularExpression("`(.+?)`"), "<code>\\1</code>");

    // Links [text](url)
    result.replace(QRegularExpression("\\[(.+?)\\]\\((.+?)\\)"), "<a href=\"\\2\">\\1</a>");

    return result;
}

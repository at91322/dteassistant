#ifndef SPREADSHEETVIEW_H
#define SPREADSHEETVIEW_H

#include "validationerror.h"

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QStringList>
#include <QMap>
#include <QVariant>

class SpreadsheetView : public QWidget
{
    Q_OBJECT

public:
    enum class HighlightType {
        None,        // Clear highlighting
        Processing,  // Yellow - currently being processed
        Success,     // Green - completed successfully
        Error,       // Red - validation error or automation failure
        Warning      // Orange - warning but not blocking
    };

    explicit SpreadsheetView(QWidget *parent = nullptr);
    ~SpreadsheetView();

    // CSV Operations
    bool loadFromCSV(const QString &filePath);
    bool exportToCSV(const QString &filePath);
    QString getCurrentFilePath() const { return m_originalFilePath; }

    // Data Access
    QVariant getCellData(int row, int col) const;
    QString getCellText(int row, int col) const;
    QMap<QString, QVariant> getRowData(int row) const;
    int getRowCount() const;
    int getColumnCount() const;
    QString getColumnHeader(int col) const;

    // Validation
    QList<ValidationError> validateData();
    bool hasValidationErrors() const;
    int getErrorCount() const { return m_errorCount; }
    int getWarningCount() const { return m_warningCount; }
    bool isValid() const { return m_isValid; }

    // Visual Feedback
    void highlightRow(int row, HighlightType type);
    void highlightCell(int row, int col, HighlightType type);
    void clearHighlights();
    void clearRowHighlight(int row);

    // Table State
    void setLocked(bool locked);
    bool isLocked() const { return m_isLocked; }

    // Auto-save
    bool autoSaveTempFile();
    QString getTempFilePath() const { return m_tempFilePath; }
    bool hasTempFile() const;
    void cleanupTempFile();

    // Required Headers (must be set before loading CSV)
    void setRequiredHeaders(const QStringList &headers);
    QStringList getRequiredHeaders() const { return m_requiredHeaders; }

signals:
    void dataLoaded(int rowCount);
    void dataValidated(bool success, int errorCount, int warningCount);
    void validationStatusChanged(bool isValid);
    void cellChanged(int row, int col);
    void csvImported(const QString &filePath);
    void csvExported(const QString &filePath);

private slots:
    void onCellChanged(int row, int column);
    void onImportClicked();
    void onExportClicked();

private:
    // UI Components
    QTableWidget *m_table;
    QLabel *m_statusLabel;
    QPushButton *m_importButton;
    QPushButton *m_exportButton;
    QVBoxLayout *m_mainLayout;

    // State
    QString m_originalFilePath;
    QString m_tempFilePath;
    QStringList m_requiredHeaders;
    bool m_isValid;
    bool m_isLocked;
    int m_errorCount;
    int m_warningCount;
    QList<ValidationError> m_validationErrors;

    // Setup
    void setupUI();
    void setupTable();
    void connectSignals();

    // CSV Helpers
    bool validateHeaders(const QStringList &headers);
    QString parseCsvLine(const QString &line, QStringList &fields);

    // Validation Helpers
    void applyValidationHighlighting();
    void clearValidationHighlighting();

    // Highlighting Helpers
    QColor getHighlightColor(HighlightType type) const;
    Qt::GlobalColor getTextColor(HighlightType type) const;
};

#endif // SPREADSHEETVIEW_H

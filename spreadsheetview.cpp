#include "spreadsheetview.h"

#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>

SpreadsheetView::SpreadsheetView(QWidget *parent)
    : QWidget(parent)
    , m_table(nullptr)
    , m_statusLabel(nullptr)
    , m_importButton(nullptr)
    , m_exportButton(nullptr)
    , m_mainLayout(nullptr)
    , m_isValid(false)
    , m_isLocked(false)
    , m_errorCount(0)
    , m_warningCount(0)
{
    setupUI();
    connectSignals();
}

SpreadsheetView::~SpreadsheetView()
{
    cleanupTempFile();
}

void SpreadsheetView::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);

    // Top button bar
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_importButton = new QPushButton("Import CSV", this);
    m_exportButton = new QPushButton("Export CSV", this);
    m_exportButton->setEnabled(false); // Disabled until data is loaded

    buttonLayout->addWidget(m_importButton);
    buttonLayout->addWidget(m_exportButton);
    buttonLayout->addStretch();

    m_mainLayout->addLayout(buttonLayout);

    // Table
    setupTable();
    m_mainLayout->addWidget(m_table);

    // Status bar
    m_statusLabel = new QLabel("No data loaded", this);
    m_statusLabel->setStyleSheet("QLabel { padding: 5px; background-color: #f0f0f0; }");
    m_mainLayout->addWidget(m_statusLabel);

    setLayout(m_mainLayout);
}

void SpreadsheetView::setupTable()
{
    m_table = new QTableWidget(this);
    m_table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(true);
}

void SpreadsheetView::connectSignals()
{
    connect(m_importButton, &QPushButton::clicked, this, &SpreadsheetView::onImportClicked);
    connect(m_exportButton, &QPushButton::clicked, this, &SpreadsheetView::onExportClicked);
    connect(m_table, &QTableWidget::cellChanged, this, &SpreadsheetView::onCellChanged);
}

void SpreadsheetView::setRequiredHeaders(const QStringList &headers)
{
    m_requiredHeaders = headers;
}

bool SpreadsheetView::loadFromCSV(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Import Error",
                             QString("Could not open file: %1").arg(filePath));
        return false;
    }

    QTextStream in(&file);
    QStringList lines;

    while (!in.atEnd()) {
        lines.append(in.readLine());
    }
    file.close();

    if (lines.isEmpty()) {
        QMessageBox::warning(this, "Import Error", "CSV file is empty");
        return false;
    }

    // Parse header row
    QStringList headers;
    parseCsvLine(lines[0], headers);

    // Validate headers if required headers are set
    if (!m_requiredHeaders.isEmpty() && !validateHeaders(headers)) {
        return false;
    }

    // Block signals while populating table
    m_table->blockSignals(true);

    // Setup table structure
    m_table->clear();
    m_table->setRowCount(lines.size() - 1); // Exclude header
    m_table->setColumnCount(headers.size());
    m_table->setHorizontalHeaderLabels(headers);

    // Populate data
    for (int i = 1; i < lines.size(); ++i) {
        QStringList fields;
        parseCsvLine(lines[i], fields);

        // Ensure we have enough fields (pad with empty if needed)
        while (fields.size() < headers.size()) {
            fields.append("");
        }

        for (int j = 0; j < headers.size() && j < fields.size(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(fields[j]);
            m_table->setItem(i - 1, j, item);
        }
    }

    m_table->blockSignals(false);

    // Store file path and create temp file
    m_originalFilePath = filePath;
    autoSaveTempFile();

    // Update UI
    m_exportButton->setEnabled(true);
    m_statusLabel->setText(QString("Loaded: %1 rows, %2 columns")
                               .arg(m_table->rowCount())
                               .arg(m_table->columnCount()));

    emit dataLoaded(m_table->rowCount());
    emit csvImported(filePath);

    return true;
}

bool SpreadsheetView::exportToCSV(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Export Error",
                             QString("Could not write to file: %1").arg(filePath));
        return false;
    }

    QTextStream out(&file);

    // Write headers
    QStringList headers;
    for (int col = 0; col < m_table->columnCount(); ++col) {
        QString header = m_table->horizontalHeaderItem(col)->text();
        // Quote headers if they contain commas
        if (header.contains(',')) {
            header = QString("\"%1\"").arg(header);
        }
        headers.append(header);
    }
    out << headers.join(',') << '\n';

    // Write data
    for (int row = 0; row < m_table->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < m_table->columnCount(); ++col) {
            QString value = getCellText(row, col);
            // Quote values if they contain commas or quotes
            if (value.contains(',') || value.contains('"')) {
                value.replace("\"", "\"\""); // Escape quotes
                value = QString("\"%1\"").arg(value);
            }
            rowData.append(value);
        }
        out << rowData.join(',') << '\n';
    }

    file.close();

    m_statusLabel->setText(QString("Exported to: %1").arg(filePath));
    emit csvExported(filePath);

    return true;
}

bool SpreadsheetView::autoSaveTempFile()
{
    if (m_originalFilePath.isEmpty()) {
        return false;
    }

    // Create temp file path (same directory as original, with .temp suffix)
    QFileInfo fileInfo(m_originalFilePath);
    m_tempFilePath = fileInfo.absolutePath() + "/" +
                     fileInfo.completeBaseName() + ".temp.csv";

    return exportToCSV(m_tempFilePath);
}

void SpreadsheetView::cleanupTempFile()
{
    if (hasTempFile()) {
        QFile::remove(m_tempFilePath);
        m_tempFilePath.clear();
    }
}

bool SpreadsheetView::hasTempFile() const
{
    return !m_tempFilePath.isEmpty() && QFile::exists(m_tempFilePath);
}

QString SpreadsheetView::parseCsvLine(const QString &line, QStringList &fields)
{
    fields.clear();
    QString field;
    bool inQuotes = false;

    for (int i = 0; i < line.length(); ++i) {
        QChar ch = line[i];

        if (ch == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                // Escaped quote
                field.append('"');
                ++i;
            } else {
                // Toggle quote state
                inQuotes = !inQuotes;
            }
        } else if (ch == ',' && !inQuotes) {
            // Field separator
            fields.append(field.trimmed());
            field.clear();
        } else {
            field.append(ch);
        }
    }

    // Add last field
    fields.append(field.trimmed());

    return QString();
}

bool SpreadsheetView::validateHeaders(const QStringList &headers)
{
    // Check if all required headers are present
    for (const QString &required : m_requiredHeaders) {
        if (!headers.contains(required)) {
            QMessageBox::warning(this, "Invalid CSV Headers",
                                 QString("Missing required column: %1\n\n"
                                         "Expected headers:\n%2\n\n"
                                         "Found headers:\n%3")
                                     .arg(required)
                                     .arg(m_requiredHeaders.join(", "))
                                     .arg(headers.join(", ")));
            return false;
        }
    }

    return true;
}

QVariant SpreadsheetView::getCellData(int row, int col) const
{
    if (row < 0 || row >= m_table->rowCount() ||
        col < 0 || col >= m_table->columnCount()) {
        return QVariant();
    }

    QTableWidgetItem *item = m_table->item(row, col);
    if (!item) {
        return QVariant();
    }

    return item->data(Qt::EditRole);
}

QString SpreadsheetView::getCellText(int row, int col) const
{
    QVariant data = getCellData(row, col);
    return data.toString();
}

QMap<QString, QVariant> SpreadsheetView::getRowData(int row) const
{
    QMap<QString, QVariant> rowData;

    if (row < 0 || row >= m_table->rowCount()) {
        return rowData;
    }

    for (int col = 0; col < m_table->columnCount(); ++col) {
        QString header = getColumnHeader(col);
        rowData[header] = getCellData(row, col);
    }

    return rowData;
}

int SpreadsheetView::getRowCount() const
{
    return m_table->rowCount();
}

int SpreadsheetView::getColumnCount() const
{
    return m_table->columnCount();
}

QString SpreadsheetView::getColumnHeader(int col) const
{
    if (col < 0 || col >= m_table->columnCount()) {
        return QString();
    }

    QTableWidgetItem *headerItem = m_table->horizontalHeaderItem(col);
    return headerItem ? headerItem->text() : QString();
}

QList<ValidationError> SpreadsheetView::validateData()
{
    // Clear previous validation state
    clearValidationHighlighting();
    m_validationErrors.clear();
    m_errorCount = 0;
    m_warningCount = 0;

    // This is a placeholder - actual validation will be done by
    // derived classes or external validators
    // For now, just check for empty required cells

    // Apply highlighting
    applyValidationHighlighting();

    // Update validation state
    m_isValid = (m_errorCount == 0);

    emit dataValidated(m_isValid, m_errorCount, m_warningCount);
    emit validationStatusChanged(m_isValid);

    return m_validationErrors;
}

bool SpreadsheetView::hasValidationErrors() const
{
    return m_errorCount > 0;
}

void SpreadsheetView::applyValidationHighlighting()
{
    for (const ValidationError &error : m_validationErrors) {
        HighlightType type = error.isError() ? HighlightType::Error : HighlightType::Warning;
        highlightCell(error.row, error.col, type);
    }

    // Update status
    if (m_isValid) {
        m_statusLabel->setText("✓ Validation passed");
        m_statusLabel->setStyleSheet("QLabel { padding: 5px; background-color: #d4edda; color: #155724; }");
    } else {
        m_statusLabel->setText(QString("✗ Validation failed - Errors: %1, Warnings: %2")
                                   .arg(m_errorCount)
                                   .arg(m_warningCount));
        m_statusLabel->setStyleSheet("QLabel { padding: 5px; background-color: #f8d7da; color: #721c24; }");
    }
}

void SpreadsheetView::clearValidationHighlighting()
{
    for (int row = 0; row < m_table->rowCount(); ++row) {
        clearRowHighlight(row);
    }
}

void SpreadsheetView::highlightRow(int row, HighlightType type)
{
    if (row < 0 || row >= m_table->rowCount()) {
        return;
    }

    QColor bgColor = getHighlightColor(type);
    Qt::GlobalColor textColor = getTextColor(type);

    for (int col = 0; col < m_table->columnCount(); ++col) {
        QTableWidgetItem *item = m_table->item(row, col);
        if (item) {
            item->setBackground(QBrush(bgColor));
            item->setForeground(QBrush(textColor));
        }
    }
}

void SpreadsheetView::highlightCell(int row, int col, HighlightType type)
{
    if (row < 0 || row >= m_table->rowCount() ||
        col < 0 || col >= m_table->columnCount()) {
        return;
    }

    QTableWidgetItem *item = m_table->item(row, col);
    if (!item) {
        item = new QTableWidgetItem();
        m_table->setItem(row, col, item);
    }

    QColor bgColor = getHighlightColor(type);
    Qt::GlobalColor textColor = getTextColor(type);

    item->setBackground(QBrush(bgColor));
    item->setForeground(QBrush(textColor));
}

void SpreadsheetView::clearHighlights()
{
    for (int row = 0; row < m_table->rowCount(); ++row) {
        clearRowHighlight(row);
    }
}

void SpreadsheetView::clearRowHighlight(int row)
{
    if (row < 0 || row >= m_table->rowCount()) {
        return;
    }

    for (int col = 0; col < m_table->columnCount(); ++col) {
        QTableWidgetItem *item = m_table->item(row, col);
        if (item) {
            item->setBackground(QBrush(Qt::white));
            item->setForeground(QBrush(Qt::black));
        }
    }
}

QColor SpreadsheetView::getHighlightColor(HighlightType type) const
{
    switch (type) {
    case HighlightType::Processing:
        return QColor(255, 255, 200); // Light yellow
    case HighlightType::Success:
        return QColor(200, 255, 200); // Light green
    case HighlightType::Error:
        return QColor(255, 200, 200); // Light red
    case HighlightType::Warning:
        return QColor(255, 220, 150); // Light orange
    case HighlightType::None:
    default:
        return Qt::white;
    }
}

Qt::GlobalColor SpreadsheetView::getTextColor(HighlightType type) const
{
    switch (type) {
    case HighlightType::Error:
    case HighlightType::Warning:
        return Qt::black;
    default:
        return Qt::black;
    }
}

void SpreadsheetView::setLocked(bool locked)
{
    m_isLocked = locked;

    if (locked) {
        m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_importButton->setEnabled(false);
        m_exportButton->setEnabled(false);
    } else {
        m_table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
        m_importButton->setEnabled(true);
        m_exportButton->setEnabled(true);
    }
}

void SpreadsheetView::onCellChanged(int row, int column)
{
    // Auto-save to temp file on any change
    autoSaveTempFile();

    emit cellChanged(row, column);
}

void SpreadsheetView::onImportClicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Import CSV File",
        QDir::homePath(),
        "CSV Files (*.csv);;All Files (*)"
        );

    if (!filePath.isEmpty()) {
        loadFromCSV(filePath);
    }
}

void SpreadsheetView::onExportClicked()
{
    QString defaultPath = m_originalFilePath.isEmpty() ?
                              QDir::homePath() + "/export.csv" :
                              m_originalFilePath;

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Export CSV File",
        defaultPath,
        "CSV Files (*.csv);;All Files (*)"
        );

    if (!filePath.isEmpty()) {
        exportToCSV(filePath);
    }
}

#ifndef EXCELMANAGER_H
#define EXCELMANAGER_H

#include <QString>
#include <QVariant>

#ifdef _WIN32
#include <windows.h>
#include <comdef.h>
#endif

class ExcelManager
{
public:
    ExcelManager();
    ~ExcelManager();

    // Connect to running Excel instance (like ComObjActive)
    bool connectToActiveExcel();

    // Check if connected
    bool isConnected() const;

    // Get cell value
    QVariant getCellValue(const QString &cellAddress);

    // Set cell value
    bool setCellValue(const QString &cellAddress, const QVariant &value);

    // Get active sheet name
    QString getActiveSheetName();

    // Release Excel connection
    void disconnect();

private:
#ifdef _WIN32
    IDispatch *m_excelApp;
    IDispatch *m_workbooks;
    IDispatch *m_activeWorkbook;
    IDispatch *m_activeSheet;

    // Helper to invoke COM methods
    HRESULT invokeMethod(IDispatch *obj, LPCOLESTR method, VARIANT *result,
                         WORD flags = DISPATCH_METHOD, int argCount = 0, ...);

    // Helper to get COM property
    HRESULT getProperty(IDispatch *obj, LPCOLESTR property, VARIANT *result);
#endif
};

#endif // EXCELMANAGER_H


#include "excelmanager.h"

#ifdef _WIN32

ExcelManager::ExcelManager()
    : m_excelApp(nullptr)
    , m_workbooks(nullptr)
    , m_activeWorkbook(nullptr)
    , m_activeSheet(nullptr)
{
    CoInitialize(nullptr);
}

ExcelManager::~ExcelManager()
{
    disconnect();
    CoUninitialize();
}

bool ExcelManager::connectToActiveExcel()
{
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(L"Excel.Application", &clsid);

    if (FAILED(hr)) {
        return false;
    }

    // Get active Excel instance (like ComObjActive)
    hr = GetActiveObject(clsid, nullptr, (IUnknown**)&m_excelApp);

    if (FAILED(hr) || m_excelApp == nullptr) {
        return false;
    }

    // Get active workbook
    VARIANT result;
    VariantInit(&result);

    hr = getProperty(m_excelApp, L"ActiveWorkbook", &result);
    if (SUCCEEDED(hr) && result.vt == VT_DISPATCH) {
        m_activeWorkbook = result.pdispVal;
    }

    // Get active sheet
    VariantInit(&result);
    hr = getProperty(m_excelApp, L"ActiveSheet", &result);
    if (SUCCEEDED(hr) && result.vt == VT_DISPATCH) {
        m_activeSheet = result.pdispVal;
    }

    return (m_activeWorkbook != nullptr && m_activeSheet != nullptr);
}

bool ExcelManager::isConnected() const
{
    return (m_excelApp != nullptr && m_activeSheet != nullptr);
}

QVariant ExcelManager::getCellValue(const QString &cellAddress)
{
    if (!isConnected()) {
        return QVariant();
    }

    // Get Range object
    VARIANT result;
    VariantInit(&result);

    std::wstring wCellAddr = cellAddress.toStdWString();

    VARIANT arg;
    VariantInit(&arg);
    arg.vt = VT_BSTR;
    arg.bstrVal = SysAllocString(wCellAddr.c_str());

    DISPPARAMS params;
    params.cArgs = 1;
    params.rgvarg = &arg;
    params.cNamedArgs = 0;
    params.rgdispidNamedArgs = nullptr;

    DISPID dispid;
    OLECHAR methodName[] = L"Range";  // Mutable array instead of const literal
    LPOLESTR pMethodName = methodName;

    HRESULT hr = m_activeSheet->GetIDsOfNames(IID_NULL, &pMethodName, 1,
                                              LOCALE_USER_DEFAULT, &dispid);
    if (FAILED(hr)) {
        VariantClear(&arg);
        return QVariant();
    }

    IDispatch *rangeObj = nullptr;
    hr = m_activeSheet->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                               DISPATCH_PROPERTYGET, &params, &result, nullptr, nullptr);

    VariantClear(&arg);

    if (FAILED(hr) || result.vt != VT_DISPATCH) {
        return QVariant();
    }

    rangeObj = result.pdispVal;

    // Get Value property
    VariantInit(&result);
    hr = getProperty(rangeObj, L"Value", &result);

    rangeObj->Release();

    if (FAILED(hr)) {
        return QVariant();
    }

    // Convert VARIANT to QVariant
    QVariant qValue;
    if (result.vt == VT_BSTR) {
        qValue = QString::fromWCharArray(result.bstrVal);
    } else if (result.vt == VT_I4) {
        qValue = QVariant(static_cast<int>(result.lVal));  // Explicit cast to int
    } else if (result.vt == VT_R8) {
        qValue = QVariant(result.dblVal);  // Explicit QVariant construction
    }

    VariantClear(&result);
    return qValue;
}

void ExcelManager::disconnect()
{
    if (m_activeSheet) {
        m_activeSheet->Release();
        m_activeSheet = nullptr;
    }

    if (m_activeWorkbook) {
        m_activeWorkbook->Release();
        m_activeWorkbook = nullptr;
    }

    if (m_workbooks) {
        m_workbooks->Release();
        m_workbooks = nullptr;
    }

    if (m_excelApp) {
        m_excelApp->Release();
        m_excelApp = nullptr;
    }
}

HRESULT ExcelManager::getProperty(IDispatch *obj, LPCOLESTR property, VARIANT *result)
{
    DISPID dispid;
    LPOLESTR propName = const_cast<LPOLESTR>(property);

    HRESULT hr = obj->GetIDsOfNames(IID_NULL, &propName, 1,
                                    LOCALE_USER_DEFAULT, &dispid);

    if (FAILED(hr)) {
        return hr;
    }

    DISPPARAMS params = {nullptr, nullptr, 0, 0};

    return obj->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                       DISPATCH_PROPERTYGET, &params, result, nullptr, nullptr);
}

#endif // _WIN32


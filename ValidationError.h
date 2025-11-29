#ifndef VALIDATIONERROR_H
#define VALIDATIONERROR_H

#include <QString>

struct ValidationError
{
    enum Severity {
        Error,      // Blocking - must be fixed before automation can run
        Warning     // Non-blocking - user can proceed but should be aware
    };

    int row;
    int col;
    QString message;
    Severity severity;

    ValidationError(int r, int c, const QString &msg, Severity sev = Error)
        : row(r), col(c), message(msg), severity(sev)
    {}

    bool isError() const { return severity == Error; }
    bool isWarning() const { return severity == Warning; }
};

#endif // VALIDATIONERROR_H

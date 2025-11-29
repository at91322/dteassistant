#ifndef TRANSCRIPTREVIEWVALIDATOR_H
#define TRANSCRIPTREVIEWVALIDATOR_H

#include "validationerror.h"
#include <QTableWidget>
#include <QList>
#include <QString>

class TranscriptReviewValidator
{
public:
    // Main validation entry point
    static QList<ValidationError> validate(const QTableWidget *table);

    // Column indices (based on your schema)
    enum Column {
        AttPeriodNo = 0,    // A: "Att. Per. No." - integer
        Term = 1,           // B: "Term" - 6 digit integer
        Subject = 2,        // C: "Subject" - string
        Course = 3,         // D: "Course" - string
        Title = 4,          // E: "Title" - string
        Credits = 5,        // F: "Credits" - two-decimal float
        Grade = 6,          // G: "Grade" - string
        Duplicate = 7,      // H: "Duplicate" - integer
        CustomTitle = 8,    // I: "CustomTitle" - string
        LCCSubject = 9,     // J: "Subject" (LCC equivalent) - string
        LCCCourse = 10,     // K: "Course" (LCC equivalent) - string
        LCCTitle = 11,      // L: "Title" (LCC equivalent) - string
        StartTerm = 12,     // M: "Start Term" - 6 digit integer
        LCCCredits = 13,    // N: "Credits" (LCC equivalent) - two-decimal float
        Action = 14,        // O: "Action" - list of strings
        Combo = 15          // P: "Combo" - TBD
    };

    // Expected headers
    static QStringList getRequiredHeaders();

private:
    // Individual field validators
    static bool validateInteger(const QString &value, bool allowEmpty = false);
    static bool validateTermCode(const QString &value);
    static bool validateString(const QString &value, bool allowEmpty = false);
    static bool validateFloat(const QString &value, bool allowEmpty = false);
    static bool validateAction(const QString &value);

    // Value range validators
    static bool isValidTermFormat(const QString &term);
    static bool isReasonableCredits(double credits);

    // Helper to get cell value
    static QString getCellValue(const QTableWidget *table, int row, int col);
};

#endif // TRANSCRIPTREVIEWVALIDATOR_H


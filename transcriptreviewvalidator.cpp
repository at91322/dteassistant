#include "transcriptreviewvalidator.h"
#include <QTableWidgetItem>
#include <QRegularExpression>

QStringList TranscriptReviewValidator::getRequiredHeaders()
{
    return QStringList{
        "Att. Per. No.",  // A
        "Term",           // B
        "Subject",        // C
        "Course",         // D
        "Title",          // E
        "Credits",        // F
        "Grade",          // G
        "Duplicate",      // H
        "CustomTitle",    // I
        "Subject",        // J (LCC - will be duplicate header name)
        "Course",         // K (LCC - will be duplicate header name)
        "Title",          // L (LCC - will be duplicate header name)
        "Start Term",     // M
        "Credits",        // N (LCC - will be duplicate header name)
        "Action",         // O
        "Combo"           // P
    };
}

QList<ValidationError> TranscriptReviewValidator::validate(const QTableWidget *table)
{
    QList<ValidationError> errors;

    if (!table) {
        return errors;
    }

    // Validate each row
    for (int row = 0; row < table->rowCount(); ++row) {
        // Column A: Att. Per. No. - integer (required)
        QString attPeriodNo = getCellValue(table, row, AttPeriodNo);
        if (!validateInteger(attPeriodNo, false)) {
            errors.append(ValidationError(
                row, AttPeriodNo,
                "Attendance Period Number must be an integer",
                ValidationError::Error
                ));
        }

        // Column B: Term - 6 digit integer (required)
        QString term = getCellValue(table, row, Term);
        if (!validateTermCode(term)) {
            errors.append(ValidationError(
                row, Term,
                "Term must be a 6-digit term code (e.g., 202630)",
                ValidationError::Error
                ));
        }

        // Column C: Subject - string (required)
        QString subject = getCellValue(table, row, Subject);
        if (!validateString(subject, false)) {
            errors.append(ValidationError(
                row, Subject,
                "Subject code is required",
                ValidationError::Error
                ));
        }

        // Column D: Course - string (required)
        QString course = getCellValue(table, row, Course);
        if (!validateString(course, false)) {
            errors.append(ValidationError(
                row, Course,
                "Course number is required",
                ValidationError::Error
                ));
        }

        // Column E: Title - string (required)
        QString title = getCellValue(table, row, Title);
        if (!validateString(title, false)) {
            errors.append(ValidationError(
                row, Title,
                "Course title is required",
                ValidationError::Error
                ));
        }

        // Column F: Credits - two-decimal float (required)
        QString credits = getCellValue(table, row, Credits);
        if (!validateFloat(credits, false)) {
            errors.append(ValidationError(
                row, Credits,
                "Credits must be a number (e.g., 4.00)",
                ValidationError::Error
                ));
        } else {
            double creditsValue = credits.toDouble();
            if (!isReasonableCredits(creditsValue)) {
                errors.append(ValidationError(
                    row, Credits,
                    QString("Credits value %1 seems unreasonable (expected 0.5-20)").arg(creditsValue),
                    ValidationError::Warning
                    ));
            }
        }

        // Column G: Grade - string (required)
        QString grade = getCellValue(table, row, Grade);
        if (!validateString(grade, false)) {
            errors.append(ValidationError(
                row, Grade,
                "Grade is required",
                ValidationError::Error
                ));
        }

        // Column H: Duplicate - integer (optional)
        QString duplicate = getCellValue(table, row, Duplicate);
        if (!duplicate.isEmpty() && !validateInteger(duplicate, true)) {
            errors.append(ValidationError(
                row, Duplicate,
                "Duplicate must be an integer if provided",
                ValidationError::Error
                ));
        }

        // Column I: CustomTitle - string (optional)
        // No validation needed - any string is acceptable

        // Column J: LCC Subject - string (optional for now)
        QString lccSubject = getCellValue(table, row, LCCSubject);
        if (!lccSubject.isEmpty() && lccSubject.trimmed().isEmpty()) {
            errors.append(ValidationError(
                row, LCCSubject,
                "LCC Subject should not be whitespace only",
                ValidationError::Warning
                ));
        }

        // Column K: LCC Course - string (optional for now)
        QString lccCourse = getCellValue(table, row, LCCCourse);
        if (!lccCourse.isEmpty() && lccCourse.trimmed().isEmpty()) {
            errors.append(ValidationError(
                row, LCCCourse,
                "LCC Course should not be whitespace only",
                ValidationError::Warning
                ));
        }

        // Column L: LCC Title - string (optional for now)
        // No validation needed

        // Column M: Start Term - 6 digit integer (optional)
        QString startTerm = getCellValue(table, row, StartTerm);
        if (!startTerm.isEmpty() && !validateTermCode(startTerm)) {
            errors.append(ValidationError(
                row, StartTerm,
                "Start Term must be a 6-digit term code if provided",
                ValidationError::Error
                ));
        }

        // Column N: LCC Credits - two-decimal float (optional)
        QString lccCredits = getCellValue(table, row, LCCCredits);
        if (!lccCredits.isEmpty() && !validateFloat(lccCredits, true)) {
            errors.append(ValidationError(
                row, LCCCredits,
                "LCC Credits must be a number if provided",
                ValidationError::Error
                ));
        }

        // Column O: Action - specific values (optional)
        QString action = getCellValue(table, row, Action);
        if (!action.isEmpty() && !validateAction(action)) {
            errors.append(ValidationError(
                row, Action,
                "Action must be one of: Add, Update, No Change",
                ValidationError::Error
                ));
        }

        // Column P: Combo - TBD (no validation for now)
    }

    return errors;
}

bool TranscriptReviewValidator::validateInteger(const QString &value, bool allowEmpty)
{
    if (value.isEmpty() || value.trimmed().isEmpty()) {
        return allowEmpty;
    }

    bool ok;
    value.toInt(&ok);
    return ok;
}

bool TranscriptReviewValidator::validateTermCode(const QString &value)
{
    if (value.isEmpty() || value.trimmed().isEmpty()) {
        return false;
    }

    // Must be exactly 6 digits
    if (value.length() != 6) {
        return false;
    }

    // Must be all digits
    bool ok;
    int termCode = value.toInt(&ok);
    if (!ok) {
        return false;
    }

    // Basic format check: YYYYTT where TT is 10, 20, 30, or 40
    return isValidTermFormat(value);
}

bool TranscriptReviewValidator::validateString(const QString &value, bool allowEmpty)
{
    if (value.isEmpty() || value.trimmed().isEmpty()) {
        return allowEmpty;
    }

    return true;
}

bool TranscriptReviewValidator::validateFloat(const QString &value, bool allowEmpty)
{
    if (value.isEmpty() || value.trimmed().isEmpty()) {
        return allowEmpty;
    }

    bool ok;
    value.toDouble(&ok);
    return ok;
}

bool TranscriptReviewValidator::validateAction(const QString &value)
{
    QString normalized = value.trimmed();

    // Case-insensitive comparison of valid actions
    QStringList validActions = {"Add", "Update", "No Change"};

    for (const QString &validAction : validActions) {
        if (normalized.compare(validAction, Qt::CaseInsensitive) == 0) {
            return true;
        }
    }

    return false;
}

bool TranscriptReviewValidator::isValidTermFormat(const QString &term)
{
    if (term.length() != 6) {
        return false;
    }

    // Extract last 2 digits
    QString lastTwo = term.right(2);

    // Valid term codes end in 10 (Summer), 20 (Fall), 30 (Winter), or 40 (Spring)
    return (lastTwo == "10" || lastTwo == "20" || lastTwo == "30" || lastTwo == "40");
}

bool TranscriptReviewValidator::isReasonableCredits(double credits)
{
    // Most courses are between 0.5 and 20 credits
    return (credits >= 0.5 && credits <= 20.0);
}

QString TranscriptReviewValidator::getCellValue(const QTableWidget *table, int row, int col)
{
    if (!table || row < 0 || row >= table->rowCount() ||
        col < 0 || col >= table->columnCount()) {
        return QString();
    }

    QTableWidgetItem *item = table->item(row, col);
    return item ? item->text().trimmed() : QString();
}


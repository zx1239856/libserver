#ifndef _QCRON_H
#define _QCRON_H

#include <QObject>
#include <QDateTime>
#include "qcronfield.h"

class QCron : public QObject
{
    Q_OBJECT

public:
    QCron();
    QCron(const QString & pattern);
    ~QCron();

    // Accessors.
    bool isValid() const
        { return _is_valid; }

    const QString & error() const
        { return _error; }

    // Features.

    QDateTime next();
    QDateTime next(QDateTime dt);
    void catchUp(QDateTime & dt, EField field, int value);
    bool match(const QDateTime & dt) const;
    void add(QDateTime & dt, EField field, int value);

signals:
    void activated(QCron *);
    void deactivated(QCron *);

  private:
    bool _is_valid;
    bool _is_active;
    QString _error;
    QCronField _fields[6];

    void _init();
    void _setError(const QString & error);
    void _parsePattern(const QString & pattern);
    void _parseField(QString & field_str,
                     EField field);
    QString _validCharacters(EField field);
    void _process(QDateTime & dt, EField field);

private slots:
    void _checkState();
};

#endif

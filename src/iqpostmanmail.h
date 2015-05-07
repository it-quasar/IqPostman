/**********************************************************************************
 * Copyright © 2015 Pavel A. Puchkov                                              *
 *                                                                                *
 * This file is part of IqPostman.                                                *
 *                                                                                *
 * IqPostman is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or              *
 * (at your option) any later version.                                            *
 *                                                                                *
 * IqPostman is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 * GNU Lesser General Public License for more details.                            *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public License       *
 * along with IqPostman.  If not, see <http://www.gnu.org/licenses/>.             *
 **********************************************************************************/

#ifndef IQPOSTMANMAIL_H
#define IQPOSTMANMAIL_H

#include <QObject>
#include <QDateTime>
#include "iqpostman_global.h"

class IQPOSTMANSHARED_EXPORT IqPostmanMail {
public:
    explicit IqPostmanMail();
    enum MailFlag
    {
        Seen = 0x1,
        Answered = 0x2,
        Deleted = 0x4,
        Draft = 0x8
    };
    Q_DECLARE_FLAGS(MailFlags, MailFlag)

public:
    QString sourceId() const;
    void setSourceId(const QString &sourceId);

    MailFlags flags() const;
    void setFlags(const MailFlags &flags);

    QString subject() const;
    void setSubject(const QString &subject);

    QString from() const;
    void setFrom(const QString &from);

    QString to() const;
    void setTo(const QString &to);

    QString cc() const;
    void setCc(const QString &cc);

    QDateTime date() const;
    void setDate(const QDateTime &date);

private:
    QString m_sourceId;
    MailFlags m_flags;
    QString m_subject;
    QString m_from;
    QString m_to;
    QString m_cc;
    QDateTime m_date;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(IqPostmanMail::MailFlags)

#endif // IQPOSTMANMAIL_H

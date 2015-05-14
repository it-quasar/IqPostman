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
#include <QSharedData>
#include "iqpostmanmailheader.h"
#include "iqpostmanabstractcontent.h"
#include "iqpostman_global.h"

class IQPOSTMANSHARED_EXPORT IqPostmanMail : public QObject
{
    Q_OBJECT
    Q_PROPERTY(IqPostmanMailHeader * header READ header CONSTANT)
    Q_PROPERTY(QString sourceId READ sourceId WRITE setSourceId NOTIFY sourceIdChanged)
    Q_PROPERTY(MailFlags flags READ flags WRITE setFlags NOTIFY flagsChanged)
    Q_PROPERTY(IqPostmanAbstractContent * content READ content WRITE setContent NOTIFY contentChanged)
public:
    enum MailFlag
    {
        UnknownFlag = 0x0,
        Seen = 0x1,
        Answered = 0x2,
        Deleted = 0x4,
        Draft = 0x8
    };
    Q_DECLARE_FLAGS(MailFlags, MailFlag)

    explicit IqPostmanMail(QObject *parent = Q_NULLPTR);
    ~IqPostmanMail();

public:
    QString sourceId() const;
    void setSourceId(const QString &sourceId);

    MailFlags flags() const;
    void setFlags(const MailFlags &flags);

    IqPostmanMailHeader *header() const;

    IqPostmanAbstractContent *content() const;
    void setContent(IqPostmanAbstractContent *content);

signals:
    void sourceIdChanged();
    void flagsChanged();
    void contentChanged();

private:
    IqPostmanMailHeader *m_header;
    QString m_sourceId;
    MailFlags m_flags;
    IqPostmanAbstractContent *m_content;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(IqPostmanMail::MailFlags)

#endif // IQPOSTMANMAIL_H

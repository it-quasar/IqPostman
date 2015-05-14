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

#ifndef IQPOSTMANMAILHEADER_H
#define IQPOSTMANMAILHEADER_H

#include <QObject>
#include <QDateTime>
#include <QSharedData>
#include "iqpostmanabstractcontenttype.h"
#include "iqpostman_global.h"
#include "iqpostmanmime.h"

class IQPOSTMANSHARED_EXPORT IqPostmanMailHeader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString messageId READ messageId WRITE setMessageId NOTIFY messageIdChanged)
    Q_PROPERTY(QString subject READ subject WRITE setSubject NOTIFY subjectChanged)
    Q_PROPERTY(QString from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QString to READ to WRITE setTo NOTIFY toChanged)
    Q_PROPERTY(QString cc READ cc WRITE setCc NOTIFY ccChanged)
    Q_PROPERTY(QDateTime date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QString mimeVersion READ mimeVersion WRITE setMimeVersion NOTIFY mimeVersionChanged)
    Q_PROPERTY(IqPostmanMime::ContentTransferEncoding contentTransferEncoding READ contentTransferEncoding WRITE setContentTransferEncoding NOTIFY contentTransferEncodingChanged)
    Q_PROPERTY(IqPostmanAbstractContentType * contentType READ contentType WRITE setContentType NOTIFY contentTypeChanged)
public:
    explicit IqPostmanMailHeader(QObject *parent = Q_NULLPTR);
    ~IqPostmanMailHeader();

    void fromString(const QString &string);

public:
    QString messageId() const;
    void setMessageId(const QString &messageId);

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

    QString mimeVersion() const;
    void setMimeVersion(const QString &mimeVersion);

    IqPostmanMime::ContentTransferEncoding contentTransferEncoding() const;
    void setContentTransferEncoding(const IqPostmanMime::ContentTransferEncoding &contentTransferEncoding);

    IqPostmanAbstractContentType* contentType() const;
    void setContentType(IqPostmanAbstractContentType* contentType);

signals:
    void messageIdChanged();
    void subjectChanged();
    void fromChanged();
    void toChanged();
    void ccChanged();
    void dateChanged();
    void mimeVersionChanged();
    void contentTransferEncodingChanged();
    void contentTypeChanged();

private:
    QString headerDecode(const QString &string) const;

private:
    IqPostmanAbstractContentType *m_contentType;
    QString m_messageId;
    QString m_subject;
    QString m_from;
    QString m_to;
    QString m_cc;
    QDateTime m_date;
    QString m_mimeVersion;
    IqPostmanMime::ContentTransferEncoding m_contentTransferEncoding;
};


#endif // IQPOSTMANMAILHEADER_H

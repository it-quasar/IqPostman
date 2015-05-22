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

#include "iqpostmanmailheader.h"
#include "iqpostmanmail.h"
#include "iqpostmantextcontenttype.h"
#include "iqpostmanabstractclient.h"
#include <QStringList>
#include <QRegExp>
#include <QByteArray>
#include <QTextCodec>

IqPostmanMailHeader::IqPostmanMailHeader(QObject *parent) :
    QObject(parent),
    m_contentType(Q_NULLPTR),
    m_contentTransferEncoding(IqPostmanAbstractContent::Encoding7bit)
{
}

IqPostmanMailHeader::~IqPostmanMailHeader()
{
}

void IqPostmanMailHeader::fromStringList(const QStringList &stringList)
{
    QStringList headerLines;
    foreach (const QString &headerRow, stringList) {
        QRegExp newRowRx("^[^ ]+: ");
        if (newRowRx.indexIn(headerRow) != -1)
            headerLines << headerRow.trimmed();
        else
            headerLines.last().append(headerRow.trimmed());
    }

    foreach (const QString &headerRow, headerLines) {
        QRegExp messageIdRx("^Message-ID: (.*)$", Qt::CaseInsensitive);
        QRegExp fromRx("^From: (.*)$", Qt::CaseInsensitive);
        QRegExp toRx("^To: (.*)$", Qt::CaseInsensitive);
        QRegExp ccRx("^Cc: (.*)$", Qt::CaseInsensitive);
        QRegExp subjectRx("^Subject: (.*)$", Qt::CaseInsensitive);
        QRegExp dateRx("^Date: (.*)$", Qt::CaseInsensitive);
        QRegExp mimeVerstionRx("^MIME-Version: (.*)$", Qt::CaseInsensitive);
        QRegExp contentTransferEncodingRx("^Content-Transfer-Encoding: (.*)$", Qt::CaseInsensitive);
        QRegExp contentTypeRx("^Content-Type: (.*)$", Qt::CaseInsensitive);

        if (messageIdRx.indexIn(headerRow) != -1)
            setMessageId(messageIdRx.cap(1));
        else if (fromRx.indexIn(headerRow) != -1)
            setFrom(headerDecode(fromRx.cap(1)));
        else if (toRx.indexIn(headerRow) != -1)
            setTo(headerDecode(toRx.cap(1)));
        else if (ccRx.indexIn(headerRow) != -1)
            setCc(headerDecode(ccRx.cap(1)));
        else if (subjectRx.indexIn(headerRow) != -1)
            setSubject(headerDecode(subjectRx.cap(1)));
        else if (dateRx.indexIn(headerRow) != -1) {
            QDateTime date = QDateTime::fromString(dateRx.cap(1), Qt::RFC2822Date);
            setDate(date);
        } else if (mimeVerstionRx.indexIn(headerRow) != -1)
            setMimeVersion(mimeVerstionRx.cap(1));
        else if (contentTransferEncodingRx.indexIn(headerRow) != -1)
            setContentTransferEncoding(IqPostmanAbstractContent::contentTransferEncodingFromString(headerRow));
        else if (contentTypeRx.indexIn(headerRow) != -1)
            setContentType(IqPostmanAbstractContentType::createFromString(headerRow));
    }
}


QString IqPostmanMailHeader::headerDecode(const QString &string) const
{
    QString result;
    QRegExp partRx ("=\\?([^\\?]+)\\?B\\?([^\\?]*)\\?=");
    int offset = 0;
    while (partRx.indexIn(string, offset) != -1) {
        if (offset != partRx.pos(0))
            result.append(string.mid(offset, partRx.pos(0) - offset));
        offset = partRx.pos(0) + partRx.matchedLength();

        QByteArray base64decode = QByteArray::fromBase64(partRx.cap(2).toLocal8Bit());
        QString encodedStr = QTextCodec::codecForName(partRx.cap(1).toLocal8Bit().constData())->toUnicode(base64decode);
        result.append(encodedStr);
    }

    if (offset != string.length() - 1)
        result.append(string.mid(offset));

    return result;
}


QString IqPostmanMailHeader::messageId() const
{
    return m_messageId;
}

void IqPostmanMailHeader::setMessageId(const QString &messageId)
{
    if (m_messageId != messageId) {
        m_messageId = messageId;
        emit messageIdChanged();
    }
}

QString IqPostmanMailHeader::subject() const
{
    return m_subject;
}

void IqPostmanMailHeader::setSubject(const QString &subject)
{
    if (m_subject != subject) {
        m_subject = subject;
        emit subjectChanged();
    }
}

QString IqPostmanMailHeader::from() const
{
    return m_from;
}

void IqPostmanMailHeader::setFrom(const QString &from)
{
    if (m_from != from) {
        m_from = from;
        emit fromChanged();
    }
}

QString IqPostmanMailHeader::to() const
{
    return m_to;
}

void IqPostmanMailHeader::setTo(const QString &to)
{
    if (m_to != to) {
        m_to = to;
        emit toChanged();
    }
}

QString IqPostmanMailHeader::cc() const
{
    return m_cc;
}

void IqPostmanMailHeader::setCc(const QString &cc)
{
    if (m_cc != cc) {
        m_cc = cc;
        emit ccChanged();
    }
}

QDateTime IqPostmanMailHeader::date() const
{
    return m_date;
}

void IqPostmanMailHeader::setDate(const QDateTime &date)
{
    if (m_date != date) {
        m_date = date;
        emit dateChanged();
    }
}

QString IqPostmanMailHeader::mimeVersion() const
{
    return m_mimeVersion;
}

void IqPostmanMailHeader::setMimeVersion(const QString &mimeVersion)
{
    if (m_mimeVersion != mimeVersion) {
        m_mimeVersion = mimeVersion;
        emit mimeVersionChanged();
    }
}

IqPostmanAbstractContent::ContentTransferEncoding IqPostmanMailHeader::contentTransferEncoding() const
{
    return m_contentTransferEncoding;
}

void IqPostmanMailHeader::setContentTransferEncoding(const IqPostmanAbstractContent::ContentTransferEncoding &contentTransferEncoding)
{
    if (m_contentTransferEncoding != contentTransferEncoding) {
        m_contentTransferEncoding = contentTransferEncoding;
        emit contentTransferEncodingChanged();
    }
}

IqPostmanAbstractContentType *IqPostmanMailHeader::contentType() const
{
    return m_contentType;
}

void IqPostmanMailHeader::setContentType(IqPostmanAbstractContentType *contentType)
{
    if (m_contentType != contentType) {
        if (m_contentType)
            m_contentType->deleteLater();

        m_contentType = contentType;
        if (m_contentType)
            m_contentType->setParent(this);
        emit contentTypeChanged();
    }
}

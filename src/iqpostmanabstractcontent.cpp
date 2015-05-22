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

#define ENCODING_7BIT "7bit"
#define ENCODING_QUOTED_PRINABLE "quoted-printable"
#define ENCODING_BASE64 "base64"
#define ENCODING_8BIT "8bit"
#define ENCODING_BINARY "binary"

#include "iqpostmanabstractcontent.h"
#include <QRegExp>
#include <QStringList>
#include "iqpostmantextcontent.h"
#include "iqpostmanmultipartcontent.h"
#include "iqpostmanimagecontent.h"
#include "iqpostmanaudiocontent.h"
#include "iqpostmanvideocontent.h"
#include "iqpostmanapplicationcontent.h"
#include "iqpostmanabstractclient.h"

IqPostmanAbstractContent::IqPostmanAbstractContent(QObject *parent):
    QObject(parent),
    m_contentTransferEncoding(EncodingUnknown),
    m_contentDisposition(new IqPostmanContentDisposition(this))
{
}

IqPostmanAbstractContent::~IqPostmanAbstractContent()
{
}

bool IqPostmanAbstractContent::fromStringList(const QStringList &stringList)
{
    IqPostmanContentData contentData = splitContent(stringList);
    if (!contentType()->fromString(contentData.contentType))
        return false;
    if (!contentDisposition()->fromString(contentData.contentDisposition))
        return false;

    setTransferEncoding(IqPostmanAbstractContent::contentTransferEncodingFromString(contentData.contentTransferEncoding));

    if (!contentData.contentId.isEmpty()) {
        QRegExp contentIdRx ("^Content-ID: <([^>]+)>", Qt::CaseInsensitive);
        contentIdRx.indexIn(contentData.contentId);
        setContentId(contentIdRx.cap(1));
    }

    return fromContentData(contentData);
}

QStringList IqPostmanAbstractContent::toStringList() const
{
    IqPostmanContentData contentData = toContentData();

    QStringList result;
    result.append(contentType()->toString());
    result.append(contentDisposition()->toString());

    if (!contentId().isEmpty())
        result.append(QString("Content-ID: <%0>")
                      .arg(contentId()));

    if (transferEncoding() != IqPostmanAbstractContent::EncodingUnknown)
        result.append(IqPostmanAbstractContent::contentTransferEncodingToString(transferEncoding()));

    result.append("");

    result.append(contentData.content);

    return result;
}

IqPostmanAbstractContent *IqPostmanAbstractContent::createFromStringList(const QStringList &stringList)
{
    if (stringList.isEmpty())
        return Q_NULLPTR;

    switch (IqPostmanAbstractContentType::contentTypeFromString(stringList.first())) {
    case IqPostmanAbstractContentType::TypeText: {
        IqPostmanTextContent *result = new IqPostmanTextContent();
        if (result->fromStringList(stringList))
            return result;
        delete result;
        break;
    }
    case IqPostmanAbstractContentType::TypeMultipart: {
        IqPostmanMultipartContent *result = new IqPostmanMultipartContent();
        if (result->fromStringList(stringList))
            return result;
        delete result;
        break;
    }
    case IqPostmanAbstractContentType::TypeImage: {
        IqPostmanImageContent *result = new IqPostmanImageContent();
        if (result->fromStringList(stringList))
            return result;
        delete result;
        break;
    }
    case IqPostmanAbstractContentType::TypeAudio: {
        IqPostmanAudioContent *result = new IqPostmanAudioContent();
        if (result->fromStringList(stringList))
            return result;
        delete result;
        break;
    }
    case IqPostmanAbstractContentType::TypeVideo: {
        IqPostmanVideoContent *result = new IqPostmanVideoContent();
        if (result->fromStringList(stringList))
            return result;
        delete result;
        break;
    }
    case IqPostmanAbstractContentType::TypeApplication: {
        IqPostmanApplicationContent *result = new IqPostmanApplicationContent();
        if (result->fromStringList(stringList))
            return result;
        delete result;
        break;
    }
    case IqPostmanAbstractContentType::TypeUnknown:
        break;
    }

    return Q_NULLPTR;
}

IqPostmanAbstractContent::ContentTransferEncoding IqPostmanAbstractContent::transferEncoding() const
{
    return m_contentTransferEncoding;
}

void IqPostmanAbstractContent::setTransferEncoding(ContentTransferEncoding transferEncoding)
{
    if (m_contentTransferEncoding != transferEncoding) {
        m_contentTransferEncoding = transferEncoding;
        emit transferEncodingChanged();
    }
}

IqPostmanContentDisposition *IqPostmanAbstractContent::contentDisposition() const
{
    return m_contentDisposition;
}

const IqPostmanContentData IqPostmanAbstractContent::splitContent(const QStringList &stringList)
{
    IqPostmanContentData result;

    QRegExp contentTypeRx ("^Content-Type: .*", Qt::CaseInsensitive);
    QRegExp contentTransferEncodingRx ("^Content-Transfer-Encoding: .*", Qt::CaseInsensitive);
    QRegExp contentDispositionRx ("^Content-Disposition: .*", Qt::CaseInsensitive);
    QRegExp contentIdRx ("^Content-ID: .*", Qt::CaseInsensitive);
    QRegExp safeTransferRx ("^\\s+.*");

    QString *currentField = Q_NULLPTR;
    int i = -1;
    foreach (const QString &line, stringList) {
        ++i;
        if (line.isEmpty()) {
            result.content = stringList.mid(i + 1);
            break;
        }
        if (contentTypeRx.indexIn(line) != -1) {
            currentField = &(result.contentType);
            currentField->append(line);
            continue;
        }
        if (contentTransferEncodingRx.indexIn(line) != -1) {
            currentField = &(result.contentTransferEncoding);
            currentField->append(line);
            continue;
        }
        if (contentDispositionRx.indexIn(line) != -1) {
            currentField = &(result.contentDisposition);
            currentField->append(line);
            continue;
        }
        if (contentIdRx.indexIn(line) != -1) {
            currentField = &(result.contentId);
            currentField->append(line);
            continue;
        }
        if(safeTransferRx.indexIn(line) != -1) {
            if (currentField) {
                currentField->append(line);
                continue;
            }
        }
    }

    return result;
}

QByteArray IqPostmanAbstractContent::decode(const QStringList &stringList, IqPostmanAbstractContent::ContentTransferEncoding transferEncoding)
{
    switch (transferEncoding)
    {
    case IqPostmanAbstractContent::Encoding7bit:
    case IqPostmanAbstractContent::Encoding8bit:
    case IqPostmanAbstractContent::EncodingBinary:
    case IqPostmanAbstractContent::EncodingUnknown: {
        return QByteArray(stringList.join(IqPostmanAbstractClient::crlf()).toLocal8Bit().constData());
        break;
    }
    case IqPostmanAbstractContent::EncodingBase64: {
        return QByteArray::fromBase64(stringList.join(IqPostmanAbstractClient::crlf()).toLocal8Bit().constData());
        break;
    }
    case IqPostmanAbstractContent::EncodingQuotedPrintable: {
        return fromQuotedPrintable(stringList);
        break;
    }
    }

    return QByteArray();
}

QStringList IqPostmanAbstractContent::encode(const QByteArray &data, IqPostmanAbstractContent::ContentTransferEncoding transferEncoding)
{
    switch (transferEncoding)
    {
    case IqPostmanAbstractContent::Encoding7bit:
    case IqPostmanAbstractContent::Encoding8bit:
    case IqPostmanAbstractContent::EncodingBinary:
    case IqPostmanAbstractContent::EncodingUnknown: {
        return QString(data).split(IqPostmanAbstractClient::crlf());
        break;
    }
    case IqPostmanAbstractContent::EncodingBase64: {
        return QString(data.toBase64()).split(IqPostmanAbstractClient::crlf());
        break;
    }
    case IqPostmanAbstractContent::EncodingQuotedPrintable: {
        return toQuotedPrintable(data);
        break;
    }
    }

    return QStringList();
}

QByteArray IqPostmanAbstractContent::fromQuotedPrintable(const QStringList &stringList)
{
    QStringList lines;
    bool safeTransfer = false;
    foreach(const QString &line, stringList) {
        QString lineToAdd = line;
        if (line.right(1) == "=")
            lineToAdd.remove(lineToAdd.length() - 1, 1);

        if (safeTransfer)
            lines.last().append(lineToAdd);
        else
            lines.append(lineToAdd);

        if (line.right(1) == "=")
            safeTransfer = true;
         else
            safeTransfer = false;
    }

    QByteArray result;
    foreach(const QString &line, lines) {
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] == '=') {
                if (i < line.length() - 2) {
                    result.append(QByteArray::fromHex(line.mid(i+1, 2).toLocal8Bit()));
                    ++i;
                    ++i;
                } else
                    break;
            } else
                result.append(line[i].toLatin1());
        }

        result.append(IqPostmanAbstractClient::crlf());
    }

    return result;
}

QStringList IqPostmanAbstractContent::toQuotedPrintable(const QByteArray &data)
{
    QString encoded;
    for (int i = 0; i < data.length(); ++i) {
        if (data.mid(i, 2) == IqPostmanAbstractClient::crlf()) {
            encoded.append(IqPostmanAbstractClient::crlf());
            ++i;
        } else
            encoded.append("=" + data.mid(i, 1).toHex());
    }

    QStringList result;
    QStringList lines = encoded.split(IqPostmanAbstractClient::crlf());
    foreach(QString line, lines) {
        while (!line.isEmpty()) {
            if (line.length() > 76)
                result.append(line.left(76) + "=");
            else
                result.append(line);

            line.remove(0, 76);
        }
    }

    return result;
}

QString IqPostmanAbstractContent::contentId() const
{
    return m_contentId;
}

void IqPostmanAbstractContent::setContentId(const QString &contentId)
{
    if (m_contentId != contentId) {
        m_contentId = contentId;
        emit contentIdChanged();
    }
}


IqPostmanAbstractContent::ContentTransferEncoding IqPostmanAbstractContent::contentTransferEncodingFromString(const QString &string)
{
    QRegExp contentTransferEncodingRx ("^Content-Transfer-Encoding: \"?([^\";]*)\"?", Qt::CaseInsensitive);
    if (contentTransferEncodingRx.indexIn(string.trimmed()) == -1)
        return EncodingUnknown;

    if (contentTransferEncodingRx.cap(1).compare(ENCODING_7BIT, Qt::CaseInsensitive) == 0)
        return Encoding7bit;
    else if (contentTransferEncodingRx.cap(1).compare(ENCODING_QUOTED_PRINABLE, Qt::CaseInsensitive) == 0)
        return EncodingQuotedPrintable;
    else if (contentTransferEncodingRx.cap(1).compare(ENCODING_BASE64, Qt::CaseInsensitive) == 0)
        return EncodingBase64;
    else if (contentTransferEncodingRx.cap(1).compare(ENCODING_8BIT, Qt::CaseInsensitive) == 0)
        return Encoding8bit;
    else if (contentTransferEncodingRx.cap(1).compare(ENCODING_BINARY, Qt::CaseInsensitive) == 0)
        return EncodingBinary;

    return EncodingUnknown;
}


QString IqPostmanAbstractContent::contentTransferEncodingToString(ContentTransferEncoding encoding)
{
    QString encodingString;
    switch (encoding) {
    case Encoding7bit:
        encodingString = ENCODING_7BIT;
        break;
    case EncodingQuotedPrintable:
        encodingString = ENCODING_QUOTED_PRINABLE;
        break;
    case EncodingBase64:
        encodingString = ENCODING_BASE64;
        break;
    case Encoding8bit:
        encodingString = ENCODING_8BIT;
        break;
    case EncodingBinary:
        encodingString = ENCODING_BINARY;
        break;
    case EncodingUnknown:
        return "";
        break;
    }

    return "Content-Transfer-Encoding: " + encodingString;
}

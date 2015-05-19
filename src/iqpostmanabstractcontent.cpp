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
    m_contentTransferEncoding(IqPostmanMime::EncodingUnknown),
    m_disposition(IqPostmanMime::DispositionUnknown)
{
}

IqPostmanAbstractContent::~IqPostmanAbstractContent()
{
}

bool IqPostmanAbstractContent::fromString(const QString &string)
{
    IqPostmanContentData contentData = splitContent(string);
    if (!contentType()->fromString(contentData.contentType))
        return false;
    setTransferEncoding(IqPostmanMime::contentTransferEncodingFromString(contentData.contentTransferEncoding));
    setDisposition(IqPostmanMime::contentDispositionFromString(contentData.contentDisposition));

    return fromContentData(contentData);
}

QString IqPostmanAbstractContent::toString() const
{
    IqPostmanContentData contentData = toContentData();

    QString result;
    result.append(contentType()->toString());
    result.append(IqPostmanAbstractClient::crlf());
    if (transferEncoding() != IqPostmanMime::EncodingUnknown) {
        result.append(IqPostmanMime::contentTransferEncodingToString(transferEncoding()));
        result.append(IqPostmanAbstractClient::crlf());
    }
    if (disposition() != IqPostmanMime::DispositionUnknown) {
        result.append(IqPostmanMime::contentDispositionToString(disposition()));
        result.append(IqPostmanAbstractClient::crlf());
    }
    result.append(contentData.content);

    return result;
}

IqPostmanAbstractContent *IqPostmanAbstractContent::createFromString(const QString &string)
{
    switch (IqPostmanMime::contentTypeFromString(string)) {
    case IqPostmanMime::TypeText: {
        IqPostmanTextContent *result = new IqPostmanTextContent();
        if (result->fromString(string))
            return result;
        delete result;
        break;
    }
    case IqPostmanMime::TypeMultipart: {
        IqPostmanMultipartContent *result = new IqPostmanMultipartContent();
        if (result->fromString(string))
            return result;
        delete result;
        break;
    }
    case IqPostmanMime::TypeImage: {
        IqPostmanImageContent *result = new IqPostmanImageContent();
        if (result->fromString(string))
            return result;
        delete result;
        break;
    }
    case IqPostmanMime::TypeAudio: {
        IqPostmanAudioContent *result = new IqPostmanAudioContent();
        if (result->fromString(string))
            return result;
        delete result;
        break;
    }
    case IqPostmanMime::TypeVideo: {
        IqPostmanVideoContent *result = new IqPostmanVideoContent();
        if (result->fromString(string))
            return result;
        delete result;
        break;
    }
    case IqPostmanMime::TypeApplication: {
        IqPostmanApplicationContent *result = new IqPostmanApplicationContent();
        if (result->fromString(string))
            return result;
        delete result;
        break;
    }
    case IqPostmanMime::TypeUnknown:
        break;
    }

    return Q_NULLPTR;
}

IqPostmanMime::ContentTransferEncoding IqPostmanAbstractContent::transferEncoding() const
{
    return m_contentTransferEncoding;
}

void IqPostmanAbstractContent::setTransferEncoding(IqPostmanMime::ContentTransferEncoding transferEncoding)
{
    if (m_contentTransferEncoding != transferEncoding) {
        m_contentTransferEncoding = transferEncoding;
        emit transferEncodingChanged();
    }
}

IqPostmanMime::ContentDisposition IqPostmanAbstractContent::disposition() const
{
    return m_disposition;
}

void IqPostmanAbstractContent::setDisposition(const IqPostmanMime::ContentDisposition &disposition)
{
    if (m_disposition != disposition) {
        m_disposition = disposition;
        emit dispositionChanged();
    }
}

const IqPostmanContentData IqPostmanAbstractContent::splitContent(const QString &string)
{
    IqPostmanContentData result;

    QStringList lines = string.trimmed().split(IqPostmanAbstractClient::crlf());

    QRegExp contentTypeRx ("^Content-Type: .*", Qt::CaseInsensitive);
    QRegExp contentTransferEncodingRx ("^Content-Transfer-Encoding: .*", Qt::CaseInsensitive);
    QRegExp contentDispositionRx ("^Content-Disposition: .*", Qt::CaseInsensitive);
    QRegExp contentIdRx ("^Content-ID: .*", Qt::CaseInsensitive);
    QRegExp safeTransferRx ("^\\s+.*");

    bool contentStart = false;
    QString *currentField = Q_NULLPTR;
    foreach (const QString &line, lines) {
        if (!contentStart) {
            if (contentTypeRx.indexIn(line) != -1) {
                currentField = &(result.contentType);
                currentField->append(line + IqPostmanAbstractClient::crlf());
                continue;
            }
            if (contentTransferEncodingRx.indexIn(line) != -1) {
                currentField = &(result.contentTransferEncoding);
                currentField->append(line + IqPostmanAbstractClient::crlf());
                continue;
            }
            if (contentDispositionRx.indexIn(line) != -1) {
                currentField = &(result.contentDisposition);
                currentField->append(line + IqPostmanAbstractClient::crlf());
                continue;
            }
            if (contentIdRx.indexIn(line) != -1) {
                currentField = &(result.contentId);
                currentField->append(line + IqPostmanAbstractClient::crlf());
                continue;
            }
            if(safeTransferRx.indexIn(line) != -1) {
                if (currentField) {
                    currentField->append(line + IqPostmanAbstractClient::crlf());
                    continue;
                }
            }
            contentStart = true;
            result.content.append(line + IqPostmanAbstractClient::crlf());
        } else
            result.content.append(line + IqPostmanAbstractClient::crlf());
    }

    return result;
}

QByteArray IqPostmanAbstractContent::decode(const QString &string, IqPostmanMime::ContentTransferEncoding transferEncoding)
{
    switch (transferEncoding)
    {
    case IqPostmanMime::Encoding7bit:
    case IqPostmanMime::Encoding8bit:
    case IqPostmanMime::EncodingBinary:
    case IqPostmanMime::EncodingUnknown: {
        return QByteArray(string.toLocal8Bit().constData());
        break;
    }
    case IqPostmanMime::EncodingBase64: {
        return QByteArray::fromBase64(string.toLocal8Bit().constData());
        break;
    }
    case IqPostmanMime::EncodingQuotedPrintable: {
        return fromQuotedPrintable(string);
        break;
    }
    }

    return QByteArray();
}

QString IqPostmanAbstractContent::encode(const QByteArray &data, IqPostmanMime::ContentTransferEncoding transferEncoding)
{
    switch (transferEncoding)
    {
    case IqPostmanMime::Encoding7bit:
    case IqPostmanMime::Encoding8bit:
    case IqPostmanMime::EncodingBinary:
    case IqPostmanMime::EncodingUnknown: {
        return QString(data);
        break;
    }
    case IqPostmanMime::EncodingBase64: {
        return data.toBase64();
        break;
    }
    case IqPostmanMime::EncodingQuotedPrintable: {
        return toQuotedPrintable(data);
        break;
    }
    }

    return "";
}

QByteArray IqPostmanAbstractContent::fromQuotedPrintable(const QString &string)
{
    QStringList sourceLines = string.split("\r\n");
    QStringList lines;
    bool safeTransfer = false;
    foreach(const QString &line, sourceLines) {
        if (safeTransfer)
            lines.last().append(line.left(line.length() - 1));
        else
            lines.append(line);

        if (line.right(1) == "=")
            safeTransfer = true;
        else
            safeTransfer = false;
    }

    QByteArray result;
    foreach(const QString &line, sourceLines) {
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

QString IqPostmanAbstractContent::toQuotedPrintable(const QByteArray &data)
{
    QString encoded;
    for (int i = 0; i < data.length(); ++i) {
        if (data.at(i) == QChar(QChar::CarriageReturn).toLatin1()
                && i < data.length() - 1
                && data.at(i + 1) == QChar(QChar::LineFeed).toLatin1()) {
            encoded.append(IqPostmanAbstractClient::crlf());
            ++i;
        } else
            encoded.append("=" + data.mid(i, 1).toHex());
    }

    QString result;
    QStringList lines = encoded.split(IqPostmanAbstractClient::crlf());
    foreach(QString line, lines) {
        while (!line.isEmpty()) {
            if (line.length() > 76)
                result.append(line.left(76) + "=" + IqPostmanAbstractClient::crlf());
            else
                result.append(line + IqPostmanAbstractClient::crlf());

            line.remove(0, 76);
        }
    }

    return result;
}

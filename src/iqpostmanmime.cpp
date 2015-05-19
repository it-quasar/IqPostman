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

#define DISPOSITION_INLINE "inline"
#define DISPOSITION_ATTACHMENT "attachment"

#include "iqpostmanmime.h"
#include "iqpostmanmultipartcontenttype.h"
#include "iqpostmantextcontenttype.h"
#include "iqpostmanimagecontenttype.h"
#include <QRegExp>

IqPostmanMime::ContentType IqPostmanMime::contentTypeFromString(const QString &string)
{
    QRegExp contetnTypeRx("^Content-Type: ([^\\/]*)", Qt::CaseInsensitive);
    if (contetnTypeRx.indexIn(string.trimmed()) == -1)
        return TypeUnknown;

    if (contetnTypeRx.cap(1).compare(IqPostmanTextContentType::staticTypeName(), Qt::CaseInsensitive) == 0)
        return TypeText;
    else if (contetnTypeRx.cap(1).compare(IqPostmanMultipartContentType::staticTypeName(), Qt::CaseInsensitive) == 0)
        return TypeMultipart;
    else if (contetnTypeRx.cap(1).compare("message", Qt::CaseInsensitive) == 0)
        return TypeMessage;
    else if (contetnTypeRx.cap(1).compare(IqPostmanImageContentType::staticTypeName(), Qt::CaseInsensitive) == 0)
        return TypeImage;
    else if (contetnTypeRx.cap(1).compare("audio", Qt::CaseInsensitive) == 0)
        return TypeAudio;
    else if (contetnTypeRx.cap(1).compare("video", Qt::CaseInsensitive) == 0)
        return TypeVideo;
    else if (contetnTypeRx.cap(1).compare("application", Qt::CaseInsensitive) == 0)
        return TypeApplication;

    return TypeUnknown;
}


IqPostmanMime::ContentTransferEncoding IqPostmanMime::contentTransferEncodingFromString(const QString &string)
{
    QRegExp contentTransferEncodingRx ("^Content-Transfer-Encoding: (.*)", Qt::CaseInsensitive);
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

IqPostmanMime::ContentDisposition IqPostmanMime::contentDispositionFromString(const QString &string)
{
    QRegExp contentTransferEncodingRx ("^Content-Disposition: (.*)", Qt::CaseInsensitive);
    if (contentTransferEncodingRx.indexIn(string.trimmed()) == -1)
        return DispositionUnknown;

    if (contentTransferEncodingRx.cap(1).compare(DISPOSITION_INLINE, Qt::CaseInsensitive) == 0)
        return DispositionInline;
    else if (contentTransferEncodingRx.cap(1).compare(DISPOSITION_ATTACHMENT, Qt::CaseInsensitive) == 0)
        return DispositionAttachment;

    return DispositionUnknown;
}

QString IqPostmanMime::contentDispositionToString(IqPostmanMime::ContentDisposition disposition)
{
    QString dispositionString;
    switch (disposition) {
    case DispositionAttachment:
        dispositionString = DISPOSITION_INLINE;
        break;
    case DispositionInline:
        dispositionString = DISPOSITION_ATTACHMENT;
        break;
    case DispositionUnknown:
        return "";
        break;
    }

    return "Content-Disposition: " + dispositionString;
}

QString IqPostmanMime::contentTransferEncodingToString(IqPostmanMime::ContentTransferEncoding encoding)
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

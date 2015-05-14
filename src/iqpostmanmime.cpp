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

#include "iqpostmanmime.h"
#include <QRegExp>

IqPostmanMime::IqPostmanMime()
{
}

IqPostmanMime::ContentType IqPostmanMime::contentTypeFromString(const QString &string)
{
    QRegExp contetnTypeRx("^Content-Type: ([^\\/]*)", Qt::CaseInsensitive);
    if (contetnTypeRx.indexIn(string.trimmed()) == -1)
        return TypeUnknown;

    if (contetnTypeRx.cap(1).compare("text", Qt::CaseInsensitive) == 0)
        return TypeText;
    else if (contetnTypeRx.cap(1).compare("multipart", Qt::CaseInsensitive) == 0)
        return TypeMultipart;
    else if (contetnTypeRx.cap(1).compare("message", Qt::CaseInsensitive) == 0)
        return TypeMessage;
    else if (contetnTypeRx.cap(1).compare("image", Qt::CaseInsensitive) == 0)
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
    QRegExp contentTransferEncodingRx ("^Content-Transfer-Encoding: (.*)$", Qt::CaseInsensitive);
    if (contentTransferEncodingRx.indexIn(string.trimmed()) == 0)
        return IqPostmanMime::Encoding7bit;

    if (contentTransferEncodingRx.cap(1).compare("7bit", Qt::CaseInsensitive) == 0)
        return Encoding7bit;
    else if (contentTransferEncodingRx.cap(1).compare("quoted-printable", Qt::CaseInsensitive) == 0)
        return EncodingQuotedPrintable;
    else if (contentTransferEncodingRx.cap(1).compare("base64", Qt::CaseInsensitive) == 0)
        return EncodingBase64;
    else if (contentTransferEncodingRx.cap(1).compare("8bit", Qt::CaseInsensitive) == 0)
        return Encoding8bit;
    else if (contentTransferEncodingRx.cap(1).compare("binary", Qt::CaseInsensitive) == 0)
        return EncodingBinary;

    return EncodingUnknown;
}

QString IqPostmanMime::contentTransferEncodingToString(IqPostmanMime::ContentTransferEncoding encoding)
{
    QString encodingString;
    switch (encoding) {
    case Encoding7bit:
        encodingString = "7bit";
        break;
    case EncodingQuotedPrintable:
        encodingString = "quoted-printable";
        break;
    case EncodingBase64:
        encodingString = "base64";
        break;
    case Encoding8bit:
        encodingString = "8bit";
        break;
    case EncodingBinary:
        encodingString = "binary";
        break;
    case EncodingUnknown:
        return "";
        break;
    }

    return "Content-Transfer-Encoding: " + encodingString;
}

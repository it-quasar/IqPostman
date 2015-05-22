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

#define DISPOSITION_INLINE "inline"
#define DISPOSITION_ATTACHMENT "attachment"

#include "iqpostmancontentdisposition.h"
#include <QRegExp>
#include <QStringList>

IqPostmanContentDisposition::IqPostmanContentDisposition(QObject *parent) :
    QObject(parent),
    m_type(DispositionUnknown)
{
}

IqPostmanContentDisposition::~IqPostmanContentDisposition()
{
}

bool IqPostmanContentDisposition::fromString(const QString &string)
{
    if (string.isEmpty())
        return true;

    QRegExp contentTransferEncodingRx ("^Content-Disposition: \"?([^\";]*)\"?", Qt::CaseInsensitive);
    if (contentTransferEncodingRx.indexIn(string.trimmed()) == -1)
        return false;

    ContentDisposition type = DispositionUnknown;
    if (contentTransferEncodingRx.cap(1).compare(DISPOSITION_INLINE, Qt::CaseInsensitive) == 0)
        type = DispositionInline;
    else if (contentTransferEncodingRx.cap(1).compare(DISPOSITION_ATTACHMENT, Qt::CaseInsensitive) == 0)
        type = DispositionAttachment;
    setType(type);

    QRegExp fileNameRx("filename=\"?([^\"\\s]+)\"?");
    fileNameRx.indexIn(string);
    setFileName(fileNameRx.cap(1));

    return true;
}

QString IqPostmanContentDisposition::toString() const
{
    QStringList result;

    QString dispositionString;
    switch (type()) {
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
    result << QString("Content-Disposition: %0")
              .arg(dispositionString);

    if (!fileName().isEmpty())
        result << QString("filename=\"%0\"")
                  .arg(fileName());

    return result.join("; ");
}

IqPostmanContentDisposition::ContentDisposition IqPostmanContentDisposition::type() const
{
    return m_type;
}

void IqPostmanContentDisposition::setType(const ContentDisposition &type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

QString IqPostmanContentDisposition::fileName() const
{
    return m_fileName;
}

void IqPostmanContentDisposition::setFileName(const QString &fileName)
{
    if (m_fileName != fileName) {
        m_fileName = fileName;
        emit fileNameChanged();
    }
}

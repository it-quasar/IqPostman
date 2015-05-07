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

#include "iqpostmanabstractclient.h"
#include <QTextCodec>
#include <QRegExp>
#include <QByteArray>

IqPostmanAbstractClient::IqPostmanAbstractClient(QObject *parent) :
    QObject(parent)
{
}

IqPostmanAbstractClient::~IqPostmanAbstractClient()
{

}

QString IqPostmanAbstractClient::headerDecode(const QString &string) const
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

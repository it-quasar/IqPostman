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

#ifndef IQPOSTMANABSTRACTCONTENT_H
#define IQPOSTMANABSTRACTCONTENT_H

#include <QObject>
#include "iqpostman_global.h"
#include "iqpostmanabstractcontenttype.h"

class IQPOSTMANSHARED_EXPORT IqPostmanAbstractContent: public QObject
{
    Q_OBJECT
    Q_PROPERTY(IqPostmanMime::ContentTransferEncoding transferEncoding READ transferEncoding WRITE setTransferEncoding NOTIFY transferEncodingChanged)
public:
    explicit IqPostmanAbstractContent(QObject *parent = Q_NULLPTR);
    virtual ~IqPostmanAbstractContent();

    virtual IqPostmanAbstractContentType *contentType() const = 0;

    virtual bool fromString(const QString &string) = 0;

    virtual QString toString() const = 0;

    static IqPostmanAbstractContent *createFromString(const QString &string);

protected:
    static void splitContent(const QString &string,
                             QString *contentType,
                             QString *contentTransferEncoding,
                             QString *content);

    static QByteArray decode(const QString &string,
                             IqPostmanMime::ContentTransferEncoding transferEncoding);

    static QString encode(const QByteArray &data,
                          IqPostmanMime::ContentTransferEncoding transferEncoding);

public:
    IqPostmanMime::ContentTransferEncoding transferEncoding() const;
    void setTransferEncoding(IqPostmanMime::ContentTransferEncoding transferEncoding);

signals:
    void transferEncodingChanged();

private:
    static QByteArray fromQuotedPrintable(const QString &string);
    static QString toQuotedPrintable(const QByteArray &data);

private:
    IqPostmanMime::ContentTransferEncoding m_contentTransferEncoding;
};

#endif // IQPOSTMANABSTRACTCONTENT_H

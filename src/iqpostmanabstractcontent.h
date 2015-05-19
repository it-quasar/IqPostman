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
#include "iqpostmancontentdata.h"

class IQPOSTMANSHARED_EXPORT IqPostmanAbstractContent: public QObject
{
    Q_OBJECT
    Q_PROPERTY(IqPostmanMime::ContentTransferEncoding transferEncoding READ transferEncoding WRITE setTransferEncoding NOTIFY transferEncodingChanged)
    Q_PROPERTY(IqPostmanMime::ContentDisposition disposition READ disposition WRITE setDisposition NOTIFY dispositionChanged)
public:
    explicit IqPostmanAbstractContent(QObject *parent = Q_NULLPTR);
    virtual ~IqPostmanAbstractContent();

    virtual IqPostmanAbstractContentType *contentType() const = 0;

    bool fromString(const QString &string);
    QString toString() const;

    static IqPostmanAbstractContent *createFromString(const QString &string);

protected:
    virtual bool fromContentData(const IqPostmanContentData &data) = 0;
    virtual const IqPostmanContentData toContentData() const = 0;

    static QByteArray decode(const QString &string,
                             IqPostmanMime::ContentTransferEncoding transferEncoding);

    static QString encode(const QByteArray &data,
                          IqPostmanMime::ContentTransferEncoding transferEncoding);

public:
    IqPostmanMime::ContentTransferEncoding transferEncoding() const;
    void setTransferEncoding(IqPostmanMime::ContentTransferEncoding transferEncoding);

    IqPostmanMime::ContentDisposition disposition() const;
    void setDisposition(const IqPostmanMime::ContentDisposition &disposition);

signals:
    void transferEncodingChanged();
    void dispositionChanged();

private:
    static const IqPostmanContentData splitContent(const QString &string);
    static QByteArray fromQuotedPrintable(const QString &string);
    static QString toQuotedPrintable(const QByteArray &data);

private:
    IqPostmanMime::ContentTransferEncoding m_contentTransferEncoding;
    IqPostmanMime::ContentDisposition m_disposition;
};

#endif // IQPOSTMANABSTRACTCONTENT_H

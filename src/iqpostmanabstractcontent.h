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
#include "iqpostmancontentdisposition.h"

class IQPOSTMANSHARED_EXPORT IqPostmanAbstractContent: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString contentId READ contentId WRITE setContentId NOTIFY contentIdChanged)
    Q_PROPERTY(ContentTransferEncoding transferEncoding READ transferEncoding WRITE setTransferEncoding NOTIFY transferEncodingChanged)
    Q_PROPERTY(IqPostmanContentDisposition *contentDisposition READ contentDisposition CONSTANT)
    Q_ENUMS(ContentTransferEncoding)
public:
    enum ContentTransferEncoding
    {
        EncodingUnknown,
        Encoding7bit,
        EncodingQuotedPrintable,
        EncodingBase64,
        Encoding8bit,
        EncodingBinary
    };
    explicit IqPostmanAbstractContent(QObject *parent = Q_NULLPTR);
    virtual ~IqPostmanAbstractContent();

    virtual IqPostmanAbstractContentType *contentType() const = 0;

    bool fromStringList(const QStringList &stringList);
    QStringList toStringList() const;

    static IqPostmanAbstractContent *createFromStringList(const QStringList &stringList);

    static ContentTransferEncoding contentTransferEncodingFromString(const QString &string);
    static QString contentTransferEncodingToString(ContentTransferEncoding encoding);

protected:
    virtual bool fromContentData(const IqPostmanContentData &data) = 0;
    virtual const IqPostmanContentData toContentData() const = 0;

    static QByteArray decode(const QStringList &stringList,
                             IqPostmanAbstractContent::ContentTransferEncoding transferEncoding);

    static QStringList encode(const QByteArray &data,
                              IqPostmanAbstractContent::ContentTransferEncoding transferEncoding);

public:
    ContentTransferEncoding transferEncoding() const;
    void setTransferEncoding(ContentTransferEncoding transferEncoding);

    IqPostmanContentDisposition *contentDisposition() const;

    QString contentId() const;
    void setContentId(const QString &contentId);

signals:
    void contentIdChanged();
    void transferEncodingChanged();

private:
    static const IqPostmanContentData splitContent(const QStringList &stringList);
    static QByteArray fromQuotedPrintable(const QStringList &stringList);
    static QStringList toQuotedPrintable(const QByteArray &data);

private:
    QString m_contentId;
    ContentTransferEncoding m_contentTransferEncoding;
    IqPostmanContentDisposition *m_contentDisposition;
};

#endif // IQPOSTMANABSTRACTCONTENT_H

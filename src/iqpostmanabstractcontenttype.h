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

#ifndef IQPOSTMANABSTRACTCONTENTTYPE_H
#define IQPOSTMANABSTRACTCONTENTTYPE_H

#include <QObject>
#include "iqpostman_global.h"

class IQPOSTMANSHARED_EXPORT IqPostmanAbstractContentType : public QObject
{
    Q_OBJECT
    Q_ENUMS(ContentType)
public:
    enum ContentType
    {
        TypeUnknown,
        TypeText,
        TypeMultipart,
        TypeMessage,
        TypeImage,
        TypeAudio,
        TypeVideo,
        TypeApplication
    };

    explicit IqPostmanAbstractContentType(QObject *parent = Q_NULLPTR);
    virtual ~IqPostmanAbstractContentType();

    virtual ContentType type() const = 0;

    virtual QString typeName() const = 0;

    bool fromString(const QString &string);

    QString toString() const;

    static ContentType contentTypeFromString(const QString &string);
    static IqPostmanAbstractContentType *createFromString(const QString &string);

protected:
    virtual int subTypeNumber() const = 0;
    virtual void setSubTypeFromNumber(int subTypeNumber) = 0;
    virtual QHash<int, QString> subTypeNames() const = 0;
    virtual bool setData(const QHash<QString, QString> &data) = 0;
    virtual QHash<QString, QString> data() const = 0;

private:
    QHash<QString, QString> parseData(const QString &string) const;
};

#endif // IQPOSTMANABSTRACTCONTENTTYPE_H

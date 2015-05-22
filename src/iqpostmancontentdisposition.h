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

#ifndef IQPOSTMANCONTENTDISPOSITION_H
#define IQPOSTMANCONTENTDISPOSITION_H

#include <QObject>
#include "iqpostman_global.h"

class IQPOSTMANSHARED_EXPORT IqPostmanContentDisposition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ContentDisposition type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_ENUMS(ContentDisposition)
public:
    enum ContentDisposition
    {
        DispositionUnknown,
        DispositionInline,
        DispositionAttachment
    };

    explicit IqPostmanContentDisposition(QObject *parent = 0);
    ~IqPostmanContentDisposition();

    bool fromString(const QString &string);

    QString toString() const;

public:
    ContentDisposition type() const;
    void setType(const ContentDisposition &type);

    QString fileName() const;
    void setFileName(const QString &fileName);

signals:
    void typeChanged();
    void fileNameChanged();

private:
    ContentDisposition m_type;
    QString m_fileName;
};

#endif // IQPOSTMANCONTENTDISPOSITION_H

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

#ifndef IQPOSTMANTEXTCONTENT_H
#define IQPOSTMANTEXTCONTENT_H

#include "iqpostmanabstractcontent.h"
#include "iqpostman_global.h"
#include "iqpostmantextcontenttype.h"

class IQPOSTMANSHARED_EXPORT IqPostmanTextContent : public IqPostmanAbstractContent
{
    Q_OBJECT
    Q_PROPERTY(IqPostmanTextContentType * contentType READ contentType CONSTANT)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:
    explicit IqPostmanTextContent(QObject *parent = Q_NULLPTR);
    ~IqPostmanTextContent();

    virtual IqPostmanTextContentType *contentType() const Q_DECL_OVERRIDE;
    virtual bool fromString(const QString &string) Q_DECL_OVERRIDE;
    virtual QString toString() const Q_DECL_OVERRIDE;

public:
    QString text() const;
    void setText(const QString &text);

signals:
    void textChanged();

private:
    IqPostmanTextContentType *m_contentType;
    QString m_text;
};

#endif // IQPOSTMANTEXTCONTENT_H

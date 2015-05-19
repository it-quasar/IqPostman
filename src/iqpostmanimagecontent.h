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

#ifndef IQPOSTMANIMAGECONTENT_H
#define IQPOSTMANIMAGECONTENT_H

#include "iqpostmanattachmentcontent.h"
#include "iqpostmanimagecontenttype.h"
#include "iqpostman_global.h"
#include <QImage>

class IQPOSTMANSHARED_EXPORT IqPostmanImageContent : public IqPostmanAttachmentContent
{
    Q_OBJECT
    Q_PROPERTY(IqPostmanImageContentType * contentType READ contentType CONSTANT)
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
public:
    explicit IqPostmanImageContent(QObject *parent = Q_NULLPTR);
    ~IqPostmanImageContent();

    virtual IqPostmanImageContentType *contentType() const Q_DECL_OVERRIDE;

public:
    virtual QByteArray data() const Q_DECL_OVERRIDE;
    virtual bool setData(const QByteArray &data) Q_DECL_OVERRIDE;

public:
    QImage image() const;
    void setImage(const QImage &image);

signals:
    void imageChanged();

private:
    IqPostmanImageContentType *m_contentType;
    QImage m_image;
};

#endif // IQPOSTMANIMAGECONTENT_H

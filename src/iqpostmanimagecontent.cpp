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

#include "iqpostmanimagecontent.h"
#include "iqpostmanabstractclient.h"
#include <QBuffer>

IqPostmanImageContent::IqPostmanImageContent(QObject *parent) :
    IqPostmanAttachmentContent(parent),
    m_contentType(new IqPostmanImageContentType(this))
{
}

IqPostmanImageContent::~IqPostmanImageContent()
{
}

IqPostmanImageContentType *IqPostmanImageContent::contentType() const
{
    return m_contentType;
}

QString IqPostmanImageContent::format() const
{
    return m_contentType->imageFormat();
}

QImage IqPostmanImageContent::image() const
{
    return m_image;
}

void IqPostmanImageContent::setImage(const QImage &image)
{
    if (m_image != image) {
        m_image = image;
        emit imageChanged();
        emit dataChanged();
    }
}


QByteArray IqPostmanImageContent::data() const
{
    QByteArray imageData;
    if (image().isNull())
        return imageData;

    QBuffer buffer(&imageData);
    if (!buffer.open(QBuffer::WriteOnly))
        return imageData;

    image().save(&buffer,m_contentType->imageFormat().toLocal8Bit().constData());

    return imageData;
}

bool IqPostmanImageContent::setData(const QByteArray &data)
{
    QImage image;
    bool loadResult = image.loadFromData(data, m_contentType->imageFormat().toLocal8Bit().constData());
    setImage(image);

    return loadResult;
}

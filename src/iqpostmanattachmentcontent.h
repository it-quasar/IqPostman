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

#ifndef IQPOSTMANATTACHMENTCONTENT_H
#define IQPOSTMANATTACHMENTCONTENT_H

#include "iqpostmanabstractcontent.h"
#include "iqpostman_global.h"


class IQPOSTMANSHARED_EXPORT IqPostmanAttachmentContent : public IqPostmanAbstractContent
{
    Q_OBJECT
    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged)
public:
    explicit IqPostmanAttachmentContent(QObject *parent = Q_NULLPTR);

    virtual QString format() const;

protected:
    virtual bool fromContentData(const IqPostmanContentData &data) Q_DECL_OVERRIDE;
    virtual const IqPostmanContentData toContentData() const Q_DECL_OVERRIDE;

public:
    virtual QByteArray data() const;
    virtual bool setData(const QByteArray &data);

signals:
    void dataChanged();

private:
    QByteArray m_data;
};

#endif // IQPOSTMANATTACHMENTCONTENT_H

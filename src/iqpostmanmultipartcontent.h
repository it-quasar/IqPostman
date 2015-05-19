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

#ifndef IQPOSTMANMULTIPARTCONTENT_H
#define IQPOSTMANMULTIPARTCONTENT_H

#include "iqpostmanabstractcontent.h"
#include "iqpostmanmultipartcontenttype.h"

class IqPostmanMultipartContent : public IqPostmanAbstractContent
{
    Q_OBJECT
    Q_PROPERTY(IqPostmanMultipartContentType * contentType READ contentType CONSTANT)
    Q_PROPERTY(QList<IqPostmanAbstractContent *> parts READ parts WRITE setParts NOTIFY partsChanged)
public:
    explicit IqPostmanMultipartContent(QObject *parent = Q_NULLPTR);
    ~IqPostmanMultipartContent();

    virtual IqPostmanMultipartContentType *contentType() const Q_DECL_OVERRIDE;

protected:
    virtual bool fromContentData(const IqPostmanContentData &data) Q_DECL_OVERRIDE;
    virtual const IqPostmanContentData toContentData() const Q_DECL_OVERRIDE;

public:
    QList<IqPostmanAbstractContent *> parts() const;
    void setParts(const QList<IqPostmanAbstractContent *> &parts);

signals:
    void partsChanged();

private:
    IqPostmanMultipartContentType *m_contentType;
    QList<IqPostmanAbstractContent *> m_parts;
};

#endif // IQPOSTMANMULTIPARTCONTENT_H

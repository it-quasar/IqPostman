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

#ifndef IQPOSTMANAPPLICATIONCONTENT_H
#define IQPOSTMANAPPLICATIONCONTENT_H

#include "iqpostmanattachmentcontent.h"
#include "iqpostmanapplicationcontenttype.h"
#include "iqpostman_global.h"
#include <QImage>

class IQPOSTMANSHARED_EXPORT IqPostmanApplicationContent : public IqPostmanAttachmentContent
{
    Q_OBJECT
    Q_PROPERTY(IqPostmanApplicationContentType * contentType READ contentType CONSTANT)
public:
    explicit IqPostmanApplicationContent(QObject *parent = Q_NULLPTR);
    ~IqPostmanApplicationContent();

    virtual IqPostmanApplicationContentType *contentType() const Q_DECL_OVERRIDE;

private:
    IqPostmanApplicationContentType *m_contentType;
};

#endif // IQPOSTMANAUDIOCONTENT_H

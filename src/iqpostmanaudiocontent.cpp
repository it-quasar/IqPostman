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

#include "iqpostmanaudiocontent.h"
#include <QBuffer>

IqPostmanAudioContent::IqPostmanAudioContent(QObject *parent) :
    IqPostmanAttachmentContent(parent),
    m_contentType(new IqPostmanAudioContentType(this))
{
}

IqPostmanAudioContent::~IqPostmanAudioContent()
{
}

IqPostmanAudioContentType *IqPostmanAudioContent::contentType() const
{
    return m_contentType;
}

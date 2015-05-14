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

#include "iqpostmanmail.h"

IqPostmanMail::IqPostmanMail(QObject *parent) :
    QObject(parent),
    m_header(new IqPostmanMailHeader(this)),
    m_content(Q_NULLPTR)
{
}

IqPostmanMail::~IqPostmanMail()
{
}

QString IqPostmanMail::sourceId() const
{
    return m_sourceId;
}

void IqPostmanMail::setSourceId(const QString &sourceId)
{
    if (m_sourceId != sourceId) {
        m_sourceId = sourceId;
        emit sourceIdChanged();
    }
}

IqPostmanMail::MailFlags IqPostmanMail::flags() const
{
    return m_flags;
}

void IqPostmanMail::setFlags(const MailFlags &flags)
{
    if (m_flags != flags) {
        m_flags = flags;
        emit flagsChanged();
    }
}

IqPostmanMailHeader *IqPostmanMail::header() const
{
    return m_header;
}

IqPostmanAbstractContent *IqPostmanMail::content() const
{
    return m_content;
}

void IqPostmanMail::setContent(IqPostmanAbstractContent *content)
{
    if (m_content != content) {
        if (m_content)
            m_content->deleteLater();

        m_content = content;
        if (m_content)
            m_content->setParent(this);
        emit contentChanged();
    }
}


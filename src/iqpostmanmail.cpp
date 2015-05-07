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

IqPostmanMail::IqPostmanMail()
{
}

QString IqPostmanMail::sourceId() const
{
    return m_sourceId;
}

void IqPostmanMail::setSourceId(const QString &sourceId)
{
    m_sourceId = sourceId;
}

IqPostmanMail::MailFlags IqPostmanMail::flags() const
{
    return m_flags;
}

void IqPostmanMail::setFlags(const MailFlags &flags)
{
    m_flags = flags;
}

QString IqPostmanMail::subject() const
{
    return m_subject;
}

void IqPostmanMail::setSubject(const QString &subject)
{
    m_subject = subject;
}

QString IqPostmanMail::from() const
{
    return m_from;
}

void IqPostmanMail::setFrom(const QString &from)
{
    m_from = from;
}

QString IqPostmanMail::to() const
{
    return m_to;
}

void IqPostmanMail::setTo(const QString &to)
{
    m_to = to;
}

QString IqPostmanMail::cc() const
{
    return m_cc;
}

void IqPostmanMail::setCc(const QString &cc)
{
    m_cc = cc;
}

QDateTime IqPostmanMail::date() const
{
    return m_date;
}

void IqPostmanMail::setDate(const QDateTime &date)
{
    m_date = date;
}


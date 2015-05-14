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

#include "iqpostmanabstractcontenttype.h"

#include "iqpostmantextcontenttype.h"
#include "iqpostmanmultipartcontenttype.h"

#include <QRegExp>

IqPostmanAbstractContentType::IqPostmanAbstractContentType(QObject *parent) :
    QObject(parent)
{
}

IqPostmanAbstractContentType::~IqPostmanAbstractContentType()
{
}

IqPostmanAbstractContentType *IqPostmanAbstractContentType::createFromString(const QString &string)
{
    switch (IqPostmanMime::contentTypeFromString(string)) {
    case IqPostmanMime::TypeText: {
        IqPostmanTextContentType *result = new IqPostmanTextContentType();
        if (result->fromString(string))
            return result;
        delete result;
        break;
    }
    case IqPostmanMime::TypeMultipart: {
        IqPostmanMultipartContentType *result = new IqPostmanMultipartContentType();
        if (result->fromString(string))
            return result;
        delete result;
        break;
    }
    case IqPostmanMime::TypeUnknown:
        break;
    }

    return Q_NULLPTR;
}
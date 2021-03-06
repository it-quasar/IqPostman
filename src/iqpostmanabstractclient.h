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

#ifndef IQPOSTMANABSTRACTCLIENT_H
#define IQPOSTMANABSTRACTCLIENT_H

#include <QObject>
#include <QSet>
#include "iqpostmanmail.h"
#include "iqpostman_global.h"

class IQPOSTMANSHARED_EXPORT IqPostmanAbstractClient : public QObject
{
    Q_OBJECT
public:
    enum ConnectMode
    {
        Tcp,
        Ssl
    };

    enum MimeType
    {
        Text,
        Html
    };

    explicit IqPostmanAbstractClient(QObject *parent = 0);
    virtual ~IqPostmanAbstractClient();

    virtual bool connectToHost(const QString &host,
                               quint16 port,
                               ConnectMode mode,
                               qint32 reconectCount = 5,
                               qint32 reconectWaitTime = 7000) = 0;

    virtual bool login(const QString &user,
                       const QString &password) const = 0;

    virtual QStringList folders(bool *ok) const = 0;

    virtual bool checkMails(const QString &folderName,
                            const QList<QSharedPointer<IqPostmanMail> > &existMails,
                            QList<QSharedPointer<IqPostmanMail> > *newMails,
                            QList<QSharedPointer<IqPostmanMail> > *changedMails,
                            QList<QSharedPointer<IqPostmanMail> > *removedMails) const = 0;

    virtual bool loadMailsContent(const QString &folderName,
                                  const QList<QSharedPointer<IqPostmanMail> > &mails) const = 0;

    bool loadMailContent(const QString &folderName,
                         QSharedPointer<IqPostmanMail> mail) const;

    static QString crlf();
};

#endif // IQPOSTMANABSTRACTCLIENT_H

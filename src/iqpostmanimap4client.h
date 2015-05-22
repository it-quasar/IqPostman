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

#ifndef IQPOSTMANIMAP4CLIENT_H
#define IQPOSTMANIMAP4CLIENT_H

#include <QSslSocket>
#include "iqpostmanabstractclient.h"
#include "iqpostmanmail.h"
#include "iqpostman_global.h"

class IQPOSTMANSHARED_EXPORT IqPostmanImap4Client : public IqPostmanAbstractClient
{
    Q_OBJECT
public:
    explicit IqPostmanImap4Client(QObject *parent = Q_NULLPTR);
    ~IqPostmanImap4Client();

    virtual bool connectToHost(const QString &host,
                               quint16 port,
                               ConnectMode mode,
                               qint32 reconectCount = 5,
                               qint32 reconectWaitTime = 7000) Q_DECL_OVERRIDE;

    virtual bool login(const QString &user, const QString &password) const Q_DECL_OVERRIDE;

    virtual QStringList folders(bool *ok) const Q_DECL_OVERRIDE;

    virtual bool checkMails(const QString &folderName,
                            const QList<QSharedPointer<IqPostmanMail> > &existMails,
                            QList<QSharedPointer<IqPostmanMail> > *newMails,
                            QList<QSharedPointer<IqPostmanMail> > *changedMails,
                            QList<QSharedPointer<IqPostmanMail> > *removedMails) const Q_DECL_OVERRIDE;

    virtual bool loadMailsContent(const QString &folderName,
                                  const QList<QSharedPointer<IqPostmanMail> > &mails) const Q_DECL_OVERRIDE;

private:
    enum ResponseResult
    {
        ReadError,
        Ok,
        Bad,
        No
    };

    bool sendRequest(const QString &tag, const QString &request) const;
    ResponseResult readResponse(const QString &tag, QStringList *responseData) const;

    QString uniqueTag() const;

    bool selectFolder(const QString &folderName) const;

    QHash<QString, IqPostmanMail::MailFlags> mailFlags(const QString &folderName,
                                                       const QStringList &mails,
                                                       bool *ok) const;

    enum MailData
    {
        Header,
        Content
    };

    bool fetchMailData(const QString &folderName, const QList<QSharedPointer<IqPostmanMail> > &mails, MailData data) const;

    QString utf7Decode(const QString &string) const;
    QString utf7Encode(const QString &string) const;

private:
    QSslSocket *m_socket;
    mutable qint64 m_currentTagId;
    static bool m_randInitiated;
};


#endif // IQPOSTMANIMAP4CLIENT_H

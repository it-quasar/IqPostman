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

#include "iqpostmanimap4client.h"
#include <QTextCodec>

bool IqPostmanImap4Client::m_randInitiated = false;

IqPostmanImap4Client::IqPostmanImap4Client(QObject *parent) :
    IqPostmanAbstractClient(parent),
    m_socket(new QSslSocket(this)),
    m_currentTagId(0)
{
    if (!m_randInitiated) {
        qsrand(QDateTime::currentMSecsSinceEpoch());
        m_randInitiated = true;
    }
}

IqPostmanImap4Client::~IqPostmanImap4Client()
{
    m_socket->abort();
}

bool IqPostmanImap4Client::connectToHost(const QString &host, quint16 port, ConnectMode mode, qint32 reconectCount, qint32 reconectWaitTime)
{
    for (int i = 0; i < reconectCount; ++i) {
        qDebug() << tr("Try connect to \"%0:%1\" (%2).")
                    .arg(host)
                    .arg(port)
                    .arg(i);

        QTime connectTime;
        connectTime.start();
        if (m_socket->state() == QSslSocket::ConnectedState)
            return true;

        switch (mode) {
        case Ssl: {
            m_socket->connectToHostEncrypted(host, port);
            qint32 elapsedTime = reconectWaitTime - connectTime.elapsed();
            if (elapsedTime < 0)
                elapsedTime = 0;
            if (!m_socket->waitForConnected(elapsedTime))
                continue;

            elapsedTime = reconectWaitTime - connectTime.elapsed();
            if (elapsedTime < 0)
                elapsedTime = 0;
            if (!m_socket->waitForEncrypted(elapsedTime))
                continue;
            break;
        }
        case Tcp: {
            qint32 elapsedTime = reconectWaitTime - connectTime.elapsed();
            if (elapsedTime < 0)
                elapsedTime = 0;
            m_socket->connectToHost(host, port);
            if (!m_socket->waitForConnected(elapsedTime))
                continue;
            break;
        }
        }

        qint32 elapsedTime = reconectWaitTime - connectTime.elapsed();
        if (elapsedTime < 0)
            elapsedTime = 0;
        if (!m_socket->waitForReadyRead(elapsedTime))
            continue;

        QStringList requestData = QString(m_socket->readAll()).split(IqPostmanAbstractClient::crlf());
        if (requestData.size() > 0)
            requestData.removeLast();
        if (!requestData.last().startsWith("* OK"))
            return false;

        return true;
    }
    return false;
}

bool IqPostmanImap4Client::login(const QString &user, const QString &password) const
{
    if (m_socket->state() != QSslSocket::ConnectedState)
        return false;

    QString tag = uniqueTag();
    if (!sendRequest(tag, "LOGIN " + user + " " + password))
        return false;

    QStringList responseData;
    ResponseResult responseResult = readResponse(tag, &responseData);
    if (responseResult != Ok)
        return false;

    return true;
}

QStringList IqPostmanImap4Client::folders(bool *ok) const
{
    if (m_socket->state() != QSslSocket::ConnectedState) {
        *ok = false;
        return QStringList();
    }

    QString tag = uniqueTag();
    if (!sendRequest(tag, "LIST \"/\" *")) {
        *ok = false;
        return QStringList();
    }

    QStringList responseData;
    ResponseResult responseResult = readResponse(tag, &responseData);
    if (responseResult != Ok) {
        *ok = false;
        return QStringList();
    }

    QStringList result;
    foreach (const QString &listRow, responseData) {
        QRegExp listRx ("^\\* LIST \\(([^\\)]*)\\) \"/\" \"([^\\)]*)\"");
        if (listRx.indexIn(listRow) == -1)
            continue;

        //Пропускаем папки, которые нельзя выбрать
        if (listRx.cap(1).contains("\\Noselect", Qt::CaseInsensitive))
            continue;

        result << utf7Decode(listRx.cap(2).trimmed());
    }

    *ok = true;
    return result;
}

bool IqPostmanImap4Client::checkMails(const QString &folderName,
                                      const QList<QSharedPointer<IqPostmanMail> > &existMails,
                                      QList<QSharedPointer<IqPostmanMail> > *newMails,
                                      QList<QSharedPointer<IqPostmanMail> > *changedMails,
                                      QList<QSharedPointer<IqPostmanMail> > *removedMails) const
{
    newMails->clear();
    changedMails->clear();
    removedMails->clear();

    if (!selectFolder(folderName))
        return false;

    QString tag = uniqueTag();
    if (!sendRequest(tag, "UID SEARCH ALL"))
        return false;

    QStringList responseData;
    ResponseResult responseResult = readResponse(tag, &responseData);
    if (responseResult != Ok)
        return false;

    QStringList mailsOnServer;
    foreach (QString searchRow, responseData) {
        if (!searchRow.startsWith("* SEARCH"))
            continue;

        searchRow = searchRow.trimmed();
        searchRow.remove(0, 8);
        mailsOnServer.append(searchRow.split(" ", QString::SkipEmptyParts));
    }

    bool ok;
    QHash<QString, IqPostmanMail::MailFlags> flags = mailFlags(folderName, mailsOnServer, &ok);
    if (!ok)
        return false;

    QHash<QString, QSharedPointer<IqPostmanMail> > existMailsHash;
    foreach (QSharedPointer<IqPostmanMail> mail, existMails) {
        if (mail)
            existMailsHash[mail->sourceId()] = mail;
    }

    QHashIterator<QString, IqPostmanMail::MailFlags> flagsI(flags);
    while (flagsI.hasNext()) {
        flagsI.next();

        if (flagsI.value().testFlag(IqPostmanMail::Deleted)) {
            mailsOnServer.removeOne(flagsI.key());
            continue;
        }

        if (existMailsHash.contains(flagsI.key())) {
            QSharedPointer<IqPostmanMail> existMail = existMailsHash[flagsI.key()];
            if (existMail->flags() != flagsI.value()) {
                if (!changedMails->contains(existMail))
                    changedMails->append(existMail);
                existMail->setFlags(flagsI.value());
            }
        }
    };

    QHash<QString, QSharedPointer<IqPostmanMail> > removedMailsHash = existMailsHash;
    foreach (const QString &mail, mailsOnServer) {
        if (!existMailsHash.contains(mail)) {
            QSharedPointer<IqPostmanMail> newMail (new IqPostmanMail);
            newMail->setSourceId(mail);
            newMail->setFlags(flags[mail]);
            newMails->append(newMail);
        } else {
            removedMailsHash.remove(mail);
        }
    }

    *removedMails = removedMailsHash.values();

//    if (!loadMailData(folderName, newMails))
//        return false;

    return true;
}

bool IqPostmanImap4Client::loadMailsContent(const QString &folderName,
                                            const QList<QSharedPointer<IqPostmanMail> > &mails) const
{
    if (!fetchMailData(folderName, mails, Header))
        return false;
    if (!fetchMailData(folderName, mails, Content))
        return false;

    return true;
}

QHash<QString, IqPostmanMail::MailFlags> IqPostmanImap4Client::mailFlags(const QString &folderName, const QStringList &mails, bool *ok) const
{
    Q_CHECK_PTR(ok);
    *ok = false;
    QHash<QString, IqPostmanMail::MailFlags> result;

    if (mails.isEmpty()) {
        *ok = true;
        return result;
    }

    if (!selectFolder(folderName))
        return result;

    if (m_socket->state() != QSslSocket::ConnectedState)
        return result;

    QString tag = uniqueTag();
    if (!sendRequest(tag, QString("UID FETCH %0:%1 FLAGS")
                     .arg(mails.first())
                     .arg(mails.last())))
        return result;

    QStringList responseData;
    ResponseResult responseResult = readResponse(tag, &responseData);
    if (responseResult != Ok)
        return result;

    foreach (const QString &flagRow, responseData) {
        QRegExp flagsRx ("^\\* \\d+ FETCH \\(UID (\\d+) FLAGS \\(([^\\)]*)\\)\\)");
        if (flagsRx.indexIn(flagRow) == -1)
            continue;
        QString uid = flagsRx.cap(1);
        QString flagsStr = flagsRx.cap(2);
        QStringList flagsStrList = flagsStr.split(' ', QString::SkipEmptyParts);
        IqPostmanMail::MailFlags flags;
        foreach (const QString &flagStr, flagsStrList) {
            if (flagStr.compare("\\Seen", Qt::CaseInsensitive) == 0) {
                flags |= IqPostmanMail::Seen;
            } else if (flagStr.compare("\\Answered", Qt::CaseInsensitive) == 0) {
                flags |= IqPostmanMail::Answered;
            } else if (flagStr.compare("\\Deleted", Qt::CaseInsensitive) == 0) {
                flags |= IqPostmanMail::Deleted;
            } else if (flagStr.compare("\\Draft", Qt::CaseInsensitive) == 0) {
                flags |= IqPostmanMail::Draft;
            }
        }

        result[uid] = flags;
    }

    *ok = true;
    return result;
}

bool IqPostmanImap4Client::fetchMailData(const QString &folderName, const QList<QSharedPointer<IqPostmanMail> > &mails, MailData data) const
{
    if (mails.isEmpty())
        return true;

    if (!selectFolder(folderName))
        return false;

    if (m_socket->state() != QSslSocket::ConnectedState)
        return false;

    foreach (QSharedPointer<IqPostmanMail> mail, mails) {
        QTime profileTime;
        profileTime.start();

        Q_CHECK_PTR(mail);
        QString mailUid = mail->sourceId();

        QString dataStr;
        switch (data) {
        case Header:
            dataStr = "HEADER";
            break;
        case Content:
            dataStr = "TEXT";
            break;
        }

        QString tag = uniqueTag();
        if (!sendRequest(tag, QString("UID FETCH %0 BODY.PEEK[%2]")
                         .arg(mailUid)
                         .arg(dataStr)))
            return false;

        qDebug() << "Prepare to fetch mail in " << profileTime.elapsed() << " msec.";

        QStringList responseData;
        ResponseResult responseResult = readResponse(tag, &responseData);
        if (responseResult != Ok)
            return false;
       qDebug() << "Fetch mail in " << profileTime.elapsed() << " msec.";

        QStringList usefulResponseDataList = responseData.mid(1, responseData.length() - 2);
        if (usefulResponseDataList.isEmpty())
            return false;
        //Удалим закрывающую скобку
        QString last = usefulResponseDataList.last();
        last.remove(last.length() - 1, 1);
        usefulResponseDataList.removeLast();
        usefulResponseDataList.append(last);
        qDebug() << "Restikt response in " << profileTime.elapsed() << " msec.";

        switch (data) {
        case Header:
            mail->header()->fromStringList(usefulResponseDataList);
            break;
        case Content: {
            QString conentTypeStr = mail->header()->contentType()->toString();
            QString conentTransferEncodingStr = IqPostmanAbstractContent::contentTransferEncodingToString(mail->header()->contentTransferEncoding());
            QStringList contentStrList;
            contentStrList.append(conentTypeStr);
            contentStrList.append(conentTransferEncodingStr);
            contentStrList.append("");
            contentStrList.append(usefulResponseDataList);
            IqPostmanAbstractContent *content = IqPostmanAbstractContent::createFromStringList(contentStrList);
            mail->setContent(content);
            break;
        }
        }
        qDebug() << "End fetch in " << profileTime.elapsed() << " msec.";
    }

    return true;
}

QString IqPostmanImap4Client::utf7Decode(const QString &string) const
{
    QString result;
    QRegExp partRx ("([^&-]*)&([^-]*)-");
    int offset = 0;
    while (partRx.indexIn(string, offset) != -1) {
        offset = partRx.pos(0) + partRx.matchedLength();

        result.append(partRx.cap(1));

        QString base64 = partRx.cap(2);
        if (base64.isEmpty()) {
            result.append("&");
            continue;
        }

        base64.replace(',', '/');

        QByteArray decodeBase64 = QByteArray::fromBase64(base64.toLocal8Bit());
        for (int i = 0; i + 1 < decodeBase64.size();) {
            QChar ch (decodeBase64[i] << 8 | decodeBase64[i + 1]);
            i++;
            i++;
            result.append(ch);
        }
    }

    if (offset != string.length() - 1)
        result.append(string.mid(offset));

    return result;
}

QString IqPostmanImap4Client::utf7Encode(const QString &string) const
{
    QString result;
    QString latinString;
    QByteArray notLatinBuffer;

    auto addNotLatinBuffer = [&result, &notLatinBuffer]() {
        QByteArray base64 = notLatinBuffer.toBase64();
        QString notLatinString (base64);
        notLatinString.remove("=");
        notLatinString.replace('/', ',');
        result.append("&" + notLatinString + "-");
        notLatinBuffer.clear();
    };

    auto addLatinString = [&result, &latinString]() {
        latinString.replace('&', "&-");
        result.append(latinString);
        latinString.clear();
    };

    for (int i = 0; i < string.length(); ++i) {
        QChar qc = string.at(i);
        char c = qc.toLatin1();
        if (c >= 0x20 && c <= 0x7e) {
            if (!notLatinBuffer.isEmpty()) {
                addNotLatinBuffer();
            }
            latinString.append(qc);
        } else {
            if (!latinString.isEmpty()) {
                addLatinString();
            }

            ushort uc = qc.unicode();
            notLatinBuffer.append(uc >> 8);
            notLatinBuffer.append(uc & 0xFF);
        }
    }

    Q_ASSERT(!(!notLatinBuffer.isEmpty() && !latinString.isEmpty()));

    if (!notLatinBuffer.isEmpty())
        addNotLatinBuffer();

    if (!latinString.isEmpty())
        addLatinString();

    return result;
}

bool IqPostmanImap4Client::sendRequest(const QString &tag, const QString &request) const
{
    QString requestToSend = QString("%0 %1%2")
            .arg(tag)
            .arg(request)
            .arg(IqPostmanAbstractClient::crlf());
    m_socket->write(requestToSend.toLocal8Bit().constData());

    qDebug() << "Send request " << requestToSend.trimmed();

    if (!m_socket->waitForBytesWritten())
        return false;

    return true;
}

IqPostmanImap4Client::ResponseResult IqPostmanImap4Client::readResponse(const QString &tag, QStringList *responseData) const
{
    QString lastString;
    QRegExp responseRx(QString("^(%0) (OK|NO|BAD)")
                       .arg(tag)
                       , Qt::CaseInsensitive);
    while(m_socket->waitForReadyRead()) {
        lastString.append(QLatin1String(m_socket->readLine()));
        while(!lastString.isEmpty()) {
            if (lastString.endsWith(IqPostmanAbstractClient::crlf())) {
                responseData->append(lastString.left(lastString.length() - 2));
            } else {
                break;
            }
            lastString = QLatin1String(m_socket->readLine());
        }

        if (!responseData->isEmpty()) {
            QString fullLastString = responseData->last();

            if (responseRx.indexIn(fullLastString) == -1)
                continue;

            qDebug() << "Reserve response " << fullLastString;

            QString responseResult = responseRx.cap(2);
            if (responseResult.compare("OK", Qt::CaseInsensitive) == 0)
                return Ok;
            else if (responseResult.compare("NO", Qt::CaseInsensitive) == 0)
                return No;
            else if (responseResult.compare("BAD", Qt::CaseInsensitive) == 0)
                return Bad;
        }
    }

    qWarning() << "Error on reserve response for request with tag " << tag;
    return ReadError;
}

QString IqPostmanImap4Client::uniqueTag() const
{
    return QString("RequestN%0R%1")
            .arg(m_currentTagId++)
            .arg(qrand());
}

bool IqPostmanImap4Client::selectFolder(const QString &folderName) const
{
    if (m_socket->state() != QSslSocket::ConnectedState)
        return false;

    QString tag = uniqueTag();
    QString utf7FolderName = utf7Encode(folderName);
    if (!sendRequest(tag, "SELECT \"" + utf7FolderName + "\""))
        return false;

    QStringList responseData;
    ResponseResult responseResult = readResponse(tag, &responseData);
    if (responseResult != Ok)
        return false;

    return true;
}


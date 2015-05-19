#ifndef IQPOSTMANCONTENTDATA_H
#define IQPOSTMANCONTENTDATA_H

#include <QString>

class IqPostmanContentData
{
public:
    QString contentType;
    QString contentTransferEncoding;
    QString contentDisposition;
    QString contentId;
    QString content;
};

#endif // IQPOSTMANCONTENTDATA_H

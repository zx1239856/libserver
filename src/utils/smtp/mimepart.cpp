#include "mimepart.h"
#include "quotedprintable.h"

/* [1] Constructors and Destructors */

MimePart::MimePart()
{
    cEncoding = _7Bit;
    prepared = false;
    cBoundary = "";
}

MimePart::~MimePart()
{
    return;
}

/* [1] --- */


/* [2] Getters and Setters */

void MimePart::setContent(const QByteArray & content)
{
    this->content = content;
}

void MimePart::setHeader(const QString & header)
{
    this->header = header;
}

void MimePart::addHeaderLine(const QString & line)
{
    this->header += line + "\r\n";
}

const QString& MimePart::getHeader() const
{
    return header;
}

const QByteArray& MimePart::getContent() const
{
    return content;
}

void MimePart::setContentId(const QString & cId)
{
    this->cId = cId;
}

const QString & MimePart::getContentId() const
{
    return this->cId;
}

void MimePart::setContentName(const QString & cName)
{
    this->cName = cName;
}

const QString & MimePart::getContentName() const
{
    return this->cName;
}

void MimePart::setContentType(const QString & cType)
{
    this->cType = cType;
}

const QString & MimePart::getContentType() const
{
    return this->cType;
}

void MimePart::setCharset(const QString & charset)
{
    this->cCharset = charset;
}

const QString & MimePart::getCharset() const
{
    return this->cCharset;
}

void MimePart::setEncoding(Encoding enc)
{
    this->cEncoding = enc;
}

MimePart::Encoding MimePart::getEncoding() const
{
    return this->cEncoding;
}

MimeContentFormatter& MimePart::getContentFormatter()
{
    return this->formatter;
}

/* [2] --- */


/* [3] Public methods */

QString MimePart::toString()
{
    if (!prepared)
        prepare();

    return mimeString;
}

/* [3] --- */


/* [4] Protected methods */

void MimePart::prepare()
{
    mimeString = QString();

    /* === Header Prepare === */

    /* Content-Type */
    mimeString.append("Content-Type: ").append(cType);

    if (cName != "")
        mimeString.append("; name=\"").append(cName).append("\"");

    if (cCharset != "")
        mimeString.append("; charset=").append(cCharset);

    if (cBoundary != "")
        mimeString.append("; boundary=").append(cBoundary);

    mimeString.append("\r\n");
    /* ------------ */

    /* Content-Transfer-Encoding */
    mimeString.append("Content-Transfer-Encoding: ");
    switch (cEncoding)
    {
    case _7Bit:
        mimeString.append("7bit\r\n");
        break;
    case _8Bit:
        mimeString.append("8bit\r\n");
        break;
    case Base64:
        mimeString.append("base64\r\n");
        break;
    case QuotedPrintable:
        mimeString.append("quoted-printable\r\n");
        break;
    }
    /* ------------------------ */

    /* Content-Id */
    if (cId != NULL)
        mimeString.append("Content-ID: <").append(cId).append(">\r\n");
    /* ---------- */

    /* Addition header lines */

    mimeString.append(header).append("\r\n");

    /* ------------------------- */

    /* === End of Header Prepare === */

    /* === Content === */
    switch (cEncoding)
    {
    case _7Bit:
        mimeString.append(QString(content).toLatin1());
        break;
    case _8Bit:
        mimeString.append(content);
        break;
    case Base64:
        mimeString.append(formatter.format(content.toBase64()));
        break;
    case QuotedPrintable:
        mimeString.append(formatter.format(QuotedPrintable::encode(content), true));
        break;
    }
    mimeString.append("\r\n");
    /* === End of Content === */

    prepared = true;
}

/* [4] --- */

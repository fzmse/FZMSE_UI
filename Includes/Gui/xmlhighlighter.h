#pragma once

#include <QSyntaxHighlighter>

class XMLHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    // ctor
    XMLHighlighter(QTextDocument *parent = 0);

protected:
    // An overridden highlight method for highlighting
    // patterms based on the rules defined in this class
    //
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:

    QTextCharFormat validElementFormat; // The format for XML elements
    QTextCharFormat attributeNameFormat; // The format for XML attribute names
    QTextCharFormat attributeValueFormat; // The format for XML attribute values
    QTextCharFormat commentFormat; // formatting for XML comments
};

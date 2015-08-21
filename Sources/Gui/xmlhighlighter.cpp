#include "Gui/xmlhighlighter.h"
#include <QDebug>

XMLHighlighter::XMLHighlighter(QTextDocument *parent)
    :QSyntaxHighlighter(parent)
{
    attributeValueFormat.setForeground(Qt::darkGreen);
    attributeValueFormat.setFontWeight(QFont::Normal);

    validElementFormat.setForeground(Qt::darkBlue);
    validElementFormat.setFontWeight(QFont::Bold);

    attributeNameFormat.setForeground(Qt::darkRed);
    attributeNameFormat.setFontWeight(QFont::Bold);

    commentFormat.setForeground(Qt::gray);
    commentFormat.setFontItalic(true);
}


void XMLHighlighter::highlightBlock(const QString &text)
{

    enum {
        NormalState = -1,
        Element,
        AttributeName,
        AttributeEquals,
        AttributeValue,
        Comment
    };

    int state = previousBlockState();
    int offset = 0;

    for (int i = 0; i < text.length(); ++i)
    {
        switch(state)
        {
        case Element:

            if (text.mid(i, 1) == ">")
            {
                state = NormalState;
                setFormat(offset, i - offset + 1, validElementFormat);
            }

            else if (text.mid(i, 1) == " ")
            {
                state = AttributeName;
                setFormat(offset, i - offset, validElementFormat);
                offset = i + 1;
            }

            break;
        case AttributeName:

            if (text.mid(i, 1) == "=")
            {
                state = AttributeEquals;
                setFormat(offset, i - offset, attributeNameFormat);
                offset = i;
            }

            else if (text.mid(i, 1) == ">")
            {
                state = NormalState;
                setFormat(offset, i - offset + 1, validElementFormat);
            }

            break;
        case AttributeEquals:

            if (text.mid(i, 1) == "\"")
            {
                state = AttributeValue;
                setFormat(offset, i - offset, attributeNameFormat);
                offset = i;
            }

            break;
        case AttributeValue:

            if (text.mid(i, 1) == "\"")
            {
                state = AttributeName;
                setFormat(offset, i - offset + 1, attributeValueFormat);
                offset = i + 1;
            }

            break;
        case Comment:

            if (text.mid(i, 3) == "-->")
            {
                state = NormalState;
                setFormat(offset, i - offset + 3, commentFormat);
                i+=2;
            }
        default:

            if (text.mid(i, 4) == ">!--")
            {
                offset = i;
                state = Comment;
                i+=3;
            }
            else if (text.mid(i, 1) == "<")
            {
                offset = i;
                state = Element;
            }

            break;
        }
    }
}


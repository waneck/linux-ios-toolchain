#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H
#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QTextCharFormat>
#include <QtCore/QVector>
#include <QtCore/QSet>
#include <clang-c/Index.h>

class QTextDocument;

class Highlight : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlight(QTextDocument* document = NULL);
    CXIndex cx_index;
    CXTranslationUnit cx_tu;
    CXSourceRange range;

    void setCXFile(CXFile f);
    void setCXTranslationUnit(CXTranslationUnit tu);
    void setCXSourceRange(CXSourceRange r);
protected:
    void highlightBlock(QString const& text);
    // formats
    QTextCharFormat m_formatFunction;
    QTextCharFormat m_formatSingleLineComment;
    QTextCharFormat m_formatMultiLineComment;
    QTextCharFormat m_formatKeyword;
    QTextCharFormat m_formatUserKeyword;
    QTextCharFormat m_formatOperator;
    QTextCharFormat m_formatNumber;
    QTextCharFormat m_formatEscapeChar;
    QTextCharFormat m_formatMacro;
    QTextCharFormat m_formatString;
    CXFile file;


signals:
    
public slots:
    
};

#endif // HIGHLIGHT_H

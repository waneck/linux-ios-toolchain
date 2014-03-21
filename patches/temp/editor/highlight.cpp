#include "highlight.h"

#include <QDebug>

Highlight::Highlight(QTextDocument* document) : QSyntaxHighlighter(document)
{
    m_formatFunction.setForeground(Qt::black);
    m_formatSingleLineComment.setForeground(Qt::red);
    m_formatKeyword.setForeground(Qt::blue);
    m_formatUserKeyword.setForeground(Qt::darkBlue);
    m_formatOperator.setForeground(Qt::black);
    m_formatNumber.setForeground(Qt::darkMagenta);
    m_formatEscapeChar.setForeground(Qt::darkBlue);
    m_formatMacro.setForeground(Qt::darkGreen);
    m_formatMultiLineComment.setForeground(Qt::red);
    m_formatString.setForeground(Qt::darkCyan);
}

void Highlight::setCXFile(CXFile f)
{
    file = f;
}

void Highlight::setCXTranslationUnit(CXTranslationUnit tu)
{
    cx_tu = tu;
}

void Highlight::setCXSourceRange(CXSourceRange r)
{
    range = r;
}

void Highlight::highlightBlock(const QString &text)
{
    CXToken *tokens;
    unsigned tokenCount;
    clang_tokenize(cx_tu,range, &tokens, &tokenCount);

    if (tokenCount > 0)
    {
      CXCursor *cursors = NULL;
      cursors = (CXCursor *)calloc(sizeof(CXCursor), tokenCount);
      clang_annotateTokens(cx_tu, tokens, tokenCount, cursors);

      for (unsigned i=0 ; i<tokenCount ; i++)
      {
        CXSourceRange sr = clang_getTokenExtent(cx_tu, tokens[i]);
        unsigned start, end;
        CXSourceLocation s = clang_getRangeStart(sr);
        CXSourceLocation e = clang_getRangeEnd(sr);
        clang_getInstantiationLocation(s, 0, 0, 0, &start);
        clang_getInstantiationLocation(e, 0, 0, 0, &end);
        qDebug()<<"start:"<<start<<"end:"<<end;
    /*    if(start >end) {
            int tmp = start;
            start = end;
            end = tmp;
        }
*/

        switch (cursors[i].kind)
        {
        case CXCursor_FirstRef... CXCursor_LastRef:

          break;
        case CXCursor_MacroDefinition:

          break;
        case CXCursor_MacroInstantiation:

          break;
        case CXCursor_FirstDecl...CXCursor_LastDecl:

          break;
        case CXCursor_ObjCMessageExpr:

          break;
        case CXCursor_DeclRefExpr:

          break;
        case CXCursor_PreprocessingDirective: {

        }
          break;
        default:
          break;
        }
     /*   if(cursors[i].kind == CXCursor_FunctionDecl)
            setFormat(start, end-start, m_formatSingleLineComment);*/
      }
    }
    setFormat(1, 6, m_formatSingleLineComment);

    qDebug()<<"position"<<currentBlock().position();
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::darkMagenta);
    QString pattern = "\\bMy[A-Za-z]+\\b";


    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
    }
}

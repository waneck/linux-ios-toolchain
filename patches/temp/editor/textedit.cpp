#include "textedit.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTextBlock>
#include <QScrollBar>
#include <QTextLayout>
#include <QDebug>

#include "objchighlighter.h"
#include "highlight.h"

static int completion_printCompletionHeadTerm(
    CXCompletionString completion_string, FILE *fp)
{
    int i_chunk  = 0;
    int n_chunks = clang_getNumCompletionChunks(completion_string);
    CXString ac_string;

    /* inspect all chunks only to find the TypedText chunk */
    for ( ; i_chunk < n_chunks; i_chunk++)
    {
        if (clang_getCompletionChunkKind(completion_string, i_chunk)
            == CXCompletionChunk_TypedText)
        {
            /* We got it, just dump it to fp */
            ac_string = clang_getCompletionChunkText(completion_string, i_chunk);
            fprintf(fp, "COMPLETION: %s", clang_getCString(ac_string));
            clang_disposeString(ac_string);
            return n_chunks;    /* care package on the way */
        }
    }

    return -1;   /* We haven't found TypedText chunk in completion_string */
}


/* Print the completion line except the header term (COMPLETION: TypedText),
 * the output format should be identical with the result of clang -cc1
 * -code-completion-at. Here are some sample outputs from the clang code
 * completion process:

     COMPLETION: short
     COMPLETION: signed
     COMPLETION: static
     COMPLETION: Pattern : static_cast<<#type#>>(<#expression#>)
     COMPLETION: struct

 * However, here we don't handle Pattern explicitly because the emacs
 * script would simply drop those pattern lines with an regexp T T
 */
static void completion_printAllCompletionTerms(
    CXCompletionString completion_string, FILE *fp)
{
    int i_chunk  = 0;
    int n_chunks = clang_getNumCompletionChunks(completion_string);

    CXString chk_text;
    enum CXCompletionChunkKind chk_kind;

    for ( ; i_chunk < n_chunks; i_chunk++)
    {
        /* get the type and completion text of this chunk */
        chk_kind = clang_getCompletionChunkKind(completion_string, i_chunk);
        chk_text = clang_getCompletionChunkText(completion_string, i_chunk);

        /* differenct kinds of chunks has various output formats */
        switch (chk_kind)
        {
        case CXCompletionChunk_Placeholder:
            fprintf(fp, "<#%s#>", clang_getCString(chk_text));
            break;

        case CXCompletionChunk_ResultType:
            fprintf(fp, "[#%s#]", clang_getCString(chk_text));
            break;

        case CXCompletionChunk_Optional:
            /* print optional term in a recursive way */
            fprintf(fp, "{#");
            completion_printAllCompletionTerms(
                clang_getCompletionChunkCompletionString(completion_string, i_chunk),
                fp);
            fprintf(fp, "#}");
            break;

        default:
            fprintf(fp, "%s", clang_getCString(chk_text));
        }

        clang_disposeString(chk_text);
    }
}


/* Print specified completion string to fp. */
void completion_printCompletionLine(
    CXCompletionString completion_string, FILE *fp)
{
    /* print completion item head: COMPLETION: typed_string */
    if (completion_printCompletionHeadTerm(completion_string, fp) > 1)
    {
        /* If there's not only one TypedText chunk in this completion string,
         * we still have a lot of info to dump:
         *
         *     COMPLETION: typed_text : ##infos##
         */
        fprintf(fp, " : ");
        completion_printAllCompletionTerms(completion_string, fp);
    }

    fprintf(fp, "\n");
}

/* Print all completion results to fp */
void completion_printCodeCompletionResults(CXCodeCompleteResults *res, FILE *fp)
{
    unsigned int i = 0;
    for ( ; i < res->NumResults; i++) {
        completion_printCompletionLine(res->Results[i].CompletionString, fp);
    }
}


TextEdit::TextEdit(QWidget *parent): QTextEdit(parent)
{

    this->setFont(QFont("Monaco", 10));
    this->setTabStopWidth(fontMetrics().width("\t")/2);
    connect(this,SIGNAL(cursorPositionChanged()),this,SLOT(slotCursorPositionChanged()));

    QFile file("/tmp/a.m");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray data = file.readAll();
    QTextStream in(&data);
    QString decodedStr = in.readAll();
    setPlainText(decodedStr);
    file.close();
    cx_index = clang_createIndex(0,0);
    struct CXUnsavedFile unsaved_file;
    unsaved_file.Filename = "/tmp/a.m";
    unsaved_file.Contents = (char*)calloc(sizeof(char), 4096);
    unsaved_file.Length = this->toPlainText().length();
    char **cmd_args =  (char**)calloc(sizeof(char*), 6);
    int i = 0;
    for(i ; i <6; i++) {
    cmd_args[i] = (char*)calloc(sizeof(char), 1000 + 1);
    strcpy(cmd_args[i], "-cc1");
    strcpy(cmd_args[i], "-fsyntax-only");
    strcpy(cmd_args[i], "-code-completion-macros");
    strcpy(cmd_args[i], "-code-completion-patterns");
    strcpy(cmd_args[i], "-x objective-c");
    strcpy(cmd_args[i], "-I/usr/share/iPhoneOS5.0.sdk/usr/include");
    }

    cx_tu = clang_parseTranslationUnit(cx_index, "/tmp/a.m" , cmd_args, 6, &unsaved_file, 1, CXTranslationUnit_PrecompiledPreamble|CXTranslationUnit_Incomplete);
    if(!cx_tu)
        qDebug()<<"cx_tu";

    //connect(this,SIGNAL(textChanged()), this,SLOT(slotReparse()));



    completionList = new QListWidget(this);
    completionList->setSelectionMode( QAbstractItemView::SingleSelection );
    completionList->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    completionList->hide();
    completionList->setSortingEnabled( true );
    completionList->setSelectionMode( QAbstractItemView::SingleSelection );
    QPalette palette;
    palette.setBrush(QPalette::Active, QPalette::Base, Qt::white);
    completionList->setPalette(palette);

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

    //connect(m_completionList, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(slotWordCompletion(QListWidgetItem *)) );
}

TextEdit::~TextEdit()
{
  
}

void TextEdit::slotCursorPositionChanged()
{
    viewport()->update();
}

void TextEdit::slotReparse()
{
    //connect(this,SIGNAL(textChanged()), this,SLOT(slotReparse()));
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    QString prefix = tc.selectedText();
    completionList->clear();
    qDebug()<<prefix;

    //clang_disposeTranslationUnit(cx_tu);
    struct CXUnsavedFile unsaved_file;
    unsaved_file.Filename = "/tmp/a.m";
    unsaved_file.Contents = strdup(this->toPlainText().toStdString().c_str());
    unsaved_file.Length = this->toPlainText().length();

    clang_reparseTranslationUnit(
        cx_tu, 1, &unsaved_file, CXTranslationUnit_PrecompiledPreamble);

    CXFile file = clang_getFile(cx_tu, unsaved_file.Filename);

    CXSourceLocation start = clang_getLocationForOffset(cx_tu, file, 0);
    CXSourceLocation end = clang_getLocationForOffset(cx_tu, file, this->toPlainText().length());
    CXSourceRange range= clang_getRange(start,end);
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
    QTextCursor currentCursor = this->textCursor();
    currentCursor.setPosition(3);
    //setCurrentCharFormat(m_formatKeyword);
    currentCursor.setCharFormat(m_formatMacro);
   /* Highlight *hi = new Highlight();
    hi->setCXFile(file);
    hi->setCXSourceRange(range);
    hi->setCXTranslationUnit(cx_tu);

    hi->setDocument(this->document());*/
   // ObjcHighlighter *h = new ObjcHighlighter(this->document());

    //qDebug()<<tokenCount;
    int currentLine = this->textCursor().blockNumber()+1;
    int currentColumn = this->textCursor().columnNumber();
    qDebug() << currentLine <<currentColumn<<endl;
    CXCodeCompleteResults *result = clang_codeCompleteAt(
                cx_tu, "/tmp/a.m", currentLine, currentColumn,
        &unsaved_file, 1, CXCodeComplete_IncludeMacros);
    clang_sortCodeCompletionResults(result->Results, result->NumResults);

    FILE * fp = fopen("/tmp/log.txt","w");
    completion_printCodeCompletionResults(result, fp);
    fclose(fp);


    int maxwidth = 0;
    for ( int i = 0; i < result->NumResults; i++) {
        CXCompletionString cx_str= result->Results[i].CompletionString;
        CXString ac_string;
        for(int i = 0; i < clang_getNumCompletionChunks(cx_str); i++) {
            if(clang_getCompletionChunkKind(cx_str,i) == CXCompletionChunk_TypedText) {
                ac_string= clang_getCompletionChunkText(cx_str, i);
                QString result_string = clang_getCString(ac_string);

                if(!result_string.startsWith("_")) {

                    QListWidgetItem *item = new QListWidgetItem( completionList );
                    item->setText(result_string);

                    completionList->addItem(item);
                    int width = this->fontMetrics().width(result_string);
                    if(width > maxwidth)
                        maxwidth = width;
                }
               //qDebug()<<result_string;
            }
        }

        //completion_printCompletionLine(res->Results[i].CompletionString, fp);
    }


    completionList->setGeometry(cursorRect().x(), cursorRect().y()+fontMetrics().height(), maxwidth+30, 100);
    completionList->show();
}

void TextEdit::autoIndent()
{
    QTextBlock preblock;
    QTextCursor currentTextCursor = this->textCursor();
    QTextBlock block = textCursor().block();
    qDebug()<<block.text();
    if(block.previous().isValid() && !block.previous().text().isEmpty()) {
        qDebug()<<block.previous().text();
        preblock = block.previous();
    }
    else
        return;
    QString indent;
    QString text = preblock.text();
    for(int i = 0; i < text.length(); i++) {
        if(text.at(i)==' '|| text.at(i)=='\t')
            indent+=text.at(i);
        else
            break;
    }
    qDebug()<<"s"<<indent<<"s";
    currentTextCursor.insertText(indent);
    currentTextCursor.movePosition(QTextCursor::EndOfLine);
    this->setTextCursor(currentTextCursor);
}

void TextEdit::autoPair(QChar c)
{

    QTextCursor currentTextCursor = this->textCursor();
   /* QTextBlock block = currentTextCursor.block();
    qDebug()<<block.text()<<currentTextCursor.positionInBlock();
    qDebug()<<block.text().at(currentTextCursor.positionInBlock());
    //if(currentTextCursor.positionInBlock() != block.text().length())
    if(block.text().at(currentTextCursor.positionInBlock()+1).isLetterOrNumber())
        return;
*/
    if(c == '{') {
        currentTextCursor.insertText("\n");
        //currentTextCursor.insertText("\t\n");
        autoIndent();
        currentTextCursor.insertText("\n");
        autoIndent();
        currentTextCursor.insertText("}");
        currentTextCursor.movePosition(QTextCursor::Up);
        currentTextCursor.insertText("\t");
        currentTextCursor.movePosition(QTextCursor::EndOfLine);
    } else if(c == '\"') {
        currentTextCursor.insertText("\"");
        currentTextCursor.movePosition(QTextCursor::Left);
    } else if(c == '\'') {
        currentTextCursor.insertText("'");
        currentTextCursor.movePosition(QTextCursor::Left);
    } else if(c == '(') {
        currentTextCursor.insertText(")");
        currentTextCursor.movePosition(QTextCursor::Left);
    } else if(c == '[') {
        currentTextCursor.insertText("]");
        currentTextCursor.movePosition(QTextCursor::Left);
    }
    this->setTextCursor(currentTextCursor);
}

void TextEdit::paintEvent(QPaintEvent *e)
{
    QPainter painter(viewport());
    QRect r = cursorRect();
    r.setX(0);
    r.setWidth(viewport()->width());
    painter.fillRect(r,QBrush(QColor(215,252,255)));
    painter.end();
    QTextEdit::paintEvent(e);
}

void TextEdit::save()
{
    QFile file("/tmp/a.m");
    file.open(QIODevice::WriteOnly);
    QString s = this->toPlainText();
    QTextStream out(&file);
    out<<s;
    file.close();
}

void TextEdit::keyPressEvent(QKeyEvent *e)
{
    QChar key = e->key();
    QTextEdit::keyPressEvent(e);
    if(key == '{' || key == '\"' || key == '\'' || key == '(' || key == '[')
        autoPair(key);
    else if(key == Qt::Key_Enter || key == Qt::Key_Return) {
        if(!completionList->isVisible())
            autoIndent();
    }
    else if(key == Qt::Key_Q) {
        qDebug()<<"here";
        completionList->setGeometry(cursorRect().x(), cursorRect().y()+fontMetrics().height(), 100, 100);
        completionList->show();
    } else if(key == Qt::Key_Escape)
        completionList->hide();
    else if(key == Qt::Key_Alt)
        slotReparse();
    e->accept();
}

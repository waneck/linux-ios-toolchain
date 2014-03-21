/* -*-C++-*- */
#ifndef _TEXT_EDIT_H
#define _TEXT_EDIT_H
#include <QTextEdit>
#include <QTextCursor>
#include <QPainter>
#include <QListWidget>
#include <clang-c/Index.h>

class TextEdit : public QTextEdit
{
  Q_OBJECT
public:
  TextEdit(QWidget *parent);
  ~TextEdit();
  void save();
protected:
  void paintEvent(QPaintEvent *e);
  void keyPressEvent(QKeyEvent *e);
private slots:
  void slotCursorPositionChanged();
  void slotReparse();

private:
  void autoPair(QChar c);
  void autoIndent();
  QListWidget *completionList;

  CXIndex cx_index;
  CXTranslationUnit cx_tu;

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
};

#endif

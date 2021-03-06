/************************************************************************
**
** Authors:   Nils Schimmelmann <nschimme@gmail.com>
**
** This file is part of the MMapper project.
** Maintained by Nils Schimmelmann <nschimme@gmail.com>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the:
** Free Software Foundation, Inc.
** 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
************************************************************************/

#include "inputwidget.h"
#include "configuration/configuration.h"

#include <QDebug>
#include <QKeyEvent>

#include <QTextCharFormat>
#include <QFont>
#include <QFontMetrics>
#include <QTextCursor>
#include <QSizePolicy>

#define MIN_WORD_LENGTH 3

const QRegExp InputWidget::s_whitespaceRx("\\W+");

InputWidget::InputWidget(QWidget *parent)
    : QPlainTextEdit(parent)
{
    // Size Policy
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    // Minimum Size
    QFontMetrics fm(currentCharFormat().font());
    setMinimumSize(QSize(fm.averageCharWidth(),
                         fm.lineSpacing() + (document()->documentMargin() + QFrame::frameWidth()) * 2 +
                         contentsMargins().top() + contentsMargins().bottom()));
    setSizeIncrement(fm.averageCharWidth(), fm.lineSpacing());

    // Line Wrapping
    setLineWrapMode(QPlainTextEdit::NoWrap);

    // Word History
    m_lineIterator = new InputHistoryIterator(m_lineHistory);
    m_tabIterator = new TabCompletionIterator(m_tabCompletionDictionary);
    m_newInput = true;
}

QSize InputWidget::sizeHint() const
{
    return minimumSize();
}

InputWidget::~InputWidget()
{
    delete m_lineIterator;
    delete m_tabIterator;
}

void InputWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() != Qt::Key_Tab) m_tabbing = false;

    // Check for key bindings
    switch (event->key()) {
    /** Enter could mean to submit the current text or add a newline */
    case Qt::Key_Return:
    case Qt::Key_Enter:
        switch (event->modifiers()) {
        case Qt::NoModifier:    /** Submit text */
            gotInput();
            event->accept();
            break;

        case Qt::ShiftModifier: /** Add newline (i.e. ignore) */
        default:                /** Otherwise ignore  */
            QPlainTextEdit::keyPressEvent(event);

        };
        break;

    /** Key bindings for word history, tab completion, etc) */
    // NOTE: MacOS does not differentiate between arrow keys and the keypad keys
    // and as such we disable keypad movement functionality in favor of history
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Tab:
        switch (event->modifiers()) {
        case Qt::KeypadModifier:
#ifndef Q_OS_MAC
            keypadMovement(event->key());
            event->accept();
            break;
#endif
        case Qt::NoModifier:
            wordHistory(event->key());
            event->accept();
            break;
        default:
            QPlainTextEdit::keyPressEvent(event);
        };
        break;

    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
    case Qt::Key_Clear:  // Numpad 5
    case Qt::Key_Home:
    case Qt::Key_End:
#ifndef Q_OS_MAC
        switch (event->modifiers()) {
        case Qt::KeypadModifier:
            keypadMovement(event->key());
            event->accept();
            break;
        };
#endif
    // TODO, Implement these following keys
    case Qt::Key_Delete:
    case Qt::Key_Plus:
    case Qt::Key_Minus:
    case Qt::Key_Slash:
    case Qt::Key_Asterisk:
    case Qt::Key_Insert:

    /** All other keys */
    default:
        m_newInput = true;
        QPlainTextEdit::keyPressEvent(event);
    };
}

void InputWidget::keypadMovement(int key)
{
    switch (key) {
    case Qt::Key_Up:
        emit sendUserInput("north");
        break;
    case Qt::Key_Down:
        emit sendUserInput("south");
        break;
    case Qt::Key_Left:
        emit sendUserInput("west");
        break;
    case Qt::Key_Right:
        emit sendUserInput("east");
        break;
    case Qt::Key_PageUp:
        emit sendUserInput("up");
        break;
    case Qt::Key_PageDown:
        emit sendUserInput("down");
        break;
    case Qt::Key_Clear: // Numpad 5
        emit sendUserInput("exits");
        break;
    case Qt::Key_Home:
        emit sendUserInput("open exit");
        break;
    case Qt::Key_End:
        emit sendUserInput("close exit");
        break;
    case Qt::Key_Insert:
        emit sendUserInput("flee");
        break;
    case Qt::Key_Delete:
    case Qt::Key_Plus:
    case Qt::Key_Minus:
    case Qt::Key_Slash:
    case Qt::Key_Asterisk:
    default:
        qDebug() << "! Unknown keypad movement" << key;
    };
}


void InputWidget::wordHistory(int key)
{
    QTextCursor cursor = textCursor();
    switch (key) {
    case Qt::Key_Up:
        if (!cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor)) {
            // At the top of the document
            backwardHistory();

        }
        break;
    case Qt::Key_Down:
        if (!cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor)) {
            // At the end of the document
            forwardHistory();

        }
        break;
    case Qt::Key_Tab:
        tabComplete();
        break;
    };
}

void InputWidget::gotInput()
{
    QString input = toPlainText();
    if (Config().m_clientClearInputOnEnter) {
        clear();
    } else {
        selectAll();
    }
    emit sendUserInput(input);
    addLineHistory(input);
    addTabHistory(input);
    m_lineIterator->toBack();
}

void InputWidget::addLineHistory(const InputHistoryEntry &string)
{
    if (!string.isEmpty() && (m_lineHistory.empty() || m_lineHistory.last() != string)) {
        // Add to line history if it is a new entry
        m_lineHistory << string;
    }

    // Trim line history
    if (m_lineHistory.size() > Config().m_clientLinesOfInputHistory) {
        m_lineHistory.removeFirst();
    }
}


void InputWidget::addTabHistory(const WordHistoryEntry &string)
{
    QStringList list = string.split(s_whitespaceRx, QString::SkipEmptyParts);
    foreach (QString word, list) {
        if (word.length() > MIN_WORD_LENGTH) {
            // Adding this word to the dictionary
            m_tabCompletionDictionary << word;

            // Trim dictionary
            if (m_tabCompletionDictionary.size() > Config().m_clientTabCompletionDictionarySize) {
                m_tabCompletionDictionary.removeFirst();

            }
        }
    }
}


void InputWidget::forwardHistory()
{
    if (!m_lineIterator->hasNext()) {
        emit showMessage("Reached beginning of input history", 1000);
        clear();
        return ;

    }

    selectAll();
    if (m_newInput) {
        addLineHistory(toPlainText());
        m_newInput = false;
    }

    QString next = m_lineIterator->next();
    // Ensure we always get "new" input
    if (next == toPlainText() && m_lineIterator->hasNext()) {
        next = m_lineIterator->next();
    }

    insertPlainText(next);
}


void InputWidget::backwardHistory()
{
    if (!m_lineIterator->hasPrevious()) {
        emit showMessage("Reached end of input history", 1000);
        return ;

    }

    selectAll();
    if (m_newInput) {
        addLineHistory(toPlainText());
        m_newInput = false;
    }

    QString previous = m_lineIterator->previous();
    // Ensure we always get "new" input
    if (previous == toPlainText() && m_lineIterator->hasPrevious()) {
        previous = m_lineIterator->previous();
    }

    insertPlainText(previous);

}

void InputWidget::tabComplete()
{
    QTextCursor current = textCursor();
    current.select(QTextCursor::WordUnderCursor);
    if (!m_tabbing) {
        m_tabFragment = current.selectedText();
        m_tabIterator->toBack();
        m_tabbing = true;
    }

    // If we reach the end then loop back to the beginning
    if (!m_tabIterator->hasPrevious() && !m_tabCompletionDictionary.isEmpty()) {
        m_tabIterator->toBack();
    }

    // Iterate through all previous words
    while (m_tabIterator->hasPrevious()) {
        // TODO: Utilize a trie and store the search results?
        if (m_tabIterator->peekPrevious().startsWith(m_tabFragment)) {
            // Found a previous word to complete to
            current.insertText(m_tabIterator->previous());
            if (current.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor)) {
                current.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, m_tabFragment.size());
                setTextCursor(current);
            }
            return ;
        } else {
            // Try the next word
            m_tabIterator->previous();
        }
    }
}

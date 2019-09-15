/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "console.h"
#include <QDate>
#include <QScrollBar>
#include <QDebug>

Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, QColor::fromRgb(48,10,36) );
    p.setColor(QPalette::Text, Qt::white);
    setPalette(p);
    QFont font;
    font.setFamily("Ubuntu Mono");
    font.setPointSize(13);
    font.setBold(true);
    this->setFont(font);
    this->insertPlainText("Date: ");

    font.setFamily("Ubuntu Mono");
    font.setPointSize(13);
    font.setBold(false);
    this->setFont(font);
    this->insertPlainText("Terminal test text");

    QTextCursor cursor = textCursor();


    QTextCharFormat charFormat;
    font.setBold(true);
    charFormat.setFont(font);
    charFormat.setForeground( QBrush( QColor( "#6ad534" ) ) );
    cursor.insertText( " text cursor bold test: ",charFormat);

    font.setBold(false);
    charFormat.setFont(font);
    charFormat.setForeground( QBrush( QColor( "white" ) ) );
    cursor.insertText(" regular test", charFormat);
   
    
}

void Console::putTransmitData(QByteArray data) 
{
    QString timeString = QDateTime::currentDateTime().toString("mm:ss.zzz");
    timeString.append(": ");

    QTextCursor cursor = textCursor();
    QTextCharFormat charFormat;
    QFont font;
    font.setFamily("Ubuntu Mono");
    font.setPointSize(13);
    font.setBold(true);
    charFormat.setForeground( QBrush( QColor( "#5b96cf" ) ) );
    charFormat.setFont(font);

    cursor.insertText(timeString, charFormat);

    font.setFamily("Ubuntu Mono");
    font.setPointSize(13);
    font.setBold(false);
    charFormat.setForeground( QBrush( QColor( "white" ) ) );
    charFormat.setFont(font); 
    
    data.append("\n");
    cursor.insertText( data , charFormat);

    emit getData(data);

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());   
}

void Console::putReceiveData(const QByteArray &data)
{
    QByteArray newData = data;
    QString timeString = QDateTime::currentDateTime().toString("mm:ss.zzz");
    timeString.append(": ");

   
    
    QTextCursor cursor = textCursor();

     if( QString::compare(this->toPlainText().right(1), "\n", Qt::CaseSensitive) != 0 ){
         cursor.insertText("\n");
         qInfo() << "without enter detect";
     }
         
    
    QTextCharFormat charFormat;
    QFont font;
    font.setFamily("Ubuntu Mono");
    font.setPointSize(13);
    font.setBold(true);
    charFormat.setForeground( QBrush( QColor( "#6ad534" ) ) );
    charFormat.setFont(font);

    cursor.insertText(timeString, charFormat);

    font.setFamily("Ubuntu Mono");
    font.setPointSize(13);
    font.setBold(false);
    charFormat.setForeground( QBrush( QColor( "white" ) ) );
    charFormat.setFont(font);

    QString spaceData("\n");

    for(int j=0; j< timeString.length(); j++)
        spaceData.append(" ");

    for(int i=0; i<(newData.length()-1); i++) {
        if(newData[i] == '\n')
            newData.replace(i,1,spaceData.toStdString().c_str() );
    }
  
    cursor.insertText( newData , charFormat); 


    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Console::setLocalEchoEnabled(bool set)
{
    m_localEchoEnabled = set;
}

void Console::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        if (m_localEchoEnabled)
            QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
    }
}

void Console::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}

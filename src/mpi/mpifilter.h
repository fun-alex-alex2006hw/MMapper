/************************************************************************
**
** Authors:   Nils Schimmelmann <nschimme@gmail.com> (Jahara)
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

#ifndef MPIFILTER_H
#define MPIFILTER_H

#include <QObject>

#include "proxy/telnetfilter.h"

class MpiFilter : public QObject
{
    Q_OBJECT
public:
    MpiFilter(QObject *parent = nullptr);

signals:
    void sendToMud(const QByteArray &);
    void parseNewMudInput(IncomingData &data);
    void editMessage(int, QString, QString);
    void viewMessage(QString, QString);

public slots:
    void analyzeNewMudInput(IncomingData &data);

protected:
    void parseMessage(char command, QByteArray buffer);
    void parseEditMessage(QByteArray buffer);
    void parseViewMessage(QByteArray buffer);

private:
    TelnetDataType m_previousType;
    bool m_parsingMpi;

    char m_command;
    int m_remaining;
    QByteArray m_buffer;

};

#endif // MPIFILTER_H

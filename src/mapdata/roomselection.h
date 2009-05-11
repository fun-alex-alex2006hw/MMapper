/************************************************************************
**
** Authors:   Ulf Hermann <ulfonk_mennhar@gmx.de> (Alve), 
**            Marek Krejza <krejza@gmail.com> (Caligor),
**            Nils Schimmelmann <nschimme@gmail.com> (Jahara),
**            Anonymous <lachupe@gmail.com> (Azazello)
**
** This file is part of the MMapper project. 
** Maintained by Nils Schimmelmann <nschimme@gmail.com>
**
** Copyright: See COPYING file that comes with this distribution
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

#ifndef ROOMSELECTION_H
#define ROOMSELECTION_H

#include <QtGui>
#include <QtOpenGL>
#include "roomadmin.h"
#include "roomrecipient.h"
#include "room.h"
#include "coordinate.h"




class RoomSelection : public QMap<uint, const Room *>, public RoomRecipient
{

public:
    RoomSelection(RoomAdmin * admin) : m_admin(admin) {}
    void receiveRoom(RoomAdmin * admin,const Room * room);
    bool containsAll(const RoomSelection * other) const;
    
private:
    RoomAdmin * m_admin;
};


#endif

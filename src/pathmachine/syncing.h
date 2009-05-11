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

#ifndef SYNCING_H
#define SYNCING_H

#include <list>
#include "path.h"
#include "roomadmin.h"
#include "roomrecipient.h"
#include "roomsignalhandler.h"

class Syncing : public RoomRecipient
{
private:
  RoomSignalHandler * signaler;
  uint numPaths;
  PathParameters & params;
  std::list<Path *> * paths;
  Path * parent;
public:
  Syncing(PathParameters & p, std::list<Path *> * paths, RoomSignalHandler * signaler);
  void receiveRoom(RoomAdmin *, const Room *);
  std::list<Path *> * evaluate();
  ~Syncing();
};

#endif


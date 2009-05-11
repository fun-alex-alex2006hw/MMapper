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

#ifndef PARSEEVENT
#define PARSEEVENT

#include <deque>
#include "property.h"
#include "listcycler.h"
#include "coordinate.h"
#include <QVariant>

/**
 * the ParseEvents will walk around in the SearchTree
 */
class ParseEvent : public ListCycler<Property *, std::deque<Property *> >
{
public:
  ParseEvent(uint move) : moveType(move), numSkipped(0) {}
  ParseEvent(const ParseEvent & other);
  virtual ~ParseEvent();
  ParseEvent & operator=(const ParseEvent & other);

  void reset();
  void countSkipped();
  std::deque<QVariant> & getOptional() {return optional;}
  const std::deque<QVariant> & getOptional() const {return optional;}
  uint getMoveType() const {return moveType;}
  uint getNumSkipped() const {return numSkipped;}
  
private:
  std::deque<QVariant> optional;
  uint moveType;
  uint numSkipped;
  
};

#endif

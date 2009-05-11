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

#include <iostream>
#include "component.h"

using namespace std;




map<QString, ComponentCreator *> & ComponentCreator::creators() {
      static map<QString, ComponentCreator *> creators;
      return creators;
}

Component::Component(bool threaded) {
	if (threaded) {
	  thread = new ComponentThreader(this);
	}
	else {
	  thread = 0;
	}
}

void Component::start() {
  if (thread) {
    moveToThread(thread);
    thread->start();
  }
  else 
    init();
}

Component::~Component() {
	if(thread) {
		// last chance to get rid of the thread. 
		// components should handle that in their own destructors
		thread->terminate();
		delete thread;
	}
}

void ComponentThreader::run() {
  try {
    owner->runInit();
    exec();
  } catch (char const * error) {
	    cerr << error << endl;
	    throw error;
  }
}

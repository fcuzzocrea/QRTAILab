/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   as published by  the Free Software Foundation; either version 2       *
 *   of the License, or  (at your option) any later version.               *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/*
 file:		targets_manager.cpp
 describtion:
   file for the classe QRL_TargetsManager
*/

#include "targets_manager.h"


/**
* @brief Initialize Targets Manager
*/
QRL_TargetsManager::QRL_TargetsManager(QWidget *parent,QRtaiLabCore* qtargetinterface)
	:QDialog(parent),qTargetInterface(qtargetinterface)
{
	setupUi(this);

}
QRL_TargetsManager::~QRL_TargetsManager()
{
}
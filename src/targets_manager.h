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
 file:		targets_manager.h
 describtion:
   file for the class  QRL_TargetsManager
*/

#ifndef _TARGETS_MANAGER_H
#define _TARGETS_MANAGER_H 1


#include "ui_qrl_targets_manager.h"
#include "qrtailab_core.h"


/**
 * @brief Managed all targets
 */
class QRL_TargetsManager : public QDialog, private Ui::QRL_TargetsManager
{
   Q_OBJECT
public:
  QRL_TargetsManager(QWidget *parent = 0, QRtaiLabCore* qtargetinterface=NULL);
  ~QRL_TargetsManager();
public slots:
private:
  QRtaiLabCore* qTargetInterface;
  int currentTarget;
  QList<QListWidgetItem *> targetItems;
};


#endif
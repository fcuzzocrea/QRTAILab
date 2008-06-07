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
 file:		scopes_manager.h
 describtion:
   file for the class  QRL_ScopesManager
*/

#ifndef _LOGS_MANAGER_H
#define _LOGS_MANAGER_H 1


#include "ui_qrl_logs_manager.h"
#include "qrtailab_core.h"






/**
 * @brief Managed all Logs windows
 */
class QRL_LogsManager : public QDialog, private Ui::QRL_LogsManager
{
   Q_OBJECT
public:
  QRL_LogsManager(QWidget *parent = 0, QRtaiLabCore* qtargetinterface=NULL);
  ~QRL_LogsManager();
public slots:
  void showLogOptions( QListWidgetItem * item  );
  void showLogOptions( int );
  void startSaving();
  void stopSaving(int);
  void changeSaveTime(double);
  void changeFileName(const QString&);
private:
  int Num_Logs;
  Target_Logs_T *Logs;
  unsigned int currentLog;
 // GetScopeDataThread* Get_Scope_Data_Thread;
  QRtaiLabCore* qTargetInterface;
  //TargetThread* targetThread;
  QList<QListWidgetItem *> logItems;
friend QDataStream& operator<<(QDataStream &out, const QRL_LogsManager &d);
friend QDataStream& operator>>(QDataStream &in, QRL_LogsManager(&d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_LogsManager &d);
	QDataStream& operator>>(QDataStream &in, QRL_LogsManager(&d));


#endif

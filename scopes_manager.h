/*
 file:		scopes_manager.h
 describtion:
   file for the classes GetScopeDataThread and  QRL_ScopesManager

 Copyright (C) 2007 Holger Nahrstaedt

 This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/
#ifndef _SCOPES_MANAGER_H
#define _SCOPES_MANAGER_H 1


#include "ui_qrl_scopes_manager.h"
#include "qrtailab.h"
#include "scope_window.h"


/**
 * @brief QThread for reading Scope data
 * @bug data transfer to scope window stocks, if window is changing
 * @bug	plotting bug for less datapoints 
 */

class GetScopeDataThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
    int setDt(double);
 //signals:
    //void value(int,int,float);
 public slots:
    void start(void* arg,TargetThread* targetthread,QRL_ScopeWindow* scopewindow);
 private:
    int index,x,y,w,h;
    char* mbx_id;
    TargetThread* targetThread;
    QRL_ScopeWindow* ScopeWindow;
    int MsgData, MsgLen, MaxMsgLen, TracesBytes;
    long int Ndistance;
    double dt;
 };

/**
 * @brief Managed all Scope windows
 * @todo add trigger mechanism
 * @todo add own color dialog
 */
class QRL_ScopesManager : public QDialog, private Ui::QRL_ScopesManager
{
   Q_OBJECT
public:
  QRL_ScopesManager(QWidget *parent = 0, TargetThread* targetthread=NULL);
  ~QRL_ScopesManager();
  void startScopeThreads();
  void stopScopeThreads();
  QRL_ScopeWindow** getScopeWindows(){return ScopeWindows;}
public slots:
  void  showScope(int);
  void showScopeOptions( QListWidgetItem * item );
  void changeRefreshRate(double);
  void changeDataPoints(double);
  void changeSaveTime(double);
  void changeDX(const QString&);
  void startSaving();
  void stopSaving(int);
  void setOptions(int);
  void setCurrentTrace(int);
  void changeTraceColor();
  void changeTraceWidth(double);
  void changeOffset(double);
  void changeDy(double);
  void changeDisplayModus(int);
  void changeDirection(int);
private:
  int Num_Scopes;
  Target_Scopes_T *Scopes;
  unsigned int currentScope;
  GetScopeDataThread* Get_Scope_Data_Thread;
  TargetThread* targetThread;
  QRL_ScopeWindow** ScopeWindows;
  int currentTrace;
};


#endif

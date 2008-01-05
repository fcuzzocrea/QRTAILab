/*
 file:		leds_manager.h
 describtion:
   file for the classes GetLedDataThread and  QRL_LedManager

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

#ifndef _LEDS_MANAGER_H
#define _LEDS_MANAGER_H 1


#include "ui_qrl_leds_manager.h"
#include "qrtailab.h"
#include "led_window.h"

/**
 * @brief QThread for reading Led data
 */

class GetLedDataThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
//signals:
    //void value(int,float);
 public slots:
    void start(void* arg,TargetThread* targetthread,QRL_LedWindow* ledwindow);
 private:
    int index,x,y,w,h;
    char* mbx_id;
    TargetThread* targetThread;
    QRL_LedWindow* LedWindow;
 };




/**
 * @brief Managed all Led windows
 * @todo reorganize window options
 */

class QRL_LedsManager : public QDialog, private Ui::QRL_LedsManager
{
   Q_OBJECT
public:
   QRL_LedsManager(QWidget *parent = 0,  TargetThread* targetthread=NULL);
   ~QRL_LedsManager();
    void startLedThreads();
    void stopLedThreads();
    QRL_LedWindow** getLedWindows(){return LedWindows;}
public slots:
  void  close();
  void  showLed(int);
  void showLedOptions( QListWidgetItem * item );
private:
  int Num_Leds;
  Target_Leds_T *Leds;
  unsigned int currentLed;
  GetLedDataThread* Get_Led_Data_Thread;
  TargetThread* targetThread;
  QRL_LedWindow** LedWindows;
  QWidget *mainWindow;
};


#endif

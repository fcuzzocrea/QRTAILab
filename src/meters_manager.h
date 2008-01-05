/*
* file:		meters_manager.h
* describtion:
*   file for the classes GetMeterDataThread and  QRL_MetersManager
*
* Copyright (C) 2007 Holger Nahrstaedt

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

#ifndef _METERS_MANAGER_H
#define _METERS_MANAGER_H 1


#include "ui_qrl_meters_manager.h"
#include "qrtailab.h"
#include "meter_window.h"

//!QThread for reading Meter data
class GetMeterDataThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
 //signals:
   // void value(int,float);
    int setRefreshRate(double);
 public slots:
    void start(void* arg,TargetThread* targetthread, QRL_MeterWindow* meterwindow);
 private:
    int index,x,y,w,h;
    double RefreshRate;
    char* mbx_id;
    TargetThread* targetThread;
    QRL_MeterWindow* MeterWindow;
    int MsgData, MsgLen, MaxMsgLen, DataBytes;
    long int Ndistance;

 };



/**
 * @brief Managed all Meter windows
 *
 *  With this Dialog all meters can be managed
 */
class QRL_MetersManager : public QDialog, private Ui::QRL_MetersManager
{
   Q_OBJECT
public:
   QRL_MetersManager(QWidget *parent = 0,TargetThread* targetthread=NULL);
   ~QRL_MetersManager();
       void startMeterThreads();
    void stopMeterThreads();
    QRL_MeterWindow** getMeterWindows(){return MeterWindows;}
public slots:
  void  showMeter(int);
   void showMeterOptions( QListWidgetItem * item );
  void changeRefreshRate(double);
  void changeMin(double);
  void changeMax(double);
  void changeMeter(int);
  void changeThermoColor1();
 void changeThermoColor2();
  void changePipeWith(double);
  void changeAlarmThermoColor1();
 void changeAlarmThermoColor2();
 void enableThermoAlarm(int);
 void changeThermoColorType(int);
  void changeAlarmLevel(double);
  void changeThermoDirection(int);
private:
   int Num_Meters;
  Target_Meters_T *Meters;
  unsigned int currentMeter;
   QRL_MeterWindow** MeterWindows;
   TargetThread* targetThread;
   GetMeterDataThread* Get_Meter_Data_Thread;
   QWidget* ThermoOptions;
   QWidget* DialOptions;   
   QWidget* LcdOptions;
   
};



#endif

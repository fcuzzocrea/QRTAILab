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
* file:		meters_manager.h
* describtion:
*   file for the classes GetMeterDataThread and  QRL_MetersManager
*/


#ifndef _METERS_MANAGER_H
#define _METERS_MANAGER_H 1


#include "ui_qrl_meters_manager.h"
#include "qrtailab_core.h"
#include "meter_window.h"




/**
 * @brief Managed all Meter windows
 *
 *  With this Dialog all meters can be managed
 */
class QRL_MetersManager : public QDialog, private Ui::QRL_MetersManager
{
   Q_OBJECT
public:
   QRL_MetersManager(QWidget *parent = 0, QRtaiLabCore* qtargetinterface=NULL);
   ~QRL_MetersManager();
       void startMeterThreads();
    void stopMeterThreads();
    QRL_MeterWindow** getMeterWindows(){return MeterWindows;}
 void refreshView();
public slots:
   void refresh();
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
  void changeNeedleColor();
  void changeLcdFont();
private:
   int Num_Meters;
  //Target_Meters_T *Meters;
  unsigned int currentMeter;
   QRL_MeterWindow** MeterWindows;
  QRtaiLabCore* qTargetInterface;
   QWidget* ThermoOptions;
   QWidget* DialOptions;   
   QWidget* LcdOptions;
   QTimer *timer;
 double RefreshRate;
   
};



#endif
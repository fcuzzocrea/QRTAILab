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
 file:		meter_window.h
 describtion:
   file for the class QRL_MeterWindow
*/

#ifndef _METER_WINDOW_H
#define _METER_WINDOW_H 1
#include "qrtailab.h"
#include <qwt_thermo.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_engine.h>
#include <qwt_dial.h>
#include <qwt_dial_needle.h>
//#include "qmeter.h"


/**
 * @brief Display Meter 
 * @todo calculate lineargradient for horizontal layout
 */
class QRL_MeterWindow : public QMdiSubWindow
{
   Q_OBJECT
   Q_ENUMS( Meter_Type )
public:
   enum Meter_Type {THERMO,	DIAL,	LCD};
   QRL_MeterWindow(QWidget *parent = 0,char* name=NULL);
   ~QRL_MeterWindow();
   double getRefreshRate(){return RefreshRate;}
   void changeRefreshRate(double);
   void setValue(float);
   void setMin(double);
   void setMax(double);
   void setMeter(Meter_Type);
   void setThermoColor1(const QColor&);
   void setThermoColor2(const QColor&);
   void setPipeWith(double);
   void setAlarmThermoColor1(const QColor&);
   void setAlarmThermoColor2(const QColor&);
   void setThermoAlarm(int);
   void setGradientEnabled(bool);
   void setAlarmLevel(double);
   void setThermoDirection(Qt::Orientation o);
   void setLcdFont(const QFont& font);
   void setNeedleColor(const QColor&);
   Meter_Type getMeterType(){return MeterType;}
protected slots:
  void closeEvent ( QCloseEvent * event ){event->ignore(); this->hide(); }
private:
  // void setDistance(const QwtThermo* const);
  float Value;
  QwtThermo *Thermo;
  QwtDial *Dial;
 // QMeter *Dial;
//  QLCDNumber *Lcd;
  QLabel *Lcd;
  double RefreshRate;
  double Max,Min;
  Meter_Type MeterType;
  QColor thermoColor1,thermoColor2,alarmThermoColor1,alarmThermoColor2;
  bool gradientEnabled;
  int pipeWidth,pipeDistance;
  double alarmLevel;
  QBrush brush;
  QLinearGradient gradient,alarmGradient;
  QwtDialSimpleNeedle *needle;
};


#endif

/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License           *
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
#include <QtGui>
#include <qwt_thermo.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_engine.h>
#include <qwt_dial.h>
#include <qwt_dial_needle.h>
//#include "qmeter.h"
 #include <QMdiSubWindow>
#include "meter_data.h"
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
   QRL_MeterWindow(QWidget *parent = 0,QRL_MeterData *meter=0);
   ~QRL_MeterWindow();
   double getRefreshRate(){return RefreshRate;}
   void changeRefreshRate(double);
   void refresh();
   void setMin(double);
   double getMin(){return Min;}
   void setMax(double);
    double getMax(){return Max;}
   void setMeter(Meter_Type);
   Meter_Type getMeterType(){return MeterType;}
   void setThermoColor1(const QColor&);
   QColor getThermoColor1(){return thermoColor1;}
   void setThermoColor2(const QColor&);
   QColor getThermoColor2(){return thermoColor2;}
   void setPipeWith(double);
   double getPipeWidth(){return pipeWidth;}
   void setAlarmThermoColor1(const QColor&);
   QColor getAlarmThermoColor1(){return alarmThermoColor1;}
   void setAlarmThermoColor2(const QColor&);
   QColor getAlarmThermoColor2(){return alarmThermoColor2;}
   void setThermoAlarm(int);
      void setThermoAlarm(bool);
   bool getThermoAlarm(){return Thermo->alarmEnabled();}
   void setGradientEnabled(bool);
   bool getGradientEnabled(){return gradientEnabled;}
   void setAlarmLevel(double);
   double getAlarmLevel(){return alarmLevel;}
   void setThermoDirection(Qt::Orientation o);
   void setLcdFont(const QFont& font);
   QFont getLcdFont(){return Lcd->font();}
  void setLcdFormat(char c) ;
  char getLcdFormat() {return format;}
  void setLcdPrecision(int p);
  int getLcdPrecision(){return precision;}
   void setNeedleColor(const QColor&);
   QColor getNeedleColor(){return needle->palette().button().color();}
   void setFileVersion(qint32 v){fileVersion=v;}
protected slots:
  void closeEvent ( QCloseEvent * event ){event->ignore(); this->hide(); }
private:
  qint32 fileVersion;
  // void setDistance(const QwtThermo* const);
 // float Value;
   QRL_MeterData* Meter;
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
  int precision;
  char format;
  friend QDataStream& operator<<(QDataStream &out, const QRL_MeterWindow &d);
  friend QDataStream& operator>>(QDataStream &in, QRL_MeterWindow(&d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_MeterWindow &d);
	QDataStream& operator>>(QDataStream &in, QRL_MeterWindow(&d));


#endif

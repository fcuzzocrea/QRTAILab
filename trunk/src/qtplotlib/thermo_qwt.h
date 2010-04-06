#ifndef THERMO_QWT_H
#define THERMO_QWT_H

#include "qtplot_global.h"
#include <qwt_thermo.h>

class  QPL_ThermoQwt : public QwtThermo
{
   Q_OBJECT

public:
   QPL_ThermoQwt(QWidget *parent = 0);
   virtual ~QPL_ThermoQwt();
      void setMin(double);
   double getMin(){return Min;}
   void setMax(double);
    double getMax(){return Max;}
   void setThermoColor1(const QColor&);
   QColor getThermoColor1(){return thermoColor1;}
   void setThermoColor2(const QColor&);
   QColor getThermoColor2(){return thermoColor2;}
   void setPipeWith(int);
   //double getPipeWidth(){return pipeWidth;}
   void setAlarmThermoColor1(const QColor&);
   QColor getAlarmThermoColor1(){return alarmThermoColor1;}
   void setAlarmThermoColor2(const QColor&);
   QColor getAlarmThermoColor2(){return alarmThermoColor2;}
   void setThermoAlarm(int);
      void setThermoAlarm(bool);
   bool getThermoAlarm(){return this->alarmEnabled();}
   void setGradientEnabled(bool);
   bool getGradientEnabled(){return gradientEnabled;}
   void setThermoDirection(Qt::Orientation o);
private:
     double Max,Min;
       QColor thermoColor1,thermoColor2,alarmThermoColor1,alarmThermoColor2;
  bool gradientEnabled;
  int pipeDistance;
  QBrush brush;
  QLinearGradient gradient,alarmGradient;
    friend QDataStream& operator<<(QDataStream &out, const QPL_ThermoQwt &d);
  friend QDataStream& operator>>(QDataStream &in, QPL_ThermoQwt(&d));
};
        QDataStream& operator<<(QDataStream &out, const QPL_ThermoQwt &d);
        QDataStream& operator>>(QDataStream &in, QPL_ThermoQwt(&d));

#endif // THERMO_QWT_H

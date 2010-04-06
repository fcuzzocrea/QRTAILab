#ifndef DIAL_QWT_H
#define DIAL_QWT_H

#include "qtplot_global.h"
#include <qwt_dial.h>
#include <qwt_dial_needle.h>

class  QPL_DialQwt : public QwtDial
{
   Q_OBJECT

public:
   QPL_DialQwt(QWidget *parent = 0);
   virtual ~QPL_DialQwt();
      void setMin(double);
   double getMin(){return Min;}
   void setMax(double);
    double getMax(){return Max;}
       void setNeedleColor(const QColor&);
   QColor getNeedleColor(){return needle->palette().button().color();}
    private:
      double Max,Min;
        QwtDialSimpleNeedle *needle;
            friend QDataStream& operator<<(QDataStream &out, const QPL_DialQwt &d);
  friend QDataStream& operator>>(QDataStream &in, QPL_DialQwt(&d));
};
        QDataStream& operator<<(QDataStream &out, const QPL_DialQwt &d);
        QDataStream& operator>>(QDataStream &in, QPL_DialQwt(&d));

#endif // DIAL_QWT_H

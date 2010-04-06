#ifndef LCD_QLABEL_H
#define LCD_QLABEL_H

#include "qtplot_global.h"
#include <QtGui>

class  QPL_LcdQLabel : public QLabel
{
   Q_OBJECT

public:
   QPL_LcdQLabel(QWidget *parent = 0);
   virtual ~QPL_LcdQLabel();
     void setLcdFont(const QFont& font);
   QFont getLcdFont(){return this->font();}
  void setLcdFormat(char c) ;
  char getLcdFormat() {return format;}
  void setLcdPrecision(int p);
   int getLcdPrecision(){return precision;}
   void setValue(double);
private:
     int precision;
  char format;
              friend QDataStream& operator<<(QDataStream &out, const QPL_LcdQLabel &d);
  friend QDataStream& operator>>(QDataStream &in, QPL_LcdQLabel(&d));
};
        QDataStream& operator<<(QDataStream &out, const QPL_LcdQLabel &d);
        QDataStream& operator>>(QDataStream &in, QPL_LcdQLabel(&d));

#endif // LCD_QLABEL_H

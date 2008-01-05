/*
 file:		led_window.h
 describtion:
   file for the classes QRL_LedWindow

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

#ifndef _LED_WINDOW_H
#define _LED_WINDOW_H 1

#include "qrtailab.h"


class QColor;

class  QLed : public QWidget
{
 Q_OBJECT
    Q_ENUMS( LedColor )
	Q_PROPERTY(bool value READ value WRITE setValue);
	Q_PROPERTY(LedColor color READ color WRITE setColor);

public: 
    enum LedColor {Red=7,Green=8,Blue=9,Yellow=12};
    QLed(QWidget *parent = 0);
    bool value() const { return m_value; }
    LedColor color() const { return m_color; }
public slots:
	void setValue(bool);
	void setColor(LedColor);
	void toggleValue();


protected:
    bool m_value;
    LedColor m_color;
    void paintEvent(QPaintEvent *event);

};
/**
 * @brief LedWindow
 * @todo  implement multiple leds
 */
class QRL_LedWindow : public QMdiSubWindow
{
   Q_OBJECT
public:
   QRL_LedWindow(QWidget *parent = 0);
   void setValue(unsigned int);
protected slots:
  void closeEvent ( QCloseEvent * event ){event->ignore(); this->hide(); }
private:
  float Value;
  QLed *Led;
  int num_leds;
  
};


#endif

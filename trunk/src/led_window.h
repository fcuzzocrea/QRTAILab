/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
 *                         P. Sereno                                       *
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
 file:		led_window.h
 describtion:
   file for the classes QRL_LedWindow, QLed;
*/
 
#ifndef _LED_WINDOW_H
#define _LED_WINDOW_H 1

#include <QtGui>

class QColor;

/**
 * @brief QLed
 */
class  QLed : public QWidget
{
 Q_OBJECT
	Q_PROPERTY(bool value READ value WRITE setValue);
	Q_PROPERTY(QColor color READ color WRITE setColor);

public: 
    QLed(QWidget *parent = 0);
    bool value() const { return m_value; }
    QColor color() const { return m_color; }
public slots:
	void setValue(bool);
	void setColor(QColor);
	void toggleValue();


protected:
    bool m_value;
    QColor m_color;
    void paintEvent(QPaintEvent *event);

};
/**
 * @brief LedWindow
 */
class QRL_LedWindow : public QMdiSubWindow
{
   Q_OBJECT
public:
   QRL_LedWindow(QWidget *parent = 0,int num=0,QString name="");
   ~QRL_LedWindow();
   void setValue(unsigned int);
   void setLedColor(QColor);
   QColor getLedColor(){return Leds[0]->color();}
protected slots:
  void closeEvent ( QCloseEvent * event );
private:
  float Value;
  QLed **Leds;
  QFrame * frame;
  int num_leds;
  QLabel **ledLabels;
  

friend QDataStream& operator<<(QDataStream &out, const QRL_LedWindow *d);
friend QDataStream& operator>>(QDataStream &in, QRL_LedWindow(*d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_LedWindow *d);
	QDataStream& operator>>(QDataStream &in, QRL_LedWindow(*d));


#endif

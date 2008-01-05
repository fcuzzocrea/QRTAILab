/*
 file:		led_window.cpp
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

#include "led_window.h"


QLed::QLed(QWidget *parent)
    : QWidget(parent)
{
   m_value=false;
   m_color=Red;
   setMinimumSize(QSize(50,50));
   //setFocusPolicy(Qt::StrongFocus);
   
}

void QLed::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setWindow( -500,-500,1000,1000);
	
	painter.setPen(Qt::white);
	painter.drawArc(-250,-250,500,500,0,5670);
	painter.drawArc(-333+1,-333,666,666,0,5670);
	painter.setPen(Qt::darkGray);
	painter.drawArc(-333-1,-333,666,666,3400,3000);
	
    if(m_value)
    {
      QRadialGradient radialGrad(QPointF(-80, -80), 200);
      radialGrad.setColorAt(0, Qt::white);
 
      QColor color;
      switch(m_color)
      {
      	 case Red: 
      	 color=QColor(Qt::red);
      	 break;
      	 case Green: 
      	 color=QColor(Qt::green);
      	 break;
      	 case Blue: 
      	 color=QColor(Qt::blue);
      	 break;
      	 case Yellow: 
      	 color=QColor(Qt::yellow);
      	 break;
      	 default: 
      	 color=QColor(Qt::red);
      	 break;      	 
      }
      
      radialGrad.setColorAt(1, color);
   	  QBrush brush(radialGrad);
      painter.setBrush(brush);
      painter.setPen(Qt::black);
	  painter.drawEllipse(-250,-250,500,500);
    }
    else
    {
      QRadialGradient radialGrad(QPointF(-80, -80), 200);
      radialGrad.setColorAt(0, Qt::white);
      radialGrad.setColorAt(1, Qt::lightGray);
   	  QBrush brush(radialGrad);
      painter.setBrush(brush);
      //painter.setPen(Qt::black);
	  painter.drawEllipse(-250,-250,500,500);
    }	
}

void QLed::setColor(LedColor newColor)
{
   m_color=newColor;
   update();
}


void QLed::setValue(bool value)
{
   m_value=value;
   update();
}


void QLed::toggleValue()
{ 
	m_value=!m_value;
	update();
	return; 
}


QRL_LedWindow::QRL_LedWindow(QWidget *parent)
	:QMdiSubWindow(parent)
{
	if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("QRL_LedWindow"));
    this->resize(122, 119);
    Led = new QLed(this);
    Led->setObjectName(QString::fromUtf8("Led"));
    Led->setGeometry(QRect(20, 20, 101, 23));
    Led->setValue(false);
    this->setWidget(Led);
    this->setWindowTitle(QApplication::translate("QRL_LedWindow", "Dialog", 0, QApplication::UnicodeUTF8));
   // Led->setText(QApplication::translate("QRL_LedWindow", "Led1", 0, QApplication::UnicodeUTF8));
    this->setWindowIcon(QIcon(QString::fromUtf8(":/icons/icons/led_icon.xpm")));
    this->setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint );
}



void QRL_LedWindow::setValue(unsigned int v)
{
	if (v)
		emit Led->setValue(true);
	else
		emit Led->setValue(false);
//         for (int i = 0; i < num_leds; i++) {
// 		if (Led_Mask & (1 << i)) {
// 			activate_led(i);
// 		} else {
// 			deactivate_led(i);
// 		}
// 	}
}

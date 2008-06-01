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
 file:		led_window.cpp
 describtion:
   file for the classes QRL_LedWindow QLed
*/

#include "led_window.h"


QLed::QLed(QWidget *parent)
    : QWidget(parent)
{
   m_value=false;
   m_color=Qt::red;
   setMinimumSize(QSize(20,20));
   //setFocusPolicy(Qt::StrongFocus);
   
}

void QLed::paintEvent(QPaintEvent *)
{


	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setWindow( -50,-50,100,100);
	
	painter.setPen(Qt::white);
	painter.drawArc(-25,-25,50,50,0,5670);
	painter.drawArc(-32,-33,66,66,0,5670);
	painter.setPen(Qt::darkGray);
	painter.drawArc(-34,-33,66,66,3400,3000);
	
    if(m_value)
    {
      QRadialGradient radialGrad(QPointF(-8, -8), 20);
      radialGrad.setColorAt(0, Qt::white);
       
      radialGrad.setColorAt(1, m_color);
   	  QBrush brush(radialGrad);
      painter.setBrush(brush);
      painter.setPen(Qt::black);
	  painter.drawEllipse(-25,-25,50,50);
    }
    else
    {
      QRadialGradient radialGrad(QPointF(-8, -8), 20);
      radialGrad.setColorAt(0, Qt::white);
      radialGrad.setColorAt(1, Qt::lightGray);
   	  QBrush brush(radialGrad);
      painter.setBrush(brush);
	  painter.drawEllipse(-25,-25,50,50);
    }	

}

void QLed::setColor(QColor newColor)
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
}


QRL_LedWindow::QRL_LedWindow(QWidget *parent,int num,QString name)
	:QMdiSubWindow(parent),num_leds(num)
{
	if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("QRL_LedWindow"));
    this->resize(122, 119);
	frame=new QFrame(this);
	frame->resize(122, 119);
   // layout=new QGridLayout(this);
   // this->setLayout(layout);
    Leds = new QLed* [num_leds];
    ledLabels = new QLabel* [num_leds];
    for(int i=0;i<num_leds;++i){
	Leds[i] = new QLed(frame);
	//Leds[i]->setObjectName(QString::fromUtf8("Led"));
	Leds[i]->setGeometry(QRect(10, 20+i*35, 30, 30));
	Leds[i]->setValue(false);

	ledLabels[i] = new QLabel(frame);
	ledLabels[i]->setGeometry(QRect(10+40, 20+i*35, 30, 30));
	ledLabels[i]->setText(tr("%1").arg(i+1));
	//layout->addWidget(Leds[i],i,1);
	//this->setWidget(Leds[i]);
    }
	this->setWidget(frame);
    this->setMinimumSize(30,50+num_leds*35+10);
    
    this->setWindowTitle(name);
   // Led->setText(QApplication::translate("QRL_LedWindow", "Led1", 0, QApplication::UnicodeUTF8));
    this->setWindowIcon(QIcon(QString::fromUtf8(":/icons/led_icon.xpm")));
    this->setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint );
}

QRL_LedWindow::~QRL_LedWindow(){
//Plotting_Scope_Data_Thread->stopThread();
//Plotting_Scope_Data_Thread->wait();
//delete Plotting_Scope_Data_Thread;
//delete mY;
for (unsigned int j=0;j<num_leds;j++){
	delete Leds[j];
}
delete[] Leds;
delete frame;
}

void QRL_LedWindow::setLedColor(QColor c)
{
	 for (int i = 0; i < num_leds; i++) {
		Leds[i]->setColor(c);
	}

}

void QRL_LedWindow::setValue(unsigned int v)
{
        for (int i = 0; i < num_leds; i++) {
		if (v & (1 << i)) {
			Leds[i]->setValue(true);
		} else {
			Leds[i]->setValue(false);
		}
	}
}

  void QRL_LedWindow::closeEvent ( QCloseEvent * event )
{
	event->ignore(); 
	this->hide(); 
}



QDataStream& operator<<(QDataStream &out, const QRL_LedWindow &d){
	out << d.Leds[0]->color() << d.size()  << d.pos() << d.isVisible();
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_LedWindow(&d)){
	QSize s;QPoint p;bool b; QColor c;
	in >> c; d.setLedColor(c);
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	return in;
}

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
 file:		meter_window.cpp
 describtion:
   file for the class QRL_MeterWindow
*/

#include "meter_window.h"
#include <stdlib.h>


QRL_MeterWindow::QRL_MeterWindow(QWidget *parent,QRL_MeterData *meter)
	:QMdiSubWindow(parent),Meter(meter)
{
if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("QRL_MeterWindow"));
    this->move(20,70);
    this->resize(50,261);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //sizePolicy.setHorizontalStretch(0);
    //sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
  //  this->setSizePolicy(sizePolicy);
    this->setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint  );
    //this->setWindowFlags(windowFlags() ^ Qt::WindowMinMaxButtonsHint  );

  //  this->setOption(RubberBandMove,true);
    this->setWindowIcon(QIcon(QString::fromUtf8(":/icons/meter_icon.xpm")));

//dockingwindow
//this->setAllowedAreas(Qt::NoDockWidgetArea);
//this->setFloating(true);
	Min=-1.;
	Max=1.;
	MeterType=THERMO;
	pipeWidth=14;

    Thermo = new QwtThermo(this);
    Thermo->setObjectName(QString::fromUtf8("Thermo"));
    Thermo->setGeometry(QRect(50, 20, 52, 261));
    Thermo->setRange(Min,Max);
  //  Thermo->setScale(Min,Max);
    Thermo->setPipeWidth(pipeWidth);
     Thermo->setAutoScale();
     Thermo->setScaleMaxMajor(5);
    Thermo->setScaleMaxMinor(10);
	pipeDistance=Thermo->minimumSizeHint().width()-Thermo->pipeWidth()-Thermo->borderWidth()*2;
	
    thermoColor2=Qt::black;
    thermoColor1=Qt::red;
     gradient=QLinearGradient(47, 0, 52, 0);
     gradient.setColorAt(0, thermoColor1);
     gradient.setColorAt(1, thermoColor2);
     gradient.setSpread(QGradient::ReflectSpread);

Thermo->setFillBrush(QBrush(gradient));

	alarmThermoColor1=Qt::blue;
	alarmThermoColor2=Qt::black;
	alarmGradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
     		alarmGradient.setColorAt(0, alarmThermoColor1);
     		alarmGradient.setColorAt(1, alarmThermoColor2);
     		alarmGradient.setSpread(QGradient::ReflectSpread);
		Thermo->setAlarmBrush(QBrush(alarmGradient));
	alarmLevel=1.;
	this->setWidget(Thermo);
	setPipeWith(pipeWidth);
	gradientEnabled=true;

 	Dial = new QwtDial(this);
        //Dial->setObjectName(QString::fromUtf8("Dial"));
   	Dial->setGeometry(QRect(50, 20, 52, 50));
	Dial->setScaleArc(40.,320.);
	Dial->setRange(Min,Max);
	needle = new QwtDialSimpleNeedle(
        QwtDialSimpleNeedle::Arrow, true, Qt::red, 
        QColor(Qt::gray).light(130));
	Dial->setNeedle(needle);
	Dial->scaleDraw()->setSpacing(2);
	//Dial->scaleDraw()->setRadius(5);
	//printf("radius %d\n",Dial->scaleDraw()->radius());
	Dial->setWrapping(false);
    	Dial->setReadOnly(true);
    	Dial->setScaleTicks(0, 4, 8);
	Dial->setScale(5,10);
	//Dial->setScale(1, 2, 0.25);
	Dial->setScaleOptions(QwtDial::ScaleTicks | QwtDial::ScaleLabel);
	Dial->setFrameShadow(QwtDial::Sunken);
	Dial->scaleDraw()->setPenWidth(2);
        //Dial->setLineWidth(1);
	Dial->hide();

/*	Dial = new QMeter(this);
	Dial->setStartAngle(230);
	Dial->setEndAngle(-40);
	Dial->setMinValue(Min);
	Dial->setMaxValue(Max);
	Dial->setPrecision(1);
	Dial->hide();*/

	/*Lcd = new QLCDNumber(this);
	Lcd->setSmallDecimalPoint(true);
	Lcd->setNumDigits(5);
	Lcd->setSegmentStyle(QLCDNumber::Flat);
	Lcd->hide();*/
	precision=4;
	format='f';
	Lcd = new QLabel(this);
	 QFont font("Helvetica", 15, QFont::DemiBold);
 	Lcd->setFont(font);
	Lcd->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
	Lcd->hide();
      
  

    this->setWindowTitle(Meter->getName());

}

QRL_MeterWindow::~QRL_MeterWindow()
{
// 	if (Typ=="Dial")
// 		delete Thermo;
// 	else
// 		delete Dial;

}
/**
* @brief changing the meter between thermo and dial
* @param metertype meter typ
*/

void QRL_MeterWindow::setMeter(Meter_Type metertype)
{
	MeterType=metertype;
	switch (MeterType){
	case DIAL:
		Thermo->hide();
		Lcd->hide();
		 this->setWidget(Dial);
		Dial->show();
		// delete Thermo;
		break;
	case THERMO:
    		//Thermo = new QwtThermo(this);
    		//Thermo->setObjectName(QString::fromUtf8("Thermo"));
    		//Thermo->setGeometry(QRect(50, 20, 52, 261));
		//Thermo->setScale(Min,Max);
		//Thermo->setFillColor(thermoColor);
		Dial->hide();
		Lcd->hide();
		this->setWidget(Thermo);
		Thermo->show();
		//pipeDistance=Thermo->pos().x();
		pipeDistance=Thermo->minimumSizeHint().width()-Thermo->pipeWidth()-Thermo->borderWidth()*2;
		//delete Dial;
		break;
	case LCD:
		Dial->hide();
		Thermo->hide();
		this->setWidget(Lcd);
		Lcd->show();
		break;
	default:
		break;
	}
}

/**
* @brief setting the actual value of the meter
* @param v meter value
*/
void QRL_MeterWindow::refresh()
{
	double v=Meter->getMeterValue();
	QString str;
	QLocale loc;
	switch (MeterType){
	case DIAL:
		Dial->setValue(v);
		break;
	case THERMO:
		Thermo->setValue(v);
		break;
	case LCD:
		//str.setNum(v,'g',4);
		str=loc.toString(v,format,precision);
		if (v >= 0)
			str.insert(0,QString(" "));
		Lcd->setText(str);
		break;
	default:
		break;
	}
}

void QRL_MeterWindow::changeRefreshRate(double rr)
{
	RefreshRate=rr;
}


/**
* @brief setting the maximal range
* @param max maximal range value
*/
void QRL_MeterWindow::setMax(double max)
{
	Max=max;
	Thermo->setMaxValue(Max);
	pipeDistance=Thermo->minimumSizeHint().width()-Thermo->pipeWidth()-Thermo->borderWidth()*2;
	setPipeWith(pipeWidth);
	Dial->setRange(Min,Max);
}
/*
void QRL_MeterWindow::setDistance(const QwtThermo* const t){

	int Start,End;
	QFont font;
	//t->scaleDraw()->getBorderDistHint(font,Start,End);;
	printf("BorderWith %d\n",t->borderWidth());
	printf("Pipewidth %d\n",t->pipeWidth());
	printf("Sizehint w %d h %d\n",t->sizeHint().width(),t->sizeHint().height());
}
*/

/**
* @brief setting the minimal range
* @param min minimal range value
*/
void QRL_MeterWindow::setMin(double min)
{
	Min=min;
	Thermo->setMinValue(Min);
	pipeDistance=Thermo->minimumSizeHint().width()-Thermo->pipeWidth()-Thermo->borderWidth()*2;
	setPipeWith(pipeWidth);
	Dial->setRange(Min,Max);
}

void QRL_MeterWindow::setThermoColor1(const QColor& c1)
{
	thermoColor1=c1;
	//Thermo->setFillColor(thermoColor1);
	
	  gradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
	if(gradientEnabled)
     		gradient.setColorAt(1, thermoColor2);
	else
		gradient.setColorAt(1, thermoColor1);
     gradient.setColorAt(0, thermoColor1);
     gradient.setSpread(QGradient::ReflectSpread);
	Thermo->setFillBrush(QBrush(gradient));
}


void QRL_MeterWindow::setThermoColor2(const QColor& c2)
{
	thermoColor2=c2;
	//Thermo->setFillColor(c2);
	
	  gradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
	if(gradientEnabled)
     		gradient.setColorAt(1, thermoColor2);
	else
		gradient.setColorAt(1, thermoColor1);
     gradient.setColorAt(0, thermoColor1);
     gradient.setSpread(QGradient::ReflectSpread);
	Thermo->setFillBrush(QBrush(gradient));
}

void QRL_MeterWindow::setPipeWith(double pipewidth)
{
	pipeWidth=(int)pipewidth;
	Thermo->setPipeWidth(pipeWidth);
	 this->resize(pipeDistance+pipeWidth+2*Thermo->borderWidth()+50,this->size().height());
	 gradient.setStart(QPointF(pipeDistance+pipeWidth/2,0));
	 gradient.setFinalStop(QPointF(pipeDistance+pipeWidth,0));
	Thermo->setFillBrush(QBrush(gradient));
	alarmGradient.setStart(QPointF(pipeDistance+pipeWidth/2,0));
	 alarmGradient.setFinalStop(QPointF(pipeDistance+pipeWidth,0));
	Thermo->setAlarmBrush(QBrush(alarmGradient));
}

void QRL_MeterWindow::setAlarmThermoColor1(const QColor& c1)
{
	alarmThermoColor1=c1;
	//Thermo->setFillColor(thermoColor1);
	  alarmGradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
	if(gradientEnabled)
     		     alarmGradient.setColorAt(1, alarmThermoColor2);
	else
		     alarmGradient.setColorAt(1, alarmThermoColor1);
     alarmGradient.setColorAt(0, alarmThermoColor1);
     alarmGradient.setSpread(QGradient::ReflectSpread);
	Thermo->setAlarmBrush(QBrush(alarmGradient));
}


void QRL_MeterWindow::setAlarmThermoColor2(const QColor& c2)
{
	alarmThermoColor2=c2;
	//Thermo->setFillColor(c2);
	  alarmGradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
	if(gradientEnabled)
     		     alarmGradient.setColorAt(1, alarmThermoColor2);
	else
		     alarmGradient.setColorAt(1, alarmThermoColor1);
     alarmGradient.setColorAt(0, alarmThermoColor1);
     alarmGradient.setSpread(QGradient::ReflectSpread);
	Thermo->setAlarmBrush(QBrush(alarmGradient));
}

void QRL_MeterWindow::setThermoAlarm(int state)
{
	if (state==Qt::Checked){
		Thermo->setAlarmEnabled(true);
		Thermo->setAlarmLevel(alarmLevel);
		alarmGradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
		if(gradientEnabled)
     		alarmGradient.setColorAt(1, alarmThermoColor2);
		else
		alarmGradient.setColorAt(1, alarmThermoColor1);
     		alarmGradient.setColorAt(0, alarmThermoColor1);
     		alarmGradient.setSpread(QGradient::ReflectSpread);
		Thermo->setAlarmBrush(QBrush(alarmGradient));
	} else 
		Thermo->setAlarmEnabled(false);

}

void QRL_MeterWindow::setThermoAlarm(bool b)
{
	if (b){
		Thermo->setAlarmEnabled(true);
		Thermo->setAlarmLevel(alarmLevel);
		alarmGradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
		if(gradientEnabled)
     		alarmGradient.setColorAt(1, alarmThermoColor2);
		else
		alarmGradient.setColorAt(1, alarmThermoColor1);
     		alarmGradient.setColorAt(0, alarmThermoColor1);
     		alarmGradient.setSpread(QGradient::ReflectSpread);
		Thermo->setAlarmBrush(QBrush(alarmGradient));
	} else 
		Thermo->setAlarmEnabled(false);

}



void QRL_MeterWindow::setGradientEnabled(bool b)
{
	gradientEnabled=b;
	if(!gradientEnabled){
		alarmGradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
     		alarmGradient.setColorAt(0, alarmThermoColor1);
     		alarmGradient.setColorAt(1, alarmThermoColor1);
     		alarmGradient.setSpread(QGradient::ReflectSpread);
		Thermo->setAlarmBrush(QBrush(alarmGradient));

		gradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
    		gradient.setColorAt(0, thermoColor1);
     		gradient.setColorAt(1, thermoColor1);
     		gradient.setSpread(QGradient::ReflectSpread);
		Thermo->setFillBrush(QBrush(gradient));
	} else {

		alarmGradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
     		alarmGradient.setColorAt(0, alarmThermoColor1);
     		alarmGradient.setColorAt(1, alarmThermoColor2);
     		alarmGradient.setSpread(QGradient::ReflectSpread);
		Thermo->setAlarmBrush(QBrush(alarmGradient));

		gradient=QLinearGradient(QPointF(pipeDistance+pipeWidth/2,0), QPointF(pipeDistance+pipeWidth,0));
    		gradient.setColorAt(0, thermoColor1);
     		gradient.setColorAt(1, thermoColor2);
     		gradient.setSpread(QGradient::ReflectSpread);
		Thermo->setFillBrush(QBrush(gradient));

	}
}

void  QRL_MeterWindow::setAlarmLevel(double level)
{
	alarmLevel=level;
	if (Thermo->alarmEnabled())
		Thermo->setAlarmLevel(alarmLevel);
}

void  QRL_MeterWindow::setThermoDirection(Qt::Orientation o)
{
	if(o==Qt::Vertical)
		Thermo->setOrientation(o, QwtThermo::LeftScale);
	else
		Thermo->setOrientation(o, QwtThermo::BottomScale);
}

   void QRL_MeterWindow::setLcdFont(const QFont& font){
	Lcd->setFont(font);

}
void QRL_MeterWindow::setLcdPrecision(int p) {
  precision=p;

}

void QRL_MeterWindow::setLcdFormat(char c) {
  switch(c){
  case 'e':	Lcd->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
     format=c;
    break;
  case 'f': 	Lcd->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
     format=c;
    break;
  case 'g':	Lcd->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
     format=c;
    break;
  }
}

   void QRL_MeterWindow::setNeedleColor(const QColor& c){
	QPalette p(c);
	// p.setColor(QPalette::Base,c); //knob
	needle->setPalette(p);
}



QDataStream& operator<<(QDataStream &out, const QRL_MeterWindow &d){

  	out << d.RefreshRate;
	out << d.gradientEnabled;
 	out << d.thermoColor1;
	out << d.thermoColor2;
	out << d.alarmThermoColor1;
	out << d.alarmThermoColor2;
 	out << (qint32)d.pipeWidth;
	out << d.Min << d.Max;
 	out << d.alarmLevel << d.Thermo->alarmEnabled();
   	//out << (QFont)d.Lcd->font() ;
 	out << (d.needle->palette().button().color());
 	out << (qint32)d.MeterType;
	out  << d.size()  << d.pos() << d.isVisible();
	out << (qint32)d.precision << (QChar)d.format;
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_MeterWindow(&d)){
	QSize s;QPoint p;bool b; QColor c; qint32 a;QFont f; double dd;
	QChar ch;

  	in >> dd; d.changeRefreshRate(dd);
 	in >> b; d.setGradientEnabled(b);
	in >> c; d.setThermoColor1(c);
	in >> c; d.setThermoColor2(c);
	in >> c; d.setAlarmThermoColor1(c);
	in >> c; d.setAlarmThermoColor2(c);
 	in >> a; d.setPipeWith(a);
	in >> dd; d.setMin(dd);
	in >> dd;  d.setMax(dd);
 	in >> dd;d.setAlarmLevel(dd);
 	in >> b; d.setThermoAlarm(b);
 	//in >> f; d.setLcdFont(f);
 	in >> c; d.setNeedleColor(c);
 	in >> a; d.setMeter((QRL_MeterWindow::Meter_Type)a);
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	if (d.fileVersion > 103){
	  in >> a; d.setLcdPrecision(a);
	  in >> ch; d.setLcdFormat(ch.toAscii());
	}
	return in;
}

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
 file:		scopes_trace.cpp
 describtion:
   file for the classes QRL_ScopeTrace
*/

#include "scope_trace.h"
#include <stdlib.h>

QRL_ScopeTrace::QRL_ScopeTrace(QwtPlot *parent, unsigned int maxdatapoints, int j){
		qwtPlot=parent;
		MaxDataPoints=maxdatapoints;
		index = j;
		d_x = new double[MaxDataPoints+1];
		d_y = new double[MaxDataPoints+1];

		for (unsigned int i = 0; i< MaxDataPoints; i++)
	      {
		  d_x[i] =0.;     // time axis
		  d_y[i] = 0.;
	      }
		cData = new QwtPlotCurve(QObject::tr("Trace %1").arg(j));
	 	cData->attach(qwtPlot);
		cData->setPaintAttribute(QwtPlotCurve::PaintFiltered,true);
		cData->setPaintAttribute(QwtPlotCurve::ClipPolygons,true);
		xmax=1;
		lineWidth=3;
		dy=1.;
		offset=0.;
		average=0.;
		min=0.;
		max=0.;
		PP=0.;
		RMS=0.;
		visible=true;
		labelCounter=0;
		switch(j){
			case 0:brush=QBrush(Qt::red);
				break;
			case 1:brush=QBrush(Qt::green);
				break;
			case 2:brush=QBrush(Qt::yellow);
				break;
			case 3:brush=QBrush(Qt::blue);
				break;
			case 4:brush=QBrush(Qt::black);
				break;
			case 5:brush=QBrush(Qt::magenta);
				break;
			case 6:brush=QBrush(Qt::darkRed);
				break;
			case 7:brush=QBrush(Qt::darkGreen);
				break;
			case 8:brush=QBrush(Qt::darkMagenta);
				break;
			case 9:brush=QBrush(Qt::darkYellow);
				break;
			case 10:brush=QBrush(Qt::cyan);
				break;
			case 11:brush=QBrush(Qt::darkCyan);
				break;
			case 12:brush=QBrush(Qt::gray);
				break;
			case 13:brush=QBrush(Qt::darkGray);
				break;
			case 14:brush=QBrush(Qt::lightGray);
				break;
		}

		QPen pen;
       		pen.setWidth(lineWidth);
		pen.setBrush(brush);
		cData->setPen(pen);
		//cData->setRawData(d_x, d_y, NDataSoll);


		time=0;
	


		zeroAxis.setLabel(QObject::tr("%1").arg(j));    
		zeroAxis.setLabelAlignment(Qt::AlignLeft|Qt::AlignTop);
    		zeroAxis.setLineStyle(QwtPlotMarker::HLine);
    		//zeroAxis.setYValue(offset/dy);
		zeroAxis.setYValue(offset);
    		zeroAxis.setLinePen(QPen(brush,2.,Qt::DashDotLine));
    		zeroAxis.attach(qwtPlot);
		zeroAxis.hide();

		traceName=QObject::tr("Trace %1").arg(j);
		QwtText ttext(traceName);
   		 ttext.setColor(QColor(brush.color()));
    		traceLabel.setLabel(ttext);
		traceLabel.setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    		traceLabel.setLineStyle(QwtPlotMarker::NoLine);
    		//zeroAxis.setYValue(offset/dy);
		traceLabel.setXValue(0.+j*xmax/5.);
		traceLabel.setYValue(4.);
		
    		traceLabel.setLinePen(QPen(brush,2.,Qt::DashDotLine));
    		traceLabel.attach(qwtPlot);
		traceLabel.hide();

		QwtText unitText(QObject::tr("%1").arg(0.));
   		 unitText.setColor(QColor(gridColor));
    		unitLabel.setLabel(unitText);
		unitLabel.setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    		unitLabel.setLineStyle(QwtPlotMarker::NoLine);
    		//zeroAxis.setYValue(offset/dy);
		unitLabel.setXValue(0.+j*xmax/5.);
		unitLabel.setYValue(3.5);
		
    		unitLabel.setLinePen(QPen(brush,2.,Qt::DashDotLine));
    		unitLabel.attach(qwtPlot);
		unitLabel.hide();


		QwtText atext(QObject::tr("%1").arg(0.));
   		 atext.setColor(QColor(gridColor));
    		averageLabel.setLabel(atext);
		averageLabel.setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    		averageLabel.setLineStyle(QwtPlotMarker::NoLine);
    		//zeroAxis.setYValue(offset/dy);
		averageLabel.setXValue(0.+j*xmax/5.);
		averageLabel.setYValue(3.5);
		
    		averageLabel.setLinePen(QPen(brush,2.,Qt::DashDotLine));
    		averageLabel.attach(qwtPlot);
		averageLabel.hide();
		
		QwtText minText(QObject::tr("%1").arg(0.));
   		 minText.setColor(QColor(gridColor));
    		minLabel.setLabel(minText);
		minLabel.setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    		minLabel.setLineStyle(QwtPlotMarker::NoLine);
    		//zeroAxis.setYValue(offset/dy);
		minLabel.setXValue(0.+j*xmax/5.);
		minLabel.setYValue(3.5);
		
    		minLabel.setLinePen(QPen(brush,2.,Qt::DashDotLine));
    		minLabel.attach(qwtPlot);
		minLabel.hide();

		QwtText maxText(QObject::tr("%1").arg(0.));
   		 maxText.setColor(QColor(gridColor));
    		maxLabel.setLabel(maxText);
		maxLabel.setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    		maxLabel.setLineStyle(QwtPlotMarker::NoLine);
    		//zeroAxis.setYValue(offset/dy);
		maxLabel.setXValue(0.+j*xmax/5.);
		maxLabel.setYValue(3.5);
		
    		maxLabel.setLinePen(QPen(brush,2.,Qt::DashDotLine));
    		maxLabel.attach(qwtPlot);
		maxLabel.hide();

		QwtText ppText(QObject::tr("%1").arg(0.));
   		 ppText.setColor(QColor(gridColor));
    		ppLabel.setLabel(ppText);
		ppLabel.setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    		ppLabel.setLineStyle(QwtPlotMarker::NoLine);
    		//zeroAxis.setYValue(offset/dy);
		ppLabel.setXValue(0.+j*xmax/5.);
		ppLabel.setYValue(3.5);
		
    		ppLabel.setLinePen(QPen(brush,2.,Qt::DashDotLine));
    		ppLabel.attach(qwtPlot);
		ppLabel.hide();

		QwtText rmsText(QObject::tr("%1").arg(0.));
   		 rmsText.setColor(QColor(gridColor));
    		rmsLabel.setLabel(rmsText);
		rmsLabel.setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    		rmsLabel.setLineStyle(QwtPlotMarker::NoLine);
    		//zeroAxis.setYValue(offset/dy);
		rmsLabel.setXValue(0.+j*xmax/5.);
		rmsLabel.setYValue(3.5);
		
    		rmsLabel.setLinePen(QPen(brush,2.,Qt::DashDotLine));
    		rmsLabel.attach(qwtPlot);
		rmsLabel.hide();




}

QRL_ScopeTrace::~QRL_ScopeTrace(){
//Plotting_Scope_Data_Thread->stopThread();
//Plotting_Scope_Data_Thread->wait();
//delete Plotting_Scope_Data_Thread;
//delete mY;
  delete[] d_x;
  delete[] d_y;
  delete cData;

}


void QRL_ScopeTrace::refresh()
{


if ( isLabelVisible(lt_average)){
	average=0.;
	for (int k=0;k<NDataSoll;k++)
		average+=(d_y[k]-offset)*dy/((double)NDataSoll);

	QString astr;
	astr.setNum(average,'f',3);
	//QwtText atext(QObject::tr("Avg: %1").arg(average));
	QwtText atext(QObject::tr("Avg: ")+astr);
	atext.setColor(QColor(gridColor));
	averageLabel.setLabel(atext);
}
if ( isLabelVisible(lt_unit)){
	QString astr;
	astr.setNum(dy,'f',3);
	//QwtText atext(QObject::tr("Avg: %1").arg(average));
	QwtText atext(QObject::tr("U/d: ")+astr);
	atext.setColor(QColor(gridColor));
	unitLabel.setLabel(atext);
}
if ( isLabelVisible(lt_min)){
	min=(d_y[0]-offset)*dy;
	for (int k=0;k<NDataSoll;k++)
		if (min>(d_y[k]-offset)*dy)
			min=(d_y[k]-offset)*dy;

	QString astr;
	astr.setNum(min,'f',3);
	QwtText atext(QObject::tr("Min: ")+astr);
   	atext.setColor(QColor(gridColor));
    	minLabel.setLabel(atext);
}
if ( isLabelVisible(lt_max)){
	max=(d_y[0]-offset)*dy;
	for (int k=0;k<NDataSoll;k++)
		if (max<(d_y[k]-offset)*dy)
			max=(d_y[k]-offset)*dy;

	QString astr;
	astr.setNum(max,'f',3);
	QwtText atext(QObject::tr("Max: ")+astr);
   	atext.setColor(QColor(gridColor));
    	maxLabel.setLabel(atext);
}
if ( isLabelVisible(lt_pp)){
	min=(d_y[0]-offset)*dy;
	max=(d_y[0]-offset)*dy;
	for (int k=0;k<NDataSoll;k++){
		if (max<(d_y[k]-offset)*dy)
			max=(d_y[k]-offset)*dy;
		if (min>(d_y[k]-offset)*dy)
			min=(d_y[k]-offset)*dy;
	}
	PP=max-min;
	QString astr;
	astr.setNum(PP,'f',3);
	QwtText atext(QObject::tr("PP: ")+astr);
   	atext.setColor(QColor(gridColor));
    	ppLabel.setLabel(atext);
}
if ( isLabelVisible(lt_rms)){
	RMS=0.;
	for (int k=0;k<NDataSoll;k++)
		RMS+=(d_y[k]-offset)*dy*(d_y[k]-offset)*dy;
	RMS=sqrt(RMS/((double)NDataSoll));
	QString astr;
	astr.setNum(RMS,'f',3);
	QwtText atext(QObject::tr("RMS: ")+astr);
	atext.setColor(QColor(gridColor));
	rmsLabel.setLabel(atext);
}
}







   void QRL_ScopeTrace::changeNDataSoll(int ds, double dt)
{
	NDataSoll=(int)ds;

    for (unsigned int i = 0; i< NDataSoll; i++)
    {
	//if (Scope->dt<=0.05)
        d_x[i] =dt * i;     // time axis
	    d_y[i] = offset;
    }
  	cData->setRawData(d_x, d_y, NDataSoll);
}



void QRL_ScopeTrace::show(bool v){

	visible=v;
	if (!visible) {
		cData->setStyle(QwtPlotCurve::NoCurve);
	}else{	
		cData->setStyle(QwtPlotCurve::Lines);
	}
}

void QRL_ScopeTrace::setColor(const QColor& c)
{
		QPen pen;
		pen.setBrush(c);
		brush=QBrush(c);
		pen.setWidth(lineWidth);
		cData->setPen(pen);
		zeroAxis.setLinePen(QPen(brush,2.,Qt::DashDotLine));
		traceLabel.setLinePen(QPen(brush,2.,Qt::DashDotLine));
		QwtText ttext(traceName);
		ttext.setColor(QColor(c));
		traceLabel.setLabel(ttext);
}

void QRL_ScopeTrace::setWidth(int traceWidth)
{
		QPen pen;
		pen.setBrush(brush);
		pen.setWidth(traceWidth);
		lineWidth=traceWidth;
		cData->setPen(pen);
}

int  QRL_ScopeTrace::getWidth()
{
		return cData->pen().width();
}

   void QRL_ScopeTrace::setOffset(double o)
{
		//QRL_ScopeTrace[trace].offset=o;
   		 for (unsigned int i = 0; i< NDataSoll; i++)
  		  {
			d_y[i]=(((d_y[i]-offset)*dy))/dy+o;
			
		    }
	//cData[trace]->setRawData(d_x, ScopeData[trace].d_y, NDataSoll);
		offset=o;
		zeroAxis.setYValue(offset);
     

}

double  QRL_ScopeTrace::getOffset()
{
		return offset;
}
   void QRL_ScopeTrace::setDy(double d){

   		 for (unsigned int i = 0; i< NDataSoll; i++)
  		  {
			d_y[i]=(((d_y[i]-offset)*dy)/d+offset);
			
		    }
		dy=d;
		zeroAxis.setYValue(offset);
	

}

double  QRL_ScopeTrace::getDy()
{
		return dy;
}



   void QRL_ScopeTrace::showLabel(labelTypes lt){

  switch(lt){
    case lt_trace:traceLabel.show();
		  break;
    case lt_unit:
		unitLabel.setYValue(3.5-(0.5*labelCounter));
		labelCounter++;
		unitLabel.show();
		break;
  case lt_average:
	      averageLabel.setYValue(3.5-(0.5*labelCounter));
		labelCounter++;
		averageLabel.show();
	      break;
  case lt_min:
	      minLabel.setYValue(3.5-(0.5*labelCounter));
		labelCounter++;
		minLabel.show();
	      break;
   case lt_max:
	         maxLabel.setYValue(3.5-(0.5*labelCounter));
		labelCounter++;
		maxLabel.show();
		break;
  case lt_pp:
		ppLabel.setYValue(3.5-(0.5*labelCounter));
		labelCounter++;
		ppLabel.show();
		break;
  case lt_rms:
		rmsLabel.setYValue(3.5-(0.5*labelCounter));
		labelCounter++;
		rmsLabel.show();
		break;
}
		

}






   void QRL_ScopeTrace::hideLabel(labelTypes lt){




  switch(lt){
    case lt_trace:traceLabel.hide();
		  break;
    case lt_unit:
		labelCounter=0;
		unitLabel.hide();
		break;
  case lt_average:
		labelCounter=0;
		averageLabel.hide();
	      break;
  case lt_min:
		labelCounter=0;
		minLabel.hide();
	      break;
   case lt_max:
		labelCounter=0;
		maxLabel.hide();
		break;
  case lt_pp:
		labelCounter=0;
		ppLabel.hide();
		break;
  case lt_rms:
		labelCounter=0;
		rmsLabel.hide();
		break;
}



}
   bool QRL_ScopeTrace::isLabelVisible(labelTypes lt){
bool isvisible=false;
  switch(lt){
    case lt_trace:isvisible=traceLabel.isVisible();
		  break;
    case lt_unit:
		isvisible=unitLabel.isVisible();
		break;
  case lt_average:
		isvisible=averageLabel.isVisible();
	      break;
  case lt_min:
		isvisible=minLabel.isVisible();
	      break;
   case lt_max:
		isvisible=maxLabel.isVisible();
		break;
  case lt_pp:
		isvisible=ppLabel.isVisible();
		break;
  case lt_rms:
		isvisible=rmsLabel.isVisible();
		break;
}
return isvisible;




}
   void QRL_ScopeTrace::setLabelVisible(labelTypes lt, bool setvisible){
  switch(lt){
    case lt_trace:
		  traceLabel.setVisible(setvisible);
		  break;
    case lt_unit:
		unitLabel.setVisible(setvisible);
		break;
  case lt_average:
		averageLabel.setVisible(setvisible);
	      break;
  case lt_min:
		minLabel.setVisible(setvisible);
	      break;
   case lt_max:
		maxLabel.setVisible(setvisible);
		break;
  case lt_pp:
		ppLabel.setVisible(setvisible);
		break;
  case lt_rms:
		rmsLabel.setVisible(setvisible);
		break;
}




}






  void QRL_ScopeTrace::setLabelsXValue(double x){

		traceLabel.setXValue(x);
		unitLabel.setXValue(x);
		averageLabel.setXValue(x);
		minLabel.setXValue(x);
		maxLabel.setXValue(x);
		ppLabel.setXValue(x);
		rmsLabel.setXValue(x);

}

void QRL_ScopeTrace::setZeroAxis(bool b){

if (b)
zeroAxis.show();
else
zeroAxis.hide();




}



void QRL_ScopeTrace::setName(const QString &text){

	
	traceName=QString(text);
	
	QwtText ttext(traceName);
   	ttext.setColor(QColor(brush.color()));
    	traceLabel.setLabel(ttext);




}


   void QRL_ScopeTrace::setValue(int index, double v){
   
   
   d_y[index]=(v)/getDy()+getOffset();
  
   
   }

   void QRL_ScopeTrace::moveDataToRight(int time){
   
   
   		for (unsigned int i = NDataSoll - 1; i > time; i-- ){
        		d_y[i] = d_y[i-1-time];
		}
   
   
   }
   void QRL_ScopeTrace::moveDataToLeft(int time){
   
   		for (unsigned int i = 0; i < NDataSoll-time-1; i++ ){
		if ((i+time+1)<NDataSoll)
			d_y[i] = d_y[i+time+1];
		}
   }

QDataStream& operator<<(QDataStream &out, const QRL_ScopeTrace *d){
        qint32 a;
		out << d->traceName;
		out << d->zeroAxis.isVisible();
		out << d->averageLabel.isVisible();
		out << d->unitLabel.isVisible();
		out << d->minLabel.isVisible();
		out << d->maxLabel.isVisible();
		out << d->ppLabel.isVisible();
		out << d->rmsLabel.isVisible();

		out << d->traceLabel.isVisible();

		out << d->offset;
		out << d->dy;
		out << d->brush.color();
		a=d->lineWidth; out <<a;
		out << d->visible;

	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_ScopeTrace(*d)){
	QSize s;QPoint p;bool b; QColor c; qint32 a;QFont f; double dd;
	QString str; 

	
// 		in >> str; d.setName( str);
// 		in >> b; d.setZeroAxis(b);
// 		in >> b; d.averageLabel.isVisible()(b);
// 		in >> b; d.setUnitLabel(b);
// 		in >> b; d.setMinLabel(b);
// 		in >> b; d.setMaxLabel(b);
// 		in >> b; d.setPPLabel(b);
// 		in >> b; d.setRMSLabel(b);
// 		in >> b; d.setLabel(b);
// 		in >> dd; d.setOffset(dd);
// 		in >> dd; d.setDy(dd);
// 		in >> c; d.setColor(c);
// 		in >> a; d.setWidth((int)a);
// 		in >> b; d.show(b);
		in >> str;
		in >> b; 
		in >> b; 
		in >> b; 
		in >> b; 
		in >> b; 
		in >> b; 
		in >> b; 
		in >> b; 
		in >> dd; 
		in >> dd; 
		in >> c;
		in >> a;
		in >> b;




	return in;
}

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
   file for the classes TraceOptions
*/

#include "scope_trace.h"
#include <stdlib.h>

TraceOptions::TraceOptions(QwtPlot *parent, unsigned int maxdatapoints, int j){
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
		cData = new QwtPlotCurve(Scope->name);
	 	cData->attach(qwtPlot);
		cData->setPaintAttribute(QwtPlotCurve::PaintFiltered,true);
		cData->setPaintAttribute(QwtPlotCurve::ClipPolygons,true);

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
	


		zeroAxis.setLabel(tr("%1").arg(j));    
		zeroAxis.setLabelAlignment(Qt::AlignLeft|Qt::AlignTop);
    		zeroAxis.setLineStyle(QwtPlotMarker::HLine);
    		//zeroAxis.setYValue(offset/dy);
		zeroAxis.setYValue(offset);
    		zeroAxis.setLinePen(QPen(brush,2.,Qt::DashDotLine));
    		zeroAxis.attach(qwtPlot);
		zeroAxis.hide();

		traceName=tr("Trace %1").arg(j);
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

		QwtText unitText(tr("%1").arg(0.));
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


		QwtText atext(tr("%1").arg(0.));
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
		
		QwtText minText(tr("%1").arg(0.));
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

		QwtText maxText(tr("%1").arg(0.));
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

		QwtText ppText(tr("%1").arg(0.));
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

		QwtText rmsText(tr("%1").arg(0.));
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

TraceOptions::~TraceOptions(){
//Plotting_Scope_Data_Thread->stopThread();
//Plotting_Scope_Data_Thread->wait();
//delete Plotting_Scope_Data_Thread;
//delete mY;
  delete[] d_x;
  delete[] d_y;
  delete cData;

}


void TraceOptions::refresh()
{

if ( getAverageLabel()){
	average=0.;
	for (int k=0;k<NDataSoll;k++)
		average+=(ScopeData[nn].d_y[k]-offset)*dy/((double)NDataSoll);

	QString astr;
	astr.setNum(average,'f',3);
	//QwtText atext(tr("Avg: %1").arg(average));
	QwtText atext(tr("Avg: ")+astr);
	atext.setColor(QColor(gridColor));
	averageLabel.setLabel(atext);
}
if ( getUnitLabel()){
	QString astr;
	astr.setNum(dy,'f',3);
	//QwtText atext(tr("Avg: %1").arg(average));
	QwtText atext(tr("U/d: ")+astr);
	atext.setColor(QColor(gridColor));
	unitLabel.setLabel(atext);
}
if ( getMinLabel()){
	min=(d_y[0]-offset)*dy;
	for (int k=0;k<NDataSoll;k++)
		if (min>(d_y[k]-offset)*dy)
			min=(d_y[k]-offset)*dy;

	QString astr;
	astr.setNum(min,'f',3);
	QwtText atext(tr("Min: ")+astr);
   	atext.setColor(QColor(gridColor));
    	minLabel.setLabel(atext);
}
if ( getMaxLabel()){
	max=(d_y[0]-offset)*dy;
	for (int k=0;k<NDataSoll;k++)
		if (max<(d_y[k]-offset)*dy)
			max=(d_y[k]-offset)*dy;

	QString astr;
	astr.setNum(max,'f',3);
	QwtText atext(tr("Max: ")+astr);
   	atext.setColor(QColor(gridColor));
    	maxLabel.setLabel(atext);
}
if ( getPPLabel()){
	min=(d_y[0]-offset)*dy;
	max=(d_y[0]-offset)*dy;
	for (int k=0;k<NDataSoll;k++){
		if (max<(.d_y[k]-offset)*dy)
			max=(d_y[k]-offset)*dy;
		if (min>(d_y[k]-offset)*dy)
			min=(d_y[k]-offset)*dy;
	}
	PP=max-min;
	QString astr;
	astr.setNum(PP,'f',3);
	QwtText atext(tr("PP: ")+astr);
   	atext.setColor(QColor(gridColor));
    	ppLabel.setLabel(atext);
}
if ( getRMSLabel()){
	RMS=0.;
	for (int k=0;k<NDataSoll;k++)
		RMS+=(d_y[k]-offset)*dy*(d_y[k]-offset)*dy;
	RMS=sqrt(RMS/((double)NDataSoll));
	QString astr;
	astr.setNum(RMS,'f',3);
	QwtText atext(tr("RMS: ")+astr);
	atext.setColor(QColor(gridColor));
	rmsLabel.setLabel(atext);
}
}
}






   void TraceOptions::changeNDataSoll(int ds, int dt)
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



void TraceOptions::show(bool v){

	visible=v;
	if (!visible) {
		cData->setStyle(QwtPlotCurve::NoCurve);
	}else{	
		cData->setStyle(QwtPlotCurve::Lines);
	}
}

void TraceOptions::setColor(const QColor& c)
{
		QPen pen;
		pen.setBrush(c);
		brush=QBrush(c);
		pen.setWidth(lineWidth);
		cData->setPen(pen);
		zeroAxis.setLinePen(QPen(brush,2.,Qt::DashDotLine));
		traceLabel.setLinePen(QPen(brush,2.,Qt::DashDotLine));
}

void TraceOptions::setWidth(int traceWidth)
{
		QPen pen;
		pen.setBrush(brush);
		pen.setWidth(traceWidth);
		lineWidth=traceWidth;
		cData->setPen(pen);
}

int  TraceOptions::getWidth()
{
		return cData->pen().width();
}

   void TraceOptions::setOffset(double o)
{
		//TraceOptions[trace].offset=o;
   		 for (unsigned int i = 0; i< NDataSoll; i++)
  		  {
			d_y[i]=(((d_y[i]-offset)*dy))/dy+o;
			
		    }
	//cData[trace]->setRawData(d_x, ScopeData[trace].d_y, NDataSoll);
		offset=o;
		zeroAxis.setYValue(offset);
     

}

double  TraceOptions::getOffset()
{
		return offset;
}
   void TraceOptions::setDy(double d){

   		 for (unsigned int i = 0; i< NDataSoll; i++)
  		  {
			d_y[i]=(((d_y[i]-offset)*dy)/d+offset);
			
		    }
		dy=d;
		zeroAxis.setYValue(offset);
	

}

double  TraceOptions::getDy()
{
		return dy;
}



void QRL_ScopeWindow::setZeroAxis(bool b){

if (b)
zeroAxis.show();
else
zeroAxis.hide();




}

void QRL_ScopeWindow::setTraceLabel(bool b){

	
}

void QRL_ScopeWindow::setUnitLabel(bool b){




}


void QRL_ScopeWindow::setAverageLabel(bool b){


	

}

void QRL_ScopeWindow::setMinLabel(bool b){

	
}

void QRL_ScopeWindow::setMaxLabel(bool b){

	

}

void QRL_ScopeWindow::setPPLabel(bool b){

	

}

void QRL_ScopeWindow::setRMSLabel(bool b){


}

void QRL_ScopeWindow::setTraceName(const QString &text){

	
	traceName=QString(text);
	
	QwtText ttext(traceName);
   	ttext.setColor(QColor(brush.color()));
    	traceLabel.setLabel(ttext);




}




QDataStream& operator<<(QDataStream &out, const TraceOptions &d){
        qint32 a;
		out << d.traceName;
		out << d.zeroAxis.isVisible();
		out << d.averageLabel.isVisible();
		out << d.unitLabel.isVisible();
		out << d.minLabel.isVisible();
		out << d.maxLabel.isVisible();
		out << d.ppLabel.isVisible();
		out << d.rmsLabel.isVisible();

		out << d.traceLabel.isVisible();

		out << d.offset;
		out << d.dy;
		out << d.brush.color();
		a=d.lineWidth; out <<a;
		out << d.visible;

	return out;
}


QDataStream& operator>>(QDataStream &in, TraceOptions(&d)){
	QSize s;QPoint p;bool b; QColor c; qint32 a,a2;QFont f; double dd;
	QString str; int Ncurve;

	
		in >> str; d.setTraceName( str);
		in >> b; d.setZeroAxis(b);
		in >> b; d.setAverageLabel(b);
		in >> b; d.setUnitLabel(b);
		in >> b; d.setMinLabel(b);
		in >> b; d.setMaxLabel(b);
		in >> b; d.setPPLabel(b);
		in >> b; d.setRMSLabel(b);
		in >> b; d.setTraceLabel(b);
		in >> dd; d.setTraceOffset(dd);
		in >> dd; d.setTraceDy(dd);
		in >> c; d.setTraceColor(c);
		in >> a; d.setTraceWidth((int)a);
		in >> b; d.showTrace(b);




	return in;
}

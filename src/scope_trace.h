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
 file:		scopes_trace.h
 describtion:
   file for the classes TraceOptions
*/
#ifndef _SCOPE_TRACE_H
#define _SCOPE_TRACE_H 1

#include <QtGui> 

#include <qwt_plot.h>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_math.h>
#include "qrtailab.h"



/**
 * @brief Display Trace
 */

class TraceOptions
{
   
 //friend class PlottingScopeDataThread;
public:
  TraceOptions(QwtPlot *parent, unsigned int maxdatapoints, int j);
~TraceOptions();
    void refresh();
  void changeNDataSoll(int ds, int dt);
   QColor getColor(int trace){return  brush.color();}
   void setColor(const QColor&);
   void setWidth(int);
   int getWidth();
   void setOffset(double);
   double getOffset();
   void setTraceDy(double);
   double getTraceDy();
   void show(bool);
   bool isVisible(){return  visible;}
   void setZeroAxis(bool b);
   void setTraceLabel(bool b);
   void setUnitLabel(bool b);
   void setAverageLabel(bool b);
   void setMinLabel(bool b);
   void setMaxLabel(bool b);
   void setPPLabel(bool b);
   void setRMSLabel(bool b);
 bool getLabel(){return traceLabel.isVisible();}
 bool getUnitLabel(){return unitLabel.isVisible();}
 bool getAverageLabel(){return averageLabel.isVisible();}
 bool getMinLabel(){return minLabel.isVisible();}
 bool getMaxLabel(){return maxLabel.isVisible();}
 bool getPPLabel(){return ppLabel.isVisible();}
 bool getRMSLabel(){return rmsLabel.isVisible();}
   bool getZeroAxis(){return zeroAxis.isVisible();}
   //PlottingScopeDataThread* getThread(){return Plotting_Scope_Data_Thread;}
   void setName(const QString &text);
   QString getName(){return traceName;}

private:
	QwtPlot *qwtPlot;
 	int index;
	double offset;
	double dy;
	int lineWidth;
	QBrush brush;
  	QwtPlotMarker zeroAxis;
	double average;
	double min,max;
	double PP,RMS;
	QwtPlotMarker traceLabel;
	QwtPlotMarker unitLabel;
	QwtPlotMarker averageLabel;
	QwtPlotMarker minLabel;
	QwtPlotMarker maxLabel;
	QwtPlotMarker ppLabel;
	QwtPlotMarker rmsLabel;
	QString traceName;
	int labelCounter;
	bool visible;
//data
  unsigned int  MaxDataPoints,NDataSoll;
  double *d_x; 
	int dt;
	double * d_y;
	double * d_yempty;
	int time;
	int yt;
//curve
  QwtPlotCurve *cData;
  //PlottingScopeDataThread* Plotting_Scope_Data_Thread;
   friend QDataStream& operator<<(QDataStream &out, const TraceOptions &d);
   friend QDataStream& operator>>(QDataStream &in, TraceOptions(&d));
};
	QDataStream& operator<<(QDataStream &out, const TraceOptions &d);
	QDataStream& operator>>(QDataStream &in, TraceOptions(&d));


#endif

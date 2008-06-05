/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
 *                                                                         *
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
 file:		scopes_window.h
 describtion:
   file for the classes QRL_ScopeWindow
*/
#ifndef _SCOPE_WINDOW_H
#define _SCOPE_WINDOW_H 1

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

typedef struct Scopes_Data_Struct Scopes_Data_T;
struct Scopes_Data_Struct
{
	double * d_y;
	double * d_yempty;
	double* d_yt;
	double* d_yt2;
	int time;
	int yt;
};

typedef struct Trace_Options_Struct Trace_Options_T;
struct Trace_Options_Struct
{
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
};


// class QRL_ScopeWindow;
// 
// /**
//  * @brief QThread for plotting Scope data
//  */
// 
// class PlottingScopeDataThread : public QThread
//  {
//  Q_OBJECT
//  public:
//     void run();
//     void stopThread();
//     void setValue(int,float);
//     void changeRefreshRate(double rr);
//  public slots:
//     void start(QRL_ScopeWindow* scopewindow);
//     void refresh();
//  private:
//     QRL_ScopeWindow* ScopeWindow;
//     int isRunning;
//     QTimer *timer;
//     Scopes_Data_T *ScopeData;
//    QWaitCondition DataReady;
//    QMutex dataMutex;
//  };

/**
 * @brief Display Scope
 * @todo add legend
 * @todo add curser 
 * @todo add parameter labels 
 * @todo add trigger label
* @bug   continuous rolling: a to smal sec/dev value consumes to much cpu power and blocks the gui
 * @bug overwride: for more then one trace the beginning all traces can get different. -> error in time axis
 */

class QRL_ScopeWindow : public QMdiSubWindow
{
   Q_OBJECT
 //friend class PlottingScopeDataThread;
public:
   enum PlottingMode {roll,overwrite,trigger,hold};
   QRL_ScopeWindow(QWidget *parent = 0,qrl_types::Target_Scopes_T *scope=0,int ind=0);
   ~QRL_ScopeWindow();
   void changeRefreshRate(double);
   void changeDataPoints(double);
   void changeDX(double);
   double getDt(){return dt;}
   double getRefreshRate(){return RefreshRate;}
   int getDataPoints(){return NDataSoll;}
   double getDX(){return dx;}
   void setGridColor(QColor);
   QColor getGridColor(){return gridColor;}
   void setBgColor(QColor);
   QColor getBgColor(){return bgColor;}
   QColor getTraceColor(int trace){return  TraceOptions[trace].brush.color();}
   void setTraceColor(const QColor&,int);
   void setTraceWidth(int ,int );
   int getTraceWidth(int);
   void setTraceOffset(double,int);
   double getTraceOffset(int);
   void setTraceDy(double,int);
   double getTraceDy(int);
   void setSaveTime(double t) {saveTime=t;}
   double getSaveTime(){return saveTime;}
   void setFileName(QString str) {fileName=str;}
   QString getFileName() {return fileName;}
   void showTrace(bool,int);
   bool isTraceVisible(int trace){return  TraceOptions[trace].visible;}
   void setPlottingMode(PlottingMode p);
   void setPlottingDirection(Qt::LayoutDirection d);
   int getPlottingMode() {return (int)plottingMode ;}
   Qt::LayoutDirection getPlottingDirection() {return direction;}
   void setTriggerUpDirection(bool b){triggerUp=b;}
   bool getTriggerUpDirection(){return triggerUp;}
   void manualTriggerSignal(){triggerSearch=false;}
   void setTriggerLevel(double l){triggerLevel=l;}
   double getTriggerLevel(){return triggerLevel;}
   void setTriggerChannel(int trace){if (trace<Ncurve) triggerChannel=trace;}
   int getTriggerChannel(){return  triggerChannel;}
   void setSingleMode(bool b){singleMode=b;}
    bool getSingleMode(){return singleMode;}
   void startSingleRun();
   void setZeroAxis(bool b,int);
   void setTraceLabel(bool b, int);
   void setUnitLabel(bool b, int);
   void setAverageLabel(bool b, int);
   void setMinLabel(bool b, int);
   void setMaxLabel(bool b, int);
   void setPPLabel(bool b, int);
   void setRMSLabel(bool b, int);
 bool getTraceLabel(int trace){return TraceOptions[trace].traceLabel.isVisible();}
 bool getUnitLabel(int trace){return TraceOptions[trace].unitLabel.isVisible();}
 bool getAverageLabel(int trace){return TraceOptions[trace].averageLabel.isVisible();}
 bool getMinLabel(int trace){return TraceOptions[trace].minLabel.isVisible();}
 bool getMaxLabel(int trace){return TraceOptions[trace].maxLabel.isVisible();}
 bool getPPLabel(int trace){return TraceOptions[trace].ppLabel.isVisible();}
 bool getRMSLabel(int trace){return TraceOptions[trace].rmsLabel.isVisible();}
   bool getZeroAxis(int trace){return TraceOptions[trace].zeroAxis.isVisible();}
   //PlottingScopeDataThread* getThread(){return Plotting_Scope_Data_Thread;}
    void setValue(const QVector< QVector<float> > &v);
   void setTraceName(int trace, const QString &text);
   QString getTraceName(int trace){return TraceOptions[trace].traceName;}
    void setVerbose(int v){Verbose=v;}
public slots:
   void refresh();
  //void setValue(int,float);
protected slots:
  void closeEvent ( QCloseEvent * event ){event->ignore(); this->hide(); }
private:
  int Verbose;
  float Value;
  qrl_types::Target_Scopes_T *Scope;
  double *d_x; 
  unsigned int NDataMax,Ncurve,NDataSoll, MaxDataPoints;
  int time,time2;
  double xmin,xmax,dx,dt;
  double xMajorTicks,xStep;
  Scopes_Data_T *ScopeData;
  QTimer *timer;
  QwtPlot *qwtPlot;
  double RefreshRate;
  double saveTime;
  QString fileName;
  QwtPlotGrid *grid;
  QwtPlotCurve **cData;
  QColor gridColor;
  QColor bgColor;
  QwtPlotPicker *picker;
  QwtPlotMarker *zeroLine,*vertLine;
  QwtPlotMarker *bottomText;
  QwtPlotZoomer *zoomer[2];
  QwtPlotPanner *panner;
  int index;
  int yMajorTicks;
  double yStep, yOffset, dy ,ymin,ymax;
  Trace_Options_T *TraceOptions;
  Qt::LayoutDirection direction;
  PlottingMode plottingMode;
  bool triggerSearch,triggerUp,singleMode,singleModeRunning;
  double triggerLevel,lastValue;int triggerChannel;
  //PlottingScopeDataThread* Plotting_Scope_Data_Thread;
  friend QDataStream& operator<<(QDataStream &out, const QRL_ScopeWindow &d);
  friend QDataStream& operator>>(QDataStream &in, QRL_ScopeWindow(&d));
};
	QDataStream& operator<<(QDataStream &out, const QRL_ScopeWindow &d);
	QDataStream& operator>>(QDataStream &in, QRL_ScopeWindow(&d));


#endif

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

#include <QDialog>
#include <qwt_plot.h>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_math.h>
#include "qrtailab.h"
#include "qwt_plot.h"

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
 * @todo implement moving time axis
 * @todo add legend
 * @todo add offset mark
 * @todo add curser 
 * @todo add parameter labels 
 * @todo add trigger label
 */

class QRL_ScopeWindow : public QMdiSubWindow
{
   Q_OBJECT
 //friend class PlottingScopeDataThread;
public:
   QRL_ScopeWindow(QWidget *parent = 0,qrl_types::Target_Scopes_T *scope=0,int ind=0);
   ~QRL_ScopeWindow();
   void setValue(int,float);
   void changeRefreshRate(double);
   void changeDataPoints(double);
   void setSaveTime(double);
   void changeDX(double);
   double getDt(){return dt;}
   double getRefreshRate(){return RefreshRate;}
   int getDataPoints(){return NDataSoll;}
   double getDX(){return dx;}
   int start_saving(){return isSaving;}
   FILE* save_file(){return Save_File_Pointer;}
   void setGridColor(QColor);
   void setBgColor(QColor);
   void startSaving(QString);
   void stop_saving();
   int n_points_to_save();
   void setTraceColor(const QColor&,int);
   void setTraceWidth(int ,int );
   int getTraceWidth(int);
   void setTraceOffset(double,int);
   double getTraceOffset(int);
   void setTraceDy(double,int);
   double getTraceDy(int);
   void setDirection(Qt::LayoutDirection d){direction=d;}
   void setOverwriteMode(bool o){overwrite=o;}
   void setHoldMode(bool b){hold=b;}
   void setTriggerMode(bool b){trigger=b;triggerSearch=true;}
   void setTriggerUpDirection(bool b){triggerUp=b;}
   void manuelTriggerSignal(){triggerSearch=false;}
   void setTriggerLevel(double l){triggerLevel=l;}
   //PlottingScopeDataThread* getThread(){return Plotting_Scope_Data_Thread;}
signals:
   void stopSaving(int);
public slots:
   void refresh();
protected slots:
  void closeEvent ( QCloseEvent * event ){event->ignore(); this->hide(); }
private:
  float Value;
  qrl_types::Target_Scopes_T *Scope;
  double *d_x; 
  unsigned int NDataMax,Ncurve,NDataSoll;
  int time,time2;
  double xmin,xmax,dx,dt;
  Scopes_Data_T *ScopeData;
  QTimer *timer;
  QwtPlot *qwtPlot;
  double RefreshRate;
  QwtPlotGrid *grid;
  QwtPlotCurve **cData;
  QColor gridColor;
  QColor bgColor;
  int isSaving;
  FILE* Save_File_Pointer;
  double Save_Time;
  QString File_Name;
  int index;
  int yMajorTicks;
  double yStep, yOffset, dy ,ymin,ymax;
  Trace_Options_T *TraceOptions;
  Qt::LayoutDirection direction;
  bool overwrite,hold,trigger,triggerSearch,triggerUp;
  double triggerLevel,lastValue;int triggerChannel;
  //PlottingScopeDataThread* Plotting_Scope_Data_Thread;
};


#endif

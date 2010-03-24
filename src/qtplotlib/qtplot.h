#ifndef QTPLOT_H
#define QTPLOT_H

#include "qtplot_global.h"
#include "scope_trace.h"
#include "scope_data.h"


class QTPLOTSHARED_EXPORT Qtplot : public QWidget
{
   Q_OBJECT
 //friend class PlottingScopeDataThread;
public:
   enum PlottingMode {roll,overwrite,trigger,hold};
   Qtplot(QWidget *parent = 0,QtplotData *scope=0,int ind=0,QwtPlot *qwt=0);
   ~Qtplot();
   void changeRefreshRate(double);
   void changeDataPoints(double);
   void changeDivider(double);
   unsigned int getDivider(){return Divider;}
   void changeDX(double);
   double getDt(){return dt;}
   double getRefreshRate(){return RefreshRate;}
   int getDataPoints(){return NDataSoll;}
   double getDX(){return dx;}
   int getDXDistance(){return NDistance;}
   void setGridColor(QColor);
   QColor getGridColor(){return gridColor;}
   void setBgColor(QColor);
   QColor getBgColor(){return bgColor;}
   QtplotTrace* trace(int trace){if (trace<Scope->getNTraces()) return Traces[trace]; else return NULL;}
   void setPlottingMode(PlottingMode p);
   void setPlottingDirection(Qt::LayoutDirection d);
   int getPlottingMode() {return (int)plottingMode ;}
   Qt::LayoutDirection getPlottingDirection() {return direction;}
   void setTriggerUpDirection(bool b){triggerUp=b;}
   bool getTriggerUpDirection(){return triggerUp;}
   void manualTriggerSignal(){triggerSearch=false;}
   void setTriggerLevel(double l){triggerLevel=l;}
   double getTriggerLevel(){return triggerLevel;}
   void setTriggerChannel(int trace){ triggerChannel=trace;}
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
 bool getTraceLabel(int trace){return Traces[trace]->isLabelVisible(QtplotTrace::lt_trace);}
 bool getUnitLabel(int trace){return Traces[trace]->isLabelVisible(QtplotTrace::lt_unit);}
 bool getAverageLabel(int trace){return Traces[trace]->isLabelVisible(QtplotTrace::lt_average);}
 bool getMinLabel(int trace){return Traces[trace]->isLabelVisible(QtplotTrace::lt_min);}
 bool getMaxLabel(int trace){return Traces[trace]->isLabelVisible(QtplotTrace::lt_max);}
 bool getPPLabel(int trace){return Traces[trace]->isLabelVisible(QtplotTrace::lt_pp);}
 bool getRMSLabel(int trace){return Traces[trace]->isLabelVisible(QtplotTrace::lt_rms);}
   bool getZeroAxis(int trace){return Traces[trace]->zeroAxis.isVisible();}
   //PlottingScopeDataThread* getThread(){return Plotting_Scope_Data_Thread;}
    void setValue(const QVector< QVector<double> > &v);
    void setTime(const QVector<double> &t);
   void setTraceName(int trace, const QString &text);
   QString getTraceName(int trace){return Traces[trace]->getName();}
    void setVerbose(int v){Verbose=v;}
    void setFileVersion(qint32 v){fileVersion=v;}
    void setPlotting(bool b);
    bool isPlotting(){return plotting;}
public slots:
   void refresh();
  //void setValue(int,float);
//protected slots:
//  void closeEvent ( QCloseEvent * event ){event->ignore(); this->hide(); }
private:
  qint32 fileVersion;
  int Verbose;
  bool plotting;
  double Value;
  double lastTime;
 QtplotData *Scope;
  unsigned int NDataMax,Ncurve,NDataSoll, MaxDataPoints,Divider;
  int time,time2;
  double xmin,xmax,dx,dt;
  double xMajorTicks,xStep;
  int NDistance;
  QTimer *timer;
  QwtPlot *qwtPlot;
  double RefreshRate;
  QwtPlotGrid *grid;
  QColor gridColor;
  QColor bgColor;
  QwtPlotPicker *picker;
  QwtPlotMarker *zeroLine,*vertLine;
  QwtPlotMarker *bottomText;
  QwtPlotZoomer *zoomer[2];
  QwtPlotPanner *panner;
  int index;
  int style;
  int yMajorTicks;
  double yStep, yOffset, dy ,ymin,ymax;
  QtplotTrace **Traces;
  Qt::LayoutDirection direction;
  PlottingMode plottingMode;
  bool triggerSearch,triggerUp,singleMode,singleModeRunning;
  double triggerLevel,lastValue;int triggerChannel;
  //PlottingScopeDataThread* Plotting_Scope_Data_Thread;
  friend QDataStream& operator<<(QDataStream &out, const Qtplot &d);
  friend QDataStream& operator>>(QDataStream &in, Qtplot(&d));
};
        QDataStream& operator<<(QDataStream &out, const Qtplot &d);
        QDataStream& operator>>(QDataStream &in, Qtplot(&d));


#endif // QTPLOT_H

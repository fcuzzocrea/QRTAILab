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
 file:		scopes_window.cpp
 describtion:
   file for the classes QRL_ScopeWindow
*/

#include "scope_window.h"
#include <stdlib.h>
class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas):
        QwtPlotZoomer(xAxis, yAxis, canvas)
    {
        setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
        setTrackerMode(QwtPicker::AlwaysOff);
        setRubberBand(QwtPicker::NoRubberBand);

        // RightButton: zoom out by 1
        // Ctrl+RightButton: zoom out to full size

#if QT_VERSION < 0x040000
        setMousePattern(QwtEventPattern::MouseSelect2,
            Qt::RightButton, Qt::ControlButton);
#else
        setMousePattern(QwtEventPattern::MouseSelect2,
            Qt::RightButton, Qt::ControlModifier);
#endif
        setMousePattern(QwtEventPattern::MouseSelect3,
            Qt::RightButton);
    }
};



//
//  Initialize Scope window
//

QRL_ScopeWindow::QRL_ScopeWindow(QWidget *parent,QRL_ScopeData *scope,int ind)
	:QMdiSubWindow(parent),Scope(scope),index(ind)
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("QRL_ScopeWindow"));
    this->resize(350, 249);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->setWindowIcon(QIcon(QString::fromUtf8(":/icons/scope_icon.xpm")));
    this->setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint );
    qwtPlot = new QwtPlot(this);
    //qwtPlot=this;
    qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));

    this->setWidget(qwtPlot);
    this->setWindowTitle(QApplication::translate("QRL_ScopeWindow", Scope->getName(), 0, QApplication::UnicodeUTF8));
    // Disable polygon clipping
   // QwtPainter::setDeviceClipping(false);
    // We don't need the cache here
//     qwtPlot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
//     qwtPlot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);

    #if QT_VERSION >= 0x040000
    #ifdef Q_WS_X11
    /*
       Qt::WA_PaintOnScreen is only supported for X11, but leads
       to substantial bugs with Qt 4.2.x/Windows
     */
    //qwtPlot->canvas()->setAttribute(Qt::WA_PaintOnScreen, true);
    #endif
    #endif
	plotting=true;
	direction=Qt::RightToLeft;
	triggerSearch=true;triggerUp=true;triggerChannel=0;triggerLevel=0;
	singleMode=false;plottingMode=roll;
	singleModeRunning=true;
	
       yMajorTicks=10;dy=1.; yOffset=0.; 
	ymin=yOffset-0.5*(yMajorTicks*dy);
       ymax=yOffset+0.5*(yMajorTicks*dy);
      yStep=(ymax-ymin)/yMajorTicks;
	MaxDataPoints=1000000;
	dx=0.1;
	xMajorTicks=10;
	xmin=0;
       xmax=(xMajorTicks*dx);
	Divider=1;
	 xStep=(xmax-xmin)/xMajorTicks;

	gridColor=Qt::blue;
  
	bgColor=QColor(240,240,240);
    picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPicker::PointSelection | QwtPicker::DragSelection, 
        QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, 
        qwtPlot->canvas());
    picker->setRubberBandPen(QColor(gridColor));
    picker->setRubberBand(QwtPicker::CrossRubberBand);
    picker->setTrackerPen(QColor(gridColor));

    zeroLine = new QwtPlotMarker();
    //zeroLine->setLabel(QString::fromLatin1("y = 0"));
    zeroLine->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    zeroLine->setLineStyle(QwtPlotMarker::HLine);
    zeroLine->setYValue(0.0);
    zeroLine->setLinePen(QPen(QColor(gridColor),1,Qt::DotLine));
    zeroLine->attach(qwtPlot);

    vertLine = new QwtPlotMarker();
    //zeroLine->setLabel(QString::fromLatin1("y = 0"));
    vertLine->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    vertLine->setLineStyle(QwtPlotMarker::VLine);
    vertLine->setXValue(xmax/2.);
    vertLine->setLinePen(QPen(QColor(gridColor),1,Qt::DotLine));
    vertLine->attach(qwtPlot);

    zoomer[0] = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft,qwtPlot->canvas());
    zoomer[0]->setRubberBand(QwtPicker::RectRubberBand);
    zoomer[0]->setRubberBandPen(QColor(gridColor));
    zoomer[0]->setTrackerMode(QwtPicker::ActiveOnly);
    zoomer[0]->setTrackerPen(QColor(gridColor));

    zoomer[1] = new Zoomer(QwtPlot::xTop, QwtPlot::yRight,
         qwtPlot->canvas());
    
    panner = new QwtPlotPanner(qwtPlot->canvas());
    panner->setMouseButton(Qt::MidButton);
    panner->setEnabled(false);

    zoomer[0]->setEnabled(false);
    zoomer[0]->zoom(0);

    zoomer[1]->setEnabled(false);
    zoomer[1]->zoom(0);

    bottomText = new QwtPlotMarker();
    QwtText bt(tr("x: %1.%2 sec/dev  roll <-").arg(0).arg(1));
    bt.setColor(QColor(gridColor));
    bottomText->setLabel(bt);
    bottomText->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    bottomText->setLineStyle(QwtPlotMarker::NoLine);
    bottomText->setValue(0.0,-5.);
   bottomText->attach(qwtPlot);


    //picker->setEnabled(!on);
       
       //  qwtPlot->setTitle(tr(Scope->name));
       //qwtPlot->setAxisTitle(QwtPlot::xBottom, "Delta Time [sec]");
       qwtPlot->setAxisScale(QwtPlot::xBottom, xmin, xmax,xStep);
	//qwtPlot->axisScaleDiv(QwtPlot::xBottom)=new QwtScaleDiv(1,0);
       // qwtPlot->setAxisTitle(QwtPlot::yLeft, "Values");
       qwtPlot->setAxisScale(QwtPlot::yLeft, ymin, ymax,yStep);
       if (Scope->isSaveScopeTime())
	   qwtPlot->enableAxis(QwtPlot::xBottom,true);
       else
	  qwtPlot->enableAxis(QwtPlot::xBottom,false);
       qwtPlot->enableAxis(QwtPlot::yLeft,false);
//     QwtPlotMarker *mY = new QwtPlotMarker();
//     mY->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
//     mY->setLineStyle(QwtPlotMarker::HLine);
//     mY->setYValue(0.0);
//     mY->attach(Scope);



       NDataMax=(int)((xmax-xmin)/Scope->getDt());
       NDataSoll=100;
	NDataSoll=NDataMax;
	saveTime=0.;
       fileName=tr("SCOPE");
	RefreshRate=30.;
	//Werden genügend Daten bereitgestellt?
       if (NDataSoll>NDataMax)
		NDataSoll=NDataMax;
       dt=(xmax-xmin)/NDataSoll;
       NDistance=(int)(dt*(1./Scope->getDt()));  //doesnt work
	//Achtung Arrayüberlauf verhindern!
       //if ((1/dt/RefreshRate)>NDataSoll)
	//	RefreshRate=1/dt/NDataSoll;
       Ncurve=Scope->getNTraces();
//if (Ncurve>0){
       QPen pen;
	Traces = new QRL_ScopeTrace*[Ncurve];


       for (unsigned int j=0;j<Ncurve;j++){
		Traces[j] = new QRL_ScopeTrace(qwtPlot, MaxDataPoints, j);
		Traces[j]->changeNDataSoll(NDataSoll,dt);
		Traces[j]->setGridColor(gridColor);
// 		switch(j){
// 			case 0:TraceOptions[j].brush=QBrush(Qt::red);
// 				break;
// 			case 1:TraceOptions[j].brush=QBrush(Qt::green);
// 				break;
// 			case 2:TraceOptions[j].brush=QBrush(Qt::yellow);
// 				break;
// 			case 3:TraceOptions[j].brush=QBrush(Qt::blue);
// 				break;
// 			case 4:TraceOptions[j].brush=QBrush(Qt::black);
// 				break;
// 			case 5:TraceOptions[j].brush=QBrush(Qt::magenta);
// 				break;
// 			case 6:TraceOptions[j].brush=QBrush(Qt::darkRed);
// 				break;
// 			case 7:TraceOptions[j].brush=QBrush(Qt::darkGreen);
// 				break;
// 			case 8:TraceOptions[j].brush=QBrush(Qt::darkMagenta);
// 				break;
// 			case 9:TraceOptions[j].brush=QBrush(Qt::darkYellow);
// 				break;
// 			case 10:TraceOptions[j].brush=QBrush(Qt::cyan);
// 				break;
// 			case 11:TraceOptions[j].brush=QBrush(Qt::darkCyan);
// 				break;
// 			case 12:TraceOptions[j].brush=QBrush(Qt::gray);
// 				break;
// 			case 13:TraceOptions[j].brush=QBrush(Qt::darkGray);
// 				break;
// 			case 14:TraceOptions[j].brush=QBrush(Qt::lightGray);
// 				break;
// 		}

// 		TraceOptions[j].zeroAxis.setLabel(tr("%1").arg(j));    
// 		TraceOptions[j].zeroAxis.setLabelAlignment(Qt::AlignLeft|Qt::AlignTop);
//     		TraceOptions[j].zeroAxis.setLineStyle(QwtPlotMarker::HLine);
//     		//TraceOptions[j].zeroAxis.setYValue(TraceOptions[j].offset/TraceOptions[j].dy);
// 		TraceOptions[j].zeroAxis.setYValue(TraceOptions[j].offset);
//     		TraceOptions[j].zeroAxis.setLinePen(QPen(TraceOptions[j].brush,2.,Qt::DashDotLine));
//     		TraceOptions[j].zeroAxis.attach(qwtPlot);
// 		TraceOptions[j].zeroAxis.hide();
// 
// 		TraceOptions[j].traceName=tr("Trace %1").arg(j);
// 		QwtText ttext(TraceOptions[j].traceName);
//    		 ttext.setColor(QColor(TraceOptions[j].brush.color()));
//     		TraceOptions[j].traceLabel.setLabel(ttext);
// 		TraceOptions[j].traceLabel.setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
//     		TraceOptions[j].traceLabel.setLineStyle(QwtPlotMarker::NoLine);
//     		//TraceOptions[j].zeroAxis.setYValue(TraceOptions[j].offset/TraceOptions[j].dy);
// 		TraceOptions[j].traceLabel.setXValue(0.+j*xmax/5.);
// 		TraceOptions[j].traceLabel.setYValue(4.);
// 		
//     		TraceOptions[j].traceLabel.setLinePen(QPen(TraceOptions[j].brush,2.,Qt::DashDotLine));
//     		TraceOptions[j].traceLabel.attach(qwtPlot);
// 		TraceOptions[j].traceLabel.hide();
// 



		#if QT_VERSION >= 0x040000
		//to slow 
   		 //cData[j]->setRenderHint(QwtPlotItem::RenderAntialiased);
		#endif

	}

  //  }
	   // grid
    grid = new QwtPlotGrid;
    //grid->enableXMin(true);
    grid->setMajPen(QPen(gridColor, 0, Qt::DotLine));
    grid->attach(qwtPlot);


	timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
        timer->start((int)(1./RefreshRate*1000.));

//Plotting_Scope_Data_Thread = new PlottingScopeDataThread;
//Plotting_Scope_Data_Thread->start(this);

//	isSaving=0;Save_Time=1.0;File_Name=tr(Scope->name);
}

QRL_ScopeWindow::~QRL_ScopeWindow(){
//Plotting_Scope_Data_Thread->stopThread();
//Plotting_Scope_Data_Thread->wait();
//delete Plotting_Scope_Data_Thread;
//delete mY;
for (unsigned int j=0;j<Ncurve;j++){
	delete Traces[j];
}
delete[] Traces;
}


void QRL_ScopeWindow::refresh()
{
	qwtPlot->replot();
	

  for (unsigned int nn=0; nn<Ncurve;++nn){
      Traces[nn]->refresh();
  }
}

void QRL_ScopeWindow::setPlotting(bool b){
    plotting=b;
     QwtText bt;
    bt.setColor(QColor(gridColor));
    if (plotting){
      Scope->setPlotting(true);
      setPlottingMode(plottingMode);
   } else {
    	bt.setText(tr("x: %1 sec/dev  hold").arg(dx));
	bottomText->setLabel(bt);
	Scope->setPlotting(false);
    }
    
  }
  

 void QRL_ScopeWindow::setPlottingMode(PlottingMode p){
	plottingMode=p;
 QwtText bt;
    bt.setColor(QColor(gridColor));
        for (unsigned int nn=0; nn<Ncurve;nn++)
	  Traces[nn]->resetTime();
  if (plotting){
	switch(plottingMode){
	case roll:
		if (direction==Qt::LeftToRight)
			bt.setText(tr("x: %1 sec/dev  roll ->").arg(dx));
		else
			bt.setText(tr("x: %1 sec/dev  roll <-").arg(dx));
		bottomText->setLabel(bt);
		break;
	case overwrite:
		if (direction==Qt::LeftToRight)
			bt.setText(tr("x: %1 sec/dev  overwrite ->").arg(dx));
		else
			bt.setText(tr("x: %1 sec/dev  overwrite <-").arg(dx));
		bottomText->setLabel(bt);
		break;
	case trigger:
		if (direction==Qt::LeftToRight)
			bt.setText(tr("x: %1 sec/dev  trigger ->").arg(dx));
		else
			bt.setText(tr("x: %1 sec/dev  trigger <-").arg(dx));
		bottomText->setLabel(bt);
		break;
        case hold: //hold
// 		bt.setText(tr("x: %1 sec/dev  hold").arg(dx));
// 		bottomText->setLabel(bt);
// 		Scope->setPlotting(false);
                break;

	}
   }
}

 void QRL_ScopeWindow::setPlottingDirection(Qt::LayoutDirection d){
	direction=d;
        for (unsigned int nn=0; nn<Ncurve;nn++)
	  Traces[nn]->resetTime();
 QwtText bt;
    bt.setColor(QColor(gridColor));
    if (plotting){
	switch(plottingMode){
	case roll:
		if (direction==Qt::LeftToRight)
			bt.setText(tr("x: %1 sec/dev  roll ->").arg(dx));
		else
			bt.setText(tr("x: %1 sec/dev  roll <-").arg(dx));
		bottomText->setLabel(bt);
		break;
	case overwrite:
		if (direction==Qt::LeftToRight)
			bt.setText(tr("x: %1 sec/dev  overwrite ->").arg(dx));
		else
			bt.setText(tr("x: %1 sec/dev  overwrite <-").arg(dx));
		bottomText->setLabel(bt);
		break;
	case trigger:
		if (direction==Qt::LeftToRight)
			bt.setText(tr("x: %1 sec/dev  trigger ->").arg(dx));
		else
			bt.setText(tr("x: %1 sec/dev  trigger <-").arg(dx));
		bottomText->setLabel(bt);
		break;
        case hold: //hold
// 		bt.setText(tr("x: %1 sec/dev  hold").arg(dx));
// 		bottomText->setLabel(bt);
                break;

	}
    }
}
   void QRL_ScopeWindow::changeRefreshRate(double rr)
{
	RefreshRate=rr;
	if (	RefreshRate<0.)
		RefreshRate=20.;
	timer->stop();
	timer->start((int)(1./RefreshRate*1000.));
	
	//Plotting_Scope_Data_Thread->changeRefreshRate(RefreshRate);

}



 void QRL_ScopeWindow::changeDivider(double div)
{

       timer->stop();
	Divider=(unsigned int)div;
	NDataSoll=(int)((double)NDataMax/(double)Divider);

	NDistance=(int)(dt*(1./Scope->getDt()));  //doesnt work
       if (NDistance<1)
		NDistance=1;

	//NDataSoll=NDataSoll;
	if (NDataSoll>NDataMax)
		NDataSoll=NDataMax;

	if (NDataSoll<10)
		NDataSoll=10;
      //NDataSoll=NDataMax;

	

       if (NDistance<1)
		NDistance=1;
	/*if(NDataSoll<(1/dt/(xmax-xmin))){
		NDataSoll=(1/dt/(xmax-xmin));
		dt=(xmax-xmin)/NDataSoll;
	}*/

 dt=(xmax-xmin)/NDataSoll;

     for (unsigned int j=0;j<Ncurve;j++){
  	Traces[j]->changeNDataSoll(NDataSoll,dt);
     }

     timer->start((int)(1./RefreshRate*1000.));


	if (Verbose) {
	printf("xmin: %f,xmax %f, scope->dt ist %f, NDistance %d\n",xmin,xmax,Scope->getDt(),NDistance);
	printf("datasoll: %d,datamax %d, dt ist %f\n",NDataSoll,NDataMax,dt);
	}
    //changeDX(dx);  //FIXME should be removed

}

   void QRL_ScopeWindow::changeDataPoints(double dp)
{
	//if (dp<(1/((xmax-xmin)/dp)/100))
	//	return;
       timer->stop();
	NDataSoll=(int)dp;

	NDistance=(int)(dt*(1./Scope->getDt()));  //doesnt work
       if (NDistance<1)
		NDistance=1;

	//NDataSoll=NDataSoll;
	if (NDataSoll>NDataMax)
		NDataSoll=NDataMax;

	if (NDataSoll<10)
		NDataSoll=10;
      //NDataSoll=NDataMax;

	

       if (NDistance<1)
		NDistance=1;
	/*if(NDataSoll<(1/dt/(xmax-xmin))){
		NDataSoll=(1/dt/(xmax-xmin));
		dt=(xmax-xmin)/NDataSoll;
	}*/


 dt=(xmax-xmin)/NDataSoll;
     for (unsigned int j=0;j<Ncurve;j++){
  	Traces[j]->changeNDataSoll(NDataSoll,dt);
     }
     timer->start((int)(1./RefreshRate*1000.));


	if (Verbose) {
	printf("xmin: %f,xmax %f, scope->dt ist %f, NDistance %d\n",xmin,xmax,Scope->getDt(),NDistance);
	printf("datasoll: %d,datamax %d, dt ist %f\n",NDataSoll,NDataMax,dt);
	}
    //changeDX(dx);  //FIXME should be removed
}


   void QRL_ScopeWindow::changeDX(double v)
{

	//if ((xMajorTicks*v)/Scope->dt<10.)
	//	return;

//	if (((xMajorTicks*v)/Scope->dt<(1/((xmax-xmin)/(xMajorTicks*v)/Scope->dt)/100)))
//		return;

	if (v>0.)
		dx=v;
	timer->stop();
	 xmax=(xMajorTicks*dx);
	
	//xmax=dx;
	
	NDataMax=(int)((xmax-xmin)/Scope->getDt());
	if (NDataMax>MaxDataPoints){
		NDataMax=MaxDataPoints;
		dx=((NDataMax*Scope->getDt())+xmin)/xMajorTicks;
 		xmax=(xMajorTicks*dx);
	}
	NDataSoll=(int)((double)NDataMax/(double)Divider);
	if (NDataSoll>NDataMax)
		NDataSoll=NDataMax;

	//NDataSoll=NDataMax;

	 QwtText bt;
    bt.setColor(QColor(gridColor));
    if (plotting){
	switch(plottingMode){
	case roll:
		if (direction==Qt::LeftToRight)
			bt.setText(tr("x: %1 sec/dev  roll ->").arg(dx));
		else
			bt.setText(tr("x: %1 sec/dev  roll <-").arg(dx));
		bottomText->setLabel(bt);
		break;
	case overwrite:
		if (direction==Qt::LeftToRight)
			bt.setText(tr("x: %1 sec/dev  overwrite ->").arg(dx));
		else
			bt.setText(tr("x: %1 sec/dev  overwrite <-").arg(dx));
		bottomText->setLabel(bt);
		break;
	case trigger:
		if (direction==Qt::LeftToRight)
			bt.setText(tr("x: %1 sec/dev  trigger ->").arg(dx));
		else
			bt.setText(tr("x: %1 sec/dev  trigger <-").arg(dx));
		bottomText->setLabel(bt);
		break;
        case hold: //hold
// 		bt.setText(tr("x: %1 sec/dev  hold").arg(dx));
// 		bottomText->setLabel(bt);
                break;

	}
}

	dt=(xmax-xmin)/NDataSoll;
NDistance=(int)(dt*(1./Scope->getDt()));  //doesnt work
        if (NDistance<1)
		NDistance=1;
	/*delete[] d_x;
	for (unsigned int j=0;j<Ncurve;j++){
		delete[] ScopeData[j].d_y;
	}
	 d_x = new double[NDataMax];*/

       for (unsigned int j=0;j<Ncurve;j++){
		//ScopeData[j].d_y = new double[NDataMax];
		Traces[j]->changeNDataSoll(NDataSoll,dt);
		int k=0;
		for (int nn=0;nn<j;nn++)
			if (Traces[nn]->isLabelVisible(QRL_ScopeTrace::lt_trace))
				k++;
		  Traces[j]->setLabelsXValue(0.+k*xmax/5.);
	}
	xStep=(xmax-xmin)/xMajorTicks;
	qwtPlot->setAxisScale(QwtPlot::xBottom, xmin, xmax,xStep);
	    vertLine->setXValue(xmax/2.);
	if (Verbose) {
		printf("xmin: %f,xmax %f, scope->dt ist %f\n",xmin,xmax,Scope->getDt());
		printf("datasoll: %d,datamax %d, dt ist %f\n",NDataSoll,NDataMax,dt);
	}
	timer->start(1/RefreshRate*1000.);

//changeDataPoints(NDataMax);

}


void QRL_ScopeWindow::setGridColor(QColor gridcolor){
	gridColor=gridcolor;
	grid->setMajPen(QPen(gridColor, 0, Qt::DotLine));
	picker->setRubberBandPen(QColor(gridColor));
        picker->setTrackerPen(QColor(gridColor));
	QwtText bt(bottomText->label());
        bt.setColor(QColor(gridColor));
	bottomText->setLabel(bt);
	zeroLine->setLinePen(QPen(QColor(gridColor),1,Qt::DotLine));
	vertLine->setLinePen(QPen(QColor(gridColor),1,Qt::DotLine));
        for (unsigned int nn=0; nn<Ncurve;nn++)
	  Traces[nn]->setGridColor(gridcolor);
	
}
void QRL_ScopeWindow::setBgColor(QColor bgcolor){
	bgColor=bgcolor;
	qwtPlot->setCanvasBackground(bgColor);
}



void QRL_ScopeWindow::startSingleRun(){
	singleModeRunning=true;

}




void QRL_ScopeWindow::setTraceLabel(bool b,int trace){

	if (b==getTraceLabel(trace))
		return;

	if (b) {
		Traces[trace]->showLabel(QRL_ScopeTrace::lt_trace);
		for (unsigned int j=0;j<Ncurve;j++){
			int k=0;
                        for (unsigned int nn=0;nn<j;nn++)
				if (Traces[nn]->isLabelVisible(QRL_ScopeTrace::lt_trace))
					k++;
			Traces[j]->setLabelsXValue(0.+k*xmax/5.);
		}

	
	}else {
		Traces[trace]->hideLabel(QRL_ScopeTrace::lt_trace);
		setUnitLabel(false,trace);
		setAverageLabel(false,trace);
		setMinLabel(false,trace);
		setMaxLabel(false,trace);
		setPPLabel(false,trace);
		setRMSLabel(false,trace);
		Traces[trace]->setLabelCounter(0);

	}
}

void QRL_ScopeWindow::setUnitLabel(bool b,int trace){


	if (b) {
		setTraceLabel(true,trace);
		
		for (unsigned int j=0;j<Ncurve;j++){
			int k=0;
                        for (unsigned int nn=0;nn<j;nn++)
				if (Traces[nn]->isLabelVisible(QRL_ScopeTrace::lt_trace))
					k++;
			Traces[j]->setLabelsXValue(0.+k*xmax/5.);
		}
		Traces[trace]->showLabel(QRL_ScopeTrace::lt_unit);

	
	}else{
		if (getUnitLabel(trace)==false)
			return;
		Traces[trace]->hideLabel(QRL_ScopeTrace::lt_unit);
		setAverageLabel(getAverageLabel(trace),trace);
		setMinLabel(getMinLabel(trace),trace);
		setMaxLabel(getMaxLabel(trace),trace);
		setPPLabel(getPPLabel(trace),trace);
		setRMSLabel(getRMSLabel(trace),trace);
	
	}

}


void QRL_ScopeWindow::setAverageLabel(bool b,int trace){


		if (b) {
		setTraceLabel(true,trace);
		
		for (unsigned int j=0;j<Ncurve;j++){
			int k=0;
                        for (unsigned int nn=0;nn<j;nn++)
				if (Traces[nn]->isLabelVisible(QRL_ScopeTrace::lt_trace))
					k++;
			Traces[j]->setLabelsXValue(0.+k*xmax/5.);
		}
		Traces[trace]->showLabel(QRL_ScopeTrace::lt_average);
	}else{
		if (getAverageLabel(trace)==false)
			return;
		Traces[trace]->hideLabel(QRL_ScopeTrace::lt_average);
		setUnitLabel(getUnitLabel(trace),trace);
		setMinLabel(getMinLabel(trace),trace);
		setMaxLabel(getMaxLabel(trace),trace);
		setPPLabel(getPPLabel(trace),trace);
		setRMSLabel(getRMSLabel(trace),trace);
	}

}

void QRL_ScopeWindow::setMinLabel(bool b,int trace){

	if (b) {
		setTraceLabel(true,trace);
		
		for (unsigned int j=0;j<Ncurve;j++){
			int k=0;
                        for (unsigned int nn=0;nn<j;nn++)
				if (Traces[nn]->isLabelVisible(QRL_ScopeTrace::lt_trace))
					k++;
			Traces[j]->setLabelsXValue(0.+k*xmax/5.);
		}
		Traces[trace]->showLabel(QRL_ScopeTrace::lt_min);
	}else{
		if (getMinLabel(trace)==false)
			return;
		Traces[trace]->hideLabel(QRL_ScopeTrace::lt_min);
		setUnitLabel(getUnitLabel(trace),trace);
		setAverageLabel(getAverageLabel(trace),trace);
		setMaxLabel(getMaxLabel(trace),trace);
		setPPLabel(getPPLabel(trace),trace);
		setRMSLabel(getRMSLabel(trace),trace);
	}

}

void QRL_ScopeWindow::setMaxLabel(bool b,int trace){
	if (b) {
		setTraceLabel(true,trace);
		
		for (unsigned int j=0;j<Ncurve;j++){
			int k=0;
                        for (unsigned int nn=0;nn<j;nn++)
				if (Traces[nn]->isLabelVisible(QRL_ScopeTrace::lt_trace))
					k++;
			Traces[j]->setLabelsXValue(0.+k*xmax/5.);
		}
		Traces[trace]->showLabel(QRL_ScopeTrace::lt_max);
	}else{
		if (getMaxLabel(trace)==false)
			return;
		Traces[trace]->hideLabel(QRL_ScopeTrace::lt_max);
		setUnitLabel(getUnitLabel(trace),trace);
		setAverageLabel(getAverageLabel(trace),trace);
		setMinLabel(getMinLabel(trace),trace);
		setPPLabel(getPPLabel(trace),trace);
		setRMSLabel(getRMSLabel(trace),trace);
	}

}

void QRL_ScopeWindow::setPPLabel(bool b,int trace){

	if (b) {
		setTraceLabel(true,trace);
		
		for (unsigned int j=0;j<Ncurve;j++){
			int k=0;
			for (int nn=0;nn<j;nn++)
				if (Traces[nn]->isLabelVisible(QRL_ScopeTrace::lt_trace))
					k++;
			Traces[j]->setLabelsXValue(0.+k*xmax/5.);
		}
		Traces[trace]->showLabel(QRL_ScopeTrace::lt_pp);
	}else{
		if (getPPLabel(trace)==false)
			return;
		Traces[trace]->hideLabel(QRL_ScopeTrace::lt_pp);
		setUnitLabel(getUnitLabel(trace),trace);
		setAverageLabel(getAverageLabel(trace),trace);
		setMaxLabel(getMaxLabel(trace),trace);
		setMinLabel(getMinLabel(trace),trace);
		setRMSLabel(getRMSLabel(trace),trace);
	}

}

void QRL_ScopeWindow::setRMSLabel(bool b,int trace){
	if (b) {
		setTraceLabel(true,trace);
		
		for (unsigned int j=0;j<Ncurve;j++){
			int k=0;
                        for (unsigned int nn=0;nn<j;nn++)
				if (Traces[nn]->isLabelVisible(QRL_ScopeTrace::lt_trace))
					k++;
			Traces[j]->setLabelsXValue(0.+k*xmax/5.);
		}
		Traces[trace]->showLabel(QRL_ScopeTrace::lt_rms);
	}else{
		if (getRMSLabel(trace)==false)
			return;
		Traces[trace]->hideLabel(QRL_ScopeTrace::lt_rms);
		setUnitLabel(getUnitLabel(trace),trace);
		setAverageLabel(getAverageLabel(trace),trace);
		setMaxLabel(getMaxLabel(trace),trace);
		setPPLabel(getPPLabel(trace),trace);
		setMinLabel(getMinLabel(trace),trace);
	}

}




void QRL_ScopeWindow::setTime(const  QVector<float>  &t){
  if (t.size()>0){
    float meandt=t[0]-lastTime;
    for (int i=1;i<t.size();i++)
      meandt=meandt/2+(t[i]-t[i-1])/2;
//    printf("dt %f\n",meandt);
   lastTime=t[t.size()-1];
   }
}

void QRL_ScopeWindow::setValue(const QVector< QVector<float> > &v)
{




int time; int start=0;  PlottingMode PM=plottingMode;

if (v.at(0).size()>NDataSoll){  //roll mode is to cpu expensive and not necassary -> switch to overwrite mode
	if (PM==roll)
		PM=overwrite;
} 
//   for (int nn=0; nn<v.size();++nn){
//     Traces[nn]->setValue(v.at(nn));
//     }
if (plotting){
switch(PM){
case roll:

  for (int nn=0; nn<v.size();++nn){
//double *d_y=Traces[nn]->getPointerd_y(); //FIXME
     time=v.at(nn).size()-1;
	//printf("time %d, NDataSoll %d\n",time,NDataSoll);
		if (time>-1){
		if (Qt::LeftToRight==direction){ 
		  Traces[nn]->moveDataToRight(time);
		
		for ( unsigned int i = 0; i<=time; i++){
		   if ((time-i)<v.at(nn).size())
			Traces[nn]->setValue(i,((double)v.at(nn).at(time-i)));
			//d_y[i]=(double)temp[nn][time-i];
		}
		} else { //right to left
		   Traces[nn]->moveDataToLeft(time);
		for ( int i =  0; i<=time; i++){
		     if ((i)<v.at(nn).size()) 
			Traces[nn]->setValue((NDataSoll+i-time-1),((double)v.at(nn).at(i)));
			//d_y[i]=(double)temp[nn][time-i];
		}


		}
		time=0;
		//qwtPlot->replot();
	}
	Traces[nn]->setTime(time);
     }
  	
	break;
case overwrite:
   for (int k=start; k<v.at(0).size(); ++k){
      for (int nn=0; nn<v.size();++nn){
	time=Traces[nn]->getTime();
// 	double *d_y=Traces[nn]->getPointerd_y();

	Traces[nn]->setValue(time,((double)v.at(nn).at(k)));
	if (Qt::LeftToRight==direction)
		time++;
	else
		time--;
	if (time<0){
		
// 		for (int n=0;n<Ncurve;n++){ // reset time counter
// 			d_y[ScopeData[n].time]=((double)v.at(nn).at(k))/Traces[nn]->getDy()+Traces[nn]->getOffset();
// 			ScopeData[n].time=NDataSoll-1;
// 			
// 		}
		time=NDataSoll-1;
		//break;
	}
	if(time>NDataSoll-1){
		
// 		for (int n=0;n<Ncurve;n++){ // reset time counter
// 			d_y[ScopeData[n].time]=((double)v.at(nn).at(k))/Traces[nn]->getDy()+Traces[nn]->getOffset();
// 			ScopeData[n].time=0;
// 		
// 			
// 		}
		time=0;
		//break;
	}
	Traces[nn]->setTime(time);
}
}
	break;

case trigger:
     for (int k=start; k<v.at(0).size(); ++k){
 for (int nn=0; nn<v.size();++nn){
// double *d_y=Traces[nn]->getPointerd_y();
	if (triggerSearch){ //search for next trigger event
	  if (nn==triggerChannel) {
		double y= (double)v.at(nn).at(k);
		if (!singleModeRunning && singleMode) {
			// do nothing
		} else 	if (triggerUp){
			if (y>triggerLevel && lastValue<triggerLevel){
				triggerSearch=false;singleModeRunning=false;
				for (int n=0;n<Ncurve;n++){ // reset time counter
					if (Qt::LeftToRight==direction)
					Traces[nn]->setTime(0);
					else
					Traces[nn]->setTime(NDataSoll-1);
				}
			}
		} else {
			if (y<triggerLevel && lastValue>triggerLevel){
				triggerSearch=false;singleModeRunning=false;
                                for (unsigned int n=0;n<Ncurve;n++){ //reset time counter
					if (Qt::LeftToRight==direction)
					Traces[nn]->setTime(0);
					else
					Traces[nn]->setTime(NDataSoll-1);
				}
			}
		}
		lastValue=y;
	  } else { // plot last data for the other traces
		time=Traces[nn]->getTime();
		if (time>0 && time <NDataSoll-1){
			Traces[nn]->setValue(time,((double)v.at(nn).at(k)));
			if (Qt::LeftToRight==direction)
				time++;
			else
				time--;
			Traces[nn]->setTime(time);
		}

	  }
	} else {
	time=Traces[nn]->getTime();
	Traces[nn]->setValue(time,((double)v.at(nn).at(k)));
	if (Qt::LeftToRight==direction)
		time++;
	else
		time--;
	if (time<0){
		time=NDataSoll-1;
		if (nn==triggerChannel){// start searching
			triggerSearch=true;
			lastValue=(double)v.at(nn).at(k);
		}
	}
	if(time>NDataSoll-1){
		time=0;
		if (nn==triggerChannel){ // start searching
			triggerSearch=true;
			lastValue=(double)v.at(nn).at(k);
		}
	}
	Traces[nn]->setTime(time);
	}
}}
	break;

/*
case hold:
	break;*/
default:
	break;
}
//}
//	time2++;
//	time2=time2;

}


}








/*
void QRL_ScopeWindow::setValue( int nn, float v)
{			


int time;
switch(plottingMode){
case roll:

	time=ScopeData[nn].time;
	ScopeData[nn].d_yt[time]=((double)v)/Traces[nn]->getDy()+Traces[nn]->getOffset();
	
	time++;
	
        if((time>(1/dt/RefreshRate)) || time>NDataMax-1){
		//printf("time %d for trace %d\n",time,nn);
		time--;
		if (time<0)
			time=0;
		if (Qt::LeftToRight==direction){ // Segmentation fault for   ndatasoll<5 
		for (unsigned int i = NDataSoll - 1; i > time; i-- ){
        		ScopeData[nn].d_y[i] = ScopeData[nn].d_y[i-1-time];
		}
		for ( unsigned int i = 0; i<=time; i++){
			ScopeData[nn].d_y[i]=ScopeData[nn].d_yt[time-i];
			//ScopeData[nn].d_y[i]=(double)temp[nn][time-i];
		}
		} else { //right to left
		for (unsigned int i = 0; i < NDataSoll-time-1; i++ ){
		if ((i+time+1)<NDataSoll)
			ScopeData[nn].d_y[i] = ScopeData[nn].d_y[i+time+1];
		}
		for ( int i =  0; i<=time; i++){
			
			ScopeData[nn].d_y[NDataSoll+i-time-1]=ScopeData[nn].d_yt[i];
			//ScopeData[nn].d_y[i]=(double)temp[nn][time-i];
		}


		}
		time=0;
		//qwtPlot->replot();
	}
	ScopeData[nn].time=time;
	break;
case overwrite:
	time=ScopeData[nn].time;
	ScopeData[nn].d_y[time]=((double)v)/Traces[nn]->getDy()+Traces[nn]->getOffset();
	if (Qt::LeftToRight==direction)
		time++;
	else
		time--;
	if (time<0)
		time=NDataSoll-1;
	if(time>NDataSoll-1)
		time=0;
	ScopeData[nn].time=time;

	break;

case trigger:
	if (triggerSearch){ //search for next trigger event
	  if (nn==triggerChannel) {
		double y= (double)v;
		if (!singleModeRunning && singleMode) {
			// do nothing
		} else 	if (triggerUp){
			if (y>triggerLevel && lastValue<triggerLevel){
				triggerSearch=false;singleModeRunning=false;
				for (int n=0;n<Ncurve;n++){ // reset time counter
					if (Qt::LeftToRight==direction)
					ScopeData[n].time=0;
					else
					ScopeData[n].time=NDataSoll-1;
				}
			}
		} else {
			if (y<triggerLevel && lastValue>triggerLevel){
				triggerSearch=false;singleModeRunning=false;
				for (int n=0;n<Ncurve;n++){ //reset time counter
					if (Qt::LeftToRight==direction)
					ScopeData[n].time=0;
					else
					ScopeData[n].time=NDataSoll-1;
				}
			}
		}
		lastValue=y;
	  } else { // plot last data for the other traces
		time=ScopeData[nn].time;
		if (time>0 && time <NDataSoll-1){
			ScopeData[nn].d_y[time]=((double)v)/Traces[nn]->getDy()+Traces[nn]->getOffset();
			if (Qt::LeftToRight==direction)
				time++;
			else
				time--;
			ScopeData[nn].time=time;
		}

	  }
	} else {
	time=ScopeData[nn].time;
	ScopeData[nn].d_y[time]=((double)v)/Traces[nn]->getDy()+Traces[nn]->getOffset();
	if (Qt::LeftToRight==direction)
		time++;
	else
		time--;
	if (time<0){
		time=NDataSoll-1;
		if (nn==triggerChannel){// start searching
			triggerSearch=true;
			lastValue=(double)v;
		}
	}
	if(time>NDataSoll-1){
		time=0;
		if (nn==triggerChannel){ // start searching
			triggerSearch=true;
			lastValue=(double)v;
		}
	}
	ScopeData[nn].time=time;
	}
	break;


case hold:
	break;
default:
	break;
}
//}
//	time2++;
//	ScopeData[nn].time2=time2;
}*/

QDataStream& operator<<(QDataStream &out, const QRL_ScopeWindow &d){
        qint32 a;
	out  << d.size()  << d.pos() << d.isVisible();
	a=d.Ncurve; out << a;
	out << d.gridColor;
	out << d.bgColor;
	out << d.RefreshRate;
	out << d.dx;
	a=d.NDataSoll;out << a;
	a=(qint32)d.plottingMode;out << a;
	a=(qint32)d.direction; out << a;
	a=d.triggerChannel; out << a;
	out << d.triggerLevel;
	out << d.triggerUp;
	out << d.singleMode;
	out << d.saveTime;
	out << d.fileName;
	a=d.Divider;out << a;
	out <<d.plotting;
         for (unsigned int nn=0; nn<d.Ncurve;++nn){
		
		out << d.Traces[nn];
// 		out << d.Traces[nn].offset;
// 		out << d.Traces[nn].dy;
// 		out << d.Traces[nn].brush.color();
// 		a=d.Traces[nn].lineWidth; out <<a;
// 		out << d.Traces[nn].visible;
	}	

	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_ScopeWindow(&d)){
	QSize s;QPoint p;bool b; QColor c; qint32 a,a2;QFont f; double dd;
	QString str; int Ncurve;
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	in >> a; Ncurve=(int)a;
	in >> c; d.setGridColor(c);
	in >> c; d.setBgColor(c);
	in >> dd; d.changeRefreshRate(dd);
	in >> dd; d.changeDX(dd);
	in >> a; //d.changeDataPoints(a);
	in >> a >> a2;
	if ((QRL_ScopeWindow::PlottingMode)a==QRL_ScopeWindow::hold){
	  d.setPlotting(false);
	  d.setPlottingMode(QRL_ScopeWindow::roll);
	}else{
	  d.setPlottingMode((QRL_ScopeWindow::PlottingMode)a);
	  d.setPlotting(true);
	}
	d.setPlottingDirection((Qt::LayoutDirection)a2);
	in >> a; d.setTriggerChannel(a);
	in >> dd; d.setTriggerLevel(dd);
	in >> b; d.setTriggerUpDirection(b);
	in >> b; d.setSingleMode(b);
	in >> dd; d.setSaveTime(dd);
        in >> str; d.setFileName(str);
	
	if (d.fileVersion>101){
	  in >> a; d.changeDivider(a);
	}
	if (d.fileVersion>104){
	  in >> b; d.setPlotting(b);
	}
	 for (int nn=0; nn<Ncurve;++nn){
	   if (nn<d.Ncurve) {
		//in >> d.Traces[nn];
                in >> str; d.trace(nn)->setName(str);
                in >> b; d.trace(nn)->setZeroAxis(b);
		in >> b; d.setAverageLabel(b,nn);
		in >> b; d.setUnitLabel(b,nn);
		in >> b; d.setMinLabel(b,nn);
		in >> b; d.setMaxLabel(b,nn);
		in >> b; d.setPPLabel(b,nn);
		in >> b; d.setRMSLabel(b,nn);
		in >> b; d.setTraceLabel(b,nn);
                in >> dd; d.trace(nn)->setOffset(dd);
                in >> dd; d.trace(nn)->setDy(dd);
                in >> c; d.trace(nn)->setColor(c);
                in >> a; d.trace(nn)->setWidth((int)a);
                in >> b; d.trace(nn)->show(b);


	  } else {
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

	  }
	}

	
	return in;
}

////**
//* @brief Initialise PlottingScopeDataThread
//*/
// void PlottingScopeDataThread::start(QRL_ScopeWindow* scopewindow)
// {
// 	ScopeWindow=scopewindow;
// 	isRunning=1;
// 	ScopeData = new Scopes_Data_T[ScopeWindow->Ncurve];
// 	for (unsigned int j=0;j<ScopeWindow->Ncurve;j++){
// 		ScopeData[j].d_y = new double[ScopeWindow->NDataMax];
// 		ScopeData[j].d_yt = new double[ScopeWindow->NDataMax];
// 		ScopeData[j].d_yt2 = new double[ScopeWindow->NDataMax];
// 		ScopeData[j].d_yempty = new double[ScopeWindow->NDataMax];
// 		ScopeData[j].time=0;
// 		for (unsigned int i = 0; i< ScopeWindow->NDataMax; i++){
// 			ScopeData[j].d_y[i]=0.;
// 			ScopeData[j].d_yempty[i]=0.;
// 			ScopeData[j].d_yt[i]=0.;
// 			ScopeData[j].d_yt2[i]=0.;
// 		}
//     		ScopeData[j].yt=1;
// 		//double * temp=ScopeWindow->ScopeData[j].d_y;
// 		//delete[] ScopeWindow->ScopeData[j].d_yt;
// 		//ScopeWindow->ScopeData[j].d_y=ScopeData[j].d_y;
// 		//delete[] temp;
// 	}
// 	
// 	timer = new QTimer(this);
//         connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
//         timer->start(1./20.*1000.); //ms
// 	QThread::start();
// 
// }

// void PlottingScopeDataThread::stopThread(){
// 	isRunning=0;
// 	timer->stop();
// 	dataMutex.lock();
// 	DataReady.wakeAll();
// 	dataMutex.unlock();
// }
// void PlottingScopeDataThread::changeRefreshRate(double rr){
// 	timer->stop();
// 	timer->start(1/rr*1000.); //ms
// 	
// }

// void PlottingScopeDataThread::refresh()
// {
// 	ScopeWindow->refresh();
// // 	dataMutex.lock();
// // 	DataReady.wakeAll();
// // 	dataMutex.unlock();
// }


// void PlottingScopeDataThread::setValue( int nn, float v)
// {	
// 	int time=ScopeData[nn].time;
// // 	dataMutex.lock();
//  	if (ScopeData[nn].yt==1)
//  		ScopeData[nn].d_yt[time]=(double)v;
//  	else
//  		ScopeData[nn].d_yt2[time]=(double)v;
// // 	ScopeData[nn].time++;
// // 	dataMutex.unlock();
// 	time++;
// 	ScopeData[nn].time=time;
//  	if (ScopeData[nn].time>ScopeWindow->NDataMax){
//  		DataReady.wakeAll();
//  	}
// //         if(time>(1/ScopeWindow->dt/ScopeWindow->RefreshRate)){
// // 		time--;
// // 
// // 		if (ScopeData[nn].yt==1){
// // 		ScopeData[nn].yt=2;
// // 		for (unsigned int i = ScopeWindow->NDataSoll - 1; i > time; i-- )
// //         		ScopeWindow->ScopeData[nn].d_y[i] = ScopeWindow->ScopeData[nn].d_y[i-1-time];
// // 	
// // 		for (unsigned int i = 0; i<=time; i++)
// // 			ScopeWindow->ScopeData[nn].d_y[i]=ScopeData[nn].d_yt[time-i];
// // 		
// // 		}else{
// // 		ScopeData[nn].yt=1;
// // 		for (unsigned int i = ScopeWindow->NDataSoll - 1; i > time; i-- )
// //         		ScopeWindow->ScopeData[nn].d_y[i] = ScopeWindow->ScopeData[nn].d_y[i-1-time];
// // 	
// // 		for (unsigned int i = 0; i<=time; i++)
// // 			ScopeWindow->ScopeData[nn].d_y[i]=ScopeData[nn].d_yt2[time-i];
// // 
// // 		}
// // 		time=0;
// // 		//qwtPlot->replot();
// // 	}
// 
// 
// //         if(time>(1/ScopeWindow->dt/ScopeWindow->RefreshRate)){
// // 		time--;
// // 		for (unsigned int i = ScopeWindow->NDataSoll - 1; i > time; i-- )
// //         		ScopeData[nn].d_y[i] = ScopeData[nn].d_y[i-1-time];
// // 	
// // 		for (unsigned int i = 0; i<=time; i++)
// // 			ScopeData[nn].d_y[i]=ScopeData[nn].d_yt[time-i];
// // 		
// // 		time=0;
// // 		//qwtPlot->replot();
// // 	}
// }

////**
//* @brief starting PlottingScopeDataThread
//*/
// void PlottingScopeDataThread::run()
// {
// 	while(isRunning){
// 		dataMutex.lock();
// 		DataReady.wait(&dataMutex);
// 		for (unsigned int nn=0;nn<ScopeWindow->Ncurve;nn++){
// 		ScopeWindow->cData[nn]->setRawData(ScopeWindow->d_x, ScopeData[nn].d_yempty, ScopeWindow->NDataSoll);
// 		  int time=ScopeData[nn].time;
// 		  time--;
// 		  ScopeData[nn].time=0;
// 		  if (ScopeData[nn].yt==1){
// 		    ScopeData[nn].yt=2;
// 		    for (unsigned int i = ScopeWindow->NDataSoll - 1; i > time; i-- )
//         		ScopeData[nn].d_y[i] = ScopeData[nn].d_y[i-1-time];
// 	
// 		    for (unsigned int i = 0; i<=time; i++)
// 			ScopeData[nn].d_y[i]=ScopeData[nn].d_yt[time-i];
// 		     ScopeData[nn].yt=2;
// 		}else{
// 		ScopeData[nn].yt=1;
// 		for (unsigned int i = ScopeWindow->NDataSoll - 1; i > time; i-- )
//         		ScopeData[nn].d_y[i] = ScopeData[nn].d_y[i-1-time];
// 	
// 		for (unsigned int i = 0; i<=time; i++)
// 			ScopeData[nn].d_y[i]=ScopeData[nn].d_yt2[time-i];
//  		ScopeData[nn].yt=1;
// 		}
// 		time=0;
// 		//qwtPlot->replot();
// 		//ScopeData[nn].time=time;
// 		ScopeWindow->cData[nn]->setRawData(ScopeWindow->d_x, ScopeData[nn].d_y, ScopeWindow->NDataSoll);
// 		}
// 		
// 		
// 		dataMutex.unlock();
// 	}
// }

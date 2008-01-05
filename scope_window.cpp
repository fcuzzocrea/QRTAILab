/*
 file:		scopes_window.cpp
 describtion:
   file for the classes QRL_ScopeWindow

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

#include "scope_window.h"
#include <stdlib.h>


//
//  Initialize Scope window
//

QRL_ScopeWindow::QRL_ScopeWindow(QWidget *parent,qrl_types::Target_Scopes_T *scope,int ind)
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
    this->setWindowIcon(QIcon(QString::fromUtf8(":/icons/icons/scope_icon.xpm")));
    this->setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint );
    qwtPlot = new QwtPlot(this);
    //qwtPlot=this;
    qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));

    this->setWidget(qwtPlot);
    this->setWindowTitle(QApplication::translate("QRL_ScopeWindow", Scope->name, 0, QApplication::UnicodeUTF8));

	direction=Qt::RightToLeft;
	overwrite=false;
       yMajorTicks=10;dy=1.; yOffset=0.; 
	ymin=yOffset-0.5*(yMajorTicks*dy);
       ymax=yOffset+0.5*(yMajorTicks*dy);
      yStep=(ymax-ymin)/yMajorTicks;
	xmin=0;xmax=1;
       
       //  qwtPlot->setTitle(tr(Scope->name));
       //qwtPlot->setAxisTitle(QwtPlot::xBottom, "Time/seconds");
       qwtPlot->setAxisScale(QwtPlot::xBottom, xmin, xmax);
	//qwtPlot->axisScaleDiv(QwtPlot::xBottom)=new QwtScaleDiv(1,0);
       // qwtPlot->setAxisTitle(QwtPlot::yLeft, "Values");
       qwtPlot->setAxisScale(QwtPlot::yLeft, ymin, ymax,yStep);
       //qwtPlot->enableAxis(QwtPlot::xBottom,false);
       qwtPlot->enableAxis(QwtPlot::yLeft,false);
//     QwtPlotMarker *mY = new QwtPlotMarker();
//     mY->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
//     mY->setLineStyle(QwtPlotMarker::HLine);
//     mY->setYValue(0.0);
//     mY->attach(Scope);



       NDataMax=(int)((xmax-xmin)/Scope->dt);
       NDataSoll=100;
	
	RefreshRate=20.;
	//Werden genügend Daten bereitgestellt?
       if (NDataSoll>NDataMax)
		NDataSoll=NDataMax;
       dt=(xmax-xmin)/NDataSoll;
	//Achtung Arrayüberlauf verhindern!
       //if ((1/dt/RefreshRate)>NDataSoll)
	//	RefreshRate=1/dt/NDataSoll;
       Ncurve=Scope->ntraces;
       ScopeData = new Scopes_Data_T[Ncurve];
       cData = new QwtPlotCurve*[Ncurve];
       d_x = new double[NDataMax];
       QPen pen;
	TraceOptions = new Trace_Options_T[Ncurve];
       for (unsigned int j=0;j<Ncurve;j++){
		ScopeData[j].d_y = new double[NDataMax];
		ScopeData[j].d_yt = new double[NDataMax];
		cData[j] = new QwtPlotCurve(Scope->name);
	 	cData[j]->attach(qwtPlot);
		cData[j]->setPaintAttribute(QwtPlotCurve::PaintFiltered,true);
		TraceOptions[j].index=j;
		TraceOptions[j].lineWidth=3;
		TraceOptions[j].dy=1.;
		TraceOptions[j].offset=0.;
		
		if (j==0)
			TraceOptions[j].brush=QBrush(Qt::red);
		else
			TraceOptions[j].brush=QBrush(Qt::green);
       		pen.setWidth(TraceOptions[j].lineWidth);
		pen.setBrush(TraceOptions[j].brush);
		cData[j]->setPen(pen);
		cData[j]->setRawData(d_x, ScopeData[j].d_y, NDataSoll);

		ScopeData[j].time=0;
	}

       for (unsigned int i = 0; i< NDataSoll; i++)
    {
	//if (Scope->dt<=0.05)
        	d_x[i] =dt * i;     // time axis
	//else
	//d_x[i] =0.05 * i;     // time axis
	for (unsigned int j=0;j<Ncurve;j++){
        	ScopeData[j].d_y[i] = TraceOptions[j].offset;
		ScopeData[j].d_yt[i]=TraceOptions[j].offset;
	}
    }
    gridColor=Qt::blue;
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

	isSaving=0;Save_Time=1.0;File_Name=tr(Scope->name);
}

QRL_ScopeWindow::~QRL_ScopeWindow(){
//Plotting_Scope_Data_Thread->stopThread();
//Plotting_Scope_Data_Thread->wait();
//delete Plotting_Scope_Data_Thread;
//delete mY;
delete[] d_x;
for (unsigned int j=0;j<Ncurve;j++){
	delete[] ScopeData[j].d_y;
	delete[] ScopeData[j].d_yt;
	delete cData[j];
}
delete[] cData;
delete[] ScopeData;
delete[] TraceOptions;
}


void QRL_ScopeWindow::refresh()
{
	qwtPlot->replot();

}
   void QRL_ScopeWindow::changeRefreshRate(double rr)
{
	RefreshRate=rr;
	
	timer->stop();
	timer->start((int)(1./RefreshRate*1000.));
	
	//Plotting_Scope_Data_Thread->changeRefreshRate(RefreshRate);

}

  void QRL_ScopeWindow::setSaveTime(double time)
{
	Save_Time=time;
}

   void QRL_ScopeWindow::changeDataPoints(double dp)
{
	NDataSoll=(int)dp;
	if (NDataSoll>NDataMax)
		NDataSoll=NDataMax;
       dt=(xmax-xmin)/NDataSoll;
       timer->stop();
    for (unsigned int i = 0; i< NDataSoll; i++)
    {
	//if (Scope->dt<=0.05)
        d_x[i] =dt * i;     // time axis
	for (unsigned int j=0;j<Ncurve;j++){
        	ScopeData[j].d_y[i] = TraceOptions[j].offset;
		ScopeData[j].d_yt[i]=TraceOptions[j].offset;
	}
    }
     for (unsigned int j=0;j<Ncurve;j++){
	cData[j]->setRawData(d_x, ScopeData[j].d_y, NDataSoll);
     }
     timer->start((int)(1./RefreshRate*1000.));
}


   void QRL_ScopeWindow::changeDX(double v)
{
	if (v>0.)
		dx=v;
	xmax=dx;
	//timer->stop();
	NDataMax=(int)((xmax-xmin)/Scope->dt);
	if (NDataSoll>NDataMax)
		NDataSoll=NDataMax;
	dt=(xmax-xmin)/NDataSoll;
	delete[] d_x;
	for (unsigned int j=0;j<Ncurve;j++){
		delete[] ScopeData[j].d_y;
		delete[] ScopeData[j].d_yt;
	}
	 d_x = new double[NDataMax];
       for (unsigned int j=0;j<Ncurve;j++){
		ScopeData[j].d_y = new double[NDataMax];
		ScopeData[j].d_yt = new double[NDataMax];
		cData[j]->setRawData(d_x, ScopeData[j].d_y, NDataSoll);
	}
	for (unsigned int i = 0; i< NDataSoll; i++)
    	{
	//if (Scope->dt<=0.05)
        	d_x[i] =dt * i;     // time axis
    	}
	qwtPlot->setAxisScale(QwtPlot::xBottom, xmin, xmax);
	//timer->start(1/RefreshRate*1000.);
}


void QRL_ScopeWindow::setGridColor(QColor gridcolor){
	gridColor=gridcolor;
	grid->setMajPen(QPen(gridColor, 0, Qt::DotLine));
}
void QRL_ScopeWindow::setBgColor(QColor bgcolor){
	bgColor=bgcolor;
	qwtPlot->setCanvasBackground(bgColor);
}

void QRL_ScopeWindow::startSaving(QString filename){
	if (isSaving==0){
		File_Name=filename;
		if (QFile::exists(File_Name)) {
			printf("File %s exists already.",File_Name.toLocal8Bit().data() );
			QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("The File exists! Please change the name!"),
                                     QMessageBox::Abort);
			stopSaving(index);
		} else {
		isSaving=1;
		if ((Save_File_Pointer = fopen((File_Name.toLocal8Bit()).data(), "a+")) == NULL) {
			printf("Error in opening file %s",File_Name.toLocal8Bit().data() );
			isSaving=0;
		}
		}
	}
}

int QRL_ScopeWindow::n_points_to_save(){
	int n_points;

	n_points = (int)(Save_Time/Scope->dt);
	if (n_points < 0) return 0;
	return n_points;
}

void QRL_ScopeWindow::stop_saving(){
	isSaving=0;
	fclose(Save_File_Pointer);
	stopSaving(index);
	
}

void QRL_ScopeWindow::setTraceColor(const QColor& c,int trace)
{
	if (trace<Scope->ntraces){
		QPen pen;
		pen.setBrush(c);
		TraceOptions[trace].brush=QBrush(c);
		pen.setWidth(TraceOptions[trace].lineWidth);
		cData[trace]->setPen(pen);
	}
}

void QRL_ScopeWindow::setTraceWidth(int traceWidth,int trace)
{
	if (trace<Scope->ntraces){
		QPen pen;
		pen.setBrush(TraceOptions[trace].brush);
		pen.setWidth(traceWidth);
		TraceOptions[trace].lineWidth=traceWidth;
		cData[trace]->setPen(pen);
	}
}

int  QRL_ScopeWindow::getTraceWidth(int trace)
{
	if (trace<Scope->ntraces){
		return cData[trace]->pen().width();
	}
	return 1;
}

   void QRL_ScopeWindow::setTraceOffset(double o,int trace)
{
	if (trace<Scope->ntraces){
		TraceOptions[trace].offset=o;
	}

}

double  QRL_ScopeWindow::getTraceOffset(int trace)
{
	if (trace<Scope->ntraces){
		return TraceOptions[trace].offset;
	}
	return 0.;
}
   void QRL_ScopeWindow::setTraceDy(double d, int trace){

	if (trace<Scope->ntraces){
		TraceOptions[trace].dy=d;
	}

}

double  QRL_ScopeWindow::getTraceDy(int trace)
{
	if (trace<Scope->ntraces){
		return TraceOptions[trace].dy;
	}
	return 1.;
}

void QRL_ScopeWindow::setValue( int nn, float v)
{			

// 	for (unsigned int j = 0; j<Ncurve; j++)
// 		cData[j]->setRawData(d_x, temp[j], time);
		//ScopeData[j].d_y=temp[j];
	
/*
	if (time2>=(dt/Scope->dt)){
	if (nn==Ncurve-1) 
		time2=1;
	ScopeData[nn].d_yt[time]=(double)v;
	//Scope->setChecked(false)
	if (nn==Ncurve-1) {
	time++;
        if(time>(1/dt/RefreshRate)){
		time--;
		for (unsigned int j = 0; j<Ncurve; j++){
		for (unsigned int i = NDataSoll - 1; i > time; i-- )
        		ScopeData[j].d_y[i] = ScopeData[j].d_y[i-1-time];
	
		for (unsigned int i = 0; i<=time; i++)
			ScopeData[j].d_y[i]=ScopeData[j].d_yt[time-i];
		}
		time=0;
		//qwtPlot->replot();
	}
	}
	//time++;
	}
	if (nn==Ncurve-1) 
		time2++;
*/

//int time2=ScopeData[nn].time2;
//if (time2>=(dt/Scope->dt)){
//	time2=0;

if (overwrite)
{
	int time=ScopeData[nn].time;
	ScopeData[nn].d_y[time]=((double)v+TraceOptions[nn].offset)/TraceOptions[nn].dy;
	if (Qt::LeftToRight==direction)
		time++;
	else
		time--;
	if (time<0)
		time=NDataSoll-1;
	if(time>NDataSoll-1)
		time=0;
	ScopeData[nn].time=time;


}else{
	int time=ScopeData[nn].time;
	ScopeData[nn].d_yt[time]=((double)v+TraceOptions[nn].offset)/TraceOptions[nn].dy;
	time++;
        if(time>(1/dt/RefreshRate)){
		time--;
		if (Qt::LeftToRight==direction){

		for ( int i = NDataSoll - 1; i > time; i-- ){
        		ScopeData[nn].d_y[i] = ScopeData[nn].d_y[i-1-time];
		}
		for ( int i = 0; i<=time; i++){
			ScopeData[nn].d_y[i]=ScopeData[nn].d_yt[time-i];
			//ScopeData[nn].d_y[i]=(double)temp[nn][time-i];
		}
		} else { //right to left
		for ( int i = 0; i < NDataSoll-time-1; i++ ){
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
}
//}
//	time2++;
//	ScopeData[nn].time2=time2;
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

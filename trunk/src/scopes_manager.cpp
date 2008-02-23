/*
 file:		scopes_manager.cpp
 describtion:
   file for the classes GetScopeDataThread and  QRL_ScopesManager

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

#include "scopes_manager.h"


/**
* @brief Initialize Scopes Manager
*/
QRL_ScopesManager::QRL_ScopesManager(QWidget *parent,TargetThread* targetthread)
	:QDialog(parent),targetThread(targetthread)
{
	setupUi(this);
	Num_Scopes=targetThread->getScopeNumber();
	Scopes=targetThread->getScopes();
	const QIcon ScopeIcon =QIcon(QString::fromUtf8(":/icons/scope_icon.xpm"));
	ScopeWindows = new QRL_ScopeWindow* [Num_Scopes]; 
	for (int i=0; i<Num_Scopes; ++i){
		scopeItems << new QListWidgetItem(ScopeIcon,tr(Scopes[i].name), scopeListWidget);
		ScopeWindows[i]=new QRL_ScopeWindow(parent,&Scopes[i],i);
		connect( ScopeWindows[i], SIGNAL( stopSaving(int) ), this, SLOT( stopSaving(int) ) );
	}
	tabWidget->setCurrentIndex(0);
	connect( showCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( showScope(int) ) );
	connect( scopeListWidget, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( showOptions( QListWidgetItem *  ) ) );
	connect( scopeListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( showOptions( QListWidgetItem *  ) ) );
	connect( scopeListWidget, SIGNAL( 
itemSelectionChanged() ), this, SLOT( showSelectedOptions() ) );
	//connect( rrLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeRefreshRate(const QString&) ) );
	connect( rrCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeRefreshRate(double) ) );
	connect( dataCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeDataPoints(double) ) );
	connect( timeCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeSaveTime(double) ) );
	connect( dxComboBox, SIGNAL( currentIndexChanged(const QString &) ), this, SLOT( changeDX(const QString&) ) );
	connect( dxComboBox, SIGNAL( editTextChanged(const QString &) ), this, SLOT( changeDX(const QString&) ) );
	connect( savePushButton, SIGNAL( pressed() ), this, SLOT( startSaving() ) );
        connect( displayComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDisplayModus(int) ) );
	connect( directionComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDirection(int) ) );
	connect( optionComboBox, SIGNAL( activated(int) ), this, SLOT( setOptions(int) ) );
	connect( traceColorPushButton, SIGNAL( pressed()), this, SLOT(changeTraceColor()));
	connect( lineWidthCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeTraceWidth(double) ) );
	connect( offsetCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeOffset(double) ) );
	connect( offsetWheel, SIGNAL( valueChanged(double) ), this, SLOT( changeOffset(double) ) );
        connect( dyCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeDy(double) ) );
	connect( tabWidget, SIGNAL( currentChanged(int) ), this, SLOT( changeScopeList(int) ) );
	currentScope=0;
// 	for(int i=0; i<1; ++i){
// 		//tabWidget->addTab(new QWidget(tabWidget->widget(1)),tr("Trace ")+tr("%1").arg(i+1));
// 		traceComboBox->addItem(tr("Trace ")+tr("%1").arg(i+1));
// 	}
	if (Num_Scopes > 0) emit showScopeOptions(currentScope);
	
	if (Num_Scopes > 0) Get_Scope_Data_Thread = new GetScopeDataThread [Num_Scopes];
		offsetWheel->setMass(0.5);
    offsetWheel->setRange(-1e6, 1e6, 0.25);
    offsetWheel->setTotalAngle(360.0*2e6);
    offsetWheel->setFixedHeight(30);
	dxComboBox->setCompleter(0);
	dxComboBox->setValidator(new QDoubleValidator(this));
	
//	tabWidget->addTab(traceWidget,tr("trace %1").arg(Scopes[currentScope].ntraces));

}
QRL_ScopesManager::~QRL_ScopesManager()
{
	for (int i=0; i<Num_Scopes; ++i){
		ScopeWindows[i]->hide();
	}
	delete[] ScopeWindows;
	stopScopeThreads();
	if (Get_Scope_Data_Thread)
		delete[] Get_Scope_Data_Thread;
}
/**
* @brief starting all scope threads
*/
void QRL_ScopesManager::startScopeThreads()
{
	for (int n = 0; n < Num_Scopes; n++) {
		//unsigned int msg;
		Args_T thr_args;
		thr_args.index = n;
		thr_args.mbx_id = strdup((targetThread->getPreferences()).Target_Scope_Mbx_ID);
		thr_args.x = 500; 
		thr_args.y = 290;
		thr_args.w = 250;
		thr_args.h = 250;
		Get_Scope_Data_Thread[n].mutex.lock();
		Get_Scope_Data_Thread[n].start(&thr_args,targetThread,ScopeWindows[n]);
		Get_Scope_Data_Thread[n].threadStarted.wait(&Get_Scope_Data_Thread[n].mutex);
		Get_Scope_Data_Thread[n].mutex.unlock();
		//rt_receive(0, &msg);
		//((QMainWindow*)mainWindow)->addDockWidget(Qt::NoDockWidgetArea,ScopeWindows[n]);
		ScopeWindows[n]->hide();
	}
	Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
}

/**
* @brief stopping all existing scope threads
*/
void QRL_ScopesManager::stopScopeThreads()
{
	for (int n = 0; n < Num_Scopes; n++) {
		Get_Scope_Data_Thread[n].wait();
	}

}
/**
* @brief set new refresh rate
* @param rr refresh rate
*/
void QRL_ScopesManager::changeRefreshRate(double rr)
{
	//double rr=text.toDouble();
	ScopeWindows[currentScope]->changeRefreshRate(rr);
}
void QRL_ScopesManager::changeDX(const QString& text)
{
	if (!text.isEmpty() &&text.toDouble()!=0.0 ){
	double dx=text.toDouble();
	ScopeWindows[currentScope]->changeDX(dx);
	Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
	}
}

/**
* @brief change the number of datapoints in the plot
* @param dp data points
*/
void QRL_ScopesManager::changeDataPoints(double dp)
{
	//double rr=text.toDouble();
	ScopeWindows[currentScope]->changeDataPoints(dp);
	Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
	if (Num_Scopes > 0) emit showScopeOptions(currentScope);

}
void QRL_ScopesManager::changeSaveTime(double time)
{
	//double rr=text.toDouble();
	ScopeWindows[currentScope]->setSaveTime(time);

}

void QRL_ScopesManager::setOptions(int index)
{
	QColor color;
	switch(index) {
		case 1	:
		color= QColorDialog::getColor();
		ScopeWindows[currentScope]->setBgColor(color);
		break;
		case 2 :
		color= QColorDialog::getColor();
		ScopeWindows[currentScope]->setGridColor(color);
		break;
		default	:;
	}
}

void QRL_ScopesManager::startSaving()
{
	if(ScopeWindows[currentScope]->start_saving()==0){
		savePushButton->setEnabled(false);
		ScopeWindows[currentScope]->startSaving(fileLineEdit->text());
	}
}

void QRL_ScopesManager::stopSaving(int index)
{
	if(currentScope==index){
		savePushButton->setEnabled(true);
	}
}

void QRL_ScopesManager::changeScopeList(int index)
{
	if (index==0){
		scopeListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
		for(int i=0; i<scopeItems.size();i++)
			scopeItems[i]->setHidden(false);
		for(int i=0; i<traceItems.size();i++)
			traceItems[i]->setHidden(true);
		
	}else{

	//scopeListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
	for(int i=0; i<scopeItems.size();i++)
		scopeItems[i]->setHidden(true);
	if (traceItems.size()==0) {
		for(int i=0; i<Scopes[currentScope].ntraces;i++)
			traceItems << new QListWidgetItem(QIcon(),tr("trace %1").arg(i+1), scopeListWidget);
	} else {
		for(int i=0; i<traceItems.size();i++)
			traceItems[i]->setHidden(false);
	}

	}
}



void QRL_ScopesManager::showTraceOptions(int index)
{
	currentTrace=index;	lineWidthCounter->setValue(ScopeWindows[currentScope]->getTraceWidth(currentTrace));
	offsetCounter->setValue(ScopeWindows[currentScope]->getTraceOffset(currentTrace));
	dyCounter->setValue(ScopeWindows[currentScope]->getTraceDy(currentTrace));
	
		
}


/**
* @brief update manager dialog for the choosen scope
* @param item scope number
*/




void QRL_ScopesManager::showScopeOptions( int index ){

	currentScope=index;
	if(ScopeWindows[currentScope]->start_saving()==0){
		savePushButton->setEnabled(true);
	}
	tabWidget->setTabText(0,tr(Scopes[currentScope].name));
	fileLineEdit->setText(tr(Scopes[currentScope].name));
	if(ScopeWindows[currentScope]->isVisible())
		showCheckBox->setCheckState(Qt::Checked);
	else
		showCheckBox->setCheckState(Qt::Unchecked);
	rrCounter->setValue(ScopeWindows[currentScope]->getRefreshRate());
	dataCounter->setValue(ScopeWindows[currentScope]->getDataPoints());
	currentTrace=0;
	for(int i=0; i<traceItems.size();i++)
		delete traceItems[i];
	traceItems.clear();
	for(int i=0; i<Scopes[currentScope].ntraces;i++){
		traceItems << new QListWidgetItem(QIcon(),tr("trace %1").arg(i+1), scopeListWidget);
		traceItems[i]->setHidden(true);
	}

}

void QRL_ScopesManager::showOptions(QListWidgetItem * item )
{
	int index=scopeListWidget->row(item);
	if(tabWidget->currentIndex()==0){
		showScopeOptions(index);
	}else{
		showTraceOptions(index);
	}

}
void QRL_ScopesManager::showSelectedOptions()
{

}

/**
* @brief show scope windows
* @param state set display status
*/
void QRL_ScopesManager::showScope(int state) 
{
	if(state==Qt::Checked){
		ScopeWindows[currentScope]->show();
	} else {
		ScopeWindows[currentScope]->hide();
	}

}

void QRL_ScopesManager::changeDirection(int d)
{
	switch(d)
	{
	case 0:	
		ScopeWindows[currentScope]->setDirection(Qt::RightToLeft);
		break;
	case 1:
		
		ScopeWindows[currentScope]->setDirection(Qt::LeftToRight);
		break;
	default:
		break;
	}

}

void QRL_ScopesManager::changeDisplayModus(int mode)
{
	switch(mode)
	{
	case 0:	
		ScopeWindows[currentScope]->setDirection(Qt::RightToLeft);
		ScopeWindows[currentScope]->setOverwriteMode(false);
		emit directionComboBox->setCurrentIndex(0);
		break;
	case 1:
		ScopeWindows[currentScope]->setDirection(Qt::LeftToRight);
		ScopeWindows[currentScope]->setOverwriteMode(true);
		emit directionComboBox->setCurrentIndex(1);
		break;
	default:
		break;
	}

}


void QRL_ScopesManager::changeTraceColor()
{
	QColor color= QColorDialog::getColor();
	ScopeWindows[currentScope]->setTraceColor(color,currentTrace);
}

void QRL_ScopesManager::changeTraceWidth(double v)
{
	int traceWidth=(int)v;
	ScopeWindows[currentScope]->setTraceWidth(traceWidth,currentTrace);
}

void QRL_ScopesManager::changeOffset(double offset)
{
	ScopeWindows[currentScope]->setTraceOffset(offset,currentTrace);
	offsetWheel->setValue(offset);
	offsetCounter->setValue(offset);
	
}

void QRL_ScopesManager::changeDy(double dy)
{
	ScopeWindows[currentScope]->setTraceDy(dy,currentTrace);
}
/**
* @brief Initialise GetScopeDataThread
* @param arg 
* @param targetthread pointer to TargetThread
* @param scopewindow pointer to QRL_ScopeWindow
*/
void GetScopeDataThread::start(void* arg,TargetThread* targetthread,QRL_ScopeWindow* scopewindow)
{
	targetThread=targetthread;
	ScopeWindow=scopewindow;
	index = ((Args_T *)arg)->index;
	mbx_id = strdup(((Args_T *)arg)->mbx_id);
	x = ((Args_T *)arg)->x;
	y = ((Args_T *)arg)->y;
	w = ((Args_T *)arg)->w;
	h = ((Args_T *)arg)->h;
	dt=1./100;
	MsgData = 0;
	QThread::start();
}

/**
* @brief set new dt
* @param d points per second
*/
int GetScopeDataThread::setDt(double d)
{
	if (d>0.){
		dt=d;
		MsgLen = (((int)(TracesBytes*dt*(1./targetThread->getScopes()[index].dt)))/TracesBytes)*TracesBytes;
		if (MsgLen < TracesBytes) MsgLen = TracesBytes;
		if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
		MsgData = MsgLen/TracesBytes;
		// Ndistance defines the distance between plotted datapoints, to archive the given refresh rate.
		Ndistance=(long int)(dt/(targetThread->getScopes())[index].dt);

		if (Ndistance<1)
			Ndistance=1;
		return Ndistance; //TODO long int > int
	}
	return -1;
}

/**
* @brief starting GetScopeDataThread
*/
void GetScopeDataThread::run()
{
   	RT_TASK *GetScopeDataTask;
	MBX *GetScopeDataMbx;
	char GetScopeDataMbxName[7];
	long GetScopeDataPort;
	float MsgBuf[MAX_MSG_LEN/sizeof(float)];
	int  nn, js, jl;
	long int n;
	int stop_draw = false;
	int save_idx = 0;

	rt_allow_nonroot_hrt();
	if (!(GetScopeDataTask = rt_task_init_schmod(qrl::get_an_id("HGS"), 98, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetScopeData Task\n");
		//return (void *)1;
	}
	if(targetThread->getTargetNode() == 0) GetScopeDataPort = 0;
	else GetScopeDataPort = rt_request_port(targetThread->getTargetNode());
	sprintf(GetScopeDataMbxName, "%s%d", mbx_id, index);
	if (!(GetScopeDataMbx = (MBX *)RT_get_adr(targetThread->getTargetNode(), GetScopeDataPort, GetScopeDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetScopeDataMbxName);
		//return (void *)1;
	}
	TracesBytes = (targetThread->getScopes()[index].ntraces + 1)*sizeof(float);
	MaxMsgLen = (MAX_MSG_LEN/TracesBytes)*TracesBytes;
	MsgLen = (((int)(TracesBytes*dt*(1./targetThread->getScopes()[index].dt)))/TracesBytes)*TracesBytes;
	//MsgLen = (((int)(TracesBytes*REFRESH_RATE*(1./targetThread->getScopes()[index].dt)))/TracesBytes)*TracesBytes;
	if (MsgLen < TracesBytes) MsgLen = TracesBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/TracesBytes;

	// Ndistance defines the distance between plotted datapoints, to archive the given refresh rate.
	Ndistance=(long int)(dt/(targetThread->getScopes())[index].dt);
	int ntraces=(targetThread->getScopes())[index].ntraces;
	//Fl_Scope_Window *Scope_Win = new Fl_Scope_Window(x, y, w, h, RLG_Main_Workspace->viewport(), Scopes[index].name, Scopes[index].ntraces, Scopes[index].dt);
	//Scopes_Manager->Scope_Windows[index] = Scope_Win;
// 	float**   temp1 = new float*[ntraces];
// 	for (unsigned int j=0;j<ntraces;j++)
// 		temp1[j] = new float[10000];
// 	double**   temp2 = new double*[ntraces];
// 	for (unsigned int j=0;j<ntraces;j++){
// 		temp2[j] = new double[10000];
// 		for (int i=0; i<10000;i++)
// 			temp2[j][i]=0.;	
// 	}
	mutex.lock();
	threadStarted.wakeAll();
	mutex.unlock();
	//rt_send(Target_Interface_Task, 0);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	n=Ndistance;
	RTIME t0,t,told; int time=0; double time10=0.;double t10;int n10=0;
	t0 = rt_get_cpu_time_ns();
	while (true) {
		if (targetThread->getEndApp() || ! targetThread->getIsTargetConnected()) break;
		while (RT_mbx_receive_if(targetThread->getTargetNode(), GetScopeDataPort, GetScopeDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;
			rt_sleep(nano2count(TEN_MS_IN_NS));
			//msleep(10);
		}
		//Fl::lock();
// 		js = 1;
// 		for (n = 0; n < MsgData; n++) {
// 			for (nn = 0; nn < (targetThread->getScopes())[index].ntraces; nn++) {
// 			//scopeMutex.lock();
// 			//targetThread->setScopeValue(index,nn,MsgBuf[js++]);
// 			
// 			//ScopeWindow->getThread()->setValue(nn,MsgBuf[js++]);
// 			ScopeWindow->setValue(nn,MsgBuf[js++]);
// 			//scopeMutex.unlock();
// 			}
// 			js++;
// 		} 
		if (targetThread->getVerbose()){
		t = rt_get_cpu_time_ns();told=t0;
		t0=rt_get_cpu_time_ns();
		if (n10==1000) {
			printf("Time failure soll %f zu ist: %f \n",time10,(t10));
			n10=0;
			t10=0.;
			time10=0.;
		} else {
			n10++;
			t10+=(t - told)*1.0E-6;
			time10+=time*dt*1.0E3;
		}
		if (((t - told)*1.0E-6)>25.)
		   printf("Time failure  %f (%f)\n",(t - told)*1.0E-6,time*dt*1.0E3 );
		}
		time=0;
		if (n>MsgData)
			n=n-MsgData;
		else {
                  
		   int i; js = (n-1)*(ntraces+1)+1;
		   if (n==1)
			js=1;
		   while(n<=MsgData){
			for (nn = 0; nn < ntraces; nn++) {
			 if (ScopeWindow)
			    ScopeWindow->setValue(nn,MsgBuf[js++]);
				//temp1[nn][time-1]=MsgBuf[js++];
			    //ScopeWindow->getThread()->setValue(nn,MsgBuf[js++]);
			} time++;
			js=js+(Ndistance-1)*(ntraces+1)+1;
			n=n+Ndistance;
		   }
		   n=n-MsgData;
		}
		

// 		if(time>(1./dt/20.)){
// 			time--;
// 		for (int nn = 0; nn < ntraces; nn++) {
// 		for (unsigned int i = NDataSoll - 1; i > time; i-- ){
//         		temp2[nn][i] = temp2[nn][i-1-time];
// 		}
// 		for (unsigned int i = 0; i<=time; i++){
// 			temp2[nn][i]=(double)temp1[nn][time-i];
// 		}
// 		}
// 			time=0;
// 			if (onetime==1){
// 				ScopeWindow->setValue(ntraces,temp2,NDataSoll);
// 				onetime=0;
// 			}
// 		}
		
		//t0=rt_get_cpu_time_ns();


		//if (Scope_Win->is_visible() && (!stop_draw)) {
		//	Scope_Win->Plot->redraw();
		//}
		if (ScopeWindow->start_saving()) {
			jl = 0;
			printf("%d from %d saved\n",save_idx,ScopeWindow->n_points_to_save());
			for (n = 0; n < MsgData; n++) {
				for (nn = 0; nn < ntraces + 1; nn++) {
					fprintf(ScopeWindow->save_file(), "%1.10f ", MsgBuf[jl++]);
				}
				fprintf(ScopeWindow->save_file(), "\n");
				save_idx++;
				if (save_idx == ScopeWindow->n_points_to_save()) {
					ScopeWindow->stop_saving();
					save_idx = 0;
					break;
				}
			}
		}
		//Fl::unlock();
	}

end:

	if (targetThread->getVerbose()) {
		printf("Deleting scope thread number...%d\n", index);
	}
// 	for (unsigned int j=0;j<ntraces;j++){
// 		delete[] temp1[j];
// 		delete[] temp2[j];
// 	}
// 	delete[] temp1;
// 	delete[] temp2;
	//Scope_Win->hide();
	rt_release_port(targetThread->getTargetNode(), GetScopeDataPort);
	rt_task_delete(GetScopeDataTask);


}








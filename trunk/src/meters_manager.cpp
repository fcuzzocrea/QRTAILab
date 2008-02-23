/*
 file:		meters_manager.cpp
 describtion:
   file for the classes GetMeterDataThread and  QRL_MetersManager

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

#include "meters_manager.h"

extern unsigned long qrl::get_an_id(const char *root);


///  Initialize Meters Manager
QRL_MetersManager::QRL_MetersManager(QWidget *parent,TargetThread* targetthread)
	:QDialog(parent),targetThread(targetthread)
{
	setupUi(this);
	Num_Meters=targetThread->getMeterNumber();
	Meters=targetThread->getMeters();
	const QIcon MeterIcon =QIcon(QString::fromUtf8(":/icons/meter_icon.xpm"));
	MeterWindows = new QRL_MeterWindow* [Num_Meters]; 
	for (int i=0; i<Num_Meters; ++i){
		new QListWidgetItem(MeterIcon,tr((Meters)[i].name), meterListWidget);
		MeterWindows[i]=new QRL_MeterWindow(parent,Meters[i].name);
	}
	connect( showCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( showMeter(int) ) );
	connect( rrCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeRefreshRate(double) ) );
	connect( minCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeMin(double) ) );
	connect( maxCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeMax(double) ) );
	connect( meterListWidget, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( showMeterOptions( QListWidgetItem *  ) ) );
	connect( meterListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( showMeterOptions( QListWidgetItem *  ) ) );
	connect( meterComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeMeter(int) ) );
	
	if (Num_Meters > 0) Get_Meter_Data_Thread = new GetMeterDataThread [Num_Meters];
	if (Num_Meters > 0)
	emit showMeterOptions(meterListWidget->item(0));
	ThermoOptions = tabWidget->widget(1);
	DialOptions = tabWidget->widget(2);
	LcdOptions = tabWidget->widget(3);
	connect( thermoColor1PushButton, SIGNAL( pressed() ), this, SLOT( changeThermoColor1() ) );
	connect( thermoColor2PushButton, SIGNAL( pressed() ), this, SLOT( changeThermoColor2() ) );
	connect( pipeWithCounter, SIGNAL( valueChanged(double) ), this, SLOT( changePipeWith(double) ) );
	connect( alarmThermoColor1PushButton, SIGNAL( pressed() ), this, SLOT( changeAlarmThermoColor1() ) );
	connect( alarmThermoColor2PushButton, SIGNAL( pressed() ), this, SLOT( changeAlarmThermoColor2() ) );
	connect( alarmCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( enableThermoAlarm(int) ) );
	connect( colorComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeThermoColorType(int) ) );
	connect( alarmLevelCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeAlarmLevel(double) ) );
	connect( directionComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeThermoDirection(int) ) );
	currentMeter=0;
	switch(MeterWindows[currentMeter]->getMeterType())
	{
		case QRL_MeterWindow::THERMO:
			tabWidget->removeTab(3);
			tabWidget->removeTab(2);
	
			
			break;
		case QRL_MeterWindow::DIAL:
			tabWidget->removeTab(1);
			tabWidget->removeTab(3);
			break;
		case QRL_MeterWindow::LCD:
			tabWidget->removeTab(1);
			tabWidget->removeTab(2);
			break;
		default:break;
	}
	tabWidget->setCurrentIndex(0);
}



QRL_MetersManager::~QRL_MetersManager()
{
	for (int i=0; i<Num_Meters; ++i){
		MeterWindows[i]->hide();
	}
	delete[] MeterWindows;
	stopMeterThreads();
	if (Get_Meter_Data_Thread)
		delete[] Get_Meter_Data_Thread;
}

/**
* @brief starting all meter threads
*/
void QRL_MetersManager::startMeterThreads()
{
	for (int n = 0; n < Num_Meters; n++) {
		//unsigned int msg;
		Args_T thr_args;
		thr_args.index = n;
		thr_args.mbx_id = strdup((targetThread->getPreferences()).Target_Meter_Mbx_ID);
		thr_args.x = 0; 
		thr_args.y = 0;
		thr_args.w = 300;
		thr_args.h = 200;
		//pthread_create(&Get_Meter_Data_Thread[n], NULL, rt_get_meter_data, &thr_args);
		Get_Meter_Data_Thread[n].mutex.lock();
		Get_Meter_Data_Thread[n].start(&thr_args,targetThread,MeterWindows[n]);
		//wait until thread is initialized
		Get_Meter_Data_Thread[n].threadStarted.wait(&Get_Meter_Data_Thread[n].mutex);
		Get_Meter_Data_Thread[n].mutex.unlock();
		//rt_receive(0, &msg);
	}

}
/**
* @brief stopping all existing meter threads
*/
void QRL_MetersManager::stopMeterThreads()
{
	for (int n = 0; n < Num_Meters; n++) {
		Get_Meter_Data_Thread[n].wait();
	}

}

/**
* @brief send new refresh rate to the Meter Thread
* @param rr refresh rate
*/
 void QRL_MetersManager::changeRefreshRate(double rr)
{
	//double rr=text.toDouble();
	//ScopeWindows[currentScope]->changeRefreshRate(rr);
	rrCounter->setValue(rr);
	Get_Meter_Data_Thread[currentMeter].setRefreshRate(rr);
	MeterWindows[currentMeter]->changeRefreshRate(rr);
}
/**
* @brief changes the minimum of the scale
* @param min minimal value
*/
void QRL_MetersManager::changeMin(double min)
{
	//double rr=text.toDouble();
	MeterWindows[currentMeter]->setMin(min);
}
/**
* @brief changes the maximum of the scale
* @param max maximal value
*/
void QRL_MetersManager::changeMax(double max)
{
	//double rr=text.toDouble();
	MeterWindows[currentMeter]->setMax(max);

}
/**
* @brief changes between thermo and dial
* @param type meter typ
*/
void QRL_MetersManager::changeMeter(int type)
{	
	switch(type)
	{
		case 0: //thermo
			tabWidget->removeTab(1);
			tabWidget->addTab(ThermoOptions,tr("Options"));
			MeterWindows[currentMeter]->setMeter(QRL_MeterWindow::THERMO);
			break;
		case 1: //dial
			tabWidget->removeTab(1);
			tabWidget->addTab(DialOptions,tr("Options"));
			MeterWindows[currentMeter]->setMeter(QRL_MeterWindow::DIAL);
			break;
		case 2: //lcd
			tabWidget->removeTab(1);
			tabWidget->addTab(LcdOptions,tr("Options"));
			MeterWindows[currentMeter]->setMeter(QRL_MeterWindow::LCD);
			break;
		default:break;
	}
	
}

void QRL_MetersManager::changeThermoColor1()
{
	QColor color= QColorDialog::getColor();
	MeterWindows[currentMeter]->setThermoColor1(color);

}

void QRL_MetersManager::changeThermoColor2()
{
	QColor color= QColorDialog::getColor();
	MeterWindows[currentMeter]->setThermoColor2(color);

}

void QRL_MetersManager::changePipeWith(double w)
{
	MeterWindows[currentMeter]->setPipeWith(w);
}

void QRL_MetersManager::changeAlarmThermoColor1()
{
	QColor color= QColorDialog::getColor();
	MeterWindows[currentMeter]->setAlarmThermoColor1(color);

}

void QRL_MetersManager::changeAlarmThermoColor2()
{
	QColor color= QColorDialog::getColor();
	MeterWindows[currentMeter]->setAlarmThermoColor2(color);

}
void QRL_MetersManager::enableThermoAlarm(int state) 
{
	MeterWindows[currentMeter]->setThermoAlarm(state);
}

void QRL_MetersManager::changeThermoColorType(int colorType)
{
	switch(colorType){
	case 0:
		alarmThermoColor2PushButton->setEnabled(true);
		thermoColor2PushButton->setEnabled(true);
		MeterWindows[currentMeter]->setGradientEnabled(true);
		break;
	case 1:
		alarmThermoColor2PushButton->setEnabled(false);
		thermoColor2PushButton->setEnabled(false);
		MeterWindows[currentMeter]->setGradientEnabled(false);
		break;
	default:
		break;
	}
}
void QRL_MetersManager::changeAlarmLevel(double level)
{
	MeterWindows[currentMeter]->setAlarmLevel(level);
}

void QRL_MetersManager::changeThermoDirection(int d)
{
	switch(d){
	case 0:
		MeterWindows[currentMeter]->setThermoDirection(Qt::Vertical);
		break;
	case 1:
		MeterWindows[currentMeter]->setThermoDirection(Qt::Horizontal);
		break;
	default:
		break;
	}
}

/**
* @brief update manager dialog for the choosen meter
* @param item meter number
*/
void QRL_MetersManager::showMeterOptions( QListWidgetItem * item ){

	currentMeter= meterListWidget->row(item);
	tabWidget->setTabText(0,tr(Meters[currentMeter].name));
	if(MeterWindows[currentMeter]->isVisible())
		showCheckBox->setCheckState(Qt::Checked);
	else
		showCheckBox->setCheckState(Qt::Unchecked);
}

/**
* @brief show meter windows
* @param state set display status
*/
void QRL_MetersManager::showMeter(int state) 
{
	if(state==Qt::Checked){
		MeterWindows[currentMeter]->show();
	} else {
		MeterWindows[currentMeter]->hide();
	}
}

/**
* @brief Initialise GetMeterDataThread
* @param arg 
* @param targetthread pointer to TargetThread
* @param meterwindow pointer to QRL_MeterWindow
*/
void GetMeterDataThread::start(void* arg,TargetThread* targetthread,QRL_MeterWindow* meterwindow)
{
	targetThread=targetthread;
	MeterWindow=meterwindow;
	index = ((Args_T *)arg)->index;
	mbx_id = strdup(((Args_T *)arg)->mbx_id);
	x = ((Args_T *)arg)->x;
	y = ((Args_T *)arg)->y;
	w = ((Args_T *)arg)->w;
	h = ((Args_T *)arg)->h;
	RefreshRate=20.;
	MsgData = 0;
	QThread::start();
}
/**
* @brief set new refresh rate
* @param rr refresh rate 
*/
int GetMeterDataThread::setRefreshRate(double rr)
{
	if (rr>0. && rr<50.){
		RefreshRate=rr;
		MsgLen = (((int)(DataBytes/RefreshRate*(1./(targetThread->getMeters())[index].dt)))/DataBytes)*DataBytes;
		if (MsgLen < DataBytes) MsgLen = DataBytes;
		if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
		MsgData = MsgLen/DataBytes;
		Ndistance=(long int)(1./RefreshRate/(targetThread->getMeters())[index].dt);
		if (Ndistance<1)
			Ndistance=1;
		return Ndistance; //TODO long int > int
	}
	return -1;
}
/**
* @brief starting GetMeterDataThread
*/
void GetMeterDataThread::run()
{
	RT_TASK *GetMeterDataTask;
	MBX *GetMeterDataMbx;
	char GetMeterDataMbxName[7];
	long GetMeterDataPort;
	float MsgBuf[MAX_MSG_LEN/sizeof(float)];
	long int n;
	rt_allow_nonroot_hrt();
	if (!(GetMeterDataTask = rt_task_init_schmod(qrl::get_an_id("HGM"), 98, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetMeterData Task %d\n",index);
		//return (void *)1;
		exit(1);
	}
	if(targetThread->getTargetNode() == 0) GetMeterDataPort=0;
	else GetMeterDataPort = rt_request_port(targetThread->getTargetNode());

	sprintf(GetMeterDataMbxName, "%s%d", mbx_id, index);
	if (!(GetMeterDataMbx = (MBX *)RT_get_adr(targetThread->getTargetNode(), GetMeterDataPort, GetMeterDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetMeterDataMbxName);
		exit(1);
	}
	DataBytes = sizeof(float);
	MaxMsgLen = (MAX_MSG_LEN/DataBytes)*DataBytes;
	MsgLen = (((int)(DataBytes/RefreshRate*(1./(targetThread->getMeters())[index].dt)))/DataBytes)*DataBytes;
	if (MsgLen < DataBytes) MsgLen = DataBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/DataBytes;
	// Ndistance defines the distance between plotted datapoints, to archive the given refresh rate.
	Ndistance=(long int)(1./RefreshRate/(targetThread->getMeters())[index].dt);

	//Fl_Meter_Window *Meter_Win = new Fl_Meter_Window(x, y, w, h, RLG_Main_Workspace->viewport(), Meters[index].name);
	 //QRL_MeterWindow *
	
	
	//Meters_Manager->Meter_Windows[index] = Meter_Win;
	mutex.lock();
	threadStarted.wakeAll();
	mutex.unlock();

	//rt_send(Target_Interface_Task, 0);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	n=Ndistance;
	while (true) {
		if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) break;
		while (RT_mbx_receive_if(targetThread->getTargetNode(), GetMeterDataPort, GetMeterDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;

			//msleep(12); //waits for new Data from the mailbox
			rt_sleep(nano2count(TEN_MS_IN_NS));
		}
		//Fl::lock();
	//	for (n = 0; n < MsgData; n++) {
			//meterMutex.lock();
			//targetThread->setMeterValue(index,MsgBuf[n]);
			//meterMutex.unlock();
		if (n>MsgData)
			n=n-MsgData;
		else {
                   
		   int i;
		   for (i=(int)n-1; i < MsgData; i=i+(int)Ndistance){
			if (MeterWindow)
			    MeterWindow->setValue(MsgBuf[i]);
		   }
		   n=Ndistance;
		}
		//Fl::unlock();
	}
end:
	if (targetThread->getVerbose()) {
		printf("Deleting meter thread number...%d\n", index);
	}
	//Meter_Win->hide();
	rt_release_port(targetThread->getTargetNode(), GetMeterDataPort);
	rt_task_delete(GetMeterDataTask);

	//return 0;
}

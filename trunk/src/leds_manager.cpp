/*
 file:		leds_manager.cpp
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

#include "leds_manager.h"



/**
* @brief Initialize Meters Manager
*/
QRL_LedsManager::QRL_LedsManager(QWidget *parent,TargetThread* targetthread)
	:QDialog(parent),targetThread(targetthread)
{
	setupUi(this);
	Num_Leds=targetThread->getLedNumber();
	Leds=targetThread->getLeds();
	const QIcon LedIcon =QIcon(QString::fromUtf8(":/icons/led_icon.xpm"));
	LedWindows = new QRL_LedWindow* [Num_Leds]; 
	for (int i=0; i<Num_Leds; ++i){
		new QListWidgetItem(LedIcon,tr(Leds[i].name), ledListWidget);
		LedWindows[i]=new QRL_LedWindow(parent,Leds[i].n_leds,Leds[i].name);
	}
	connect( showCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( showLed(int) ) );
	connect( ledColorComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeLedColor(int) ) );
	connect( ledListWidget, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( showLedOptions( QListWidgetItem *  ) ) );
	connect( ledListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( showLedOptions( QListWidgetItem *  ) ) );

	if (Num_Leds > 0) Get_Led_Data_Thread = new GetLedDataThread [Num_Leds];
	if (Num_Leds > 0) emit showLedOptions(ledListWidget->item(0));
}

QRL_LedsManager::~QRL_LedsManager()
{
	for (int i=0; i<Num_Leds; ++i){
		LedWindows[i]->hide();
	}
	delete[] LedWindows;
	stopLedThreads();
	if (Get_Led_Data_Thread)
		delete[] Get_Led_Data_Thread;
	
}

void QRL_LedsManager::changeLedColor(int color)
{
	switch(color){
	case 0:
		LedWindows[currentLed]->setLedColor(QColor(Qt::red));
		break;
	case 1:
		LedWindows[currentLed]->setLedColor(QColor(Qt::green));
		break;
	case 2:
		LedWindows[currentLed]->setLedColor(QColor(Qt::blue));
		break;
	case 3:
		LedWindows[currentLed]->setLedColor(QColor(Qt::yellow));
		break;
	default:
		LedWindows[currentLed]->setLedColor(QColor(Qt::red));
		break;
	}
}

/**
* @brief starting all led threads
*/
void QRL_LedsManager::startLedThreads()
{
	if (Num_Leds > 0) Get_Led_Data_Thread = new GetLedDataThread [Num_Leds];
	for (int n = 0; n < Num_Leds; n++) {
		unsigned int msg;
		Args_T thr_args;
		thr_args.index = n;
		thr_args.mbx_id = strdup(targetThread->getPreferences().Target_Led_Mbx_ID);
		thr_args.x = 500; 
		thr_args.y = 290;
		thr_args.w = 250;
		thr_args.h = 250;
		Get_Led_Data_Thread[n].mutex.lock();
		//pthread_create(&Get_Led_Data_Thread[n], NULL, rt_get_led_data, &thr_args);
		Get_Led_Data_Thread[n].start(&thr_args,targetThread,LedWindows[n]);
		//rt_receive(0, &msg);
		Get_Led_Data_Thread[n].threadStarted.wait(&Get_Led_Data_Thread[n].mutex);
		Get_Led_Data_Thread[n].mutex.unlock();
		LedWindows[n]->hide();
	}
}

/**
* @brief stopping all existing led threads
*/
void QRL_LedsManager::stopLedThreads()
{
	for (int n = 0; n < Num_Leds; n++) {
		Get_Led_Data_Thread[n].wait();
	}

}


/**
* @brief update manager dialog for the choosen led
* @param item led number
*/
void QRL_LedsManager::showLedOptions( QListWidgetItem * item ){

	currentLed= ledListWidget->row(item);
	tabWidget->setTabText(0,tr(Leds[currentLed].name));

	if(LedWindows[currentLed]->isVisible())
		showCheckBox->setCheckState(Qt::Checked);
	else
		showCheckBox->setCheckState(Qt::Unchecked);
}


/**
* @brief show scope windows
* @param state set display status
*/
void QRL_LedsManager::showLed(int state) 
{
	if(state==Qt::Checked){
		LedWindows[currentLed]->show();
	} else {
		LedWindows[currentLed]->hide();
	}

}

/**
* @brief Initialise GetLedDataThread
* @param arg 
* @param targetthread pointer to TargetThread
* @param ledwindow pointer to QRL_ScopeWindow
*/
void GetLedDataThread::start(void* arg,TargetThread* targetthread,QRL_LedWindow* ledwindow)
{
	targetThread=targetthread;
	LedWindow=ledwindow;
	index = ((Args_T *)arg)->index;
	mbx_id = strdup(((Args_T *)arg)->mbx_id);
	x = ((Args_T *)arg)->x;
	y = ((Args_T *)arg)->y;
	w = ((Args_T *)arg)->w;
	h = ((Args_T *)arg)->h;
	QThread::start();
}

/**
* @brief starting GetLedDataThread
*/
void GetLedDataThread::run()
{

	RT_TASK *GetLedDataTask;
	MBX *GetLedDataMbx;
	char GetLedDataMbxName[7];
	long GetLedDataPort;
	int MsgData = 0, MsgLen, MaxMsgLen, DataBytes;
	unsigned int MsgBuf[MAX_MSG_LEN/sizeof(unsigned int)];
	unsigned int Led_Mask = 0;
	int n;

	rt_allow_nonroot_hrt();
	if (!(GetLedDataTask = rt_task_init_schmod(qrl::get_an_id("HGE"), 99, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetLedData Task\n");
		//return (void *)1;
		exit(1);
	}
	if(targetThread->getTargetNode() == 0) GetLedDataPort=0;
	else GetLedDataPort = rt_request_port(targetThread->getTargetNode());
	sprintf(GetLedDataMbxName, "%s%d", mbx_id, index);
	if (!(GetLedDataMbx = (MBX *)RT_get_adr(targetThread->getTargetNode(), GetLedDataPort, GetLedDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetLedDataMbxName);
		exit(1);
	}
	DataBytes = sizeof(unsigned int);
	MaxMsgLen = (MAX_MSG_LEN/DataBytes)*DataBytes;
	MsgLen = (((int)(DataBytes*REFRESH_RATE*(1./(targetThread->getLeds())[index].dt)))/DataBytes)*DataBytes;
	if (MsgLen < DataBytes) MsgLen = DataBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/DataBytes;

	//Fl_Led_Window *Led_Win = new Fl_Led_Window(x, y, w, h, RLG_Main_Workspace->viewport(),  (targetThread->getLeds())[index].name, (targetThread->getLeds())[index].n_leds);
	//Leds_Manager->Led_Windows[index] = Led_Win;


	
	mutex.lock();
	threadStarted.wakeAll();
	mutex.unlock();

	//rt_send(Target_Interface_Task, 0);
	mlockall(MCL_CURRENT | MCL_FUTURE);

	while (true) {
		if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) break;
		while (RT_mbx_receive_if(targetThread->getTargetNode(), GetLedDataPort, GetLedDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;

			msleep(12);
		}
		//Fl::lock();
		//for (n = 0; n < MsgData; n++) {
			//Led_Mask = MsgBuf[n];
			Led_Mask = MsgBuf[0];
			//mutex.lock();
			////targetThread->setLedValue(index,Led_Mask);
			if (LedWindow)
			    LedWindow->setValue(Led_Mask);
			//mutex.unlock();
			//Led_Win->led_mask(Led_Mask);
			//Led_Win->led_on_off();
			//Led_Win->update();
		//}
		//Fl::unlock();
	}
end:
	if (targetThread->getVerbose()) {
		printf("Deleting led thread number...%d\n", index);
	}
	//Led_Win->hide();
	rt_release_port(targetThread->getTargetNode(), GetLedDataPort);
	rt_task_delete(GetLedDataTask);

	//return 0;
}




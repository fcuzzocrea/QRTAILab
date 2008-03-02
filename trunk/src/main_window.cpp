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
 file:		main_window.cpp
 describtion:
   file for the classes QRL_MainWindow
*/

#include "main_window.h"

//static RT_TASK *RLG_Main_Task;
//extern unsigned long qrl::get_an_id(const char *root);

QRL_connectDialog::QRL_connectDialog(QWidget *parent)
	:QDialog(parent)
{
    setupUi(this);
    // signals/slots mechanism in action
    //connect( actionExit, SIGNAL(activated()), this , SLOT(close()));
        Preferences_T Preferences;
	Preferences.Target_IP="127.0.0.1";
	Preferences.Target_Interface_Task_Name="IFTASK";
	Preferences.Target_Scope_Mbx_ID="RTS";
	Preferences.Target_Log_Mbx_ID="RTL";
	Preferences.Target_ALog_Mbx_ID="RAL"; //aggiunto 4/5
	Preferences.Target_Led_Mbx_ID="RTE";
	Preferences.Target_Meter_Mbx_ID="RTM";
	Preferences.Target_Synch_Mbx_ID="RTY";
	setPreferences(Preferences);
}


 void QRL_connectDialog::setPreferences(Preferences_T p)
{ 
	Preferences=p;
	ipLineEdit->setText(QByteArray(Preferences.Target_IP));
	taskLineEdit->setText(QByteArray(Preferences.Target_Interface_Task_Name));
	scopeLineEdit->setText(QByteArray(Preferences.Target_Scope_Mbx_ID));
	logLineEdit->setText(QByteArray(Preferences.Target_Log_Mbx_ID));
	alogLineEdit->setText(QByteArray(Preferences.Target_ALog_Mbx_ID));
	ledLineEdit->setText(QByteArray(Preferences.Target_Led_Mbx_ID));
	meterLineEdit->setText(QByteArray(Preferences.Target_Meter_Mbx_ID));
	synchLineEdit->setText(QByteArray(Preferences.Target_Synch_Mbx_ID));
}

Preferences_T QRL_connectDialog::getPreferences()
{

	QByteArray a;
	a=ipLineEdit->text().toAscii();
	Preferences.Target_IP=qstrdup(a.data());
	a=taskLineEdit->text().toAscii() ;
	Preferences.Target_Interface_Task_Name=qstrdup(a.data());
	a=scopeLineEdit->text().toAscii() ;
	Preferences.Target_Scope_Mbx_ID=qstrdup(a.data());
	a=logLineEdit->text().toAscii() ;
	Preferences.Target_Log_Mbx_ID=qstrdup(a.data());
	a=alogLineEdit->text().toAscii() ;
	Preferences.Target_ALog_Mbx_ID=qstrdup(a.data());
	a=ledLineEdit->text().toAscii();
	Preferences.Target_Led_Mbx_ID=qstrdup(a.data());
	a=meterLineEdit->text().toAscii() ;
	Preferences.Target_Meter_Mbx_ID=qstrdup(a.data());
	a=synchLineEdit->text().toAscii() ;
	Preferences.Target_Synch_Mbx_ID=qstrdup(a.data());


	return Preferences;
}



/*
void QRL_connectDialog::accept() 
{
	
close();

}

void QRL_connectDialog::reject() 
{

close();

}
*/

/**
* @brief MainWindow
*/

QRL_MainWindow::QRL_MainWindow()
{
    setupUi(this);



//  unsigned int msg;
    // signals/slots mechanism in action
	ScopesManager=NULL;
	LedsManager=NULL;
	MetersManager=NULL;
	ParametersManager=NULL;
	connect( actionConnect, SIGNAL( triggered() ), this, SLOT( connectDialog() ) ); 
	connect( actionDisconnect, SIGNAL( triggered() ), this, SLOT( disconnectDialog() ) ); 
	connect( actionStart, SIGNAL( triggered() ), this, SLOT( start() ) ); 
	connect( actionStop, SIGNAL( triggered() ), this, SLOT( stop() ) ); 
	connect( actionConnect_WProfile, SIGNAL( triggered() ), this, SLOT( connect_WProfile() ) ); 
	connect( actionSaveProfile, SIGNAL( triggered() ), this, SLOT( saveProfile() ) ); 
	connect( actionAbout_QRtaiLab, SIGNAL( triggered() ), this, SLOT( about() ) ); 
	connect( actionExit, SIGNAL(triggered()), this , SLOT(close()));
	connect( actionShowScope, SIGNAL( triggered() ), this, SLOT( showScopesManager() ) ); 
	connect( actionShowMeter, SIGNAL( triggered() ), this, SLOT( showMetersManager() ) ); 
	connect( actionShowLed, SIGNAL( triggered() ), this, SLOT( showLedsManager() ) ); 
	connect( actionShowParameter, SIGNAL( triggered() ), this, SLOT( showParametersManager() ) ); 
   enableActionConnect(true);
   enableActionConnectWProfile(true);
   enableActionDisconnect(false);
   enableActionStart(false);
   enableActionStop(false);
   enableActionSaveProfile(false);
   enableActionDeleteProfile(false);
   enableActionShowScope(false);
   enableActionShowMeter(false); 
   enableActionShowLed(false); 
   enableActionShowParameter(false);

/*
   rt_allow_nonroot_hrt();
   if (!(RLG_Main_Task = rt_task_init_schmod(qrl::get_an_id("RLGM"), 98, 0, 0, SCHED_FIFO, 0xFF))) {
               printf("Cannot init RTAI-Lab GUI main task\n");
               close();
    }
*/  
  statusMessage = new QLabel(this);
    statusBar()->addWidget(statusMessage);


    qTargetInterface = new QRtaiLabCore(this,Verbose);
    targetthread=qTargetInterface->getTargetThread();
    //targetthread = new TargetThread();
    //targetthread->start();
  //  connect( this, SIGNAL( sendOrder(int) ), targetthread, SLOT( getOrder(int) ) ); 

  
   connect( qTargetInterface, SIGNAL( statusBarMessage(const QString &) ), this, SLOT( setStatusBarMessage(const QString &) ) ); 

     mdiArea = new QMdiArea;
     setCentralWidget(mdiArea);
   // rt_receive(0, &msg);

	profileName=tr("demo");
	Verbose=0;

/*

        Preferences_T Preferences;
	Preferences.Target_IP="127.0.0.1";
	Preferences.Target_Interface_Task_Name="IFTASK";
	Preferences.Target_Scope_Mbx_ID="RTS";
	Preferences.Target_Log_Mbx_ID="RTL";
	Preferences.Target_ALog_Mbx_ID="RAL"; //aggiunto 4/5
	Preferences.Target_Led_Mbx_ID="RTE";
	Preferences.Target_Meter_Mbx_ID="RTM";
	Preferences.Target_Synch_Mbx_ID="RTY";
*/
	ConnectDialog = new QRL_connectDialog(this);

        //targetthread->setPreferences(Preferences);


}

void QRL_MainWindow::setStatusBarMessage(const QString & message){

//	statusBar()->showMessage(message);
	statusMessage->setText(message);

}


void QRL_MainWindow::closeEvent(QCloseEvent *event)
{
	qTargetInterface->disconnectFromTarget();
	//if (Parameters_Manager) Parameters_Manager->hide();
	if (ScopesManager) {
		ScopesManager->hide();
		delete ScopesManager;
	}
	//if (Logs_Manager) Logs_Manager->hide();
	//if (ALogs_Manager) ALogs_Manager->hide();
	if (LedsManager) {
		LedsManager->hide();
		delete LedsManager;
	}
	if (MetersManager) {
		MetersManager->hide();
		delete MetersManager;
	}
	if (ParametersManager) {
		ParametersManager->hide();
		delete ParametersManager;
	}
	//if (Synchs_Manager) Synchs_Manager->hide();
	//rt_send(Target_Interface_Task, CLOSE);
	//pthread_join(Target_Interface_Thread, NULL);
	//rlg_write_pref(GEOMETRY_PREF, "rtailab");
	//targetthread->closeThread();
 	//targetthread->wait();
	//delete targetthread;
	delete qTargetInterface;
// 	rt_task_delete(RLG_Main_Task);
	qDebug() << "Quitting Main window";
}

 void QRL_MainWindow::connectDialog() 
{
if(qTargetInterface->getIsTargetConnected()==0){

  //QRL_connectDialog *connectDialog = new QRL_connectDialog(this);
    if(qTargetInterface->getPreferences().Target_IP==NULL) {
   	 if (ConnectDialog->exec()) 
		qTargetInterface->setPreferences(ConnectDialog->getPreferences());
	else
		return;
    }
	//targetthread->setVerbose(Verbose);
	qTargetInterface->connectToTarget();
	//sendOrder(qrl_types::CONNECT_TO_TARGET);
	if (qTargetInterface->getIsTargetConnected()==1){
		enableActionDisconnect(true);
		enableActionConnect(false);
		enableActionConnectWProfile(false);
		enableActionSaveProfile(true);

			if (qTargetInterface->getMeterNumber()>0){
			enableActionShowMeter(true);
			if (! MetersManager){
				MetersManager = new QRL_MetersManager(this,targetthread);
				for (int i=0; i<qTargetInterface->getMeterNumber(); ++i){
					mdiArea->addSubWindow(MetersManager->getMeterWindows()[i]);
				}
			}
			if (MetersManager) {
				//targetthread->startMeterThreads();//MetersManager->getMeterWindows());
				MetersManager->setGeometry(100,100,200,300);
				//(MetersManager->getMeterWindows()[i])->setGeometry(0,0,200,300);
				bool view_flag=false;
				if (!view_flag) {
					MetersManager->hide();
				} else {
					MetersManager->show();
				}
				// schlechte loesung
				//targetthread->setMetersManager(MetersManager);
			}
			}
			if (qTargetInterface->getLedNumber()>0){
			enableActionShowLed(true);
			if (! LedsManager){
				LedsManager = new QRL_LedsManager(this,targetthread);
				for (int i=0; i<qTargetInterface->getLedNumber(); ++i){
						 mdiArea->addSubWindow(LedsManager->getLedWindows()[i]);
				}
			}
			if (LedsManager) {
				//targetthread->startLedThreads();//LedsManager->getLedWindows());
				LedsManager->setGeometry(0,0,200,300);
				
				bool view_flag=false;
				if (!view_flag) {
					LedsManager->hide();
				} else {
					LedsManager->show();
				}
			}
			}
			if (qTargetInterface->getScopeNumber()>0){
				enableActionShowScope(true);
				if (! ScopesManager){
					ScopesManager = new QRL_ScopesManager(this,targetthread);
					for (int i=0; i<qTargetInterface->getScopeNumber(); ++i){
						 mdiArea->addSubWindow(ScopesManager->getScopeWindows()[i]);
					}
				}
				if (ScopesManager) {
					//targetthread->startScopeThreads();//ScopesManager->getScopeWindows());
					ScopesManager->setGeometry(100,100,200,300);
					
					bool view_flag=false;
					if (!view_flag) {
						ScopesManager->hide();
					} else {
						ScopesManager->show();
					}
				}
				//schlechte loesung
				//targetthread->setScopesManager(ScopesManager);
			}
			if (qTargetInterface->getParameterNumber()>0){
				enableActionShowParameter(true);
				if (! ParametersManager){
					ParametersManager = new QRL_ParametersManager(this,qTargetInterface);
					//connect( ParametersManager, SIGNAL( uploadParameter(int,int) ), this, SLOT( uploadParameter(int,int) ) );
				}
				if (ParametersManager) {
					ParametersManager->update();
					ParametersManager->setGeometry(0,0,200,300);
					bool view_flag=false;
					if (!view_flag) {
						ParametersManager->hide();
					} else {
						ParametersManager->show();
					}
				}
			}
		if (qTargetInterface->getIsTargetRunning()){
			enableActionStart(false);
			enableActionStop(true);
		}else{
			enableActionStart(true);
			enableActionStop(false);
		}
	}else
	{
		enableActionConnect(true);
		enableActionConnectWProfile(true);
		enableActionSaveProfile(false);
		enableActionDisconnect(false);
   		enableActionStart(false);
   		enableActionStop(false);
		enableActionShowScope(false);
        	enableActionShowMeter(false); 
        	enableActionShowLed(false); 
        	enableActionShowParameter(false);
	}
	//rt_send(Target_Interface_Task, CONNECT_TO_TARGET);
}

}
 void QRL_MainWindow::disconnectDialog() 
{
	qTargetInterface->disconnectFromTarget();
	//if (MetersManager) {
	//	MetersManager->stopMeterThreads();
	//}
	//if (ScopesManager) {
	//	ScopesManager->stopScopeThreads();
	//}
	//if (LedsManager) {
	//	LedsManager->stopLedThreads();
	//}
	if (qTargetInterface->getIsTargetConnected()==0){
		enableActionDisconnect(false);
		enableActionConnect(true);
		enableActionConnectWProfile(true);
		enableActionSaveProfile(false);
		enableActionStart(false);
		enableActionStop(false);
	}
	
}
 
void QRL_MainWindow::connect_WProfile() {
     QSettings profiles("QRtaiLab", "profiles");
     int size = profiles.beginReadArray("profiles");
     profiles.endArray();
     if (size>0) {

	int ind=0;
	profiles.beginGroup("lastProfile");
	ind=profiles.value("nr").toInt();
	profiles.endGroup();
	
        profiles.beginReadArray("profiles");
        profiles.setArrayIndex(ind);
        profiles.value("profile",profileName);
        profiles.endArray();
	
	
	QSettings settings("QRtaiLab", profileName);
	

	
	Preferences_T Preferences;
	settings.beginGroup("Preferences");
	Preferences.Target_IP=qstrdup(settings.value("Target_IP",QByteArray("127.0.0.1")).toByteArray().data());
	Preferences.Target_Interface_Task_Name=qstrdup(settings.value("Target_Interface_Task_Name",QByteArray("IFTASK")).toByteArray().data());
	Preferences.Target_Scope_Mbx_ID=qstrdup(settings.value("Target_Scope_Mbx_ID",QByteArray("RTS")).toByteArray().data());
	Preferences.Target_Log_Mbx_ID=qstrdup(settings.value("Target_Log_Mbx_ID",QByteArray("RTL")).toByteArray().data());
	Preferences.Target_ALog_Mbx_ID=qstrdup(settings.value("Target_ALog_Mbx_ID",QByteArray("RAL")).toByteArray().data());
	Preferences.Target_Led_Mbx_ID=qstrdup(settings.value("Target_Led_Mbx_ID",QByteArray("RTE")).toByteArray().data());
	Preferences.Target_Meter_Mbx_ID=qstrdup(settings.value("Target_Meter_Mbx_ID",QByteArray("RTM")).toByteArray().data());
	Preferences.Target_Synch_Mbx_ID=qstrdup(settings.value("Target_Synch_Mbx_ID",QByteArray("RTY")).toByteArray().data());
         settings.endGroup();
	qTargetInterface->setPreferences(Preferences);

connectDialog();

	settings.beginGroup("MainWindow");
	resize(settings.value("size", QSize(400, 400)).toSize());
	move(settings.value("pos", QPoint(200, 200)).toPoint());
	settings.endGroup();



	if (ScopesManager){
		settings.beginGroup("ScopesManager");
		ScopesManager->resize(settings.value("size", QSize(400, 400)).toSize());
		ScopesManager->move(settings.value("pos", QPoint(200, 200)).toPoint());
		ScopesManager->setVisible(settings.value("isVisible",false).toBool());
		settings.endGroup();
	
		size = settings.beginReadArray("ScopeWindow");
		if (size>qTargetInterface->getScopeNumber())
			size=qTargetInterface->getScopeNumber();
		for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		ScopesManager->getScopeWindows()[i]->resize(settings.value("size", QSize(400, 400)).toSize());
		ScopesManager->getScopeWindows()[i]->move(settings.value("pos", QPoint(200, 200)).toPoint());
		ScopesManager->getScopeWindows()[i]->setVisible(settings.value("isVisible",false).toBool());
		ScopesManager->getScopeWindows()[i]->changeRefreshRate(settings.value("RefreshRate",20.).toDouble());
		ScopesManager->getScopeWindows()[i]->changeDataPoints(settings.value("DataPoints",100.).toDouble());
		//ScopesManager->getScopeWindows()[i]->changeDX(settings.value("DX",1.).toDouble());

		}
		settings.endArray();
	}
	if (MetersManager){
		settings.beginGroup("MetersManager");
		MetersManager->resize(settings.value("size", QSize(400, 400)).toSize());
		MetersManager->move(settings.value("pos", QPoint(200, 200)).toPoint());
		MetersManager->setVisible(settings.value("isVisible",false).toBool());
		settings.endGroup();
	
		size = settings.beginReadArray("MeterWindow");
		if (size>qTargetInterface->getMeterNumber())
			size=qTargetInterface->getMeterNumber();
		for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		//MetersManager->getMeterWindows()[i]->resize(settings.value("size", QSize(400, 400)).toSize());
		MetersManager->getMeterWindows()[i]->move(settings.value("pos", QPoint(200, 200)).toPoint());
		MetersManager->getMeterWindows()[i]->setVisible(settings.value("isVisible",false).toBool());
		MetersManager->getMeterWindows()[i]->changeRefreshRate(settings.value("RefreshRate",20.).toDouble());
		MetersManager->getMeterWindows()[i]->setMin(settings.value("Min",-1.).toDouble());
		MetersManager->getMeterWindows()[i]->setMax(settings.value("Max",1.).toDouble());
		MetersManager->getMeterWindows()[i]->setMeter((QRL_MeterWindow::Meter_Type)settings.value("MeterType",(int)QRL_MeterWindow::THERMO).toInt());
		MetersManager->getMeterWindows()[i]->setThermoColor1(settings.value("ThermoColor1",QColor(Qt::red)).value<QColor>());
		MetersManager->getMeterWindows()[i]->setThermoColor2(settings.value("ThermoColor2",QColor(Qt::black)).value<QColor>());
		MetersManager->getMeterWindows()[i]->setPipeWith(settings.value("PipeWidth",1.).toDouble());
		MetersManager->getMeterWindows()[i]->setThermoAlarm(settings.value("AlarmEnabled",false).toBool());
		MetersManager->getMeterWindows()[i]->setAlarmThermoColor1(settings.value("AlarmThermoColor1",QColor(Qt::blue)).value<QColor>());
		MetersManager->getMeterWindows()[i]->setAlarmThermoColor2(settings.value("AlarmThermoColor2",QColor(Qt::black)).value<QColor>());
		MetersManager->getMeterWindows()[i]->setGradientEnabled(settings.value("GradientEnabled",true).toBool());
		MetersManager->getMeterWindows()[i]->setAlarmLevel(settings.value("AlarmLevel",1.).toDouble());
		MetersManager->getMeterWindows()[i]->setNeedleColor(settings.value("NeedleColor",QColor(Qt::black)).value<QColor>());
		MetersManager->getMeterWindows()[i]->setLcdFont(settings.value("LcdFont").value<QFont>());
		}
		settings.endArray();
		MetersManager->refreshView();
        }
	
	if (LedsManager){
		settings.beginGroup("LedsManager");
		LedsManager->resize(settings.value("size", QSize(400, 400)).toSize());
		LedsManager->move(settings.value("pos", QPoint(200, 200)).toPoint());
		LedsManager->setVisible(settings.value("isVisible",false).toBool());
		settings.endGroup();

		size = settings.beginReadArray("LedWindow");
		if (size>qTargetInterface->getLedNumber())
			size=qTargetInterface->getLedNumber();
		for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		QColor color=(settings.value("Color",QColor(Qt::red))).value<QColor>();
		LedsManager->getLedWindows()[i]->setLedColor(color);
		LedsManager->getLedWindows()[i]->resize(settings.value("size", QSize(400, 400)).toSize());
		LedsManager->getLedWindows()[i]->move(settings.value("pos", QPoint(200, 200)).toPoint());
		LedsManager->getLedWindows()[i]->setVisible(settings.value("isVisible",false).toBool());
		}
		settings.endArray();
		LedsManager->refreshView();
	}
	if (ParametersManager){
		settings.beginGroup("ParametersManager");
		ParametersManager->resize(settings.value("size", QSize(400, 400)).toSize());
		ParametersManager->move(settings.value("pos", QPoint(200, 200)).toPoint());
		ParametersManager->setVisible(settings.value("isVisible",false).toBool());
		settings.endGroup();
	}

     } else
	statusMessage->setText(tr("There exists no profile to load!"));
}

void QRL_MainWindow::saveProfile() {
     int ind;
     QSettings profiles("QRtaiLab", "profiles");
     int size = profiles.beginReadArray("profiles");
     profiles.endArray();
     if (size>0) {
	
        profiles.beginGroup("lastProfile");
        ind=profiles.value("nr").toInt();
        profiles.endGroup();

        profiles.beginReadArray("profiles");
        profiles.setArrayIndex(ind);
        profiles.value("profile",profileName);
        profiles.endArray();
     }
     bool ok;
     QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                          tr("User name:"), QLineEdit::Normal,
                                          profileName, &ok);
     if (ok && !text.isEmpty() && text!="profiles")
         profileName=text;
     else {
        QMessageBox::critical(this, tr("Error"),
                                     tr("Profile name is not acctepted.Try again!"),
                                     QMessageBox::Abort);
	return;
     }
     ind=-1;
     profiles.beginReadArray("profiles");
     for (int i=0;i<size;++i){
        profiles.setArrayIndex(i);
	if (profiles.value("profile")==profileName)
		ind=i;
     }
     profiles.endArray();
     if (ind<0){
	ind=size;
	profiles.beginWriteArray("profiles");
	profiles.setArrayIndex(ind);
	profiles.setValue("profile", profileName);
	profiles.endArray();
     }
     profiles.beginGroup("lastProfile");
     profiles.setValue("nr",ind);
     profiles.endGroup();

     QSettings settings("QRtaiLab",profileName);
     settings.beginGroup("MainWindow");
     settings.setValue("size", this->size());
     settings.setValue("pos", this->pos());
     settings.endGroup();
Preferences_T Preferences=qTargetInterface->getPreferences();
	settings.beginGroup("Preferences");
	settings.setValue("Target_IP",QByteArray(Preferences.Target_IP));
	settings.setValue("Target_Interface_Task_Name",QByteArray(Preferences.Target_Interface_Task_Name));
	settings.setValue("Target_Scope_Mbx_ID",QByteArray(Preferences.Target_Scope_Mbx_ID));
	settings.setValue("Target_Log_Mbx_ID",QByteArray(Preferences.Target_Log_Mbx_ID));
	settings.setValue("Target_ALog_Mbx_ID",QByteArray(Preferences.Target_ALog_Mbx_ID));
	settings.setValue("Target_Led_Mbx_ID",QByteArray(Preferences.Target_Led_Mbx_ID));
	settings.setValue("Target_Meter_Mbx_ID",QByteArray(Preferences.Target_Meter_Mbx_ID));
	settings.setValue("Target_Synch_Mbx_ID",QByteArray(Preferences.Target_Synch_Mbx_ID));
         settings.endGroup();

     if(ScopesManager){
	settings.beginGroup("ScopesManager");
	settings.setValue("size", ScopesManager->size());
	settings.setValue("pos", ScopesManager->pos());
	settings.setValue("isVisible",ScopesManager->isVisible());
	settings.endGroup();
	
	settings.beginWriteArray("ScopeWindow");
	for (int i = 0; i < qTargetInterface->getScopeNumber(); ++i) {
		settings.setArrayIndex(i);
		settings.setValue("size", ScopesManager->getScopeWindows()[i]->size());
		settings.setValue("pos", ScopesManager->getScopeWindows()[i]->pos());
		settings.setValue("isVisible",ScopesManager->getScopeWindows()[i]->isVisible());
		settings.setValue("RefreshRate",ScopesManager->getScopeWindows()[i]->getRefreshRate());
		settings.setValue("DataPoints",(double)ScopesManager->getScopeWindows()[i]->getDataPoints());
		settings.setValue("DX",ScopesManager->getScopeWindows()[i]->getDX());
		

	}
	settings.endArray();
     }

     if(MetersManager){
	settings.beginGroup("MetersManager");
	settings.setValue("size", MetersManager->size());
	settings.setValue("pos", MetersManager->pos());
	settings.setValue("isVisible",MetersManager->isVisible());
	settings.endGroup();

	settings.beginWriteArray("MeterWindow");
	for (int i = 0; i < qTargetInterface->getMeterNumber(); ++i) {
		settings.setArrayIndex(i);
		settings.setValue("size", MetersManager->getMeterWindows()[i]->size());
		settings.setValue("pos", MetersManager->getMeterWindows()[i]->pos());
		settings.setValue("isVisible",MetersManager->getMeterWindows()[i]->isVisible());
		settings.setValue("RefreshRate",MetersManager->getMeterWindows()[i]->getRefreshRate());
		settings.setValue("Min",MetersManager->getMeterWindows()[i]->getMin());
		settings.setValue("Max",MetersManager->getMeterWindows()[i]->getMax());
		settings.setValue("MeterType",(int)MetersManager->getMeterWindows()[i]->getMeterType());
		settings.setValue("ThermoColor1",MetersManager->getMeterWindows()[i]->getThermoColor1());
		settings.setValue("ThermoColor2",MetersManager->getMeterWindows()[i]->getThermoColor2());
		settings.setValue("PipeWidth",MetersManager->getMeterWindows()[i]->getPipeWidth());
		settings.setValue("AlarmEnabled",MetersManager->getMeterWindows()[i]->getThermoAlarm());
		settings.setValue("AlarmThermoColor1",MetersManager->getMeterWindows()[i]->getAlarmThermoColor1());
		settings.setValue("AlarmThermoColor2",MetersManager->getMeterWindows()[i]->getAlarmThermoColor2());
		settings.setValue("GradientEnabled",MetersManager->getMeterWindows()[i]->getGradientEnabled());
		settings.setValue("AlarmLevel",MetersManager->getMeterWindows()[i]->getAlarmLevel());
		settings.setValue("NeedleColor",MetersManager->getMeterWindows()[i]->getNeedleColor());
		settings.setValue("LcdFont",MetersManager->getMeterWindows()[i]->getLcdFont());

	}
	settings.endArray();
     
     }

     if(LedsManager){
	settings.beginGroup("LedsManager");
	settings.setValue("size", LedsManager->size());
	settings.setValue("pos", LedsManager->pos());
	settings.setValue("isVisible",LedsManager->isVisible());
	settings.endGroup();

	settings.beginWriteArray("LedWindow");
	for (int i = 0; i < qTargetInterface->getLedNumber(); ++i) {
		settings.setArrayIndex(i);
		settings.setValue("Color",LedsManager->getLedWindows()[i]->getLedColor());
		settings.setValue("size", LedsManager->getLedWindows()[i]->size());
		settings.setValue("pos", LedsManager->getLedWindows()[i]->pos());
		settings.setValue("isVisible",LedsManager->getLedWindows()[i]->isVisible());
	}
	settings.endArray();
     }

     if(ParametersManager){
	settings.beginGroup("ParametersManager");
	settings.setValue("size", ParametersManager->size());
	settings.setValue("pos", ParametersManager->pos());
	settings.setValue("isVisible",ParametersManager->isVisible());
	settings.endGroup();
     }

	statusMessage->setText(tr("Profile saved!"));
     }

 void QRL_MainWindow::start() 
{
	qTargetInterface->startTarget();
	if (qTargetInterface->getIsTargetRunning()){
		enableActionStart(false);
		enableActionStop(true);
	}
}

 void QRL_MainWindow::stop() 
{
	qTargetInterface->stopTarget();
	if (qTargetInterface->getIsTargetRunning()==0){
		enableActionStop(false);
		if (qTargetInterface->getIsTargetConnected()==0){
			enableActionDisconnect(false);
			enableActionConnect(true);
			enableActionConnectWProfile(true);
			enableActionSaveProfile(false);
			enableActionStart(false);
			enableActionShowScope(false);
        		enableActionShowMeter(false); 
        		enableActionShowLed(false); 
        		enableActionShowParameter(false);
		}
	}
}

void QRL_MainWindow::showScopesManager()
{
if (ScopesManager)
	if (ScopesManager->isVisible())
		ScopesManager->hide();
	else
 		ScopesManager->show();
}


void QRL_MainWindow::showMetersManager()
{
if (MetersManager)
	if (MetersManager->isVisible())
		MetersManager->hide();
	else
 		MetersManager->show();
}

void QRL_MainWindow::showLedsManager()
{
if (LedsManager)
	if (LedsManager->isVisible())
		LedsManager->hide();
	else
 		LedsManager->show();
}

void QRL_MainWindow::showParametersManager()
{
if (ParametersManager)
	if (ParametersManager->isVisible())
		ParametersManager->hide();
	else
 		ParametersManager->show();
}


void QRL_MainWindow::enableActionStart(bool b)
{
	actionStart->setEnabled(b);
}


void QRL_MainWindow::enableActionStop(bool b)
{
	actionStop->setEnabled(b);
}

void QRL_MainWindow::enableActionConnect(bool b)
{
	actionConnect->setEnabled(b);
}

void QRL_MainWindow::enableActionConnectWProfile(bool b)
{
	actionConnect_WProfile->setEnabled(b);
}


void QRL_MainWindow::enableActionDisconnect(bool b)
{
	actionDisconnect->setEnabled(b);
}

void QRL_MainWindow::enableActionSaveProfile(bool b)
{
	actionSaveProfile->setEnabled(b);
}

void QRL_MainWindow::enableActionDeleteProfile(bool b)
{
	actionDeleteProfile->setEnabled(b);
}

void QRL_MainWindow::enableActionShowMeter(bool b)
{
	actionShowMeter->setEnabled(b);
}

void QRL_MainWindow::enableActionShowScope(bool b)
{
	actionShowScope->setEnabled(b);
}

void QRL_MainWindow::enableActionShowLed(bool b)
{
	actionShowLed->setEnabled(b);
}

void QRL_MainWindow::enableActionShowParameter(bool b)
{
	actionShowParameter->setEnabled(b);
}



void QRL_MainWindow::about() 
{
    QMessageBox::about(this,"About QRtaiLab",
		"Version 0.1\n"
                "This app was coded for educational purposes.\n\n"
                "Copyright (C) 2008 Holger Nahrstaedt. \n\n"
                "Have Fun!\n");
}

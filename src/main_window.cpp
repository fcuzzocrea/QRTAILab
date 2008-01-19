/*
 file:		main_window.cpp
 describtion:
   file for the classes QRL_MainWindow

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

#include "main_window.h"

static RT_TASK *RLG_Main_Task;
extern unsigned long qrl::get_an_id(const char *root);


QRL_connectDialog::QRL_connectDialog(QWidget *parent)
	:QDialog(parent)
{
    setupUi(this);
  
    // signals/slots mechanism in action
    //connect( actionExit, SIGNAL(activated()), this , SLOT(close()));
}

void QRL_connectDialog::accept() 
{
	
close();

}

void QRL_connectDialog::reject() 
{

close();

}


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

   rt_allow_nonroot_hrt();
   if (!(RLG_Main_Task = rt_task_init_schmod(qrl::get_an_id("RLGM"), 99, 0, 0, SCHED_FIFO, 0xFF))) {
               printf("Cannot init RTAI-Lab GUI main task\n");
               close();
    }

    targetthread = new TargetThread();

    targetthread->start();
  //  connect( this, SIGNAL( sendOrder(int) ), targetthread, SLOT( getOrder(int) ) ); 

    statusMessage = new QLabel(this);
    statusBar()->addWidget(statusMessage);
    connect( targetthread, SIGNAL( statusBarMessage(const QString &) ), this, SLOT( setStatusBarMessage(const QString &) ) ); 

     mdiArea = new QMdiArea;
     setCentralWidget(mdiArea);
   // rt_receive(0, &msg);

	profileName=tr("demo");

        Preferences_T Preferences;
	Preferences.Target_IP="127.0.0.1";
	Preferences.Target_Interface_Task_Name="IFTASK";
	Preferences.Target_Scope_Mbx_ID="RTS";
	Preferences.Target_Log_Mbx_ID="RTL";
	Preferences.Target_ALog_Mbx_ID="RAL"; //aggiunto 4/5
	Preferences.Target_Led_Mbx_ID="RTE";
	Preferences.Target_Meter_Mbx_ID="RTM";
	Preferences.Target_Synch_Mbx_ID="RTY";
        targetthread->setPreferences(Preferences);


}

void QRL_MainWindow::setStatusBarMessage(const QString & message){

//	statusBar()->showMessage(message);
	statusMessage->setText(message);

}


void QRL_MainWindow::closeEvent(QCloseEvent *event)
{
	targetthread->disconnectFromTarget();
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
	targetthread->closeThread();
 	targetthread->wait();
	delete targetthread;
 	rt_task_delete(RLG_Main_Task);
	qDebug() << "Quitting Main window";
}

 void QRL_MainWindow::connectDialog() 
{
if(targetthread->getIsTargetConnected()==0){

  //QRL_connectDialog *connectDialog = new QRL_connectDialog(this);
  //connectDialog->exec();
	targetthread->connectToTarget();
	//sendOrder(qrl_types::CONNECT_TO_TARGET);
	if (targetthread->getIsTargetConnected()==1){
		enableActionDisconnect(true);
		enableActionConnect(false);
		enableActionConnectWProfile(false);
		enableActionSaveProfile(true);

			if (targetthread->getMeterNumber()>0){
			enableActionShowMeter(true);
			if (! MetersManager){
				MetersManager = new QRL_MetersManager(this,targetthread);
				for (int i=0; i<targetthread->getMeterNumber(); ++i){
					mdiArea->addSubWindow(MetersManager->getMeterWindows()[i]);
				}
			}
			if (MetersManager) {
				MetersManager->startMeterThreads();
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
			if (targetthread->getLedNumber()>0){
			enableActionShowLed(true);
			if (! LedsManager){
				LedsManager = new QRL_LedsManager(this,targetthread);
				for (int i=0; i<targetthread->getLedNumber(); ++i){
						 mdiArea->addSubWindow(LedsManager->getLedWindows()[i]);
				}
			}
			if (LedsManager) {
				LedsManager->startLedThreads();
				LedsManager->setGeometry(0,0,200,300);
				
				bool view_flag=false;
				if (!view_flag) {
					LedsManager->hide();
				} else {
					LedsManager->show();
				}
			}
			}
			if (targetthread->getScopeNumber()>0){
				enableActionShowScope(true);
				if (! ScopesManager){
					ScopesManager = new QRL_ScopesManager(this,targetthread);
					for (int i=0; i<targetthread->getScopeNumber(); ++i){
						 mdiArea->addSubWindow(ScopesManager->getScopeWindows()[i]);
					}
				}
				if (ScopesManager) {
					ScopesManager->startScopeThreads();
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
			if (targetthread->getParameterNumber()>0){
				enableActionShowParameter(true);
				if (! ParametersManager){
					ParametersManager = new QRL_ParametersManager(this,targetthread);
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
		if (targetthread->getIsTargetRunning()){
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
	targetthread->disconnectFromTarget();
	if (MetersManager) {
		MetersManager->stopMeterThreads();
	}
	if (ScopesManager) {
		ScopesManager->stopScopeThreads();
	}
	if (LedsManager) {
		LedsManager->stopLedThreads();
	}
	if (targetthread->getIsTargetConnected()==0){
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
	
	connectDialog();
	QSettings settings("QRtaiLab", profileName);
	
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
		if (size>targetthread->getScopeNumber())
			size=targetthread->getScopeNumber();
		for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		ScopesManager->getScopeWindows()[i]->changeRefreshRate(settings.value("RefreshRate",20.).toDouble());
		ScopesManager->getScopeWindows()[i]->changeDataPoints(settings.value("DataPoints",100.).toDouble());
		//ScopesManager->getScopeWindows()[i]->changeDX(settings.value("DX",1.).toDouble());
		ScopesManager->getScopeWindows()[i]->resize(settings.value("size", QSize(400, 400)).toSize());
		ScopesManager->getScopeWindows()[i]->move(settings.value("pos", QPoint(200, 200)).toPoint());
		ScopesManager->getScopeWindows()[i]->setVisible(settings.value("isVisible",false).toBool());
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
		if (size>targetthread->getMeterNumber())
			size=targetthread->getMeterNumber();
		for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		MetersManager->getMeterWindows()[i]->changeRefreshRate(settings.value("RefreshRate",20.).toDouble());
		MetersManager->getMeterWindows()[i]->resize(settings.value("size", QSize(400, 400)).toSize());
		MetersManager->getMeterWindows()[i]->move(settings.value("pos", QPoint(200, 200)).toPoint());
		MetersManager->getMeterWindows()[i]->setVisible(settings.value("isVisible",false).toBool());
		}
		settings.endArray();
        }
	if (LedsManager){
		settings.beginGroup("LedsManager");
		LedsManager->resize(settings.value("size", QSize(400, 400)).toSize());
		LedsManager->move(settings.value("pos", QPoint(200, 200)).toPoint());
		LedsManager->setVisible(settings.value("isVisible",false).toBool());
		settings.endGroup();
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
     
     if(ScopesManager){
	settings.beginGroup("ScopesManager");
	settings.setValue("size", ScopesManager->size());
	settings.setValue("pos", ScopesManager->pos());
	settings.setValue("isVisible",ScopesManager->isVisible());
	settings.endGroup();
	
	settings.beginWriteArray("ScopeWindow");
	for (int i = 0; i < targetthread->getScopeNumber(); ++i) {
		settings.setArrayIndex(i);
		settings.setValue("RefreshRate",ScopesManager->getScopeWindows()[i]->getRefreshRate());
		settings.setValue("DataPoints",(double)ScopesManager->getScopeWindows()[i]->getDataPoints());
		settings.setValue("DX",ScopesManager->getScopeWindows()[i]->getDX());
		settings.setValue("size", ScopesManager->getScopeWindows()[i]->size());
		settings.setValue("pos", ScopesManager->getScopeWindows()[i]->pos());
		settings.setValue("isVisible",ScopesManager->getScopeWindows()[i]->isVisible());
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
	for (int i = 0; i < targetthread->getMeterNumber(); ++i) {
		settings.setArrayIndex(i);
		settings.setValue("RefreshRate",MetersManager->getMeterWindows()[i]->getRefreshRate());
		settings.setValue("size", MetersManager->getMeterWindows()[i]->size());
		settings.setValue("pos", MetersManager->getMeterWindows()[i]->pos());
		settings.setValue("isVisible",MetersManager->getMeterWindows()[i]->isVisible());
	}
	settings.endArray();
     
     }

     if(LedsManager){
	settings.beginGroup("LedsManager");
	settings.setValue("size", LedsManager->size());
	settings.setValue("pos", LedsManager->pos());
	settings.setValue("isVisible",LedsManager->isVisible());
	settings.endGroup();
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
	targetthread->startTarget();
	if (targetthread->getIsTargetRunning()){
		enableActionStart(false);
		enableActionStop(true);
	}
}

 void QRL_MainWindow::stop() 
{
	targetthread->stopTarget();
	if (MetersManager) 
		MetersManager->stopMeterThreads();
	if (targetthread->getIsTargetRunning()==0){
		enableActionStop(false);
		if (targetthread->getIsTargetConnected()==0){
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
    QMessageBox::about(this,"About",
                "This app was coded for educational purposes.\n"
                "written by Holger Nahrstaedt \n\n"
                "Have Fun!\n");
}

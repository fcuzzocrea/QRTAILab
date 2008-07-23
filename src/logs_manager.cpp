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
 file:		scopes_manager.cpp
 describtion:
   file for the classe QRL_LogsManager
*/

#include "logs_manager.h"


/**
* @brief Initialize Logs Manager
*/
QRL_LogsManager::QRL_LogsManager(QWidget *parent,QRtaiLabCore* qtargetinterface)
	:QDialog(parent),qTargetInterface(qtargetinterface)
{
	setupUi(this);
	Num_Logs=qTargetInterface->getLogNumber();
	Logs=qTargetInterface->getTargetThread()->getLogs();
	const QIcon LogIcon =QIcon(QString::fromUtf8(":/icons/log_icon.xpm"));
	for (int i=0; i<Num_Logs; ++i){
		logItems << new QListWidgetItem(LogIcon,qTargetInterface->getLogName(i), logListWidget);
	}
	//connect( showCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( showScope(int) ) );
	connect( logListWidget, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( showLogOptions( QListWidgetItem *  ) ) );
	connect( logListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( showLogOptions( QListWidgetItem *  ) ) );
	//connect( rrLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeRefreshRate(const QString&) ) );
	connect( savePushButton, SIGNAL( pressed() ), this, SLOT( startSaving() ) );
	connect( fileLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeFileName(const QString&) ) );

	currentLog=0;
// 	for(int i=0; i<1; ++i){
// 		//tabWidget->addTab(new QWidget(tabWidget->widget(1)),tr("Trace ")+tr("%1").arg(i+1));
// 		traceComboBox->addItem(tr("Trace ")+tr("%1").arg(i+1));
// 	}
	if (Num_Logs > 0)  showLogOptions(currentLog);
	
//	if (Num_Scopes > 0) Get_Scope_Data_Thread = new GetScopeDataThread [Num_Scopes];
	//for (int i=0; i<Num_Scopes; ++i){
	//	connect(&Get_Scope_Data_Thread[i],SIGNAL(value(int,float)),ScopeWindows[i],SLOT(setValue(int,float)));
	//}
	
//	tabWidget->addTab(traceWidget,tr("trace %1").arg(Scopes[currentScope].ntraces));

}
QRL_LogsManager::~QRL_LogsManager()
{

	//stopScopeThreads();
	//if (Get_Scope_Data_Thread)
	//	delete[] Get_Scope_Data_Thread;
}


void QRL_LogsManager::startSaving()
{
	 FILE* Save_File_Pointer;
	double Save_Time=timeCounter->value();
	if( qTargetInterface->getTargetThread()->start_saving_log(currentLog)==0){

		QString File_Name=fileLineEdit->text();
		if (QFile::exists(File_Name)) {
			printf("File %s exists already.",File_Name.toLocal8Bit().data() );
			QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("The File exists! Please change the name!"),
                                     QMessageBox::Abort);
		} else {
		 
		  
		
		if ((Save_File_Pointer = fopen((File_Name.toLocal8Bit()).data(), "a+")) == NULL) {
			printf("Error in opening file %s",File_Name.toLocal8Bit().data() );
			QMessageBox::critical(this, tr("QMessageBox::critical()"),
                                     tr("Error in opening file!"),
                                     QMessageBox::Abort);
		} else {
			//savePushButton->setEnabled(false);
			qTargetInterface->getTargetThread()->startSavingLog(currentLog,Save_File_Pointer,Save_Time);
		}

		
		//ScopeWindows[currentScope]->startSaving(fileLineEdit->text());
		//targetThread->startSaving(fileLineEdit->text(),currentScope);


	       }
       }


}

void QRL_LogsManager::stopSaving(int index)
{
	if(currentLog==index){
		savePushButton->setEnabled(true);
	}
}





/**
* @brief update manager dialog for the choosen led
* @param item led number
*/
void QRL_LogsManager::showLogOptions( QListWidgetItem * item ){

	currentLog= logListWidget->row(item);
	tabWidget->setTabText(0,qTargetInterface->getLogName(currentLog));
//  	timeCounter->setValue(saveTime[currentLog] );
//  	fileLineEdit->setText(fileName[currentLog] );
}

void QRL_LogsManager::showLogOptions( int index ){

	currentLog=index;
	if(qTargetInterface->getTargetThread()->start_saving_log(currentLog)==0){
		savePushButton->setEnabled(true);
	}
	tabWidget->setTabText(0,tr(Logs[currentLog].name));
//  	timeCounter->setValue(saveTime[currentLog] );
//  	fileLineEdit->setText(fileName[currentLog] );

}



void QRL_LogsManager::changeSaveTime(double time)
{
	//ScopeWindows[currentScope]->setSaveTime(time);
// 	Logs[currentLog].Save_Time=time;
// 	saveTime[currentLog]=time;
	
}

void QRL_LogsManager::changeFileName(const QString& str)
{
	//ScopeWindows[currentScope]->setFileName(str);
// 	fileName[currentLog]=str;
}


QDataStream& operator<<(QDataStream &out, const QRL_LogsManager &d){
	out << d.size()  << d.pos() << d.isVisible();
	out <<(qint32) d.Num_Logs;
	out << d.fileLineEdit->text() << d.timeCounter->value();
	for (int i = 0; i < d.Num_Logs; ++i) {
		//out << d.file_name.at(i) << d.save_time.at(i);
	}
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_LogsManager(&d)){
	QSize s;QPoint p;bool b; int i;QString str;double dd;
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	qint32 a;
	in >> a;
	in >> str >> dd;
 	d.timeCounter->setValue(dd);
 	d.fileLineEdit->setText(str );

	for (int i = 0; i < (int)a; ++i) {
// 		if (d.Num_Logs>i)
// 			in>>d.file_name[i]>>d.save_time[i];
// 		else 
// 			in>>str>>dd;
	}
	d.showLogOptions(d.currentLog);
	return in;
}






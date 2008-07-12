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
 file:		targets_manager.cpp
 describtion:
   file for the classe QRL_TargetsManager
*/

#include "targets_manager.h"


/**
* @brief Initialize Targets Manager
*/
QRL_TargetsManager::QRL_TargetsManager(QWidget *parent,QRtaiLabCore* qtargetinterface)
	:QDialog(parent),qTargetInterface(qtargetinterface)
{
	setupUi(this);
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
QRL_TargetsManager::~QRL_TargetsManager()
{
}

 void QRL_TargetsManager::setPreferences(Preferences_T p)
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

Preferences_T QRL_TargetsManager::getPreferences()
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


void QRL_TargetsManager::setTargetIsConnected(bool connected){

if (connected){
	targetNameLineEdit->setText(tr(qTargetInterface->getTargetName()));
	targetNameLineEdit_2->setText(tr(qTargetInterface->getTargetName()));
	targetConnectedLabel->setText(tr("connected"));
	setTargetIsRunning(qTargetInterface->getIsTargetRunning()==1);
	targetTableWidget->clear();
	int jend=6,j=0;
	targetTableWidget->setRowCount(jend);
	targetTableWidget->setColumnCount(4);
	QTableWidgetItem *newItem;
	newItem = new QTableWidgetItem(tr("Number of tunable parameters"));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,0,newItem);
	newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getParameterNumber()));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,1,newItem);
	newItem = new QTableWidgetItem(tr("Number of scopes"));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	j++;
	targetTableWidget->setItem(j,0,newItem);
	newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getScopeNumber()));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,1,newItem);
	if (qTargetInterface->getScopeNumber()>0){
		jend=jend+qTargetInterface->getScopeNumber()*3;
		targetTableWidget->setRowCount(jend);
		for (int k=0;k<qTargetInterface->getScopeNumber();k++){
			j++;
			newItem = new QTableWidgetItem(tr("Scope"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,1,newItem);
			newItem = new QTableWidgetItem(qTargetInterface->getScopeName(k));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			j++;
			newItem = new QTableWidgetItem(tr("Number of traces"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getNumberOfTraces(k)));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,3,newItem);
			j++;
			newItem = new QTableWidgetItem(tr("Sampling time"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getTargetThread()->getScopes()[k].dt));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,3,newItem);

		}

	}
	j++;
	newItem = new QTableWidgetItem(tr("Number of logs"));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,0,newItem);
	newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getLogNumber()));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,1,newItem);
	if (qTargetInterface->getLogNumber()>0){
		jend=jend+qTargetInterface->getLogNumber()*2;
		targetTableWidget->setRowCount(jend);
		for (int k=0;k<qTargetInterface->getLogNumber();k++){
			j++;
			newItem = new QTableWidgetItem(tr("Log"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,1,newItem);
			newItem = new QTableWidgetItem(qTargetInterface->getLogName(k));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			j++;
			newItem = new QTableWidgetItem(tr("Sampling time"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getTargetThread()->getLogs()[k].dt));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,3,newItem);

		}

	}
	j++;
	newItem = new QTableWidgetItem(tr("Number of automatic logs"));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,0,newItem);
	newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getALogNumber()));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,1,newItem);
	if (qTargetInterface->getALogNumber()>0){
		jend=jend+qTargetInterface->getALogNumber()*2;
		targetTableWidget->setRowCount(jend);
		for (int k=0;k<qTargetInterface->getALogNumber();k++){
			j++;
			newItem = new QTableWidgetItem(tr("Log"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,1,newItem);
			newItem = new QTableWidgetItem(qTargetInterface->getALogName(k));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			j++;
			newItem = new QTableWidgetItem(tr("Sampling time"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getTargetThread()->getALogs()[k].dt));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,3,newItem);

		}

	}
	j++;
	newItem = new QTableWidgetItem(tr("Number of leds"));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,0,newItem);
	newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getLedNumber()));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,1,newItem);
	if (qTargetInterface->getLedNumber()>0){
		jend=jend+qTargetInterface->getLedNumber()*3;
		targetTableWidget->setRowCount(jend);
		for (int k=0;k<qTargetInterface->getLedNumber();k++){
			j++;
			newItem = new QTableWidgetItem(tr("Led"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,1,newItem);
			newItem = new QTableWidgetItem(qTargetInterface->getLedName(k));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			j++;
			newItem = new QTableWidgetItem(tr("Number of leds"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getNumberOfLeds(k)));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,3,newItem);
			j++;
			newItem = new QTableWidgetItem(tr("Sampling time"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getTargetThread()->getLeds()[k].dt));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,3,newItem);

		}

	}
	j++;
	newItem = new QTableWidgetItem(tr("Number of meters"));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,0,newItem);
	newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getMeterNumber()));
	newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
	targetTableWidget->setItem(j,1,newItem);
	if (qTargetInterface->getMeterNumber()>0){
		jend=jend+qTargetInterface->getMeterNumber()*2;
		targetTableWidget->setRowCount(jend);
		for (int k=0;k<qTargetInterface->getMeterNumber();k++){
			j++;
			newItem = new QTableWidgetItem(tr("Meter"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,1,newItem);
			newItem = new QTableWidgetItem(qTargetInterface->getMeterName(k));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			j++;
			newItem = new QTableWidgetItem(tr("Sampling time"));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,2,newItem);
			newItem = new QTableWidgetItem(tr("%1").arg(qTargetInterface->getTargetThread()->getMeters()[k].dt));
			newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
			targetTableWidget->setItem(j,3,newItem);

		}

	}

} else {
	targetNameLineEdit->setText(tr(""));
	targetStatusLineEdit->setText(tr("not running"));
	targetConnectedLabel->setText(tr("not connected"));
}
}

void QRL_TargetsManager::setTargetIsRunning(bool running){

if (running){
	targetStatusLineEdit->setText(tr("running"));
	targetRunningLabel->setText(tr("running"));


} else {

	targetStatusLineEdit->setText(tr("not running"));
	targetRunningLabel->setText(tr("not running"));

}




}

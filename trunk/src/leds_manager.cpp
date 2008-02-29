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
 file:		leds_manager.cpp
 describtion:
   file for the classes GetScopeDataThread and  QRL_ScopesManager
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

	if (Num_Leds > 0) showLedOptions(ledListWidget->item(0));

  	RefreshRate=20.;
	timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
        timer->start((int)(1./RefreshRate*1000.));

}

QRL_LedsManager::~QRL_LedsManager()
{
	for (int i=0; i<Num_Leds; ++i){
		LedWindows[i]->hide();
	}
	delete[] LedWindows;
}


void QRL_LedsManager::refresh()
{
for (int i=0; i<Num_Leds; ++i){
	//if (LedWindows[i]->isVisible()){
		LedWindows[i]->setValue(targetThread->getLedValue(i));
	//}
}
}



void QRL_LedsManager::refreshView()
{


	showLedOptions(ledListWidget->item(currentLed));


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
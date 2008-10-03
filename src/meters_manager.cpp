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
 file:		meters_manager.cpp
 describtion:
   file for the classes GetMeterDataThread and  QRL_MetersManager
*/

#include "meters_manager.h"

extern unsigned long qrl::get_an_id(const char *root);


///  Initialize Meters Manager
QRL_MetersManager::QRL_MetersManager(QWidget *parent, QRtaiLabCore* qtargetinterface)
	:QDialog(parent),qTargetInterface(qtargetinterface)
{
	setupUi(this);
	Num_Meters=qTargetInterface->getMeterNumber();
	const QIcon MeterIcon =QIcon(QString::fromUtf8(":/icons/meter_icon.xpm"));
	MeterWindows = new QRL_MeterWindow* [Num_Meters]; 
	for (int i=0; i<Num_Meters; ++i){
		new QListWidgetItem(MeterIcon,qTargetInterface->getMeterName(i), meterListWidget);
		MeterWindows[i]=new QRL_MeterWindow(parent,qTargetInterface->getMeterName(i));
	}
	connect( showCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( showMeter(int) ) );
	connect( rrCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeRefreshRate(double) ) );
	connect( minCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeMin(double) ) );
	connect( maxCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeMax(double) ) );
	connect( meterListWidget, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( showMeterOptions( QListWidgetItem *  ) ) );
	connect( meterListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( showMeterOptions( QListWidgetItem *  ) ) );
	connect( meterComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeMeter(int) ) );
	
	if (Num_Meters > 0)
	showMeterOptions(meterListWidget->item(0));
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
	connect( dialColorPushButton, SIGNAL( pressed() ), this, SLOT( changeNeedleColor() ) );
	connect( fontPushButton, SIGNAL( pressed() ), this, SLOT( changeLcdFont() ) );
    connect( precisionCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeLcdPrecision(double) ) );
    connect( formatComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeLcdFormat(int) ) );
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

	RefreshRate=20.;
	timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
        timer->start((int)(1./RefreshRate*1000.));
	
}



QRL_MetersManager::~QRL_MetersManager()
{
	for (int i=0; i<Num_Meters; ++i){
		MeterWindows[i]->hide();
		delete MeterWindows[i];
	}
	delete[] MeterWindows;
}

void QRL_MetersManager::setFileVersion(qint32 v){
      fileVersion=v;
      for (int i=0; i<Num_Meters;i++)
	MeterWindows[i]->setFileVersion(v);

}

void QRL_MetersManager::refresh()
{
for (int i=0; i<Num_Meters; ++i){
	//if (MeterWindows[i]->isVisible()){
		MeterWindows[i]->setValue(qTargetInterface->getTargetThread()->getMeterValue(i));
	//}
}
}

void QRL_MetersManager::refreshView()
{


	showMeterOptions(meterListWidget->item(currentMeter));


}


/**
* @brief send new refresh rate to the Meter Thread
* @param rr refresh rate
*/
 void QRL_MetersManager::changeRefreshRate(double rr)
{	RefreshRate=rr;
	//double rr=text.toDouble();
	//ScopeWindows[currentScope]->changeRefreshRate(rr);
	rrCounter->setValue(rr);
	qTargetInterface->getTargetThread()->setMeterRefreshRate(rr,currentMeter);
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
	//if (type!=(int)MeterWindows[currentMeter]->getMeterType()){
	switch(type)
	{
		case 0: //thermo
			tabWidget->removeTab(1);
			tabWidget->addTab(ThermoOptions,tr("Options"));
			if (MeterWindows[currentMeter]->getMeterType()!=QRL_MeterWindow::THERMO)
				MeterWindows[currentMeter]->setMeter(QRL_MeterWindow::THERMO);
			break;
		case 1: //dial
			tabWidget->removeTab(1);
			tabWidget->addTab(DialOptions,tr("Options"));
			if (MeterWindows[currentMeter]->getMeterType()!=QRL_MeterWindow::DIAL)
				MeterWindows[currentMeter]->setMeter(QRL_MeterWindow::DIAL);
			break;
		case 2: //lcd
			tabWidget->removeTab(1);
			tabWidget->addTab(LcdOptions,tr("Options"));
			if (MeterWindows[currentMeter]->getMeterType()!=QRL_MeterWindow::LCD)
				MeterWindows[currentMeter]->setMeter(QRL_MeterWindow::LCD);
			break;
		default:break;
	}
	//}
}

void QRL_MetersManager::changeThermoColor1()
{
	QColor color= QColorDialog::getColor(MeterWindows[currentMeter]->getThermoColor1());
	MeterWindows[currentMeter]->setThermoColor1(color);

}

void QRL_MetersManager::changeThermoColor2()
{
	QColor color= QColorDialog::getColor(MeterWindows[currentMeter]->getThermoColor2());
	MeterWindows[currentMeter]->setThermoColor2(color);

}

void QRL_MetersManager::changePipeWith(double w)
{
	MeterWindows[currentMeter]->setPipeWith(w);
}

void QRL_MetersManager::changeAlarmThermoColor1()
{
	QColor color= QColorDialog::getColor(MeterWindows[currentMeter]->getAlarmThermoColor1());
	MeterWindows[currentMeter]->setAlarmThermoColor1(color);

}

void QRL_MetersManager::changeAlarmThermoColor2()
{
	QColor color= QColorDialog::getColor(MeterWindows[currentMeter]->getAlarmThermoColor2());
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
	tabWidget->setTabText(0,qTargetInterface->getMeterName(currentMeter));
	if(MeterWindows[currentMeter]->isVisible())
		showCheckBox->setCheckState(Qt::Checked);
	else
		showCheckBox->setCheckState(Qt::Unchecked);

//disconnect( meterComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeMeter(int) ) );
	meterComboBox->setCurrentIndex((int)MeterWindows[currentMeter]->getMeterType());
//connect( meterComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeMeter(int) ) );

if (MeterWindows[currentMeter]->getMeterType()==QRL_MeterWindow::THERMO) {
	alarmLevelCounter->setValue(MeterWindows[currentMeter]->getAlarmLevel());
	if(MeterWindows[currentMeter]->getThermoAlarm())
		alarmCheckBox->setCheckState(Qt::Checked);
	else
		alarmCheckBox->setCheckState(Qt::Unchecked);
	
	pipeWithCounter->setValue(MeterWindows[currentMeter]->getPipeWidth());
	if (MeterWindows[currentMeter]->getGradientEnabled()) 
		colorComboBox->setCurrentIndex(0);
	else
		colorComboBox->setCurrentIndex(1);
}

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

  void QRL_MetersManager::changeNeedleColor(){

	QColor color= QColorDialog::getColor(MeterWindows[currentMeter]->getNeedleColor());
	MeterWindows[currentMeter]->setNeedleColor(color);

}
  void QRL_MetersManager::changeLcdFont(){
	bool ok;
	QFont font = QFontDialog::getFont(
                 &ok, MeterWindows[currentMeter]->getLcdFont(), this);
	MeterWindows[currentMeter]->setLcdFont(font);
}

void QRL_MetersManager::changeLcdPrecision(double p){

  MeterWindows[currentMeter]->setLcdPrecision(p);
}

void QRL_MetersManager::changeLcdFormat(int d){
  switch(d){
	case 0:
		MeterWindows[currentMeter]->setLcdFormat('e');
		break;
	case 1:
		MeterWindows[currentMeter]->setLcdFormat('f');
		break;
	case 2:
		MeterWindows[currentMeter]->setLcdFormat('g');
		break;
	default:
		break;
  }

}


QDataStream& operator<<(QDataStream &out, const QRL_MetersManager &d){
	out << d.size()  << d.pos() << d.isVisible();
	out <<(qint32) d.Num_Meters;
	for (int i = 0; i < d.Num_Meters; ++i) {
		out<<*(d.MeterWindows[i]);
	}
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_MetersManager(&d)){
	QSize s;QPoint p;bool b; int i;
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	qint32 a;
	in >> a;
	for (int i = 0; i < (int)a; ++i) {
		if (d.Num_Meters>i)
			in>>*(d.MeterWindows[i]);
		else 
			in>>*(d.MeterWindows[d.Num_Meters-1]);
	}
	return in;
}

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
 file:		scopes_manager.cpp
 describtion:
   file for the classe QRL_ScopesManager
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
	connect( scopeListWidget, SIGNAL( itemSelectionChanged() ), this, SLOT( showSelectedOptions() ) );
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
	connect( dyComboBox, SIGNAL( currentIndexChanged(const QString &) ), this, SLOT( changeDy(const QString&) ) );
	connect( dyComboBox, SIGNAL( editTextChanged(const QString &) ), this, SLOT( changeDy(const QString&) ) );
	connect( tabWidget, SIGNAL( currentChanged(int) ), this, SLOT( changeScopeList(int) ) );
	connect( triggerCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeTriggerLevel(double) ) );
	connect( triggerPushButton, SIGNAL( pressed()), this, SLOT(manualTrigger() ) );
	connect( oneShotCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( changeSingleMode(int) ) );
	connect( startTriggerPushButton, SIGNAL( pressed()), this, SLOT(startSingleRun() ) );
	connect( traceNameLineEdit,SIGNAL( textChanged ( const QString &  ) ), this , SLOT( changeTraceText(const QString & ) ) );
        connect( zeroAxisCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeZeroAxis(int) ) );

	currentScope=0;
// 	for(int i=0; i<1; ++i){
// 		//tabWidget->addTab(new QWidget(tabWidget->widget(1)),tr("Trace ")+tr("%1").arg(i+1));
// 		traceComboBox->addItem(tr("Trace ")+tr("%1").arg(i+1));
// 	}

	if (Num_Scopes > 0)  showScopeOptions(currentScope);
	
//	if (Num_Scopes > 0) Get_Scope_Data_Thread = new GetScopeDataThread [Num_Scopes];
	//for (int i=0; i<Num_Scopes; ++i){
	//	connect(&Get_Scope_Data_Thread[i],SIGNAL(value(int,float)),ScopeWindows[i],SLOT(setValue(int,float)));
	//}
		offsetWheel->setMass(0.5);
	offsetWheel->setRange(-1e6, 1e6, 0.25);
	offsetWheel->setTotalAngle(360.0*2e6);
	offsetWheel->setFixedHeight(30);
	dxComboBox->setCompleter(0);
	dxComboBox->setValidator(new QDoubleValidator(this));

	RefreshRate=30.;
	timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
        timer->start((int)(1./RefreshRate*1000.));
	
	
//	tabWidget->addTab(traceWidget,tr("trace %1").arg(Scopes[currentScope].ntraces));

}
QRL_ScopesManager::~QRL_ScopesManager()
{
	for (int i=0; i<Num_Scopes; ++i){
		ScopeWindows[i]->hide();
	}
	delete[] ScopeWindows;
	//stopScopeThreads();
	//if (Get_Scope_Data_Thread)
	//	delete[] Get_Scope_Data_Thread;
}

void QRL_ScopesManager::refresh()
{
  for (int n=0; n<Num_Scopes; ++n){
	for (int t=0; t<Scopes[n].ntraces;++t){
		QList<float> v = targetThread->getScopeValue(n,t);
		for (int k=0; k<v.size(); ++k)
			ScopeWindows[n]->setValue(t,v.at(k));
	}
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
	//Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
	targetThread->setScopeDt(ScopeWindows[currentScope]->getDt(),currentScope);

	showScopeOptions(currentScope);
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
	//Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
	targetThread->setScopeDt(ScopeWindows[currentScope]->getDt(),currentScope);

	if (Num_Scopes > 0)  showScopeOptions(currentScope);

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
		color= QColorDialog::getColor(ScopeWindows[currentScope]->getBgColor());
		ScopeWindows[currentScope]->setBgColor(color);
		break;
		case 2 :
		color= QColorDialog::getColor(ScopeWindows[currentScope]->getGridColor());
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
		//targetThread->startSaving(fileLineEdit->text(),currentScope);
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
		showTraceOptions(currentTrace+scopeItems.size());
	}
}



void QRL_ScopesManager::showTraceOptions(int index)
{
	currentTrace=index-scopeItems.size();	
	lineWidthCounter->setValue(ScopeWindows[currentScope]->getTraceWidth(currentTrace));
	offsetCounter->setValue(ScopeWindows[currentScope]->getTraceOffset(currentTrace));
	dyComboBox->setEditText(tr("%1").arg(ScopeWindows[currentScope]->getTraceDy(currentTrace)));
	traceNameLineEdit->setText(traceItems[currentTrace]->text());
	//tabWidget->setTabText(1,traceItems[currentTrace]->text());
	if (ScopeWindows[currentScope]->getZeroAxis(currentTrace))
		zeroAxisCheckBox->setCheckState(Qt::Checked);
	else
		zeroAxisCheckBox->setCheckState(Qt::Unchecked);
}

void QRL_ScopesManager::changeTraceText(const QString & text ){
	traceItems[currentTrace]->setText(text);
	//tabWidget->setTabText(1,traceItems[currentTrace]->text());
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

  void QRL_ScopesManager::changeTriggerLevel(double l) {
	ScopeWindows[currentScope]->setTriggerLevel(l);

}
void QRL_ScopesManager::manualTrigger(){

	ScopeWindows[currentScope]->manualTriggerSignal();
}
void QRL_ScopesManager::startSingleRun(){

	ScopeWindows[currentScope]->startSingleRun();

}

void QRL_ScopesManager::changeSingleMode(int state){
	if(state==Qt::Checked){
		ScopeWindows[currentScope]->setSingleMode(true);
	} else {
		ScopeWindows[currentScope]->setSingleMode(false);
	}

}

void QRL_ScopesManager::changeDirection(int d)
{
	switch(d)
	{
	case 0:	
		switch(displayComboBox->currentIndex())
		{
		case 0:	
			ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::roll,Qt::RightToLeft);
			break;
		case 1:
			ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::overwrite,Qt::RightToLeft);
			break;
		case 2://trigger down
			ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::trigger,Qt::RightToLeft);
			break;
		case 3: // trigger up
			ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::trigger,Qt::RightToLeft);
			break;
		default:
			break;
		}
		break;
	case 1:
		switch(displayComboBox->currentIndex())
		{
		case 0:	
			ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::roll,Qt::LeftToRight);
			break;
		case 1:
			ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::overwrite,Qt::LeftToRight);
			break;
		case 2://trigger down
			ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::trigger,Qt::LeftToRight);
			break;
		case 3: // trigger up
			ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::trigger,Qt::LeftToRight);
			break;
		default:
			break;
		}
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
		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::roll,Qt::RightToLeft);
		directionComboBox->setCurrentIndex(0);
		break;
	case 1:
		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::overwrite,Qt::LeftToRight);
		directionComboBox->setCurrentIndex(1);
		break;
	case 2://trigger down
		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::trigger,Qt::LeftToRight);
		directionComboBox->setCurrentIndex(1);
		ScopeWindows[currentScope]->setTriggerUpDirection(false);
		break;
	case 3: // trigger up
		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::trigger,Qt::LeftToRight);
		ScopeWindows[currentScope]->setTriggerUpDirection(true);
		directionComboBox->setCurrentIndex(1);
		break;
	case 4: //hold
		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::hold,Qt::LeftToRight);
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

void QRL_ScopesManager::changeDy(const QString& text)
{
	if (!text.isEmpty() &&text.toDouble()!=0.0 ){
		double dy=text.toDouble();
		ScopeWindows[currentScope]->setTraceDy(dy,currentTrace);
	}
}

  void QRL_ScopesManager::changeZeroAxis(int state){

	if(state==Qt::Checked){
		ScopeWindows[currentScope]->setZeroAxis(true,currentTrace);
	} else {
		ScopeWindows[currentScope]->setZeroAxis(false,currentTrace);
	}

}










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
   file for the classe QRL_ScopesManager
*/

#include "scopes_manager.h"


/**
* @brief Initialize Scopes Manager
*/
QRL_ScopesManager::QRL_ScopesManager(QWidget *parent,QRtaiLabCore* qtargetinterface)
	:QDialog(parent),qTargetInterface(qtargetinterface)
{
	setupUi(this);
	Num_Scopes=qTargetInterface->getScopeNumber();
	Scopes=qTargetInterface->getScopes();
	const QIcon ScopeIcon =QIcon(QString::fromUtf8(":/icons/scope_icon.xpm"));
	ScopeWindows = new QRL_ScopeWindow* [Num_Scopes]; 
	for (int i=0; i<Num_Scopes; ++i){
		scopeItems << new QListWidgetItem(ScopeIcon,qTargetInterface->getScopeName(i), scopeListWidget);
		//if (qTargetInterface->getNumberOfTraces(i)>0){
			ScopeWindows[i]=new QRL_ScopeWindow(parent,Scopes[i],i);
			//connect( ScopeWindows[i], SIGNAL( stopSaving(int) ), this, SLOT( stopSaving(int) ) );
			ScopeWindows[i]->setVerbose(qTargetInterface->getVerbose());
		//} else
		//	ScopeWindows[i]=NULL;
	}
	tabWidget->setCurrentIndex(0);
	connect( showCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( showScope(int) ) );
	connect( scopeListWidget, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( showOptions( QListWidgetItem *  ) ) );
	connect( scopeListWidget, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( showOptions( QListWidgetItem *  ) ) );
	connect( scopeListWidget, SIGNAL( itemSelectionChanged() ), this, SLOT( showSelectedOptions() ) );
	//connect( rrLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeRefreshRate(const QString&) ) );
	connect( rrCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeRefreshRate(double) ) );
	connect( dataCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeDataPoints(double) ) );
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
        connect( traceComboBox,SIGNAL( currentIndexChanged(int) ),this, SLOT(changeTriggerChannel(int) ) );
        connect( zeroAxisCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeZeroAxis(int) ) );
	 connect( labelCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeTraceLabel(int) ) );
	 connect( unitCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeUnitLabel(int) ) );
	 connect( averageCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeAverageLabel(int) ) );
	 connect( minCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeMinLabel(int) ) );
	 connect( maxCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeMaxLabel(int) ) );
	 connect( ppCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changePPLabel(int) ) );
	 connect( rmsCheckBox,SIGNAL( stateChanged(int)),this,SLOT(changeRMSLabel(int) ) );
	connect( showTraceCheckBox , SIGNAL( stateChanged(int) ), this, SLOT( showTrace(int) ) );
         connect( timeCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeSaveTime(double) ) );
	connect( fileLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeFileName(const QString&) ) );
	connect( dirLineEdit, SIGNAL( textEdited(const QString &) ), this, SLOT( changeFileDir(const QString&) ) );
	connect( dividerCounter, SIGNAL( valueChanged(double) ), this, SLOT( changeDivider(double) ) );
	connect( stopPushButton, SIGNAL( pressed() ), this, SLOT( stopSaving() ) );
	connect( holdCheckBox , SIGNAL( stateChanged(int) ), this, SLOT( holdPlot(int) ) );
	connect( dirPushButton, SIGNAL( pressed()), this, SLOT(setFileDirectory() ) );
	connect( setMeanPushButton, SIGNAL( pressed()), this, SLOT(setOffsetToMean() ) );
	currentScope=0;
// 	for(int i=0; i<1; ++i){
// 		//tabWidget->addTab(new QWidget(tabWidget->widget(1)),tr("Trace ")+tr("%1").arg(i+1));
// 		traceComboBox->addItem(tr("Trace ")+tr("%1").arg(i+1));
// 	}
	traceComboBox->clear();
	for(int i=0; i<Scopes[currentScope]->getNTraces();i++){
		traceComboBox->addItem(	ScopeWindows[currentScope]->getTraceName(i) );
	}
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
	
	
//	tabWidget->addTab(traceWidget,tr("trace %1").arg(Scopes[currentScope].getNTraces()));

}
QRL_ScopesManager::~QRL_ScopesManager()
{
	for (int i=0; i<Num_Scopes; ++i){
		ScopeWindows[i]->hide();
		delete ScopeWindows[i];
	  
	}
	delete[] ScopeWindows;
	//stopScopeThreads();
	//if (Get_Scope_Data_Thread)
	//	delete[] Get_Scope_Data_Thread;
}

void QRL_ScopesManager::refresh()
{

//try {
  for (int n=0; n<Num_Scopes; ++n){
	//QVector< QVector<float> > v;
	//v = qTargetInterface->getTargetThread()->getScopeValue(n);
	//if (v.size()>0) {

//	printf("size scopevector:  %d x %d\n",v.size(),v.at(0).size());

// 	for (int t=0; t<qTargetInterface->getNumberOfTraces(n);++t)
// 	for (int k=0; k<v.at(0).size(); ++k){
// 			if (k<v.at(t).size())
// 			ScopeWindows[n]->setValue(t,v.at(t).at(k));
// 	}
     
	if (Scopes[n]->getNTraces()>0) {
	  if (Scopes[n]->isSaveScopeTime())
	     ScopeWindows[n]->setTime( Scopes[n]->getScopeTime());
	  ScopeWindows[n]->setValue( Scopes[n]->getScopeValue());
	 }
	//}
   }
// } catch (...){
// 	qDebug()<<"error in ScopesManager::refresh";
// 
// }


  if (Scopes[currentScope]->getIsSaving()==0){
      savePushButton->setEnabled(true);
      stopPushButton->setEnabled(false);
       saveProgressBar->setEnabled(false);
      saveProgressBar->setMaximum(100);
      saveProgressBar->setValue(100);
  }
  else
  {
     savePushButton->setEnabled(false);
     stopPushButton->setEnabled(true);
    saveProgressBar->setEnabled(true);
    saveProgressBar->setMaximum(Scopes[currentScope]->n_points_to_save());
     saveProgressBar->setValue(Scopes[currentScope]->getSavedPoints());
  }


}

    void QRL_ScopesManager::setFileVersion(qint32 v){
      fileVersion=v;
      for (int i=0; i<Num_Scopes;i++)
	ScopeWindows[i]->setFileVersion(v);

}

/**
* @brief set new refresh rate
* @param rr refresh rate
*/
void QRL_ScopesManager::changeRefreshRate(double rr)
{
	//double rr=text.toDouble();
	ScopeWindows[currentScope]->changeRefreshRate(rr);
	Scopes[currentScope]->setScopeRefreshRate(rr);
}


void QRL_ScopesManager::changeDX(const QString& text)
{
	if (!text.isEmpty() &&text.toDouble()!=0.0 ){
	double dx=text.toDouble();
	ScopeWindows[currentScope]->changeDX(dx);
	emit dxComboBox->setEditText(tr("%1").arg(ScopeWindows[currentScope]->getDX()));
	//Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
	 Scopes[currentScope]->setScopeDt(ScopeWindows[currentScope]->getDt());

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
	//ScopeWindows[currentScope]->changeDX(dxComboBox->currentText().toDouble());
	//Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
	 Scopes[currentScope]->setScopeDt(ScopeWindows[currentScope]->getDt());

	if (Num_Scopes > 0)  showScopeOptions(currentScope);

}

/**
* @brief change the number of datapoints in the plot
* @param dp data points
*/
void QRL_ScopesManager::changeDivider(double div)
{
	//double rr=text.toDouble();
	ScopeWindows[currentScope]->changeDivider(div);
	//ScopeWindows[currentScope]->changeDataPoints(dataCounter->value()/div);
	//ScopeWindows[currentScope]->changeDX(dxComboBox->currentText().toDouble());
	//Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());
	 Scopes[currentScope]->setScopeDt(ScopeWindows[currentScope]->getDt());

	if (Num_Scopes > 0)  showScopeOptions(currentScope);

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

void QRL_ScopesManager::setFileDirectory(){

 QString dir = QFileDialog::getExistingDirectory(this, tr("File Directory"),
                                                 dirLineEdit->text(),
                                                 QFileDialog::ShowDirsOnly);
  dirLineEdit->setText(dir);


}

void QRL_ScopesManager::startSaving()
{
	 FILE* Save_File_Pointer;
	double Save_Time=timeCounter->value();
	if( Scopes[currentScope]->start_saving_scope()==0){

		QString File_Name=dirLineEdit->text()+fileLineEdit->text();
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
			Scopes[currentScope]->startSaving(Save_File_Pointer,Save_Time);
		}

		
		//ScopeWindows[currentScope]->startSaving(fileLineEdit->text());
		//targetThread->startSaving(fileLineEdit->text(),currentScope);


	       }
       }


}

void QRL_ScopesManager::stopSaving()
{
 if (Scopes[currentScope]->getIsSaving()==1){
	 Scopes[currentScope]->stop_saving();
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
	   if (Scopes[currentScope]->getNTraces()>0) {
		//scopeListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
		for(int i=0; i<scopeItems.size();i++)
			scopeItems[i]->setHidden(true);
		if (traceItems.size()==0) {
			for(int i=0; i<Scopes[currentScope]->getNTraces();i++)
				traceItems << new QListWidgetItem(QIcon(),tr("trace %1").arg(i), scopeListWidget);
		} else {
			for(int i=0; i<traceItems.size();i++)
				traceItems[i]->setHidden(false);
		}
			showTraceOptions(currentTrace+scopeItems.size());
           } else {
		tabWidget->setCurrentIndex(0);
	  }
	}
}



void QRL_ScopesManager::showTraceOptions(int index)
{
	currentTrace=index-scopeItems.size();	
	lineWidthCounter->setValue(ScopeWindows[currentScope]->getTraceWidth(currentTrace));
	offsetCounter->setValue(ScopeWindows[currentScope]->getTraceOffset(currentTrace));
	dyComboBox->setEditText(tr("%1").arg(ScopeWindows[currentScope]->getTraceDy(currentTrace)));
	if (currentTrace<traceItems.size())
	traceNameLineEdit->setText(traceItems[currentTrace]->text());
	//tabWidget->setTabText(1,traceItems[currentTrace]->text());
	if (ScopeWindows[currentScope]->getZeroAxis(currentTrace))
		zeroAxisCheckBox->setCheckState(Qt::Checked);
	else
		zeroAxisCheckBox->setCheckState(Qt::Unchecked);

	if (ScopeWindows[currentScope]->getTraceLabel(currentTrace))
		labelCheckBox->setCheckState(Qt::Checked);
	else
		labelCheckBox->setCheckState(Qt::Unchecked);

	if (ScopeWindows[currentScope]->getUnitLabel(currentTrace))
		unitCheckBox->setCheckState(Qt::Checked);
	else
		unitCheckBox->setCheckState(Qt::Unchecked);

	if (ScopeWindows[currentScope]->getAverageLabel(currentTrace))
		averageCheckBox->setCheckState(Qt::Checked);
	else
		averageCheckBox->setCheckState(Qt::Unchecked);

	if (ScopeWindows[currentScope]->getMinLabel(currentTrace))
		minCheckBox->setCheckState(Qt::Checked);
	else
		minCheckBox->setCheckState(Qt::Unchecked);

	if (ScopeWindows[currentScope]->getMaxLabel(currentTrace))
		maxCheckBox->setCheckState(Qt::Checked);
	else
		maxCheckBox->setCheckState(Qt::Unchecked);

	if (ScopeWindows[currentScope]->getPPLabel(currentTrace))
		ppCheckBox->setCheckState(Qt::Checked);
	else
		ppCheckBox->setCheckState(Qt::Unchecked);

	if (ScopeWindows[currentScope]->getRMSLabel(currentTrace))
		rmsCheckBox->setCheckState(Qt::Checked);
	else
		rmsCheckBox->setCheckState(Qt::Unchecked);

	if (ScopeWindows[currentScope]->isTraceVisible(currentTrace))
		showTraceCheckBox->setCheckState(Qt::Checked);
	else
		showTraceCheckBox->setCheckState(Qt::Unchecked);
}

void QRL_ScopesManager::changeTraceText(const QString & text ){
	if (currentTrace<traceItems.size())
	traceItems[currentTrace]->setText(text);
	//tabWidget->setTabText(1,traceItems[currentTrace]->text());
	 ScopeWindows[currentScope]->setTraceName(currentTrace, text);
	traceComboBox->setItemText(currentTrace,text);
	//traceComboBox->setCurrentIndex(	ScopeWindows[currentScope]->getTriggerChannel());
}

/**
* @brief update manager dialog for the choosen scope
* @param item scope number
*/




void QRL_ScopesManager::showScopeOptions( int index ){

	currentScope=index;
	if(Scopes[currentScope]->start_saving_scope()==0){
		savePushButton->setEnabled(true);
	}
	tabWidget->setTabText(0,tr(Scopes[currentScope]->getName()));
	//fileLineEdit->setText(tr(Scopes[currentScope]->getName()));
	if(ScopeWindows[currentScope]->isVisible())
		showCheckBox->setCheckState(Qt::Checked);
	else
		showCheckBox->setCheckState(Qt::Unchecked);
	rrCounter->setValue(ScopeWindows[currentScope]->getRefreshRate());
	dataCounter->setValue(ScopeWindows[currentScope]->getDataPoints());
	dxComboBox->setEditText(tr("%1").arg(ScopeWindows[currentScope]->getDX()));
	dividerCounter->setValue(ScopeWindows[currentScope]->getDivider());
	currentTrace=0;
	for(int i=0; i<traceItems.size();i++)
		delete traceItems[i];
	traceItems.clear();
	//traceComboBox->clear();
	for(int i=0; i<Scopes[currentScope]->getNTraces();i++){
		traceItems << new QListWidgetItem(QIcon(),ScopeWindows[currentScope]->getTraceName(i), scopeListWidget);
		if (i<traceItems.size())
			traceItems[i]->setHidden(true);
		//traceComboBox->addItem(	ScopeWindows[currentScope]->getTraceName(i) );
		traceComboBox->setItemText(i,ScopeWindows[currentScope]->getTraceName(i) );
	}
	
	traceComboBox->setCurrentIndex(	ScopeWindows[currentScope]->getTriggerChannel());
	triggerCounter->setValue(ScopeWindows[currentScope]->getTriggerLevel() );
	oneShotCheckBox->setChecked( ScopeWindows[currentScope]->getSingleMode() );
	timeCounter->setValue(ScopeWindows[currentScope]->getSaveTime() );
	//fileLineEdit->setText(ScopeWindows[currentScope]->getFileName() );
	if (!(ScopeWindows[currentScope]->getFileName()).contains(tr("/"))){
	  fileLineEdit->setText((ScopeWindows[currentScope]->getFileName()));
	  dirLineEdit->setText("./");
	}else {
	  dirLineEdit->setText((ScopeWindows[currentScope]->getFileName()).left((ScopeWindows[currentScope]->getFileName()).lastIndexOf(tr("/"))+1));
	  fileLineEdit->setText( (ScopeWindows[currentScope]->getFileName()).mid((ScopeWindows[currentScope]->getFileName()).lastIndexOf(tr("/"))+1));
	}
	
	disconnect( directionComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDirection(int) ) );
	disconnect( displayComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDisplayModus(int) ) );

	if (ScopeWindows[currentScope]->getPlottingDirection()==Qt::RightToLeft)
		directionComboBox->setCurrentIndex(0);
	else
		directionComboBox->setCurrentIndex(1);
	
	switch(ScopeWindows[currentScope]->getPlottingMode())
	{
		case 0:	
			displayComboBox->setCurrentIndex(0);
			break;
		case 1:
			displayComboBox->setCurrentIndex(1);
			break;
		case 2://trigger down
			if (ScopeWindows[currentScope]->getTriggerUpDirection())
				displayComboBox->setCurrentIndex(3);
			else
				displayComboBox->setCurrentIndex(2);
			break;
// 		case 3:
// 			displayComboBox->setCurrentIndex(4);
// 			break;
		default:
			displayComboBox->setCurrentIndex(0);
			break;
	}

	connect( directionComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDirection(int) ) );
        connect( displayComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeDisplayModus(int) ) );
	
	if(ScopeWindows[currentScope]->isPlotting())
		holdCheckBox->setCheckState(Qt::Unchecked);
	else
		holdCheckBox->setCheckState(Qt::Checked);
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

void QRL_ScopesManager::showTrace(int state) 
{
	if(state==Qt::Checked){
		ScopeWindows[currentScope]->showTrace(true,currentTrace);
	} else {
		ScopeWindows[currentScope]->showTrace(false,currentTrace);
	}

}

  void QRL_ScopesManager::changeTriggerLevel(double l) {
	ScopeWindows[currentScope]->setTriggerLevel(l);

}

  void QRL_ScopesManager::changeTriggerChannel(int trace) {
	ScopeWindows[currentScope]->setTriggerChannel(trace);

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
		ScopeWindows[currentScope]->setPlottingDirection(Qt::RightToLeft);
		break;
	case 1:
		ScopeWindows[currentScope]->setPlottingDirection(Qt::LeftToRight);
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
		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::roll);
		ScopeWindows[currentScope]->setPlottingDirection(Qt::RightToLeft);
		directionComboBox->setCurrentIndex(0);
		break;
	case 1:
		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::overwrite);
		ScopeWindows[currentScope]->setPlottingDirection(Qt::LeftToRight);
		directionComboBox->setCurrentIndex(1);
		break;
	case 2://trigger down
		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::trigger);
		ScopeWindows[currentScope]->setPlottingDirection(Qt::LeftToRight);
		directionComboBox->setCurrentIndex(1);
		ScopeWindows[currentScope]->setTriggerUpDirection(false);
		break;
	case 3: // trigger up
		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::trigger);
		ScopeWindows[currentScope]->setPlottingDirection(Qt::LeftToRight);
		ScopeWindows[currentScope]->setTriggerUpDirection(true);
		directionComboBox->setCurrentIndex(1);
		break;
// 	case 4: //hold
// 		ScopeWindows[currentScope]->setPlottingMode(QRL_ScopeWindow::hold);
// 		break;
	default:
		break;
	}

}


void QRL_ScopesManager::holdPlot(int state) {

	if( state==Qt::Checked)	
		ScopeWindows[currentScope]->setPlotting(false);
	else
		ScopeWindows[currentScope]->setPlotting(true);


}

void QRL_ScopesManager::changeTraceColor()
{
	QColor color= QColorDialog::getColor(ScopeWindows[currentScope]->getTraceColor(currentTrace));
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

void QRL_ScopesManager::setOffsetToMean()
{
	double offset = ScopeWindows[currentScope]->getTraceAverage(currentTrace);

	offset=-(offset/ScopeWindows[currentScope]->getTraceDy(currentTrace));
	changeOffset(offset);
}

void QRL_ScopesManager::changeSaveTime(double time)
{
	ScopeWindows[currentScope]->setSaveTime(time);
	
}

void QRL_ScopesManager::changeFileName(const QString& str)
{
	ScopeWindows[currentScope]->setFileName(dirLineEdit->text()+str);
}
void QRL_ScopesManager::changeFileDir(const QString& str)
{
	ScopeWindows[currentScope]->setFileName(str+fileLineEdit->text());
}


void QRL_ScopesManager::changeDy(const QString& text)
{
	if (!text.isEmpty() &&text.toDouble()!=0.0 ){
		double dy=text.toDouble();
		double offset = ScopeWindows[currentScope]->getTraceOffset(currentTrace);
		offset=(offset*ScopeWindows[currentScope]->getTraceDy(currentTrace))/dy;
		changeOffset(offset);
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

  void QRL_ScopesManager::changeTraceLabel(int state){

	if(state==Qt::Checked){
		ScopeWindows[currentScope]->setTraceLabel(true,currentTrace);
	} else {
		ScopeWindows[currentScope]->setTraceLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());

}

  void QRL_ScopesManager::changeUnitLabel(int state){

	if(state==Qt::Checked){
		ScopeWindows[currentScope]->setUnitLabel(true,currentTrace);
	} else {
		ScopeWindows[currentScope]->setUnitLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

  void QRL_ScopesManager::changeAverageLabel(int state){

	if(state==Qt::Checked){
		ScopeWindows[currentScope]->setAverageLabel(true,currentTrace);
	} else {
		ScopeWindows[currentScope]->setAverageLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

  void QRL_ScopesManager::changeMinLabel(int state){

	if(state==Qt::Checked){
		ScopeWindows[currentScope]->setMinLabel(true,currentTrace);
	} else {
		ScopeWindows[currentScope]->setMinLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

  void QRL_ScopesManager::changeMaxLabel(int state){

	if(state==Qt::Checked){
		ScopeWindows[currentScope]->setMaxLabel(true,currentTrace);
	} else {
		ScopeWindows[currentScope]->setMaxLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

  void QRL_ScopesManager::changePPLabel(int state){

	if(state==Qt::Checked){
		ScopeWindows[currentScope]->setPPLabel(true,currentTrace);
	} else {
		ScopeWindows[currentScope]->setPPLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

  void QRL_ScopesManager::changeRMSLabel(int state){

	if(state==Qt::Checked){
		ScopeWindows[currentScope]->setRMSLabel(true,currentTrace);
	} else {
		ScopeWindows[currentScope]->setRMSLabel(false,currentTrace);
	}
	showTraceOptions(currentTrace+scopeItems.size());
}

QDataStream& operator<<(QDataStream &out, const QRL_ScopesManager &d){
	out << d.size()  << d.pos() << d.isVisible();
	out <<(qint32) d.Num_Scopes;
	for (int i = 0; i < d.Num_Scopes; ++i) {
		out<<*(d.ScopeWindows[i]);
	}
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_ScopesManager(&d)){
	QSize s;QPoint p;bool b; 
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	qint32 a;
	in >> a;
	for (int i = 0; i < (int)a; ++i) {
		if (d.Num_Scopes>i){
			d.ScopeWindows[i]->setFileVersion(d.fileVersion);
			in>>*(d.ScopeWindows[i]);
		}
		else {
			d.ScopeWindows[d.Num_Scopes-1]->setFileVersion(d.fileVersion);
			in>>*(d.ScopeWindows[d.Num_Scopes-1]);
		}
	}
	d.showScopeOptions(d.currentScope);
	return in;
}






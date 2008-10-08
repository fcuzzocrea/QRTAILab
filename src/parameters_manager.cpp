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
 file:		parameters_manager.cpp
 describtion:
   file for the classes QRL_ParametersManager
*/
#include "parameters_manager.h"

/**
 * @brief Initialize Parameters Manager
 */
QRL_ParametersManager::QRL_ParametersManager(QWidget *parent, QRL_Parameters	*parameters)
	:QDialog(parent),Parameters(parameters)
{
	setupUi(this);
	
	Num_Tunable_Parameters=Parameters->getParameterNumber();
	Num_Tunable_Blocks=Parameters->getBlockNumber();
	
	connect( blockListWidget, SIGNAL(itemEntered( QListWidgetItem * ) ), this, SLOT( showTunableParameter( QListWidgetItem *  ) ) );
	connect( blockListWidget, SIGNAL(itemActivated( QListWidgetItem * ) ), this, SLOT( showTunableParameter( QListWidgetItem *  ) ) );
	connect( parameterTableWidget, SIGNAL( itemChanged( QTableWidgetItem * ) ), this, SLOT( changeTunableParameter( QTableWidgetItem *  ) ) );
	connect( batchCheckBox, SIGNAL( stateChanged( int  ) ), this, SLOT( batchMode( int  ) ) );
	connect( uploadPushButton, SIGNAL( pressed() ), this, SLOT( uploadParameters() ) );
	connect( downloadPushButton, SIGNAL( pressed() ), this, SLOT( downloadBatchParameters() ) );
	connect( fineRadioButton, SIGNAL( toggled( bool  ) ), this, SLOT( showAllBlocks( bool  ) ) );
	connect( hideRadioButton, SIGNAL( toggled( bool  ) ), this, SLOT( showBlocks( bool  ) ) );
	connect( showRadioButton, SIGNAL( toggled( bool  ) ), this, SLOT( hideBlocks( bool  ) ) );
	connect( paramLineEdit, SIGNAL( textEdited( const QString&  ) ), this, SLOT( changeSearchText( const QString&  ) ) );
	BlockIcon =QIcon(QString::fromUtf8(":/icons/block_icon.xpm"));

	for (int i=0; i<Num_Tunable_Blocks; ++i){
	   
		new QListWidgetItem(BlockIcon,Parameters->getBlockName(i), blockListWidget);
	}
	batchModus=0;
}

/**
 * @brief Delete Parameters Manager
 */
QRL_ParametersManager::~QRL_ParametersManager()
{
	blockListWidget->clear();
	parameterTableWidget->clear();
}

/**
 * @brief Enable batchmode. In batchmode, changed parameters will be downloaded to the program only by activating the uploading button
 * @param state enables batchmode
 */
void QRL_ParametersManager::batchMode(int state){
	if(state==Qt::Checked){
		downloadPushButton->setEnabled(true);
		batchModus=1;
	}else{
		downloadPushButton->setEnabled(false);
		batchModus=0;
		uploadParameters();
	}
}

void QRL_ParametersManager::showAllBlocks( bool  state){
 blockListWidget->clear();
	if(state==false){
	  QString str=paramLineEdit->text();
	  QString tmp;
	  if (hideRadioButton->isChecked()){
	    for (int i=0; i<Num_Tunable_Blocks; ++i){
	      tmp=Parameters->getBlockName(i);
	      if (tmp.contains(str, Qt::CaseInsensitive)){
		Parameters->setBlockVisible(i,false);
	      } else {
		Parameters->setBlockVisible(i,true);
		new QListWidgetItem(BlockIcon,Parameters->getBlockName(i), blockListWidget);
	      }
	    }
	  } else {
	     for (int i=0; i<Num_Tunable_Blocks; ++i){
	      tmp=Parameters->getBlockName(i);
	      if (tmp.contains(str, Qt::CaseInsensitive)){
		Parameters->setBlockVisible(i,true);
		new QListWidgetItem(BlockIcon,Parameters->getBlockName(i), blockListWidget);
	      } else {
		Parameters->setBlockVisible(i,false);
	      }
	    }
	  }
	}else{
	  for (int i=0; i<Num_Tunable_Blocks; ++i){
	     Parameters->setBlockVisible(i,true);
	      new QListWidgetItem(BlockIcon,Parameters->getBlockName(i), blockListWidget);
	  }
	}
}

void QRL_ParametersManager::showBlocks( bool  state){
  showAllBlocks(fineRadioButton->isChecked());
}

void QRL_ParametersManager::hideBlocks( bool  state){
   showAllBlocks(fineRadioButton->isChecked());
}

void QRL_ParametersManager::changeSearchText( const QString&  t){
if (!fineRadioButton->isChecked())
  showAllBlocks(fineRadioButton->isChecked());
}

/**
 * @brief Upload all parameters from the program, and updates the parameter in the widget
 */
void QRL_ParametersManager::uploadParameters()
{
	Parameters->uploadParameters();
	Parameters->resetBatchMode();
	showTunableParameter(blockListWidget->currentItem());
}
/**
 * @brief Download all changed parameters to the program.
 */
void QRL_ParametersManager::downloadBatchParameters()
{
	if(batchModus)
		Parameters->batchParameterDownload();

}

/**
 * @brief Updates a changed parameter
 */
void QRL_ParametersManager::changeTunableParameter(QTableWidgetItem * item ) 
{
	int blk =0;
	int blockCounter=-1;
	for (int j=0; j<Num_Tunable_Blocks; ++j){
	    if (Parameters->isBlockVisible(j))
		blockCounter++;
	    if ((blockCounter==blockListWidget->currentRow() ) && blk==0){
	      blk=j; 
	    }
	      
	}




	int ind=0;
        int prm_row=item->row();
	int prm_col=item->column()-1; //first column is paramter name
	double data_value;
	// get old value
	int jend;
	int prm=prm_row;
	int table_row=0;
	// get old value
	jend=Parameters->getNumberOfParameters(blk);
	for (int j = 0; j <  jend; j++) {
		unsigned int ncols = Parameters->getParameterCols(blk,j);
		unsigned int nrows = Parameters->getParameterRows(blk,j);
		for (unsigned int nr = 0; nr < nrows; nr++) {
			for (unsigned int nc = 0; nc < ncols; nc++) {
				if ((prm_row==table_row) && (prm_col==nc)){
					data_value=Parameters->getParameterValue(blk,j,nr, nc);
					prm=j;
				}
			}
			table_row++;
		}
		
	}

	QDoubleValidator *DoubleTest= new QDoubleValidator(NULL);
	QString s=item->text();
	int pos;
	if (DoubleTest->validate(s,pos)== QValidator::Invalid)
		item->setText(tr("%1").arg(data_value)); // set to old value
	else { // set to new value
		if (batchModus==0){
			double value=(item->text()).toDouble();
			//printf("Item changed to %f (%d,%d)\n",value,blk,prm);
			Parameters->updateParameterValue(blk,prm,prm_row,prm_col,value);
		} else {
			double value=(item->text()).toDouble();
			//printf("Item changed to %f (%d,%d)\n",value,blk,prm);
			Parameters->addToBatch(blk,prm,prm_row,prm_col,value);
		}
	}

	delete DoubleTest;
}
/**
 * @brief show all parameter for a given parameter block
 */
void QRL_ParametersManager::showTunableParameter(QListWidgetItem * item ) 
{
	parameterTableWidget->blockSignals(true);
	int i =0;
	int blockCounter=-1;
	for (int j=0; j<Num_Tunable_Blocks; ++j){
	    if (Parameters->isBlockVisible(j))
		blockCounter++;
	    if ((blockCounter==blockListWidget->currentRow()) && i==0){
	      i=j; 
	    }
	      
	}
	parameterTableWidget->clear();
	int jend,val_idx;
	double data_value;
	//const QIcon BlockIcon =QIcon(QString::fromUtf8(":/icons/parameters_icon.xpm"));
	jend=Parameters->getNumberOfParameters(i);
	parameterTableWidget->setRowCount(jend);
	parameterTableWidget->setColumnCount(2);
	QTableWidgetItem *newItem;
	int table_row=0;
	for (int j = 0; j <  jend; j++) {
		newItem = new QTableWidgetItem(Parameters->getParameterName(i,j));
		newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
		parameterTableWidget->setItem(table_row,0,newItem);

		unsigned int ncols = Parameters->getParameterCols(i,j);
		if ((ncols+1)>parameterTableWidget->columnCount())
			parameterTableWidget->setColumnCount(ncols+1);
		unsigned int nrows =Parameters->getParameterRows(i,j);;
		if (nrows>1)
			parameterTableWidget->setRowCount(parameterTableWidget->rowCount()+nrows-1);
		for (unsigned int nr = 0; nr < nrows; nr++) {
			if(nr>0){
				newItem = new QTableWidgetItem(tr(" "));
				newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
				parameterTableWidget->setItem(table_row,0,newItem);
			}
			for (unsigned int nc = 0; nc < ncols; nc++) {
				data_value=Parameters->getParameterValue(i,j, nr, nc);
				newItem = new QTableWidgetItem(tr("%1").arg(data_value));
				parameterTableWidget->setItem(table_row,nc+1,newItem);
			}
			table_row++;
		}
		
	}
	parameterTableWidget->blockSignals(false);
}

QDataStream& operator<<(QDataStream &out, const QRL_ParametersManager &d){
	out << d.size()  << d.pos() << d.isVisible();
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_ParametersManager(&d)){
	QSize s;QPoint p;bool b; int i;
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	return in;
}

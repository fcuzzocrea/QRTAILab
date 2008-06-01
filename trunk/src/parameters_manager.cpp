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
 file:		parameters_manager.cpp
 describtion:
   file for the classes QRL_ParametersManager
*/
#include "parameters_manager.h"

/**
 * @brief Initialize Parameters Manager
 */
QRL_ParametersManager::QRL_ParametersManager(QWidget *parent, QRtaiLabCore	*qtargetinterface)
	:QDialog(parent),qTargetInterface(qtargetinterface)
{
	setupUi(this);
	Num_Tunable_Parameters=qTargetInterface->getParameterNumber();
	Num_Tunable_Blocks=qTargetInterface->getBlockNumber();
	connect( blockListWidget, SIGNAL(itemEntered( QListWidgetItem * ) ), this, SLOT( showTunableParameter( QListWidgetItem *  ) ) );
	connect( blockListWidget, SIGNAL(itemActivated( QListWidgetItem * ) ), this, SLOT( showTunableParameter( QListWidgetItem *  ) ) );
	connect( parameterTableWidget, SIGNAL( itemChanged( QTableWidgetItem * ) ), this, SLOT( changeTunableParameter( QTableWidgetItem *  ) ) );
	connect( batchCheckBox, SIGNAL( stateChanged( int  ) ), this, SLOT( batchMode( int  ) ) );
	connect( uploadPushButton, SIGNAL( pressed() ), this, SLOT( uploadParameters() ) );
	connect( downloadPushButton, SIGNAL( pressed() ), this, SLOT( downloadBatchParameters() ) );
	const QIcon BlockIcon =QIcon(QString::fromUtf8(":/icons/block_icon.xpm"));
	for (int i=0; i<Num_Tunable_Blocks; ++i){
		new QListWidgetItem(BlockIcon,qTargetInterface->getBlockName(i), blockListWidget);
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
/**
 * @brief Upload all parameters from the program, and updates the parameter in the widget
 */
void QRL_ParametersManager::uploadParameters()
{
	qTargetInterface->uploadParameters();
	qTargetInterface->resetBatchMode();
	showTunableParameter(blockListWidget->currentItem());
}
/**
 * @brief Download all changed parameters to the program.
 */
void QRL_ParametersManager::downloadBatchParameters()
{
	if(batchModus)
		qTargetInterface->batchParameterDownload();

}

/**
 * @brief Updates a changed parameter
 */
void QRL_ParametersManager::changeTunableParameter(QTableWidgetItem * item ) 
{
	int blk =blockListWidget->currentRow() ;
	int ind=0;
        int prm_row=item->row();
	int prm_col=item->column()-1; //first column is paramter name
	double data_value;
	// get old value
	int jend;
	int prm=prm_row;
	int table_row=0;
	// get old value
	jend=qTargetInterface->getNumberOfParameters(blk);
	for (int j = 0; j <  jend; j++) {
		unsigned int ncols = qTargetInterface->getParameterCols(blk,j);
		unsigned int nrows = qTargetInterface->getParameterRows(blk,j);
		for (unsigned int nr = 0; nr < nrows; nr++) {
			for (unsigned int nc = 0; nc < ncols; nc++) {
				if ((prm_row==table_row) && (prm_col==nc)){
					data_value=qTargetInterface->getParameterValue(blk,j,nr, nc);
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
			qTargetInterface->updateParameterValue(blk,prm,prm_row,prm_col,value);
		} else {
			double value=(item->text()).toDouble();
			//printf("Item changed to %f (%d,%d)\n",value,blk,prm);
			qTargetInterface->addToBatch(blk,prm,prm_row,prm_col,value);
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
	int i = blockListWidget->row(item);
	parameterTableWidget->clear();
	int jend,val_idx;
	double data_value;
	//const QIcon BlockIcon =QIcon(QString::fromUtf8(":/icons/parameters_icon.xpm"));
	jend=qTargetInterface->getNumberOfParameters(i);
	parameterTableWidget->setRowCount(jend);
	parameterTableWidget->setColumnCount(2);
	QTableWidgetItem *newItem;
	int table_row=0;
	for (int j = 0; j <  jend; j++) {
		newItem = new QTableWidgetItem(qTargetInterface->getParameterName(i,j));
		newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
		parameterTableWidget->setItem(table_row,0,newItem);

		unsigned int ncols = qTargetInterface->getParameterCols(i,j);
		if ((ncols+1)>parameterTableWidget->columnCount())
			parameterTableWidget->setColumnCount(ncols+1);
		unsigned int nrows =qTargetInterface->getParameterRows(i,j);;
		if (nrows>1)
			parameterTableWidget->setRowCount(parameterTableWidget->rowCount()+nrows-1);
		for (unsigned int nr = 0; nr < nrows; nr++) {
			if(nr>0){
				newItem = new QTableWidgetItem(tr(" "));
				newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
				parameterTableWidget->setItem(table_row,0,newItem);
			}
			for (unsigned int nc = 0; nc < ncols; nc++) {
				data_value=qTargetInterface->getParameterValue(i,j, nr, nc);
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
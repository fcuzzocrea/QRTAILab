/*
 file:		parameters_manager.cpp
 describtion:
   file for the classes QRL_ParametersManager

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

#include "parameters_manager.h"
#include "main_window.h"

//extern QRtaiLab *mainWin;

///  Initialize Parameters Manager
QRL_ParametersManager::QRL_ParametersManager(QWidget *parent, TargetThread* targetthread)
	:QDialog(parent),targetThread(targetthread)
{
	setupUi(this);
	Num_Tunable_Parameters=targetThread->getParameterNumber();
	Num_Tunable_Blocks=targetThread->getBlockNumber();
	Tunable_Blocks=targetThread->getBlocks();
	Tunable_Parameters=targetThread->getParamters();
	connect( blockListWidget, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( showTunableParameter( QListWidgetItem *  ) ) );
	connect( parameterTableWidget, SIGNAL( itemChanged( QTableWidgetItem * ) ), this, SLOT( changeTunableParameter( QTableWidgetItem *  ) ) );
	connect( batchCheckBox, SIGNAL( stateChanged( int  ) ), this, SLOT( batchMode( int  ) ) );
	connect( uploadPushButton, SIGNAL( pressed() ), this, SLOT( uploadParameters() ) );
	connect( downloadPushButton, SIGNAL( pressed() ), this, SLOT( downloadBatchParameters() ) );
	const QIcon BlockIcon = QIcon("icons/block_icon.xpm");
	for (int i=0; i<Num_Tunable_Blocks; ++i){
		new QListWidgetItem(BlockIcon,tr(Tunable_Blocks[i].name), blockListWidget);
	}
	batchModus=0;
}

QRL_ParametersManager::~QRL_ParametersManager()
{
	blockListWidget->clear();
	parameterTableWidget->clear();
}


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
void QRL_ParametersManager::uploadParameters()
{
	targetThread->uploadParameters();
	targetThread->resetBatchMode();
	emit showTunableParameter(blockListWidget->currentItem());
}

void QRL_ParametersManager::downloadBatchParameters()
{
	if(batchModus)
		targetThread->batchParameterDownload();

}

void QRL_ParametersManager::update()
{
	Num_Tunable_Parameters=targetThread->getParameterNumber();
	Num_Tunable_Blocks=targetThread->getBlockNumber();
	Tunable_Blocks=targetThread->getBlocks();
	Tunable_Parameters=targetThread->getParamters();
}

double QRL_ParametersManager::get_parameter(Target_Parameters_T p, int nr, int nc, int *val_idx)
{
	switch (p.data_class) {
		case rt_SCALAR:
			*val_idx = 0;
			return (p.data_value[0]);
		case rt_VECTOR:
			*val_idx = nc;
			return (p.data_value[nc]);
		case rt_MATRIX_ROW_MAJOR:
			*val_idx = nr*p.n_cols+nc;
			return (p.data_value[nr*p.n_cols+nc]);
		case rt_MATRIX_COL_MAJOR:
			*val_idx = nc*p.n_rows+nr;
			return (p.data_value[nc*p.n_rows+nr]);
		default:
			return (0.0);
	}
}

int QRL_ParametersManager::update_parameter(int idx, int mat_idx, double val)
{
	Tunable_Parameters[idx].data_value[mat_idx] = val;
	return 1;
}

void QRL_ParametersManager::changeTunableParameter(QTableWidgetItem * item ) 
{
	int jend,val_idx;
	int blk =blockListWidget->currentRow() ;
	int prm = item->row();
	int ind = 0;
	int table_row=0;
	double data_value;
	if (blk == Num_Tunable_Blocks - 1) 
		jend=Num_Tunable_Parameters - Tunable_Blocks[blk].offset;
	else
		jend=Tunable_Blocks[blk+1].offset-Tunable_Blocks[blk].offset;
	for (int j = 0; j <  jend; j++) {
		unsigned int ncols = Tunable_Parameters[Tunable_Blocks[blk].offset+j].n_cols;
		unsigned int nrows = Tunable_Parameters[Tunable_Blocks[blk].offset+j].n_rows;
		for (unsigned int nr = 0; nr < nrows; nr++) {
			for (unsigned int nc = 0; nc < ncols; nc++) {
				if ((item->row()==table_row) && (item->column()==nc+1)){
					data_value=get_parameter(Tunable_Parameters[Tunable_Blocks[blk].offset+j], nr, nc, &ind);
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
		item->setText(tr("%1").arg(data_value));
	else {
		if (batchModus==0){
			double value=(item->text()).toDouble();
			//printf("Item changed to %f (%d,%d)\n",value,blk,prm);
			int map_offset = Tunable_Blocks[blk].offset + prm;
			if (update_parameter(map_offset, ind, value)) {
				targetThread->downloadParameter(ind,map_offset);
			}
		} else {
			double value=(item->text()).toDouble();
			//printf("Item changed to %f (%d,%d)\n",value,blk,prm);
			int map_offset = Tunable_Blocks[blk].offset + prm;
			if (update_parameter(map_offset, ind, value)) {
				if (targetThread->addToBatch(map_offset,ind,value)==-1)
					printf("Could not add to Batch");
			}
		}
	}

	delete DoubleTest;
}

void QRL_ParametersManager::showTunableParameter(QListWidgetItem * item ) 
{
	parameterTableWidget->blockSignals(true);
	int i = blockListWidget->row(item);
	parameterTableWidget->clear();
	int jend,val_idx;
	double data_value;
	const QIcon BlockIcon = QIcon("icons/parameters_icon.xpm");
	if (i == Num_Tunable_Blocks - 1) 
		jend=Num_Tunable_Parameters - Tunable_Blocks[i].offset;
	else
		jend=Tunable_Blocks[i+1].offset-Tunable_Blocks[i].offset;
	parameterTableWidget->setRowCount(jend);
	parameterTableWidget->setColumnCount(2);
	QTableWidgetItem *newItem;
	int table_row=0;
	for (int j = 0; j <  jend; j++) {
		newItem = new QTableWidgetItem(tr(Tunable_Parameters[Tunable_Blocks[i].offset+j].param_name));
		newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
		parameterTableWidget->setItem(table_row,0,newItem);

		unsigned int ncols = Tunable_Parameters[Tunable_Blocks[i].offset+j].n_cols;
		if ((ncols+1)>parameterTableWidget->columnCount())
			parameterTableWidget->setColumnCount(ncols+1);
		unsigned int nrows = Tunable_Parameters[Tunable_Blocks[i].offset+j].n_rows;
		if (nrows>1)
			parameterTableWidget->setRowCount(parameterTableWidget->rowCount()+nrows-1);
		for (unsigned int nr = 0; nr < nrows; nr++) {
			if(nr>0){
				newItem = new QTableWidgetItem(tr(" "));
				newItem->setFlags(!Qt::ItemIsEditable|!Qt::ItemIsSelectable);
				parameterTableWidget->setItem(table_row,0,newItem);
			}
			for (unsigned int nc = 0; nc < ncols; nc++) {
				data_value=get_parameter(Tunable_Parameters[Tunable_Blocks[i].offset+j], nr, nc, &val_idx);
				newItem = new QTableWidgetItem(tr("%1").arg(data_value));
				parameterTableWidget->setItem(table_row,nc+1,newItem);
			}
			table_row++;
		}
		
	}
	parameterTableWidget->blockSignals(false);
}

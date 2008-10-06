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
 file:		parameter.cpp
 describtion:
   file for the classes QRL_ParametersManager
*/
#include "parameters.h"

QRL_ParameterMatrix::QRL_ParameterMatrix()
{
    n_rows=0;
    n_cols=0;
    name=QString("");

}

QRL_ParameterMatrix::~QRL_ParameterMatrix(){

}

void QRL_ParameterMatrix::setDim(unsigned int r, unsigned int c){
  values.resize(r);
  for (int i=0; i<r; i++)
    values[i].resize(c);
  n_rows=r;
  n_cols=c;
}

void QRL_ParameterMatrix::setName(QString n){
  name = n;
}

/**
 * @brief Initialize Parameter Clasas
 */
QRL_ParameterBlock::QRL_ParameterBlock(QString n, int num_parameters)
	:name(n),Num_Parameters(num_parameters)
{
	ParameterMatrix = new QRL_ParameterMatrix* [Num_Parameters]; 
	for (int i=0; i<Num_Parameters; ++i){
		ParameterMatrix[i]=new QRL_ParameterMatrix();
	}
	
}



QRL_ParameterBlock::~QRL_ParameterBlock()
{
  for (int i=0; i<Num_Parameters; ++i) {
    delete ParameterMatrix[i];
  }
  delete[] ParameterMatrix;
}

void QRL_ParameterBlock::setParameterName(int param,QString name)
{
  ParameterMatrix[param]->setName(name);
}

void QRL_ParameterBlock::setParameterDim(int param, unsigned int n_rows, unsigned int n_cols){
  ParameterMatrix[param]->setDim(n_rows,n_cols);
}
void QRL_ParameterBlock::setParameterValue(int param, unsigned int n_rows, unsigned int n_cols, double value)
{
    ParameterMatrix[param]->setValue(n_rows,n_cols,value);


}
/**
 * @brief Initialize Parameter Clasas
 */
QRL_Parameters::QRL_Parameters(QRtaiLabCore	*qtargetinterface)
	:qTargetInterface(qtargetinterface)
{
	Num_Tunable_Parameters=qTargetInterface->getParameterNumber();
	Num_Tunable_Blocks=qTargetInterface->getBlockNumber();
	ParameterBlocks = new QRL_ParameterBlock* [Num_Tunable_Blocks]; 
	batchModus=0;
	for (int i=0; i<Num_Tunable_Blocks; ++i){
		ParameterBlocks[i]=new QRL_ParameterBlock(qTargetInterface->getBlockName(i), qTargetInterface->getNumberOfParameters(i));
		for (int k=0; k<qTargetInterface->getNumberOfParameters(i);k++){
		  ParameterBlocks[i]->setParameterName(k,qTargetInterface->getParameterName(i,k));
		  ParameterBlocks[i]->setParameterDim(k,qTargetInterface->getParameterRows(i,k),qTargetInterface->getParameterCols(i,k));
		}
	}
	uploadParameters();
	
}

/**
 * @brief Delete Parameters Manager
 */
QRL_Parameters::~QRL_Parameters()
{
    for (int i=0; i<Num_Tunable_Blocks; ++i){
      delete ParameterBlocks[i];
    }
    delete[] ParameterBlocks;
}

void QRL_Parameters::reload(){

 for (int i=0; i<Num_Tunable_Blocks; ++i){
      delete ParameterBlocks[i];
    }
    delete[] ParameterBlocks;
Num_Tunable_Parameters=qTargetInterface->getParameterNumber();
	Num_Tunable_Blocks=qTargetInterface->getBlockNumber();
	ParameterBlocks = new QRL_ParameterBlock* [Num_Tunable_Blocks]; 
	batchModus=0;
	for (int i=0; i<Num_Tunable_Blocks; ++i){
		ParameterBlocks[i]=new QRL_ParameterBlock(qTargetInterface->getBlockName(i), qTargetInterface->getNumberOfParameters(i));
		for (int k=0; k<qTargetInterface->getNumberOfParameters(i);k++){
		  ParameterBlocks[i]->setParameterName(k,qTargetInterface->getParameterName(i,k));
		  ParameterBlocks[i]->setParameterDim(k,qTargetInterface->getParameterRows(i,k),qTargetInterface->getParameterCols(i,k));
		}
	}
	uploadParameters();




}

void  QRL_Parameters::uploadParameters()
{
  //qTargetInterface->uploadParameters();
  //qTargetInterface->resetBatchMode();

  for (int i=0; i<Num_Tunable_Blocks; ++i)
    for (int k=0; k<ParameterBlocks[i]->getNumberOfParameters();k++)
     	 for(int r=0; r<ParameterBlocks[i]->getParameterRows(k); r++)
	   for(int c=0; c<ParameterBlocks[i]->getParameterCols(k); c++)
	    ParameterBlocks[i]->setParameterValue(k,c,r,qTargetInterface->getParameterValue(i,k, r, c));
  

}

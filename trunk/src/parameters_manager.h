/*
 file:		parameters_manager.h
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

#ifndef _PARAMETERS_MANAGER_H
#define _PARAMETERS_MANAGER_H 1


#include "ui_qrl_parameters_manager.h"
#include "qrtailab.h"

/**
 * @brief Parameter Manager
 * @todo mark batched parameters
 */

class QRL_ParametersManager : public QDialog, private Ui::QRL_ParametersManager
{
   Q_OBJECT
   Q_ENUMS (Param_Class)
public:
   enum Param_Class {rt_SCALAR,rt_VECTOR,rt_MATRIX_ROW_MAJOR,rt_MATRIX_COL_MAJOR,rt_MATRIX_COL_MAJOR_ND};
   QRL_ParametersManager(QWidget *parent = 0,  TargetThread* targetthread=NULL);
   ~QRL_ParametersManager();
public slots:
  void batchMode(int);
  void  showTunableParameter( QListWidgetItem * item );
  void  changeTunableParameter( QTableWidgetItem * item );
  void  uploadParameters();
  void downloadBatchParameters();
private:
     double get_parameter(Target_Parameters_T p, int nr, int nc, int *val_idx);
  int update_parameter(int idx, int mat_idx, double val);
  int Num_Tunable_Parameters;
  int Num_Tunable_Blocks;
  Target_Blocks_T *Tunable_Blocks;
  Target_Parameters_T *Tunable_Parameters;
   TargetThread* targetThread;
  int batchModus;
};


#endif

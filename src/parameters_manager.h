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
 file:		parameters_manager.h
 describtion:
   file for the classes QRL_ParametersManager
*/

#ifndef _PARAMETERS_MANAGER_H
#define _PARAMETERS_MANAGER_H 1


#include "ui_qrl_parameters_manager.h"
#include "qrtailab_core.h"

/**
 * @brief Parameter Manager
 * @todo mark batched parameters
 */

class QRL_ParametersManager : public QDialog, private Ui::QRL_ParametersManager
{
   Q_OBJECT
   Q_ENUMS (Param_Class)
public:
  // enum Param_Class {rt_SCALAR,rt_VECTOR,rt_MATRIX_ROW_MAJOR,rt_MATRIX_COL_MAJOR,rt_MATRIX_COL_MAJOR_ND};
   QRL_ParametersManager(QWidget *parent = 0,    QRtaiLabCore	*qtargetinterface=NULL);
   ~QRL_ParametersManager();
public slots:
  void batchMode(int);
  void  showTunableParameter( QListWidgetItem * item );
  void  changeTunableParameter( QTableWidgetItem * item );
  void  uploadParameters();
  void downloadBatchParameters();
private:
   //  double get_parameter(Target_Parameters_T p, int nr, int nc, int *val_idx);
  int update_parameter(int idx, int mat_idx, double val);
  int Num_Tunable_Parameters;
  int Num_Tunable_Blocks;
   QRtaiLabCore	*qTargetInterface;
  int batchModus;
};


#endif

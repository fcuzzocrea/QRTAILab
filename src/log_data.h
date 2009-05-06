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
 file:		parameters.h
 describtion:
   file for the classes QRL_ParametersManager
*/

#ifndef _LOG_DATA_H
#define _LOG_DATA_H 1

#include <QtGui> 

#include "qrtailab.h"

class QRL_LogData
{
public:
  // enum Param_Class {rt_SCALAR,rt_VECTOR,rt_MATRIX_ROW_MAJOR,rt_MATRIX_COL_MAJOR,rt_MATRIX_COL_MAJOR_ND};
	QRL_LogData();
        ~QRL_LogData();
          void initializeDataVectors();
	const char* getName(){return name;}
	float getDt(){return dt;} 
	int getNRow(){return nrow;}
	int getNCol(){return ncol;}
	void setDt(float d){dt=d;} 
	void setNRow(int r){nrow=r;}
	void setNCol(int c){ncol=c;}
    int get_points_counter(){return Saved_Points;}
    int getIsSaving(){return isSaving;}
    int start_saving();
    void startSaving(FILE* Save_File_Pointer,double Save_Time);
     FILE* save_file();
     void stop_saving();
      int n_points_to_save();
      void set_points_counter(int cnt);
	char name[MAX_NAMES_SIZE];
    int setLogRefreshRate(double rr);
    double getLogRefreshRate();
    void setPlotting(bool b){plotting=b;}
    bool isPlotting(){return plotting;}

        void setLogValue(float v, int row, int col);
        QVector< QVector<float> > getLogValue();
private:
	int nrow;
	int ncol;
        double logRefreshRate;
        QVector< QVector <float> >  LogValues;
	float dt;
	int isSaving;
   	FILE* Save_File_Pointer;
   	double Save_Time;
	int Saved_Points;
    double scopeRefreshRate;
   bool plotting;
    int visible;
};



#endif

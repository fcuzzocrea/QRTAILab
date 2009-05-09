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

#ifndef _SCOPE_DATA_H
#define _SCOPE_DATA_H 1

#include <QtGui> 

#include "qrtailab.h"
// #include "scope_trace.h"

class QRL_ScopeData //: public QThread
{
 // Q_OBJECT
public:
  QRL_ScopeData(int ntraces, float dt,char*);
  ~QRL_ScopeData();
  int getNTraces(){return ntraces;}
//  void setNTraces(int t){ntraces=t;}
  float getDt(){return dt;} 
//  void setDt(float d){dt=d;}
  int getIsSaving(){return isSaving;}
  int getSavedPoints(){return Saved_Points;}
  const char* getName(){return name.c_str();}
  // enum Param_Class {rt_SCALAR,rt_VECTOR,rt_MATRIX_ROW_MAJOR,rt_MATRIX_COL_MAJOR,rt_MATRIX_COL_MAJOR_ND};
  int setScopeDt(double);
    double getScopeDt();
    int setScopeRefreshRate(double rr);
    double getScopeRefreshRate();
    void setScopeValue(float v, int t);	 
    void setScopeTime(float v);	 
     QVector<float> getScopeTime();
    QVector<float> getScopeValue(int t);	 
    QVector< QVector<float> > getScopeValue();

    int start_saving_scope();
    void startSaving(FILE* Save_File_Pointer,double Save_Time);
     FILE* save_file();
     void stop_saving();
      int n_points_to_save();
    void set_points_counter_scope(int cnt);

    //char name[MAX_NAMES_SIZE];

    void setSaveScopeTime(bool b){saveScopeTime=b;}
    bool isSaveScopeTime(){return saveScopeTime;}
    
    void setPlotting(bool b){plotting=b;}
    bool isPlotting(){return plotting;}
// 	int ntraces;
// protected:
//    void run();
private:
    bool saveScopeTime;
    const int ntraces;
    const  float dt;
    std::string name;
    int isSaving;
    bool plotting;
    int visible;
    FILE* Save_File_Pointer;
    double Save_Time;
    int Saved_Points;
    double scopeRefreshRate;
//     QRL_ScopeTrace **Traces;
    QVector <int>  ScopeIndex;
    QVector< QVector <float> >  ScopeValues;
    QVector <float> ScopeTime;
    QVector <int>  ScopeIndexOut;
    QVector< QVector <float> >  ScopeValuesOut;
    double scopeDt;
};



#endif
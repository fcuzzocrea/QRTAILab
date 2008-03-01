/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
 *                   Lorenzo Dozio (dozio@aero.polimi.it)                  *
 *		    Paolo Mantegazza (mantegazza@aero.polimi.it)           *
 *		    Roberto Bucher (roberto.bucher@supsi.ch)               *
 *		    Peter Brier (pbrier@dds.nl)                            *
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
 file:		qrtailab.h
 describtion:
   file for the classes TargetThread
*/



#ifndef QRTAILAB_CORE_H
#define QRTAILAB_CORE_H

#include <QtGui> 
#include "qrtailab.h"


static RT_TASK *Target_Interface_Task;


class TargetThread;
class GetScopeDataThread;
class GetMeterDataThread;
class GetLedDataThread;
//class QRL_ScopeWindow;
//class QRL_MeterWindow;
//class QRL_LedWindow;

using namespace qrl_types;


//! controls connection to one target
class TargetThread : public QThread
 {
 Q_OBJECT
 Q_ENUMS( Manager_Type )
 Q_ENUMS( Commands )
 Q_ENUMS( Param_Class )
 public:
    enum Manager_Type {PARAMS_MANAGER,SCOPES_MANAGER,LOGS_MANAGER,ALOGS_MANAGER,LEDS_MANAGER,METERS_MANAGER,SYNCHS_MANAGER};
 enum Commands{	CONNECT_TO_TARGET,	CONNECT_TO_TARGET_WITH_PROFILE,	DISCONNECT_FROM_TARGET,	START_TARGET,	STOP_TARGET,	UPDATE_PARAM,	GET_TARGET_TIME,	BATCH_DOWNLOAD,	GET_PARAMS,	CLOSE} ;
   enum Param_Class {rt_SCALAR,rt_VECTOR,rt_MATRIX_ROW_MAJOR,rt_MATRIX_COL_MAJOR,rt_MATRIX_COL_MAJOR_ND};
    ~TargetThread();
    void run();
    void setPreferences(Preferences_T);
    // communicate with rtai target
    int stopTarget();
    int startTarget();
    int connectToTarget();
    int disconnectFromTarget();
    void closeThread();

    //get information about target
    unsigned int getIsTargetConnected(){return Is_Target_Connected;}
    int getIsTargetRunning(){return Is_Target_Running;}
    int getScopeNumber(){return Num_Scopes;}
    int getMeterNumber(){return Num_Meters;}
    int getLedNumber(){return Num_Leds;}
    int getParameterNumber(){return Num_Tunable_Parameters;}
    int getBlockNumber(){return Num_Tunable_Blocks;}
      long getTargetNode(){return Target_Node;}
    int getEndApp(){return End_App;}
    int getVerbose(){return Verbose;}
    // should  be removed
    Target_Scopes_T* getScopes(){return Scopes;}
    Target_Meters_T* getMeters(){return Meters;}
    Target_Leds_T* getLeds(){return Leds;}
    //Target_Parameters_T* getParamters(){return Tunable_Parameters;}
    //Target_Blocks_T* getBlocks(){return Tunable_Blocks;}
   // Batch_Parameters_T* getBatchParameters(){return Batch_Parameters;}

   //prameter down- and upload
 void downloadParameter(int,int);
 int addToBatch(int map_offset,int ind,double value);
    double get_parameter(Target_Parameters_T p, int nr, int nc, int *val_idx);
    double get_parameter(int blk,int prm, int nr,int nc);
    int update_parameter(int idx, int mat_idx, double val);
    int get_map_offset(int blk, int prm_row,int prm_col);
    int get_parameter_ind(int blk, int prm_row,int prm_col);
    void batchParameterDownload();
    void resetBatchMode();
    void uploadParameters();
    int get_Number_of_Parameters(int blk);
   unsigned int getParameterCols(int blk,int prm);
   unsigned int getParameterRows(int blk,int prm);
   QString getParameterName(int blk,int prm);
   QString getBlockName(int blk);


    void setVerbose(int v){Verbose=v;}
    Preferences_T getPreferences(){return Preferences;}


    void startScopeThreads(); //QRL_ScopeWindow** ScopeWindows);
    void stopScopeThreads();
    int setScopeDt(double,int);
    void setScopeValue(float v,int t, int n);	 
    QList<float> getScopeValue(int t, int n);	 
    QVector< QList<float> > getScopeValue(int n);
    QString getScopeName(int);
/*
    int start_saving(int n);
     FILE* save_file(int n);
      void setSaveTime(int n);
     void stop_saving(int n);
      int n_points_to_save(int n);
      void startSaving(QString, int n);
*/
    void startMeterThreads();//QRL_MeterWindow** MeterWindows);
    void stopMeterThreads();
    int setMeterRefreshRate(double rr,int n);
    void setMeterValue(float v, int n);
    float getMeterValue(int n);
    QString getMeterName(int);

    void startLedThreads();//QRL_LedWindow** LedWindows);
    void stopLedThreads();
    void setLedValue(unsigned int v, int n);
    unsigned int getLedValue(int n);
    QString getLedName(int);
// friend
 signals:
   void statusBarMessage(const QString &);
   
 public slots:
    void start();
    void getOrder(int);
 private:

    void setEndApp(int EndApp){End_App=EndApp;}
    void setLedValue(int,unsigned int);
   void rlg_update_after_connect(void);
   void rlg_manager_window(int, int, int, int, int, int, int);
   int  get_synch_blocks_info(long port, RT_TASK *task, const char *mbx_id);
   int get_meter_blocks_info(long port, RT_TASK *task, const char *mbx_id);
   int get_led_blocks_info(long port, RT_TASK *task, const char *mbx_id);
   int  get_alog_blocks_info(long port, RT_TASK *task, const char *mbx_id);
   int  get_log_blocks_info(long port, RT_TASK *task, const char *mbx_id);
   int  get_scope_blocks_info(long port, RT_TASK *task, const char *mbx_id);
   int  get_parameters_info(long port, RT_TASK *task);
   long try_to_connect(const char *IP);
   void upload_parameters_info(long port, RT_TASK *task);
  int Num_Tunable_Parameters;
  int Num_Tunable_Blocks;
  int Num_Scopes;
  int Num_Logs;
  int Num_ALogs;
  int Num_Leds;
  int Num_Meters;
  int Num_Synchs;
  Target_Parameters_T *Tunable_Parameters;
  Target_Blocks_T *Tunable_Blocks;
  Target_Scopes_T *Scopes;
  Target_Logs_T *Logs;
  Target_ALogs_T *ALogs;
  Target_Leds_T *Leds;
  Target_Meters_T *Meters;
  Target_Synchs_T *Synchs;
  Batch_Parameters_T Batch_Parameters[MAX_BATCH_PARAMS];
  Profile_T *Profile;
  const char *RLG_Target_Name;
  Preferences_T Preferences;
   int Verbose;
   int Is_Target_Connected;
   unsigned int Is_Target_Running;
   int End_App;
   long Target_Node;
   int test;
   int batchCounter;
  GetScopeDataThread* Get_Scope_Data_Thread;
  GetMeterDataThread* Get_Meter_Data_Thread;
  GetLedDataThread* Get_Led_Data_Thread;
 
  QVector<unsigned int> LedValues; 
  QVector< QList <float> > MeterValues;
  QVector< QVector< QList <float> > > ScopeValues;

  QMutex scopeMutex;

 };






#endif



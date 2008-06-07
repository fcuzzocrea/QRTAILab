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
static RT_TASK *RLG_Main_Task;
using namespace qrl_types;

/**
* @brief controls connection to one target
* @bug   scopethread, meterthread, ledthread are in conflict to QRtaiLabCore -> not threadsafe ->segfault
* @todo  prevent multiple access to variables at the same time. make it threadsafe

*/
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
     TargetThread();
    ~TargetThread();
    void run();
    void setPreferences(Preferences_T);

    //get information about target //should be removed not threadsafe -> segfault?
    unsigned int getIsTargetConnected(){return Is_Target_Connected;}
    int getIsTargetRunning(){return Is_Target_Running;}
    int getScopeNumber(){return Num_Scopes;}
    int getMeterNumber(){return Num_Meters;}
    int getLedNumber(){return Num_Leds;}
    int getLogNumber(){return Num_Logs;}
    int getParameterNumber(){return Num_Tunable_Parameters;}
    int getBlockNumber(){return Num_Tunable_Blocks;}
      long getTargetNode(){return Target_Node;}
    int getEndApp(){return End_App;}
    int getVerbose(){return Verbose;}
    // should  be removed
    Target_Scopes_T* getScopes(){return Scopes;}
    Target_Meters_T* getMeters(){return Meters;}
    Target_Leds_T* getLeds(){return Leds;}
   Target_ALogs_T* getALogs(){return ALogs;}
    Target_Logs_T* getLogs(){return Logs;}
    Target_Parameters_T* getParameters(){return Tunable_Parameters;}
    Target_Blocks_T* getBlocks(){return Tunable_Blocks;}
   // Batch_Parameters_T* getBatchParameters(){return Batch_Parameters;}

   //prameter down- and upload

 int addToBatch(int map_offset,int ind,double value);
    double get_parameter(Target_Parameters_T p, int nr, int nc, int *val_idx);

    int update_parameter(int idx, int mat_idx, double val);
    int get_map_offset(int blk, int prm);
    int get_parameter_ind(int blk, int prm, int prm_row,int prm_col);

    void resetBatchMode();

   int getBatchCounter(){return batchCounter;}

    void setVerbose(int v){Verbose=v;}
    Preferences_T getPreferences(){return Preferences;}


    void startScopeThreads(); //QRL_ScopeWindow** ScopeWindows);
    void stopScopeThreads();
    int setScopeDt(double,int);
    double getScopeDt(int);
    int setScopeRefreshRate(double rr,int n);
    double getScopeRefreshRate(int n);
    void setScopeValue(float v,int nn, int t);	 
    QVector<float> getScopeValue(int t, int n);	 
    QVector< QVector<float> > getScopeValue(int n);
    QString getScopeName(int);

    int start_saving_scope(int index);
    void startSaving(int index,FILE* Save_File_Pointer,double Save_Time);
     FILE* save_file(int index);
     void stop_saving(int index);
      int n_points_to_save(int index);
   void startMeterThreads();//QRL_MeterWindow** MeterWindows);
    void stopMeterThreads();
    int setMeterRefreshRate(double rr,int n);
    double getMeterRefreshRate(int n);
    void setMeterValue(float v, int n);
    float getMeterValue(int n);
    QString getMeterName(int);

    void startLedThreads();//QRL_LedWindow** LedWindows);
    void stopLedThreads();
    void setLedValue(unsigned int v, int n);
    unsigned int getLedValue(int n);
    QString getLedName(int);

    void startALogThreads();
    void stopALogThreads();
    void startLogThreads();
    void stopLogThreads();

    int start_saving_log(int index);
    void startSavingLog(int index,FILE* Save_File_Pointer,double Save_Time);
     FILE* save_file_log(int index);
     void stop_saving_log(int index);
      int n_points_to_save_log(int index);


    const char* getTargetName(){return RLG_Target_Name;}
    long getTargetPort(){return Target_Port;}
    RT_TASK * getTask(){return Target_Interface_Task;}

// friend
   
 public slots:
    void start();
 private:

    void setEndApp(int EndApp){End_App=EndApp;}
    void setLedValue(int,unsigned int);
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
  long Target_Port;
  const char *RLG_Target_Name;
  Preferences_T Preferences;
   int Verbose;
   int Is_Target_Connected;
   unsigned int Is_Target_Running;
   int End_App;
   long Target_Node;
   int test;
   int batchCounter;
  //GetScopeDataThread* Get_Scope_Data_Thread;
  //GetMeterDataThread* Get_Meter_Data_Thread;
  //GetLedDataThread* Get_Led_Data_Thread;
 
  QVector<unsigned int> LedValues; 
  QVector< float > MeterValues;
  QVector< QVector< QVector <float> > > ScopeValues;
  QVector<double> scopeDt;
  QVector<double> meterRefreshRate;
  QVector<double> scopeRefreshRate;
  QVector< QVector <int> > ScopeIndex;

 pthread_t *Get_Led_Data_Thread;
 pthread_t *Get_Meter_Data_Thread;
 pthread_t *Get_Scope_Data_Thread;
 pthread_t *Get_ALog_Data_Thread;
 pthread_t *Get_Log_Data_Thread;



 };






class QRtaiLabCore :  public QObject {
	
	Q_OBJECT

public:
    QRtaiLabCore(QObject *parent=0,int Verbose = 0);
    ~QRtaiLabCore();
    TargetThread* getTargetThread(){return targetthread;}
    int stopTarget();
    int startTarget();
    int connectToTarget();
    int disconnectFromTarget();
    void uploadParameters();
    void batchParameterDownload();
    void closeTargetThread();

    void setPreferences(Preferences_T p){targetthread->setPreferences(p);}
    Preferences_T getPreferences(){return targetthread->getPreferences();}

    unsigned int getIsTargetConnected(){return targetthread->getIsTargetConnected();}
    int getIsTargetRunning(){return targetthread->getIsTargetRunning();}
    int getScopeNumber();
    int getMeterNumber();
    int getLedNumber();
    int getLogNumber();
    int getParameterNumber(){return targetthread->getParameterNumber();}
    int getBlockNumber(){return targetthread->getBlockNumber();}
    int getEndApp(){return targetthread->getEndApp();}
    int getVerbose(){return targetthread->getVerbose();}

   QString getParameterName(int blk,int prm);
   QString getBlockName(int blk);
   void resetBatchMode(){targetthread->resetBatchMode();}
    int getNumberOfParameters(int blk);
   unsigned int getParameterCols(int blk,int prm);
   unsigned int getParameterRows(int blk,int prm);
    double getParameterValue(int blk,int prm, int nr,int nc);
    void updateParameterValue(int blk,int prm, int nr,int nc,double value);
    void addToBatch(int blk,int prm, int nr,int nc, double value);

   QString getScopeName(int);
    int	    getScopeDt(int);
    int     getNumberOfTraces(int);

    QString getMeterName(int);
    int	    getMeterDt(int);


    QString getLedName(int);
    int	    getLedDt(int);
    int     getNumberOfLeds(int);

    QString getLogName(int);

public slots:
    //void refreshData();
 signals:
   void statusBarMessage(const QString &);
	
private:
	TargetThread* targetthread;
	Preferences_T Preferences;
	  QTimer *timer;
};



#endif



/*
 file:		qrtailab.h
 describtion:
   file for the classes TargetThread

 Copyright (C) 2007 Holger Nahrstaedt (nahrstaedt@control.tu-berlin.de)
                    Lorenzo Dozio (dozio@aero.polimi.it)
		    Paolo Mantegazza (mantegazza@aero.polimi.it)
		    Roberto Bucher (roberto.bucher@supsi.ch)
		    Peter Brier (pbrier@dds.nl)


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


#ifndef QRTAILAB_H
#define QRTAILAB_H

#include <QtGui> 

#include <stdio.h>
#include <pthread.h>
#include <sys/poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/io.h>
#include <sys/poll.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <rtai_netrpc.h>
#include <rtai_msg.h>
#include <rtai_mbx.h>


#define MAX_NHOSTS              100
#define MAX_NAMES_SIZE          256
#define MAX_DATA_SIZE           30
#define MAX_RTAI_SCOPES         20
#define MAX_RTAI_LOGS           20
#define MAX_RTAI_METERS         20
#define MAX_RTAI_LEDS           20
#define MAX_RTAI_SYNCHS         20
#define MAX_BATCH_PARAMS        1000
#define MAX_TRACES_PER_SCOPE    10

#define FLTK_EVENTS_TICK        0.05
#define QT_EVENTS_TICK          50

#define MAX_MSG_LEN             (MAX_MSG_SIZE - 100)
#define REFRESH_RATE            0.05
#define msleep(t)       do { poll(0, 0, t); } while (0)



namespace qrl {

static volatile int GlobalRet[16];

static inline void RT_RPC(RT_TASK *task, unsigned int msg, unsigned int *reply)
{
	QWaitCondition sleep;
	GlobalRet[msg & 0xf] = 0;
	rt_send(task, msg);
	while (!GlobalRet[msg & 0xf]) {
		usleep(1000*QT_EVENTS_TICK); //linux
	}
}

static inline void RT_RETURN(RT_TASK *task, unsigned int reply)
{
	GlobalRet[reply] = 1;
}
 unsigned long get_an_id(const char *root);
}

namespace qrl_types {

typedef struct Target_Parameters_Struct Target_Parameters_T;
typedef struct Target_Blocks_Struct Target_Blocks_T;
typedef struct Target_Scopes_Struct Target_Scopes_T;
typedef struct Target_Logs_Struct Target_Logs_T;
typedef struct Target_ALogs_Struct Target_ALogs_T;
typedef struct Target_Leds_Struct Target_Leds_T;
typedef struct Target_Meters_Struct Target_Meters_T;
typedef struct Target_Synchs_Struct Target_Synchs_T;
typedef struct Batch_Parameters_Struct Batch_Parameters_T;

typedef struct Preferences_Struct Preferences_T;
typedef struct Profile_Struct Profile_T;
//typedef struct W_Geometry_Struct W_Geometry_T;
//typedef struct RGB_Color_Struct RGB_Color_T;

typedef struct Args_Struct Args_T;
typedef struct Args_Struct_ALog Alog_T;             //aggiunta 5/5 per nomi alog

typedef struct s_idx_T {
	int scope_idx;
	int trace_idx;
};

typedef struct p_idx_T {
	int block_idx;
	int param_idx;
	int val_idx;
};


struct Args_Struct
{
	const char *mbx_id;
	int index;
	int x, y, w, h;
};
struct Args_Struct_ALog
{
	const char *mbx_id;
	int index;
	int x, y, w, h;
	const char *alog_name;
};
struct Target_Parameters_Struct
{
	char model_name[MAX_NAMES_SIZE];
	char block_name[MAX_NAMES_SIZE];
	char param_name[MAX_NAMES_SIZE];
	unsigned int n_rows;
	unsigned int n_cols;
	unsigned int data_type;
	unsigned int data_class;
	double data_value[MAX_DATA_SIZE];
};

struct Target_Blocks_Struct
{
	char name[MAX_NAMES_SIZE];
	int offset;
};

struct Target_Scopes_Struct
{
	char name[MAX_NAMES_SIZE];
	int ntraces;
	int visible;
	float dt;
};

struct Target_Logs_Struct
{
	char name[MAX_NAMES_SIZE];
	int nrow;
	int ncol;
	float dt;
};

struct Target_ALogs_Struct
{
	char name[MAX_NAMES_SIZE];
	int nrow;
	int ncol;
	float dt;
};

struct Target_Leds_Struct
{
	char name[MAX_NAMES_SIZE];
	int n_leds;
	int visible;
	float dt;
};

struct Target_Meters_Struct
{
	char name[MAX_NAMES_SIZE];
	int visible;
	float dt;
};

struct Target_Synchs_Struct
{
	char name[MAX_NAMES_SIZE];
	int visible;
	float dt;
};

struct Batch_Parameters_Struct
{
	int index;
	int mat_index;
	double value;
};

struct Preferences_Struct
{
	char *Target_IP;
	char *Target_Interface_Task_Name;
	char *Target_Scope_Mbx_ID;
	char *Target_Log_Mbx_ID;
	char *Target_ALog_Mbx_ID;
	char *Target_Led_Mbx_ID;
	char *Target_Meter_Mbx_ID;
	char *Target_Synch_Mbx_ID;
	int MW_w, MW_h, MW_x, MW_y;
	char *Current_Profile;
	int Num_Profiles;
	//Fl_String_List Profiles;
};
/*
struct W_Geometry_Struct
{
	int x, y, w, h;
	int visible;
};*/
/*
struct RGB_Color_Struct
{
	float r, g, b;
};*/
/*
struct Profile_Struct
{
	char *Target_IP;
	char *Target_Interface_Task_Name;
	char *Target_Scope_Mbx_ID;
	char *Target_Log_Mbx_ID;
	char *Target_ALog_Mbx_ID;
	char *Target_Led_Mbx_ID;
	char *Target_Meter_Mbx_ID;
	char *Target_Synch_Mbx_ID;
	char *Target_Name;

	int n_params, n_blocks;
	int n_scopes, n_logs, n_alogs;           //aggiunto 4/5
	int n_meters, n_leds;
	int n_synchs;

	W_Geometry_T P_Mgr_W;
	int P_Mgr_Batch;

	W_Geometry_T S_Mgr_W;			// scopes manager geometry 
	int S_Mgr_Show[MAX_RTAI_SCOPES];	// scopes manager show-hide buttons 
	int S_Mgr_Grid[MAX_RTAI_SCOPES];	// scopes manager grid on-off buttons 
	int S_Mgr_PT[MAX_RTAI_SCOPES];		// scopes manager points-time buttons 
	W_Geometry_T S_W[MAX_RTAI_SCOPES];	// scope geometry 
	RGB_Color_T S_Bg_C[MAX_RTAI_SCOPES];	// scope background color 
	RGB_Color_T S_Grid_C[MAX_RTAI_SCOPES];	// scope grid color 
	float S_Mgr_SecDiv[MAX_RTAI_SCOPES];	// scope seconds per division value 
	int S_Mgr_PSave[MAX_RTAI_SCOPES];	// scope n points to save 
	float S_Mgr_TSave[MAX_RTAI_SCOPES];	// scope n seconds to save 
	char *S_Mgr_File[MAX_RTAI_SCOPES];	// scopes manager save file name 
	int S_Mgr_Flags[MAX_RTAI_SCOPES];	// scopes Flags  
	int S_Mgr_Trigger[MAX_RTAI_SCOPES];	// scopes Trigger mode  
	int S_Mgr_T_Show[MAX_TRACES_PER_SCOPE][MAX_RTAI_SCOPES];	// scopes manager trace show-hide buttons 
	float S_Mgr_T_UnitDiv[MAX_TRACES_PER_SCOPE][MAX_RTAI_SCOPES];	// scope trace units per division value 
	RGB_Color_T S_Trace_C[MAX_TRACES_PER_SCOPE][MAX_RTAI_SCOPES];	// scope trace color 
	float S_Mgr_T_Width[MAX_TRACES_PER_SCOPE][MAX_RTAI_SCOPES];	// scope trace width 
	float S_Mgr_T_Offset[MAX_TRACES_PER_SCOPE][MAX_RTAI_SCOPES];	// scope trace offset 
	int S_Mgr_T_Options[MAX_TRACES_PER_SCOPE][MAX_RTAI_SCOPES];	// scope trace options 

	int n_traces[MAX_RTAI_SCOPES];

	W_Geometry_T Log_Mgr_W;			// logs manager geometry 
	int Log_Mgr_PT[MAX_RTAI_LOGS];		// logs manager points-time buttons 
	int Log_Mgr_PSave[MAX_RTAI_LOGS];	// logs manager n points to save 
	float Log_Mgr_TSave[MAX_RTAI_LOGS];	// logs manager n seconds to save 
	char *Log_Mgr_File[MAX_RTAI_LOGS];	// logs manager save file name 
	
	W_Geometry_T ALog_Mgr_W;		// logs manager geometry 			//added 4/5
	char *ALog_Mgr_File[MAX_RTAI_LOGS];	// logs manager save file name 

	W_Geometry_T Led_Mgr_W;			// leds manager geometry 
	int Led_Mgr_Show[MAX_RTAI_LEDS];	// leds manager show-hide buttons 
	//Fl_String Led_Mgr_Color[MAX_RTAI_LEDS];	// led color 
	W_Geometry_T Led_W[MAX_RTAI_LEDS];	// led geometry 

	W_Geometry_T M_Mgr_W;			// meters manager geometry 
	int M_Mgr_Show[MAX_RTAI_METERS];	// meters manager show-hide buttons 
	float M_Mgr_Maxv[MAX_RTAI_METERS];	// meters manager max values 
	float M_Mgr_Minv[MAX_RTAI_METERS];	// meters manager min values 
	RGB_Color_T M_Bg_C[MAX_RTAI_METERS];	// meter background color 
	RGB_Color_T M_Arrow_C[MAX_RTAI_METERS];	// meter arrow color 
	RGB_Color_T M_Grid_C[MAX_RTAI_METERS];	// meter grid color 
	W_Geometry_T M_W[MAX_RTAI_METERS];	// meter geometry 

	W_Geometry_T Sy_Mgr_W;			// synchs manager geometry 
	int Sy_Mgr_Show[MAX_RTAI_SYNCHS];	// synchs manager show-hide buttons 
	W_Geometry_T Sy_W[MAX_RTAI_SYNCHS];	// synch geometry 

	char *Text;
};*/
/*
typedef enum {
	R_COLOR,
	G_COLOR,
	B_COLOR
} RGB_Colors;
*/


/*

typedef enum {
	GEOMETRY_PREF,
	CONNECT_PREF,
	PROFILES_PREF,
	PROFILE_PREF
} Pref_Type;
*/

}

class TargetThread;
using namespace qrl_types;


//! controls connection to one target
class TargetThread : public QThread
 {
 Q_OBJECT
 Q_ENUMS( Manager_Type )
 Q_ENUMS( Commands )
 public:
    enum Manager_Type {PARAMS_MANAGER,SCOPES_MANAGER,LOGS_MANAGER,ALOGS_MANAGER,LEDS_MANAGER,METERS_MANAGER,SYNCHS_MANAGER};
 enum Commands{	CONNECT_TO_TARGET,	CONNECT_TO_TARGET_WITH_PROFILE,	DISCONNECT_FROM_TARGET,	START_TARGET,	STOP_TARGET,	UPDATE_PARAM,	GET_TARGET_TIME,	BATCH_DOWNLOAD,	GET_PARAMS,	CLOSE} ;
    ~TargetThread();
    void run();
    void setPreferences(Preferences_T);
    unsigned int getIsTargetConnected(){return Is_Target_Connected;}
    int getIsTargetRunning(){return Is_Target_Running;}
    int getScopeNumber(){return Num_Scopes;}
    int getMeterNumber(){return Num_Meters;}
    int getLedNumber(){return Num_Leds;}
    int getParameterNumber(){return Num_Tunable_Parameters;}
    int getBlockNumber(){return Num_Tunable_Blocks;}
    int stopTarget();
    int startTarget();
    int connectToTarget();
    int disconnectFromTarget();
    void downloadParameter(int,int);
    int addToBatch(int map_offset,int ind,double value);
    void closeThread();
    Target_Scopes_T* getScopes(){return Scopes;}
    Target_Meters_T* getMeters(){return Meters;}
    Target_Leds_T* getLeds(){return Leds;}
    Target_Parameters_T* getParamters(){return Tunable_Parameters;}
    Target_Blocks_T* getBlocks(){return Tunable_Blocks;}
    Batch_Parameters_T* getBatchParameters(){return Batch_Parameters;}
    double get_parameter(Target_Parameters_T p, int nr, int nc, int *val_idx);
  long getTargetNode(){return Target_Node;}
    int getEndApp(){return End_App;}
    int getVerbose(){return Verbose;}
    void setVerbose(int v){Verbose=v;}
    Preferences_T getPreferences(){return Preferences;}
    void batchParameterDownload();
    void resetBatchMode();
    void uploadParameters();
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
 };


 
#endif



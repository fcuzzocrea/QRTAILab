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
 file:		qrtailab.cpp
 describtion:
   file for the classes TargetThread
*/

#include "target_thread.h"
#include "qrtailab_listener.h"
//#include "scope_window.h"
//#include "meter_window.h"
//#include "led_window.h"

using namespace qrl_types;
//static pthread_t *Get_Scope_Data_Thread;



//Target_Synch_T Synchronoscope;
TargetThread::TargetThread(){

	Num_Tunable_Blocks=0;
	Num_Scopes=0;
	Num_Logs=0;
	Num_ALogs=0;
	Num_Leds=0;
	Num_Meters=0;
	hardRealTimeScope=1;
	hardRealTimeLog=1;
	hardRealTimeALog=1;

}


TargetThread::~TargetThread(){


//  	if (Tunable_Blocks)
//  		delete[] Tunable_Blocks;
//  	if (Scopes)
//  		delete[] Scopes;
	if (Num_Scopes>0){
	    for (int i=0; i<Num_Scopes;i++)
	      delete Scopes[i];
	    delete[] Scopes;
	}
 	if (Num_Logs>0){
	    for (int i=0; i<Num_Logs;i++)
	      delete Logs[i];
	    delete[] Logs;
	}
 	if (Num_ALogs>0){
	    for (int i=0; i<Num_ALogs;i++)
	      delete ALogs[i];
	    delete[] ALogs;
	}
 	if (Num_Leds>0){
	    for (int i=0; i<Num_Leds;i++)
	      delete Leds[i];
	    delete[] Leds;
	}
 	if (Num_Meters>0){
	    for (int i=0; i<Num_Meters;i++)
	      delete Meters[i];
	    delete[] Meters;
	}
/*	stopScopeThreads();
	//if (Get_Scope_Data_Thread)
	//	delete[] Get_Scope_Data_Thread;
	stopLedThreads();
	//if (Get_Led_Data_Thread)
		//delete[] Get_Led_Data_Thread;
	stopMeterThreads();
	//if (Get_Meter_Data_Thread)
	//	delete[] Get_Meter_Data_Thread;
	stopALogThreads();
	stopLogThreads();*/
}
/**
 * @brief Gets the Pameter value for a given entry. 
 * @param p Parameter Struct
 * @param nr parameter number
 * @param nr row
 * @param nc column
 * @param *val_idx returns the array index
 */
double TargetThread::get_parameter(Target_Parameters_T p, int nr, int nc, int *val_idx)
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



 int TargetThread::get_map_offset(int blk, int prm){

	int map_offset = Tunable_Blocks[blk].offset + prm;
	return map_offset;

}
 int TargetThread::get_parameter_ind(int blk, int prm, int prm_row,int prm_col){

	int val_idx;
	get_parameter(Tunable_Parameters[Tunable_Blocks[blk].offset+prm], prm_row, prm_col, &val_idx);
	return val_idx;

}

/**
 * @brief Change one Parameter entry in the Parameter structure
 * @param idx map_offset = Tunable_Blocks[blk].offset + prm;
 * @param mat_idx  array index(val_idx)
 * @param val new value
 */
int TargetThread::update_parameter(int idx, int mat_idx, double val)
{
	Tunable_Parameters[idx].data_value[mat_idx] = val;
	return 1;
}



long TargetThread::try_to_connect(const char *IP)
{
	int counter = 0;
	int counter_max = 5;
	char buf[100];
	long port;
	struct sockaddr_in addr;

	sprintf(buf, "Trying to connect to %s", IP);
	//statusBarMessage(tr(buf));
	if (Verbose) {
		printf("%s...", buf);
		fflush(stdout);
	}
	inet_aton(IP, &addr.sin_addr);
	Target_Node = addr.sin_addr.s_addr;
	while ((port = rt_request_port(Target_Node)) <= 0 && counter++ <= counter_max) {
		msleep(100);
	}
	return port;
}


int TargetThread::get_parameters_info(long port, RT_TASK *task)
{
	unsigned int req = 'c';
	char c_req = 'i';
	int blk_index = 0;
	int n_params = 0;

	RT_rpc(Target_Node, port, task, req, &Is_Target_Running);
	n_params = Is_Target_Running & 0xffff;
	Is_Target_Running >>= 16;
	if (n_params > 0) Tunable_Parameters = new Target_Parameters_T [n_params];
	else               Tunable_Parameters = new Target_Parameters_T [1];

	RT_rpcx(Target_Node, port, task, &c_req, &Tunable_Parameters[0], sizeof(char), sizeof(Target_Parameters_T));
        RLG_Target_Name = strdup(Tunable_Parameters[0].model_name);

	for (int n = 0; n < n_params; n++) {
		RT_rpcx(Target_Node, port, task, &c_req, &Tunable_Parameters[n], sizeof(char), sizeof(Target_Parameters_T));
		if (n > 0) {
			if (strcmp(Tunable_Parameters[n-1].block_name, Tunable_Parameters[n].block_name)) {
				Num_Tunable_Blocks++;
			}
		} else {
			Num_Tunable_Blocks = 1;
		}
	}
	if (Num_Tunable_Blocks > 0) Tunable_Blocks = new Target_Blocks_T [Num_Tunable_Blocks];
	blk_index = 0;
	for (int n = 0; n < n_params; n++) {
		if (n > 0) {
			if (strcmp(Tunable_Parameters[n-1].block_name, Tunable_Parameters[n].block_name)) {
				blk_index++;
				strncpy(Tunable_Blocks[blk_index].name, Tunable_Parameters[n].block_name + strlen(Tunable_Parameters[0].model_name) + 1, MAX_NAMES_SIZE);
				Tunable_Blocks[blk_index].offset = n;
			}
		} else {
			strncpy(Tunable_Blocks[0].name, Tunable_Parameters[0].block_name + strlen(Tunable_Parameters[0].model_name) + 1, MAX_NAMES_SIZE);
			Tunable_Blocks[0].offset = 0;
		}
	}

	return n_params;

}

void TargetThread::upload_parameters_info(long port, RT_TASK *task)
{
	unsigned int req = 'g';
	char c_req = 'i';
	int n;

	RT_rpc(Target_Node, port, task, req, &Is_Target_Running);
	if (Verbose) {
		printf("Upload parameters...\n");
	}
	for (n = 0; n < Num_Tunable_Parameters; n++) {
		RT_rpcx(Target_Node, port, task, &c_req, &Tunable_Parameters[n], sizeof(char), sizeof(Target_Parameters_T));
	}
}

 int TargetThread::get_scope_blocks_info(long port, RT_TASK *task, const char *mbx_id)
{
	int n_scopes = 0;
	int req = -1, msg;

	for (int n = 0; n < MAX_RTAI_SCOPES; n++) {
		char mbx_name[7];
		sprintf(mbx_name, "%s%d", mbx_id, n);
		if (!RT_get_adr(Target_Node, port, mbx_name)) {
			n_scopes = n;
			break;
		}
	}
	if (n_scopes > 0) {
	  if (Num_Scopes>0){
	      for (int i=0; i<Num_Scopes;i++)
		delete Scopes[i];
	      delete[] Scopes;
	  }
	  Scopes = new QRL_ScopeData* [n_scopes];
	}
	for (int n = 0; n < n_scopes; n++) {
		char scope_name[MAX_NAMES_SIZE];
		int ntraces;
		float dt;
// 		Scopes[n].visible = false;
// 		Scopes[n].isSaving=0;
// 		Scopes[n].Save_File_Pointer=NULL;
		RT_rpcx(Target_Node, port, task, &n, &ntraces, sizeof(int), sizeof(int));
		RT_rpcx(Target_Node, port, task, &n, &scope_name, sizeof(int), sizeof(scope_name));
		RT_rpcx(Target_Node, port, task, &n, &dt, sizeof(int), sizeof(float));
		Scopes[n] = new QRL_ScopeData();
		Scopes[n]->setDt(dt);
		Scopes[n]->setNTraces(ntraces);
		strncpy(Scopes[n]->name, scope_name, MAX_NAMES_SIZE);
	}
	RT_rpcx(Target_Node, port, task, &req, &msg, sizeof(int), sizeof(int));

	return n_scopes;
}

int TargetThread::get_log_blocks_info(long port, RT_TASK *task, const char *mbx_id)
{
	int n_logs = 0;
	int req = -1, msg;

	for (int n = 0; n < MAX_RTAI_LOGS; n++) {
		char mbx_name[7];
		sprintf(mbx_name, "%s%d", mbx_id, n);
		if (!RT_get_adr(Target_Node, port, mbx_name)) {
			n_logs = n;
			break;
		}
	}	
	if (n_logs > 0) {
	  if (Num_Logs>0){
	      for (int i=0; i<Num_Logs;i++)
		delete Logs[i];
	      delete[] Logs;
	  }
	  Logs = new QRL_LogData* [n_logs];
	}
	for (int n = 0; n < n_logs; n++) {
		char log_name[MAX_NAMES_SIZE];
// 		Logs[n].isSaving=0;
// 		Logs[n].Save_File_Pointer=NULL;
		int nrow;
		int ncol;
		float dt;
		Logs[n] = new QRL_LogData();
		RT_rpcx(Target_Node, port, task, &n, &nrow, sizeof(int), sizeof(int));
		RT_rpcx(Target_Node, port, task, &n, &ncol, sizeof(int), sizeof(int));
		RT_rpcx(Target_Node, port, task, &n, &log_name, sizeof(int), sizeof(log_name));
		strncpy(Logs[n]->name, log_name, MAX_NAMES_SIZE);
		RT_rpcx(Target_Node, port, task, &n, &dt, sizeof(int), sizeof(float));
		Logs[n]->setNRow(nrow);
		Logs[n]->setNCol(ncol);
		Logs[n]->setDt(dt);
	}
	RT_rpcx(Target_Node, port, task, &req, &msg, sizeof(int), sizeof(int));

	return n_logs;
}

int TargetThread::get_alog_blocks_info(long port, RT_TASK *task, const char *mbx_id)            //added il 4/5/2005
{
	int n_alogs = 0;
	int req = -1, msg;
	for (int n = 0; n < MAX_RTAI_LOGS; n++) {
		char mbx_name[7];
		sprintf(mbx_name, "%s%d", mbx_id, n);
		if (!RT_get_adr(Target_Node, port, mbx_name)) {
			n_alogs = n;
			break;
		}
	}	
	if (n_alogs > 0) {
	  if (Num_ALogs>0){
	      for (int i=0; i<Num_ALogs;i++)
		delete ALogs[i];
	      delete[] ALogs;
	  }
	 ALogs = new QRL_ALogData* [n_alogs];
	}
	for (int n = 0; n < n_alogs; n++) {
		char alog_name[MAX_NAMES_SIZE];
		int nrow;
		int ncol;
		float dt;
		ALogs[n] = new QRL_ALogData();
		RT_rpcx(Target_Node, port, task, &n, &nrow, sizeof(int), sizeof(int));
		RT_rpcx(Target_Node, port, task, &n, &ncol, sizeof(int), sizeof(int));
		RT_rpcx(Target_Node, port, task, &n, &alog_name, sizeof(int), sizeof(alog_name));
		strncpy(ALogs[n]->name, alog_name, MAX_NAMES_SIZE);
		RT_rpcx(Target_Node, port, task, &n, &dt, sizeof(int), sizeof(float));
		ALogs[n]->setNRow(nrow);
		ALogs[n]->setNCol(ncol);
		ALogs[n]->setDt(dt);
	}
	RT_rpcx(Target_Node, port, task, &req, &msg, sizeof(int), sizeof(int));

	return n_alogs;
}

int TargetThread::get_led_blocks_info(long port, RT_TASK *task, const char *mbx_id)
{
	int n_leds = 0;
	int req = -1, msg;

	for (int n = 0; n < MAX_RTAI_LEDS; n++) {
		char mbx_name[7];
		sprintf(mbx_name, "%s%d", mbx_id, n);
		if (!RT_get_adr(Target_Node, port, mbx_name)) {
			n_leds = n;
			break;
		}
	}
	if (n_leds > 0) {
	  if (Num_Leds>0){
	      for (int i=0; i<Num_Leds;i++)
		delete Leds[i];
	      delete[] Leds;
	  }
	  Leds = new QRL_LedData* [n_leds];
	}
	for (int n = 0; n < n_leds; n++) {
		char led_name[MAX_NAMES_SIZE];
		float dt;
		int n_leds;
		//Leds[n].visible = false;
		Leds[n] = new QRL_LedData();
		RT_rpcx(Target_Node, port, task, &n, &n_leds, sizeof(int), sizeof(int));
		RT_rpcx(Target_Node, port, task, &n, &led_name, sizeof(int), sizeof(led_name));
		strncpy(Leds[n]->name, led_name, MAX_NAMES_SIZE);
		RT_rpcx(Target_Node, port, task, &n, &dt, sizeof(int), sizeof(float));
		Leds[n]->setDt(dt);
		Leds[n]->setNLeds(n_leds);
	}
	RT_rpcx(Target_Node, port, task, &req, &msg, sizeof(int), sizeof(int));

	return n_leds;
}

int TargetThread::get_meter_blocks_info(long port, RT_TASK *task, const char *mbx_id)
{
	int n_meters = 0;
	int req = -1, msg;

	for (int n = 0; n < MAX_RTAI_METERS; n++) {
		char mbx_name[7];
		sprintf(mbx_name, "%s%d", mbx_id, n);
		if (!RT_get_adr(Target_Node, port, mbx_name)) {
			n_meters = n;
			break;
		}
	}
	if (n_meters > 0){ 	  
	     if (Num_Meters>0){
	      for (int i=0; i<Num_Meters;i++)
		delete Meters[i];
	      delete[] Meters;
	  }
	  Meters = new QRL_MeterData* [n_meters];
	}
	for (int n = 0; n < n_meters; n++) {
		char meter_name[MAX_NAMES_SIZE];
		float dt;
// 		Meters[n].visible = false;
		RT_rpcx(Target_Node, port, task, &n, &meter_name, sizeof(int), sizeof(meter_name));
		
		RT_rpcx(Target_Node, port, task, &n, &dt, sizeof(int), sizeof(float));
		Meters[n] = new QRL_MeterData();
		Meters[n]->setDt(dt);
		strncpy(Meters[n]->name, meter_name, MAX_NAMES_SIZE);
	}
	RT_rpcx(Target_Node, port, task, &req, &msg, sizeof(int), sizeof(int));

	return n_meters;
}

int TargetThread::get_synch_blocks_info(long port, RT_TASK *task, const char *mbx_id)
{
	int n_synchs = 0;
	int req = -1, msg;

	for (int n = 0; n < MAX_RTAI_SYNCHS; n++) {
		char mbx_name[7];
		sprintf(mbx_name, "%s%d", mbx_id, n);
		if (!RT_get_adr(Target_Node, port, mbx_name)) {
			n_synchs = n;
			break;
		}
	}
	if (n_synchs > 0) Synchs = new Target_Synchs_T [n_synchs];
	for (int n = 0; n < n_synchs; n++) {
		char synch_name[MAX_NAMES_SIZE];
		Synchs[n].visible = false;
		RT_rpcx(Target_Node, port, task, &n, &synch_name, sizeof(int), sizeof(synch_name));
		strncpy(Synchs[n].name, synch_name, MAX_NAMES_SIZE);
		RT_rpcx(Target_Node, port, task, &n, &Synchs[n].dt, sizeof(int), sizeof(float));
	}
	RT_rpcx(Target_Node, port, task, &req, &msg, sizeof(int), sizeof(int));

	return n_synchs;
}




void TargetThread::start()
{
  test=1;
  Is_Target_Connected=0;
  End_App=0;
  //MetersManager=NULL;
  QThread::start();
}

void TargetThread::run()
{


	unsigned int code, U_Request;
	Target_Port = 0;
	RT_TASK *If_Task = NULL, *task;

	rt_allow_nonroot_hrt();
	//mlockall(MCL_CURRENT | MCL_FUTURE);
	if (!(Target_Interface_Task = rt_task_init_schmod(qrl::get_an_id("HTI"), 97, 0, 0, SCHED_FIFO, 0xFF))) {
		printf("Cannot init Target_Interface_Task\n");
		exit(1);
	}
	//munlockall();
	rt_send(RLG_Main_Task, 0);
	
	while (!End_App) {
	
		if (!(task = rt_receive(0, &code))) {
		  sleep(0);
		  continue;
		}
		if (Verbose) {
			printf("Received code %d from task %p\n", code, task);
		}
		switch (code & 0xf) {

			case CONNECT_TO_TARGET:
end:
				if (Verbose) {
					printf("Reading target settings\n");
				}
				//rlg_read_pref(CONNECT_PREF, "rtailab", 0);
				if (!strcmp(Preferences.Target_IP, "0")) {
					Target_Node = 0;
				} else {
					Target_Port = try_to_connect(Preferences.Target_IP);
					//RLG_Connect_Button->activate();
					if (Target_Port <= 0) {
						//statusBarMessage(tr("Sorry, no route to target"));
						if (Verbose) {
							printf(" Sorry, no route to target\n");
						}
						qrl::RT_RETURN(task, CONNECT_TO_TARGET);
						break;
					}
					if (Verbose) {
						printf(" Ok\n");
					}
				}

			if (!(If_Task = (RT_TASK *)RT_get_adr(Target_Node, Target_Port, Preferences.Target_Interface_Task_Name))) {
					//statusBarMessage(tr("No target or bad interface task identifier"));
					if (Verbose) {
						printf("No target or bad interface task identifier\n");
					}
					qrl::RT_RETURN(task, CONNECT_TO_TARGET);
					break;
				}
				Num_Tunable_Parameters = get_parameters_info(Target_Port, If_Task);
				if (Verbose) {
					printf("Target is running...%s\n", Is_Target_Running ? "yes" : "no");
					printf("Number of target tunable parameters...%d\n", Num_Tunable_Parameters);
					for (int n = 0; n < Num_Tunable_Parameters; n++) {
						printf("Block: %s\n", Tunable_Parameters[n].block_name);
						printf(" Parameter: %s\n", Tunable_Parameters[n].param_name);
						printf(" Number of rows: %d\n", Tunable_Parameters[n].n_rows);
						printf(" Number of cols: %d\n", Tunable_Parameters[n].n_cols);
						for (unsigned int nr = 0; nr < Tunable_Parameters[n].n_rows; nr++) {
							for (unsigned int nc = 0; nc < Tunable_Parameters[n].n_cols; nc++) {
								printf(" Value    : %f\n", Tunable_Parameters[n].data_value[nr*Tunable_Parameters[n].n_cols+nc]);
							}
						}
					}
				}
				Num_Scopes = get_scope_blocks_info(Target_Port, If_Task, Preferences.Target_Scope_Mbx_ID);
				if (Verbose) {
					printf("Number of target real time scopes: %d\n", Num_Scopes);
					for (int n = 0; n < Num_Scopes; n++) {
						printf("Scope: %s\n", Scopes[n]->getName());
						printf(" Number of traces...%d\n", Scopes[n]->getNTraces());
						printf(" Sampling time...%f\n", Scopes[n]->getDt());
					}
				}
				Num_Logs = get_log_blocks_info(Target_Port, If_Task, Preferences.Target_Log_Mbx_ID);
				if (Verbose) {
					printf("Number of target real time logs: %d\n", Num_Logs);
					for (int n = 0; n < Num_Logs; n++) {
						printf("Log: %s\n", Logs[n]->getName());
						printf(" Number of rows...%d\n", Logs[n]->getNRow());
						printf(" Number of cols...%d\n", Logs[n]->getNCol());
						printf(" Sampling time...%f\n", Logs[n]->getDt());
					}
				}
				Num_ALogs = get_alog_blocks_info(Target_Port, If_Task, Preferences.Target_ALog_Mbx_ID);
				if (Verbose) {
					printf("Number of target real time automatic logs: %d\n", Num_ALogs);
					for (int n = 0; n < Num_ALogs; n++) {
						printf("Log: %s\n", ALogs[n]->getName());
						printf(" Number of rows...%d\n", ALogs[n]->getNRow());
						printf(" Number of cols...%d\n", ALogs[n]->getNCol());
						printf(" Sampling time...%f\n", ALogs[n]->getDt());
					}
				}
				Num_Leds = get_led_blocks_info(Target_Port, If_Task, Preferences.Target_Led_Mbx_ID);
				if (Verbose) {
					printf("Number of target real time leds: %d\n", Num_Leds);
					for (int n = 0; n < Num_Leds; n++) {
						printf("Led: %s\n", Leds[n]->getName());
						printf(" Number of leds...%d\n", Leds[n]->getNLeds());
						printf(" Sampling time...%f\n", Leds[n]->getDt());
					}
				}
				Num_Meters = get_meter_blocks_info(Target_Port, If_Task, Preferences.Target_Meter_Mbx_ID);
				if (Verbose) {
					printf("Number of target real time meters: %d\n", Num_Meters);
					for (int n = 0; n < Num_Meters; n++) {
						printf("Meter: %s\n", Meters[n]->getName());
						printf(" Sampling time...%f\n", Meters[n]->getDt());
					}
				}
				Num_Synchs = get_synch_blocks_info(Target_Port, If_Task, Preferences.Target_Synch_Mbx_ID);
				if (Verbose) {
					printf("Number of target real time synchronoscopes: %d\n", Num_Synchs);
					for (int n = 0; n < Num_Synchs; n++) {
						printf("Synchronoscope: %s\n", Synchs[n].name);
						printf(" Sampling time...%f\n", Synchs[n].dt);
					}
				}
				if (Num_Synchs>0)
					printf("Synchronoscopes are not supported!!!\n");
				Is_Target_Connected = 1;
				
				if (Verbose) {
					printf("Target %s is correctly connected\n", RLG_Target_Name);
				}
				
				//if (Num_Scopes > 0) Get_Scope_Data_Thread = new GetScopeDataThread [Num_Scopes];
				if (Num_Scopes > 0) {
					Get_Scope_Data_Thread = new pthread_t [Num_Scopes];
					startScopeThreads();
				}

				if (Num_Logs > 0) {
					Get_Log_Data_Thread = new pthread_t [Num_Logs];
					startLogThreads();
				}
				/*for (int n = 0; n < Num_Logs; n++) {
					unsigned int msg;
					Args_T thr_args;
					thr_args.index = n;
					thr_args.mbx_id = strdup(Preferences.Target_Log_Mbx_ID);
					pthread_create(&Get_Log_Data_Thread[n], NULL, rt_get_log_data, &thr_args);
					rt_receive(0, &msg);
				}*/
				if (Num_ALogs > 0) {
					Get_ALog_Data_Thread = new pthread_t [Num_ALogs];
					startALogThreads();
				}
				
				/*for (int n = 0; n < Num_ALogs; n++) {
					unsigned int msg;
					Alog_T thr_args;
					thr_args.index = n;
					thr_args.mbx_id = strdup(Preferences.Target_ALog_Mbx_ID);
					thr_args.alog_name = strdup(ALogs[n].name);
					printf("%s alog name\n", ALogs[n].name);
					pthread_create(&Get_ALog_Data_Thread[n], NULL, rt_get_alog_data, &thr_args);
					rt_receive(0, &msg);
				}*/
 				//if (Num_Leds > 0) Get_Led_Data_Thread = new GetLedDataThread [Num_Leds];
				if (Num_Leds > 0) {
					Get_Led_Data_Thread = new pthread_t [Num_Leds];
					startLedThreads();
				}
// 				for (int n = 0; n < Num_Leds; n++) {
// 					unsigned int msg;
// 					Args_T thr_args;
// 					thr_args.index = n;
// 					thr_args.mbx_id = strdup(Preferences.Target_Led_Mbx_ID);
// 					//pthread_create(&Get_Led_Data_Thread[n], NULL, rt_get_led_data, &thr_args);
// 					Get_Led_Data_Thread[n].start(&thr_args,this);
// 					rt_receive(0, &msg);
// 				}
 			//	if (Num_Meters > 0) Get_Meter_Data_Thread = new GetMeterDataThread [Num_Meters];
				if (Num_Meters > 0) {
 					Get_Meter_Data_Thread = new pthread_t [Num_Meters];
 					startMeterThreads();
				}
// 				for (int n = 0; n < Num_Meters; n++) {
// 					unsigned int msg;
// 					Args_T thr_args;
// 					thr_args.index = n;
// 					thr_args.mbx_id = strdup(Preferences.Target_Meter_Mbx_ID);
// 					//pthread_create(&Get_Meter_Data_Thread[n], NULL, rt_get_meter_data, &thr_args);
// 					Get_Meter_Data_Thread[n].start(&thr_args,this);
// 					rt_receive(0, &msg);
// 					
// 
// 				}
				/*if (Num_Synchs > 0) Get_Synch_Data_Thread = new pthread_t [Num_Synchs];
				for (int n = 0; n < Num_Synchs; n++) {
					unsigned int msg;
					Args_T thr_args;
					thr_args.index = n;
					thr_args.mbx_id = strdup(Preferences.Target_Synch_Mbx_ID);
					pthread_create(&Get_Synch_Data_Thread[n], NULL, rt_get_synch_data, &thr_args);
					rt_receive(0, &msg);
				}*/
				if (Verbose) {
					printf("Connected succesfully.\n");
				}
				qrl::RT_RETURN(task, CONNECT_TO_TARGET);
				 
				break;



			case DISCONNECT_FROM_TARGET:
				if (Verbose) {
					printf("Disconnecting from target %s\n", Tunable_Parameters[0].model_name);
				}
				Is_Target_Connected = 0;
				stopScopeThreads();
				stopMeterThreads();
				stopLedThreads();
				stopALogThreads();
				stopLogThreads();
				if (Num_Tunable_Blocks>0)
					delete[] Tunable_Blocks;

/*				for (int n = 0; n < Num_Logs; n++) {
					pthread_join(Get_Log_Data_Thread[n], NULL);
				}
				for (int n = 0; n < Num_ALogs; n++) {				//modifiche 29/06/05
					pthread_join(Get_ALog_Data_Thread[n], NULL);
				}*/

/*
				for (int n = 0; n < Num_Synchs; n++) {
					pthread_join(Get_Synch_Data_Thread[n], NULL);
				}
//				pthread_join(GetTargetTimeThread, NULL);
*/
				rt_release_port(Target_Node, Target_Port);
				Target_Port = 0;
				free(Tunable_Parameters);
				//statusBarMessage(tr("Ready..."));
				if (Verbose) {
					printf("Disconnected succesfully.\n");
				}
				qrl::RT_RETURN(task, DISCONNECT_FROM_TARGET);
				break;


			case START_TARGET:

				if (!Is_Target_Running) {
					if (Verbose) {
						printf("Starting real time code...");
					}
					U_Request = 's';
					RT_rpc(Target_Node, Target_Port, If_Task, U_Request, &Is_Target_Running);
					//RLG_Start_Button->deactivate();
					//RLG_Stop_Button->activate();
					if (Verbose) {
						printf("ok\n");
					}
				}
				qrl::RT_RETURN(task, START_TARGET);
				break;


			case STOP_TARGET:
				if (Is_Target_Running) {
					U_Request = 't';
					Is_Target_Connected = 0;
					stopScopeThreads();
					stopMeterThreads();
					stopLedThreads();
					stopALogThreads();
					stopLogThreads();

/*					for (int n = 0; n < Num_Logs; n++) {
						pthread_join(Get_Log_Data_Thread[n], NULL);
					}

					for (int n = 0; n < Num_Synchs; n++) {
						pthread_join(Get_Synch_Data_Thread[n], NULL);
					}
//					pthread_join(GetTargetTimeThread, NULL);
					*/
					if (Verbose) {
						printf("Stopping real time code...");
					}
					RT_rpc(Target_Node, Target_Port, If_Task, U_Request, &Is_Target_Running);
					rt_release_port(Target_Node, Target_Port);
					Target_Node = 0;
					Target_Port = 0;	
				if (Num_Tunable_Blocks>0)
					delete[] Tunable_Blocks;

					//statusBarMessage(tr("Ready..."));
					if (Verbose) {
						printf("ok\n");
					}
				}
				qrl::RT_RETURN(task, STOP_TARGET);
				break;

			
			case UPDATE_PARAM: {

				U_Request = 'p';
				int Map_Offset = (code >> 4) & 0xffff;
				int Mat_Idx = (code >> 20) & 0xfff;
//				int Map_Offset = code >> 4;
//				int Map_Offset = code >> 16;
				int Is_Param_Updated;
				double new_value = (double)Tunable_Parameters[Map_Offset].data_value[Mat_Idx];
				RT_rpc(Target_Node, Target_Port, If_Task, U_Request, &Is_Target_Running);
				RT_rpcx(Target_Node, Target_Port, If_Task, &Map_Offset, &Is_Param_Updated, sizeof(int), sizeof(int));
				RT_rpcx(Target_Node, Target_Port, If_Task, &new_value, &Is_Param_Updated, sizeof(double), sizeof(int));
				RT_rpcx(Target_Node, Target_Port, If_Task, &Mat_Idx, &Is_Param_Updated, sizeof(int), sizeof(int));
				qrl::RT_RETURN(task, UPDATE_PARAM);
				break;
			}

			case BATCH_DOWNLOAD: {

				U_Request = 'd';
				int Is_Param_Updated;
				//int Counter = Parameters_Manager->batch_counter();
				int Counter=batchCounter;
				RT_rpc(Target_Node, Target_Port, If_Task, U_Request, &Is_Target_Running);
				RT_rpcx(Target_Node, Target_Port, If_Task, &Counter, &Is_Param_Updated, sizeof(int), sizeof(int));
				RT_rpcx(Target_Node, Target_Port, If_Task, &Batch_Parameters, &Is_Param_Updated, sizeof(Batch_Parameters_T)*Counter, sizeof(int));
				qrl::RT_RETURN(task, BATCH_DOWNLOAD);
				break;
			}


			case GET_PARAMS: {

				upload_parameters_info(Target_Port, If_Task);/*
				if (Verbose) {
					printf("Target is running...%s\n", Is_Target_Running ? "yes" : "no");
					printf("Number of target tunable parameters...%d\n", Num_Tunable_Parameters);
					for (int n = 0; n < Num_Tunable_Parameters; n++) {
						printf("Block: %s\n", Tunable_Parameters[n].block_name);
						printf(" Parameter: %s\n", Tunable_Parameters[n].param_name);
						printf(" Number of rows: %d\n", Tunable_Parameters[n].n_rows);
						printf(" Number of cols: %d\n", Tunable_Parameters[n].n_cols);
						for (unsigned int nr = 0; nr < Tunable_Parameters[n].n_rows; nr++) {
							for (unsigned int nc = 0; nc < Tunable_Parameters[n].n_cols; nc++) {
								printf(" Value    : %f\n", Tunable_Parameters[n].data_value[nr*Tunable_Parameters[n].n_cols+nc]);
							}
						}
					}
				}*/
				//Parameters_Manager->reload_params();
				qrl::RT_RETURN(task, GET_PARAMS);
				break;
			}
			case CLOSE:
				End_App=1;
				rt_task_delete(Target_Interface_Task);
				
				//for (int n = 0; n < Num_Logs; n++) {
					//pthread_join(Get_Log_Data_Thread[n], NULL);
				//}
				//for (int n = 0; n < Num_Synchs; n++) {
					//pthread_join(Get_Synch_Data_Thread[n], NULL);
				//}
				
			default:
				break;
		}

	}
	rt_task_delete(Target_Interface_Task);

}

void TargetThread::setPreferences(Preferences_T preferences)
{
  Preferences=preferences;
}




int TargetThread::addToBatch(int map_offset,int ind,double value){

	if (batchCounter < MAX_BATCH_PARAMS) {
		Batch_Parameters[batchCounter].index = map_offset;
		Batch_Parameters[batchCounter].value = value;
		Batch_Parameters[batchCounter++].mat_index = ind;
		return batchCounter;
	} else {
		return -1;
	}
}

void TargetThread::resetBatchMode()
{
	batchCounter=0;
}




/**
* @brief starting all scope threads
*/
void TargetThread::startScopeThreads(  )//QRL_ScopeWindow** ScopeWindows)
{

	for (int n = 0; n < Num_Scopes; n++) {
		unsigned int msg;
		Scopes[n]->initializeDataVectors();
		Args_T thr_args;
		thr_args.index = n;
		thr_args.mbx_id = strdup((getPreferences()).Target_Scope_Mbx_ID);
		thr_args.targetThread=(void*)this;
		thr_args.hardRealTime=hardRealTimeScope;
		pthread_create(&Get_Scope_Data_Thread[n], NULL, rt_get_scope_data, &thr_args);
// 		Get_Scope_Data_Thread[n].mutex.lock();
// 		Get_Scope_Data_Thread[n].start(&thr_args,this);//,ScopeWindows[n]);
// 		Get_Scope_Data_Thread[n].threadStarted.wait(&Get_Scope_Data_Thread[n].mutex);
// 		Get_Scope_Data_Thread[n].mutex.unlock();
 		rt_receive(0, &msg);
		//((QMainWindow*)mainWindow)->addDockWidget(Qt::NoDockWidgetArea,ScopeWindows[n]);
	}
	//Get_Scope_Data_Thread[currentScope].setDt(ScopeWindows[currentScope]->getDt());


}

/**
* @brief stopping all existing scope threads
*/
void TargetThread::stopScopeThreads()
{
	for (int n = 0; n < Num_Scopes; n++) {
		pthread_join(Get_Scope_Data_Thread[n], NULL);
		//Get_Scope_Data_Thread[n].wait();
	}
	delete[] Get_Scope_Data_Thread;

}


   QString TargetThread::getScopeName(int i){
	return tr(Scopes[i]->getName());
}



/**
* @brief starting all meter threads
*/
void TargetThread::startMeterThreads()//QRL_MeterWindow** MeterWindows)
{
	//MeterValues.resize(Num_Meters);
	//meterRefreshRate.resize(Num_Meters);
	for (int n = 0; n < Num_Meters; n++) {
// 		meterRefreshRate[n]=20.;
// 		MeterValues[n]=0;
		unsigned int msg;
		Args_T thr_args;
		thr_args.index = n;
		thr_args.mbx_id = strdup((getPreferences()).Target_Meter_Mbx_ID);
		thr_args.hardRealTime=0;
		thr_args.targetThread=(void*)this;
		pthread_create(&Get_Meter_Data_Thread[n], NULL, rt_get_meter_data, &thr_args);
		//Get_Meter_Data_Thread[n].mutex.lock();
		//Get_Meter_Data_Thread[n].start(&thr_args,this);//,MeterWindows[n]);
		//wait until thread is initialized
		//Get_Meter_Data_Thread[n].threadStarted.wait(&Get_Meter_Data_Thread[n].mutex);
		//Get_Meter_Data_Thread[n].mutex.unlock();
 		rt_receive(0, &msg);
		
	}

}


/**
* @brief stopping all existing meter threads
*/
void TargetThread::stopMeterThreads()
{
	for (int n = 0; n < Num_Meters; n++) {
		pthread_join(Get_Meter_Data_Thread[n], NULL);
		//Get_Meter_Data_Thread[n].wait();
	}
	if (Num_Meters>0)
	  delete[] Get_Meter_Data_Thread;
}



   QString TargetThread::getMeterName(int i){
	return tr(Meters[i]->getName());
}

/**
* @brief starting all led threads
*/
void TargetThread::startLedThreads()//QRL_LedWindow** LedWindows)
{
// 	LedValues.resize(Num_Leds);
	for (int n = 0; n < Num_Leds; n++) {
		unsigned int msg;
		Args_T thr_args;
		thr_args.index = n;
		thr_args.mbx_id = strdup(getPreferences().Target_Led_Mbx_ID);
		thr_args.hardRealTime=0;
		thr_args.targetThread=(void*)this;
//		Get_Led_Data_Thread[n].mutex.lock();
		pthread_create(&Get_Led_Data_Thread[n], NULL, rt_get_led_data, &thr_args);
//		Get_Led_Data_Thread[n].start(&thr_args,this);//,LedWindows[n]);
 		rt_receive(0, &msg);
//		Get_Led_Data_Thread[n].threadStarted.wait(&Get_Led_Data_Thread[n].mutex);
//		Get_Led_Data_Thread[n].mutex.unlock();
		
	}
}

/**
* @brief stopping all existing led threads
*/
void TargetThread::stopLedThreads()
{
	for (int n = 0; n < Num_Leds; n++) {
		pthread_join(Get_Led_Data_Thread[n], NULL);
		//Get_Led_Data_Thread[n].wait();
	}
	if (Num_Leds>0)
	  delete[] Get_Led_Data_Thread;
}



   QString TargetThread::getLedName(int i){
	return tr(Leds[i]->getName());
}




/**
* @brief starting all alog threads
*/
void TargetThread::startALogThreads()//QRL_LedWindow** LedWindows)
{
	for (int n = 0; n < Num_ALogs; n++) {
		unsigned int msg;
		Alog_T thr_args;
		thr_args.index = n;
		thr_args.mbx_id = strdup(getPreferences().Target_ALog_Mbx_ID);
		thr_args.alog_name = strdup(ALogs[n]->getName());
		printf("%s alog name\n", ALogs[n]->getName());
		thr_args.targetThread=(void*)this;
		thr_args.hardRealTime=hardRealTimeALog;
		pthread_create(&Get_ALog_Data_Thread[n], NULL, rt_get_alog_data, &thr_args);
		rt_receive(0, &msg);
	}
}

/**
* @brief stopping all existing led threads
*/
void TargetThread::stopALogThreads()
{
	for (int n = 0; n < Num_ALogs; n++) {
		pthread_join(Get_ALog_Data_Thread[n], NULL);
		//Get_Led_Data_Thread[n].wait();
	}
	if (Num_ALogs>0)
	  delete[] Get_ALog_Data_Thread;

}


/**
* @brief starting all log threads
*/
void TargetThread::startLogThreads()
{
	for (int n = 0; n < Num_Logs; n++) {
		unsigned int msg;
		Args_T thr_args;
		thr_args.index = n;
		thr_args.mbx_id = strdup(getPreferences().Target_Log_Mbx_ID);
		thr_args.targetThread=(void*)this;
		thr_args.hardRealTime=hardRealTimeLog;
		pthread_create(&Get_Log_Data_Thread[n], NULL, rt_get_log_data, &thr_args);
 		rt_receive(0, &msg);
	}
}

/**
* @brief stopping all existing log threads
*/
void TargetThread::stopLogThreads()
{
	for (int n = 0; n < Num_Logs; n++) {
		pthread_join(Get_Log_Data_Thread[n], NULL);
		//Get_Led_Data_Thread[n].wait();
	}
	if (Num_Logs>0)
	  delete[] Get_Log_Data_Thread;
}



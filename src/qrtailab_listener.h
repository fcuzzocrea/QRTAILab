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

#include "qrtailab_core.h"


/**
 * @brief QThread for reading Scope data
 */

/*
class GetScopeDataThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
    int setDt(double);
QWaitCondition threadStarted;
QMutex mutex;
    QMutex data;
 //signals:
   // void value(int,float);
 public slots:
    void start(void* arg,TargetThread* targetthread);//,QRL_ScopeWindow* scopewindow);
 private:
    int index,x,y,w,h;
    char* mbx_id;
    TargetThread* targetThread;
  //  QRL_ScopeWindow* ScopeWindow;
    int MsgData, MsgLen, MaxMsgLen, TracesBytes,MsgWait;
    long int Ndistance;
    double dt;
 };
*/






static void *rt_get_scope_data(void *arg)
{
	RT_TASK *GetScopeDataTask;
	MBX *GetScopeDataMbx;
	char GetScopeDataMbxName[7];
	long GetScopeDataPort;
	int MsgData = 0, MsgLen, MaxMsgLen, TracesBytes,MsgWait;
	float MsgBuf[MAX_MSG_LEN/sizeof(float)];
	int n, nn, js, jl;
	int index = ((Args_T *)arg)->index;
	char *mbx_id = strdup(((Args_T *)arg)->mbx_id);
	int x = ((Args_T *)arg)->x;
	int y = ((Args_T *)arg)->y;
	int w = ((Args_T *)arg)->w;
	int h = ((Args_T *)arg)->h;
	int stop_draw = false;
	int save_idx = 0;
	TargetThread* targetThread=(TargetThread*)((Args_T *)arg)->targetThread;
	double dt=targetThread->getScopeDt(index);
	
	rt_allow_nonroot_hrt();
	if (!(GetScopeDataTask = rt_task_init_schmod(qrl::get_an_id("HGS"), 96, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetScopeData Task\n");
		return (void *)1;
	}
	if(targetThread->getTargetNode() == 0) GetScopeDataPort = 0;
	else GetScopeDataPort = rt_request_port(targetThread->getTargetNode());
	sprintf(GetScopeDataMbxName, "%s%d", mbx_id, index);
	if (!(GetScopeDataMbx = (MBX *)RT_get_adr(targetThread->getTargetNode(), GetScopeDataPort, GetScopeDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetScopeDataMbxName);
		return (void *)1;
	}

	TracesBytes = (targetThread->getScopes()[index].ntraces + 1)*sizeof(float);
	MaxMsgLen = (MAX_MSG_LEN/TracesBytes)*TracesBytes;
	MsgLen = (((int)(TracesBytes*dt*(1./targetThread->getScopes()[index].dt)))/TracesBytes)*TracesBytes;
	if (MsgLen < TracesBytes) MsgLen = TracesBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/TracesBytes;

	MsgWait = (int)(((int)((MAX_MSG_SIZE-MsgLen)/TracesBytes))*targetThread->getScopes()[index].dt*1000);
	MsgWait=(MsgWait>WAIT_MIN)?MsgWait:WAIT_MIN;
	MsgWait=(MsgWait<WAIT_MAX)?MsgWait:WAIT_MAX;

	// Ndistance defines the distance between plotted datapoints, to archive the given refresh rate.
	long int Ndistance=(long int)(dt/(targetThread->getScopes())[index].dt);
	int ntraces=(targetThread->getScopes())[index].ntraces;

	rt_send(Target_Interface_Task, 0);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	n=Ndistance;
	RTIME t0,t,told; int time=0; double time10=0.;double t10;int n10=0;
	t0 = rt_get_cpu_time_ns();
	while (true) {
		if (targetThread->getEndApp() || ! targetThread->getIsTargetConnected()) break;
		while (RT_mbx_receive_if(targetThread->getTargetNode(), GetScopeDataPort, GetScopeDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;
			//rt_sleep(nano2count(TEN_MS_IN_NS));
			//msleep(10);
			msleep(MsgWait);
		}

	if (targetThread->getVerbose()){
		t = rt_get_cpu_time_ns();told=t0;
		t0=rt_get_cpu_time_ns();
		if (n10==1000) {
			printf("Time failure soll %f zu ist: %f \n",time10,(t10));
			n10=0;
			t10=0.;
			time10=0.;
		} else {
			n10++;
			t10+=(t - told)*1.0E-6;
			time10+=time*dt*1.0E3;
		}
		if (((t - told)*1.0E-6)>25.)
		   printf("Time failure  %f (%f)\n",(t - told)*1.0E-6,time*dt*1.0E3 );
		}
		time=0;
		if (n>MsgData)
			n=n-MsgData;
		else {
                  
		   js = (n-1)*(ntraces+1)+1;
		   if (n==1)
			js=1;
		   while(n<=MsgData){
			for (nn = 0; nn < ntraces; nn++) {
			
			// if (ScopeWindow)
			  //  ScopeWindow->setValue(nn,MsgBuf[js++]);
			targetThread->setScopeValue(MsgBuf[js++],index,nn);
			    //emit value(nn,MsgBuf[js++]);
				//temp1[nn][time-1]=MsgBuf[js++];
			    //ScopeWindow->getThread()->setValue(nn,MsgBuf[js++]);
			} time++;
			js=js+(Ndistance-1)*(ntraces+1)+1;
			n=n+Ndistance;
		   }
		   n=n-MsgData;
		}

		if (targetThread->getScopeDt(index)!=dt){
			dt=targetThread->getScopeDt(index);
			MsgLen = (((int)(TracesBytes*dt*(1./targetThread->getScopes()[index].dt)))/TracesBytes)*TracesBytes;
			if (MsgLen < TracesBytes) MsgLen = TracesBytes;
			if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
			MsgData = MsgLen/TracesBytes;
			// Ndistance defines the distance between plotted datapoints, to archive the given refresh rate.
			Ndistance=(long int)(dt/(targetThread->getScopes())[index].dt);
	
			if (Ndistance<1)
				Ndistance=1;
			MsgWait = (int)(((int)((MAX_MSG_SIZE-MsgLen)/TracesBytes))*targetThread->getScopes()[index].dt*1000);
			MsgWait=(MsgWait>WAIT_MIN)?MsgWait:WAIT_MIN;
			MsgWait=(MsgWait<WAIT_MAX)?MsgWait:WAIT_MAX;
		}




/*		if (ScopeWindow->start_saving()) {
			jl = 0;
			printf("%d from %d saved\n",save_idx,ScopeWindow->n_points_to_save());
			for (n = 0; n < MsgData; n++) {
				for (nn = 0; nn < ntraces + 1; nn++) {
					fprintf(ScopeWindow->save_file(), "%1.10f ", MsgBuf[jl++]);
				}
				fprintf(ScopeWindow->save_file(), "\n");
				save_idx++;
				if (save_idx == ScopeWindow->n_points_to_save()) {
					ScopeWindow->stop_saving();
					save_idx = 0;
					break;
				}
			}
		}*/
	}

end:
	if (targetThread->getVerbose()) {
		printf("Deleting scope thread number...%d\n", index);
	}
	rt_release_port(targetThread->getTargetNode(), GetScopeDataPort);
	rt_task_delete(GetScopeDataTask);

	return 0;
}







 //!QThread for reading Meter data
/*
class GetMeterDataThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
   QWaitCondition threadStarted;
   QMutex mutex;
 //signals:
   // void value(int,float);
    int setRefreshRate(double);
 public slots:
    void start(void* arg,TargetThread* targetthread); //, QRL_MeterWindow* meterwindow);
 private:
    int index,x,y,w,h;
    double RefreshRate;
    char* mbx_id;
    TargetThread* targetThread;
    //QRL_MeterWindow* MeterWindow;
    int MsgData, MsgLen, MaxMsgLen, DataBytes;
    long int Ndistance;
    double value;

 };
*/




static void *rt_get_meter_data(void *arg)
{
	RT_TASK *GetMeterDataTask;
	MBX *GetMeterDataMbx;
	char GetMeterDataMbxName[7];
	long GetMeterDataPort;
	int MsgData = 0, MsgLen, MaxMsgLen, DataBytes;
	float MsgBuf[MAX_MSG_LEN/sizeof(float)];
	int n;
	 long int Ndistance;
	int index = ((Args_T *)arg)->index;
	char *mbx_id = strdup(((Args_T *)arg)->mbx_id);
	int x = ((Args_T *)arg)->x;
	int y = ((Args_T *)arg)->y;
	int w = ((Args_T *)arg)->w;
	int h = ((Args_T *)arg)->h;
        TargetThread* targetThread=(TargetThread*)((Args_T *)arg)->targetThread;
	double RefreshRate=targetThread->getMeterRefreshRate(index);
	rt_allow_nonroot_hrt();
	if (!(GetMeterDataTask = rt_task_init_schmod(qrl::get_an_id("HGM"), 97, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetMeterData Task\n");
		return (void *)1;
	}

	if(targetThread->getTargetNode() == 0) GetMeterDataPort=0;
	else GetMeterDataPort = rt_request_port(targetThread->getTargetNode());

	sprintf(GetMeterDataMbxName, "%s%d", mbx_id, index);
	if (!(GetMeterDataMbx = (MBX *)RT_get_adr(targetThread->getTargetNode(), GetMeterDataPort, GetMeterDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetMeterDataMbxName);
		exit(1);
	}

	DataBytes = sizeof(float);
	MaxMsgLen = (MAX_MSG_LEN/DataBytes)*DataBytes;
	MsgLen = (((int)(DataBytes/RefreshRate*(1./(targetThread->getMeters())[index].dt)))/DataBytes)*DataBytes;
	if (MsgLen < DataBytes) MsgLen = DataBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/DataBytes;
	Ndistance=(long int)(1./RefreshRate/(targetThread->getMeters())[index].dt);

	rt_send(Target_Interface_Task, 0);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	n=Ndistance;


	while (true) {
		if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) break;
		while (RT_mbx_receive_if(targetThread->getTargetNode(), GetMeterDataPort, GetMeterDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;

			//msleep(12); //waits for new Data from the mailbox
			rt_sleep(nano2count(TEN_MS_IN_NS));
		}

		if (n>MsgData)
			n=n-MsgData;
		else {
                   
		   int i;
		   for (i=(int)n-1; i < MsgData; i=i+(int)Ndistance){
			//if (MeterWindow)
			  //  MeterWindow->setValue(MsgBuf[i]);
			targetThread->setMeterValue(MsgBuf[i],index);
		   }
		   n=Ndistance;
		}
		//Fl::unlock();
		if (RefreshRate!=targetThread->getMeterRefreshRate(index)){
		
		RefreshRate=targetThread->getMeterRefreshRate(index);
		MsgLen = (((int)(DataBytes/RefreshRate*(1./(targetThread->getMeters())[index].dt)))/DataBytes)*DataBytes;
		if (MsgLen < DataBytes) MsgLen = DataBytes;
		if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
		MsgData = MsgLen/DataBytes;
		Ndistance=(long int)(1./RefreshRate/(targetThread->getMeters())[index].dt);
		if (Ndistance<1)
			Ndistance=1;
		
		}
	}       


end:
	if (targetThread->getVerbose()) {
		printf("Deleting meter thread number...%d\n", index);
	}
	//Meter_Win->hide();
	rt_release_port(targetThread->getTargetNode(), GetMeterDataPort);
	rt_task_delete(GetMeterDataTask);

	return 0;
}







/**
 * @brief QThread for reading Led data
 */
/*
class GetLedDataThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
QWaitCondition threadStarted;
QMutex mutex;
//signals:
    //void value(int,float);
 public slots:
    void start(void* arg,TargetThread* targetthread);//,QRL_LedWindow* ledwindow);
 private:
    int index,x,y,w,h;
    char* mbx_id;
    TargetThread* targetThread;
  //  QRL_LedWindow* LedWindow;
    unsigned int Led_Mask;
 };
*/

static void *rt_get_led_data(void *arg)
{

	RT_TASK *GetLedDataTask;
	MBX *GetLedDataMbx;
	char GetLedDataMbxName[7];
	long GetLedDataPort;
	int MsgData = 0, MsgLen, MaxMsgLen, DataBytes;
	unsigned int MsgBuf[MAX_MSG_LEN/sizeof(unsigned int)];	
	int n;
	int index = ((Args_T *)arg)->index;
	char *mbx_id = strdup(((Args_T *)arg)->mbx_id);
	int x = ((Args_T *)arg)->x;
	int y = ((Args_T *)arg)->y;
	int w = ((Args_T *)arg)->w;
	int h = ((Args_T *)arg)->h;
	unsigned int Led_Mask = 0;
	TargetThread* targetThread=(TargetThread*)((Args_T *)arg)->targetThread;
	rt_allow_nonroot_hrt();
	if (!(GetLedDataTask = rt_task_init_schmod(qrl::get_an_id("HGE"), 97, 0, 0, SCHED_RR, 0xFF))) {
		printf("Cannot init Host GetLedData Task\n");
		return (void *)1;
	}
	if(targetThread->getTargetNode() == 0) GetLedDataPort=0;
	else GetLedDataPort = rt_request_port(targetThread->getTargetNode());
	sprintf(GetLedDataMbxName, "%s%d", mbx_id, index);
	if (!(GetLedDataMbx = (MBX *)RT_get_adr(targetThread->getTargetNode(), GetLedDataPort, GetLedDataMbxName))) {
		printf("Error in getting %s mailbox address\n", GetLedDataMbxName);
		exit(1);
	}
	DataBytes = sizeof(unsigned int);
	MaxMsgLen = (MAX_MSG_LEN/DataBytes)*DataBytes;
	MsgLen = (((int)(DataBytes*REFRESH_RATE*(1./(targetThread->getLeds())[index].dt)))/DataBytes)*DataBytes;
	if (MsgLen < DataBytes) MsgLen = DataBytes;
	if (MsgLen > MaxMsgLen) MsgLen = MaxMsgLen;
	MsgData = MsgLen/DataBytes;


	rt_send(Target_Interface_Task, 0);
	mlockall(MCL_CURRENT | MCL_FUTURE);


	while (true) {
		if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) break;
		while (RT_mbx_receive_if(targetThread->getTargetNode(), GetLedDataPort, GetLedDataMbx, &MsgBuf, MsgLen)) {
			if (targetThread->getEndApp() || !targetThread->getIsTargetConnected()) goto end;

			//msleep(12);
			rt_sleep(nano2count(TEN_MS_IN_NS));
		}
			Led_Mask = MsgBuf[0];
			targetThread->setLedValue(Led_Mask,index);

	}


end:
	if (targetThread->getVerbose()) {
		printf("Deleting led thread number...%d\n", index);
	}
	//Led_Win->hide();
	rt_release_port(targetThread->getTargetNode(), GetLedDataPort);
	rt_task_delete(GetLedDataTask);

	return 0;
}




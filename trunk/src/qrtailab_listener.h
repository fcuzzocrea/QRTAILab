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

 //!QThread for reading Meter data
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





/**
 * @brief QThread for reading Led data
 */

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

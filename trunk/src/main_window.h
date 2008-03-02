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
 file:		main_window.h
 describtion:
   file for the classes QRL_MainWindow
*/

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H 1

#include "ui_qrl_main_window.h"
#include "ui_qrl_connectdialog.h"
#include <QtGui>
#include "meters_manager.h"
#include "leds_manager.h"
#include "scopes_manager.h"
#include "parameters_manager.h"
#include "qrtailab_core.h"
/**
 * @brief Main QThread 
 */

/*
class MainThread : public QThread
 {
 Q_OBJECT
 public:
    void run();
    QWaitCondition threadStarted;
    QMutex mutex;
 //signals:
   // void value(int,float);
 public slots:
    void start();
 private:
    TargetThread* targetThread;
 };

*/







/**
 * @brief Connect Dialog
 */

class QRL_connectDialog : public QDialog, private Ui::QRL_connectDialogWindow
{
    Q_OBJECT
 
public:
    QRL_connectDialog(QWidget *parent = 0);
    void setPreferences(Preferences_T p);
    Preferences_T getPreferences();
 
public slots:
  //void  accept();
  //void  reject();
private:
    Preferences_T Preferences;
};

/**
 * @brief Main Window
 * @todo save and load settings
 */
class QRL_MainWindow : public QMainWindow, private Ui::QRL_MainWindow
{
    Q_OBJECT
 friend class TestQrtaiLab;
public:
    QRL_MainWindow();
  //  ~QRL_MainWindow();
//signals:
 //    void sendOrder(int);
    void setVerbose(int v){Verbose=v;}
    void setTargetThread( TargetThread* t){targetthread=t;}
protected:
     void closeEvent(QCloseEvent *event);

protected slots:
    void about();
    void disconnectDialog();
    void connectDialog();
    void connect_WProfile();
    void saveProfile();
    void start();
    void stop();
    void showMetersManager();
    void showScopesManager();
    void showLedsManager();
    void showParametersManager();
    void setStatusBarMessage(const QString &);
 
private:
    void enableActionStart(bool);
    void enableActionStop(bool);
    void enableActionConnect(bool);
    void enableActionConnectWProfile(bool);
    void enableActionDisconnect(bool);
    void enableActionShowMeter(bool);
    void enableActionShowScope(bool);
    void enableActionShowLed(bool);
    void enableActionShowParameter(bool);
    void enableActionSaveProfile(bool);
    void enableActionDeleteProfile(bool);
    TargetThread* targetthread;
    QRL_MetersManager *MetersManager;
    QRL_LedsManager *LedsManager;
    QRL_ScopesManager *ScopesManager;
    QRL_ParametersManager *ParametersManager;
    QRL_connectDialog *ConnectDialog;
    QLabel* statusMessage;
    QMdiArea *mdiArea;
    QString profileName;
    int Verbose;
    QRtaiLabCore	*qTargetInterface;
    //QSettings *settings;
};




#endif

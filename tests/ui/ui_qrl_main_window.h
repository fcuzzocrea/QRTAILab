/********************************************************************************
** Form generated from reading ui file 'qrl_main_window.ui'
**
** Created: Sun Jan 6 12:42:45 2008
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QRL_MAIN_WINDOW_H
#define UI_QRL_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

class Ui_QRL_MainWindow
{
public:
    QAction *actionConnect;
    QAction *actionAbout_QRtaiLab;
    QAction *actionDisconnect;
    QAction *actionExit;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionShowMeter;
    QAction *actionShowScope;
    QAction *actionShowLed;
    QAction *actionShowParameter;
    QAction *actionConnect_WProfile;
    QAction *actionSaveProfile;
    QAction *actionDeleteProfile;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuView;
    QMenu *menuHelp;
    QMenu *menuTarget;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *QRL_MainWindow)
    {
    if (QRL_MainWindow->objectName().isEmpty())
        QRL_MainWindow->setObjectName(QString::fromUtf8("QRL_MainWindow"));
    QRL_MainWindow->resize(802, 579);
    QRL_MainWindow->setMinimumSize(QSize(0, 0));
    QRL_MainWindow->setSizeIncrement(QSize(50, 30));
    QRL_MainWindow->setAnimated(false);
    QRL_MainWindow->setDockOptions(QMainWindow::ForceTabbedDocks);
    actionConnect = new QAction(QRL_MainWindow);
    actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
    actionConnect->setIcon(QIcon(QString::fromUtf8(":/icons/icons/connect_icon.xpm")));
    actionAbout_QRtaiLab = new QAction(QRL_MainWindow);
    actionAbout_QRtaiLab->setObjectName(QString::fromUtf8("actionAbout_QRtaiLab"));
    actionDisconnect = new QAction(QRL_MainWindow);
    actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
    actionDisconnect->setEnabled(true);
    actionDisconnect->setIcon(QIcon(QString::fromUtf8(":/icons/icons/disconnect_icon.xpm")));
    actionExit = new QAction(QRL_MainWindow);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
    actionExit->setIcon(QIcon(QString::fromUtf8(":/icons/icons/exit_icon.xpm")));
    actionStart = new QAction(QRL_MainWindow);
    actionStart->setObjectName(QString::fromUtf8("actionStart"));
    actionStart->setIcon(QIcon(QString::fromUtf8(":/icons/icons/start_icon.xpm")));
    actionStop = new QAction(QRL_MainWindow);
    actionStop->setObjectName(QString::fromUtf8("actionStop"));
    actionStop->setIcon(QIcon(QString::fromUtf8(":/icons/icons/stop_icon.xpm")));
    actionShowMeter = new QAction(QRL_MainWindow);
    actionShowMeter->setObjectName(QString::fromUtf8("actionShowMeter"));
    actionShowMeter->setIcon(QIcon(QString::fromUtf8(":/icons/icons/meter_icon.xpm")));
    actionShowScope = new QAction(QRL_MainWindow);
    actionShowScope->setObjectName(QString::fromUtf8("actionShowScope"));
    actionShowScope->setIcon(QIcon(QString::fromUtf8(":/icons/icons/scope_icon.xpm")));
    actionShowLed = new QAction(QRL_MainWindow);
    actionShowLed->setObjectName(QString::fromUtf8("actionShowLed"));
    actionShowLed->setIcon(QIcon(QString::fromUtf8(":/icons/icons/led_icon.xpm")));
    actionShowParameter = new QAction(QRL_MainWindow);
    actionShowParameter->setObjectName(QString::fromUtf8("actionShowParameter"));
    actionShowParameter->setIcon(QIcon(QString::fromUtf8(":/icons/icons/parameters_icon.xpm")));
    actionConnect_WProfile = new QAction(QRL_MainWindow);
    actionConnect_WProfile->setObjectName(QString::fromUtf8("actionConnect_WProfile"));
    actionConnect_WProfile->setIcon(QIcon(QString::fromUtf8(":/icons/icons/connect_wprofile_icon.xpm")));
    actionSaveProfile = new QAction(QRL_MainWindow);
    actionSaveProfile->setObjectName(QString::fromUtf8("actionSaveProfile"));
    actionSaveProfile->setIcon(QIcon(QString::fromUtf8(":/icons/icons/save_profile_icon.xpm")));
    actionDeleteProfile = new QAction(QRL_MainWindow);
    actionDeleteProfile->setObjectName(QString::fromUtf8("actionDeleteProfile"));
    actionDeleteProfile->setIcon(QIcon(QString::fromUtf8(":/icons/icons/del_profile_icon.xpm")));
    centralwidget = new QWidget(QRL_MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    QRL_MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(QRL_MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 802, 29));
    menuView = new QMenu(menubar);
    menuView->setObjectName(QString::fromUtf8("menuView"));
    menuHelp = new QMenu(menubar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    menuTarget = new QMenu(menubar);
    menuTarget->setObjectName(QString::fromUtf8("menuTarget"));
    QRL_MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(QRL_MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    QRL_MainWindow->setStatusBar(statusbar);
    toolBar = new QToolBar(QRL_MainWindow);
    toolBar->setObjectName(QString::fromUtf8("toolBar"));
    toolBar->setMinimumSize(QSize(0, 0));
    toolBar->setSizeIncrement(QSize(50, 30));
    toolBar->setOrientation(Qt::Horizontal);
    QRL_MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

    menubar->addAction(menuTarget->menuAction());
    menubar->addAction(menuView->menuAction());
    menubar->addAction(menuHelp->menuAction());
    menuView->addAction(actionShowParameter);
    menuView->addSeparator();
    menuView->addAction(actionShowScope);
    menuView->addAction(actionShowMeter);
    menuView->addAction(actionShowLed);
    menuHelp->addAction(actionAbout_QRtaiLab);
    menuTarget->addAction(actionConnect_WProfile);
    menuTarget->addAction(actionConnect);
    menuTarget->addAction(actionDisconnect);
    menuTarget->addSeparator();
    menuTarget->addAction(actionSaveProfile);
    menuTarget->addAction(actionDeleteProfile);
    menuTarget->addSeparator();
    menuTarget->addAction(actionStart);
    menuTarget->addAction(actionStop);
    menuTarget->addSeparator();
    menuTarget->addAction(actionExit);
    toolBar->addAction(actionExit);
    toolBar->addSeparator();
    toolBar->addAction(actionConnect_WProfile);
    toolBar->addAction(actionConnect);
    toolBar->addAction(actionDisconnect);
    toolBar->addSeparator();
    toolBar->addAction(actionSaveProfile);
    toolBar->addAction(actionDeleteProfile);
    toolBar->addSeparator();
    toolBar->addAction(actionStart);
    toolBar->addAction(actionStop);
    toolBar->addSeparator();
    toolBar->addAction(actionShowParameter);
    toolBar->addSeparator();
    toolBar->addAction(actionShowScope);
    toolBar->addAction(actionShowMeter);
    toolBar->addAction(actionShowLed);

    retranslateUi(QRL_MainWindow);

    QMetaObject::connectSlotsByName(QRL_MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *QRL_MainWindow)
    {
    QRL_MainWindow->setWindowTitle(QApplication::translate("QRL_MainWindow", "Rtai-Lab Graphical User Interface", 0, QApplication::UnicodeUTF8));
    actionConnect->setText(QApplication::translate("QRL_MainWindow", "Connect", 0, QApplication::UnicodeUTF8));
    actionAbout_QRtaiLab->setText(QApplication::translate("QRL_MainWindow", "About QRtaiLab", 0, QApplication::UnicodeUTF8));
    actionDisconnect->setText(QApplication::translate("QRL_MainWindow", "Disconnect", 0, QApplication::UnicodeUTF8));
    actionExit->setText(QApplication::translate("QRL_MainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
    actionStart->setText(QApplication::translate("QRL_MainWindow", "Start", 0, QApplication::UnicodeUTF8));
    actionStop->setText(QApplication::translate("QRL_MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
    actionShowMeter->setText(QApplication::translate("QRL_MainWindow", "Meter", 0, QApplication::UnicodeUTF8));
    actionShowScope->setText(QApplication::translate("QRL_MainWindow", "Scope", 0, QApplication::UnicodeUTF8));
    actionShowLed->setText(QApplication::translate("QRL_MainWindow", "Led", 0, QApplication::UnicodeUTF8));
    actionShowParameter->setText(QApplication::translate("QRL_MainWindow", "Parameter", 0, QApplication::UnicodeUTF8));
    actionConnect_WProfile->setText(QApplication::translate("QRL_MainWindow", "Connect With Profile", 0, QApplication::UnicodeUTF8));
    actionSaveProfile->setText(QApplication::translate("QRL_MainWindow", "Save Profile", 0, QApplication::UnicodeUTF8));
    actionDeleteProfile->setText(QApplication::translate("QRL_MainWindow", "Delete Profile", 0, QApplication::UnicodeUTF8));
    menuView->setTitle(QApplication::translate("QRL_MainWindow", "&View", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("QRL_MainWindow", "&Help", 0, QApplication::UnicodeUTF8));
    menuTarget->setTitle(QApplication::translate("QRL_MainWindow", "&Target", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QRL_MainWindow: public Ui_QRL_MainWindow {};
} // namespace Ui

#endif // UI_QRL_MAIN_WINDOW_H

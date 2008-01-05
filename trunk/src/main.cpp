 #include <QApplication>
#include "main_window.h"



/** @mainpage
QRtaiLab is a QT-based port of the software scope xrtailab of the RTAI project. It can be used as virtual oscilloscope and monitoring application for interacting with the real-time executable. More informations about RTAI can be found on the following website: https://www.rtai.org.

@section license License

QRtaiLab is distributed under the terms of the LGPL V2.

@section download Download

Stable releases, prereleases and snapshots are available at the QRtaiLab project page.
*/

int main(int argc, char *argv[])
{


     //Q_INIT_RESOURCE(application);

     QApplication app(argc, argv);
     QRL_MainWindow mainWin;
     mainWin.show();
     return app.exec();


    //a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

 }

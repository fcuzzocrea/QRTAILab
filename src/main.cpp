 #include <QApplication>
#include "main_window.h"




int main(int argc, char *argv[])
{


     //Q_INIT_RESOURCE(application);

     QApplication app(argc, argv);
     QRL_MainWindow mainWin;
     mainWin.show();
     return app.exec();


    //a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

 }

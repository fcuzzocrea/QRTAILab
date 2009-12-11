/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
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

 #include <QApplication>
#include "main_window.h"
#include <getopt.h>
#include <iostream>


static const struct option long_options[ ] = {
	{ "help" /* name of the long option */, 0 /* arguments required */, NULL /* value is returned */, 'h' /* value */ },
	{ "verbose", 0, 0, 'v' },
        { "version", 0, 0, 'V' },
        { "profile", 1, 0, 'p' },
        { "parameterfile", 1, 0, 'P' },
        { "log", 1, 0, 'L' },
        { "logfile", 1, 0, 'l' },
        { "scope", 1, 0, 'S' },
        { "scopefile", 1, 0, 's' },
        { "connect", 0, 0, 'c' },
        {0, 0, 0, 0}
};

//static RT_TASK *RLG_Main_Task;


int main(int argc, char *argv[])
{
	// ** PARSE COMMAND LINE OPTIONS ** //
	int	verboseOutput = 0;
	int		optionFlag = 0;
        bool connect=false;
        QString profilefile;
        QString parameterfile;
        QVector <int> Scopes;
        QVector <QString> ScopeFiles;

        QVector <int> Logs;
        QVector <QString> LogFiles;

     QApplication app(argc, argv);
     QRL_MainWindow mainWin(verboseOutput);
	
        while ( ( optionFlag = getopt_long( argc, argv, "hvVp:P:l:L:s:S:c", long_options, NULL ) ) != EOF ) {
 		
		switch ( optionFlag ) {
			case 'v':
				verboseOutput = 1;
                                mainWin.setVerbose(verboseOutput);
				break;
                        case 'c':
                                connect=true;
                                break;
                        case 'p':
//                                printf ("Loading profile '%s'\n", optarg);
                                profilefile=(optarg);
                                break;
                        case 'P':
//                                printf ("Loading Parameterfile '%s'\n", optarg);
                                parameterfile=optarg;
                                break;
                        case 'l':
//                                printf ("option l with value '%s'\n", optarg);
                                LogFiles.append(optarg);
                                break;
                        case 'L':
//                                printf ("option L with value '%s'\n", optarg);
                                Logs.append(QString(optarg).toInt());
                                break;
                        case 's':
//                                printf ("option s with value '%s'\n", optarg);
                                ScopeFiles.append(optarg);
                                break;
                        case 'S':
//                                printf ("option S with value '%s'\n", optarg);
                                Scopes.append(QString(optarg).toInt());
                                break;
			case 'V':
				std::cout << "QRtaiLab version " << QRTAILAB_VERSION << std::endl;
				exit( 0 );
				break;
			
			case 'h':
				std::cout << "\nUsage:  qrtailab [OPTIONS]\n" <<
					"\n" <<
					"OPTIONS:\n" <<
					"  -h, --help\n" <<
					"      print usage\n" <<
                                        "  -c, --connect\n" <<
                                        "      connect to target\n" <<
                                        "  -p [filename], --profile [filename]\n" <<
                                        "      load profile\n" <<
                                        "  -P [filename], --parameterfile [filename]\n" <<
                                        "      load a parameterfile\n" <<
                                        "  -S [number], --scope [number]\n" <<
                                        "     defines a scope for setting  a filename\n" <<
                                        "  -s [filename], --scopefile [filename]\n" <<
                                        "     sets a filename for saving for a scope\n" <<
                                        "  -L [number], --log [number]\n" <<
                                        "     defines a log for setting  a filename\n" <<
                                        "  -l [filename], --logfile [filename]\n" <<
                                        "     sets a filename for saving for a log\n" <<
					"  -v, --verbose\n" <<
					"      verbose output\n" <<
					"  -V, --version\n" <<
					"      print QRtaiLab version\n\n";
				exit( 0 );
				break;
				
			default:
                                   printf(("Try `qrtailab --help' for more information.\n") );
                                    exit(-1);
				break;
		}
	}

     //Q_INIT_RESOURCE(application);
/*
   rt_allow_nonroot_hrt();
   if (!(RLG_Main_Task = rt_task_init_schmod(qrl::get_an_id("RLGM"), 98, 0, 0, SCHED_FIFO, 0xFF))) {
               printf("Cannot init RTAI-Lab GUI main task\n");
               //close();
		return 1;
    }*/

     //mainWin.setVerbose(verboseOutput);


      if ((Scopes.size() != ScopeFiles.size())) {

            printf(("After the -S option a -s must follow! Example: -S0 -stest \n") );
            exit(-1);
     }
      if ((Logs.size() != LogFiles.size())) {

            printf(("After the -L option a -l must follow! Example: -L0 -ltest \n") );
            exit(-1);
     }

     mainWin.show();
    if (!profilefile.isEmpty())
         mainWin.loadProfile(profilefile);
    else  if (connect)
         mainWin.connectDialog();



    if (!parameterfile.isEmpty()) {
        if (profilefile.isEmpty() && !connect)
              mainWin.connectDialog();
        mainWin.loadParameters(parameterfile);
      }



     if ((Logs.size() == LogFiles.size()) && Logs.size()>0){
          for (int i=0;i<Logs.size();i++)
            mainWin.setLogFileName((int)Logs.at(i),LogFiles.at(i));

     }


     if ((Scopes.size() == ScopeFiles.size()) && Scopes.size()>0){
        for (int i=0;i<Scopes.size();i++)
            mainWin.setScopeFileName((int)Scopes.at(i),ScopeFiles.at(i));
     }

     int ret= app.exec();
    // rt_task_delete(RLG_Main_Task);
	return ret;

    //a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

 }

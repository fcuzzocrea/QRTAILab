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

 #include <QApplication>
#include "main_window.h"
#include <getopt.h>
#include <iostream>


struct option long_options[ ] = {
	{ "help" /* name of the long option */, 0 /* arguments required */, NULL /* value is returned */, 'h' /* value */ },
	{ "verbose", 0, 0, 'v' },
	{ "version", 0, 0, 'V' }
};

//static RT_TASK *RLG_Main_Task;


int main(int argc, char *argv[])
{
	// ** PARSE COMMAND LINE OPTIONS ** //
	int	verboseOutput = 0;
	int		optionFlag = 0;

	
 	while ( ( optionFlag = getopt_long( argc, argv, "hvV", long_options, NULL ) ) != EOF ) {
 		
		switch ( optionFlag ) {
			case 'v':
				verboseOutput = 1;
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
					"  -v, --verbose\n" <<
					"      verbose output\n" <<
					"  -V, --version\n" <<
					"      print QRtaiLab version\n\n";
				exit( 0 );
				break;
				
			default:
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
     QApplication app(argc, argv);
     QRL_MainWindow mainWin;
     mainWin.setVerbose(verboseOutput);
     mainWin.show();

     
     int ret= app.exec();
    // rt_task_delete(RLG_Main_Task);
	return ret;

    //a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

 }

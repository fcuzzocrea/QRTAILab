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

/*
 file:		scopes_window.cpp
 describtion:
   file for the classes QRL_ScopeWindow
*/

#include "log_window.h"
#include <stdlib.h>



//
//  Initialize Log window
//

QRL_LogWindow::QRL_LogWindow(QWidget *parent,QRL_LogData *log)
	:QMdiSubWindow(parent),Log(log)
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("QRL_LogWindow"));
    this->resize(350, 249);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->setWindowIcon(QIcon(QString::fromUtf8(":/icons/log_icon.xpm")));
    this->setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint );
   RefreshRate=20.;


       matrixPlot = new QTableWidget(Log->getNRow(),Log->getNCol(),this);

     for (int i=0;i<Log->getNRow();i++)
           for (int j=0;j<Log->getNCol();j++){
             QTableWidgetItem *newItem = new QTableWidgetItem(tr("0"));
             matrixPlot->setItem(i, j, newItem);
            }\
    //qwtPlot=this;
    matrixPlot->setObjectName(QString::fromUtf8("qwtPlot"));

    this->setWidget(matrixPlot);
    this->setWindowTitle(QApplication::translate("QRL_LogWindow", Log->name, 0, QApplication::UnicodeUTF8));














	timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
        timer->start((int)(1./RefreshRate*1000.));


}

QRL_LogWindow::~QRL_LogWindow(){


}


void QRL_LogWindow::refresh()
{
	
}

void QRL_LogWindow::setPlotting(bool b){
    plotting=b;
    if (plotting){
      Log->setPlotting(true);
   } else {
	Log->setPlotting(false);
    }
    
  }
  
   void QRL_LogWindow::changeRefreshRate(double rr)
{
	RefreshRate=rr;
	if (	RefreshRate<0.)
		RefreshRate=20.;
	timer->stop();
	timer->start((int)(1./RefreshRate*1000.));
	
	//Plotting_Scope_Data_Thread->changeRefreshRate(RefreshRate);

}
void QRL_LogWindow::setValue(const QVector< QVector<float> > &v){
for (int i=0;i<v.size();i++){
        for (int j=0;j<v.at(i).size();j++){
//             printf(" %f ",v.at(i).at(j));
             (matrixPlot->item(i,j))->setText(tr("%1").arg(v.at(i).at(j)));
         }
//         printf("\n");
}
//  printf("\n");

}




QDataStream& operator<<(QDataStream &out, const QRL_LogWindow &d){
        qint32 a;
	out  << d.size()  << d.pos() << d.isVisible();
	out <<d.plotting;
	out << d.RefreshRate;
	out << d.saveTime;
	out << d.fileName;
	return out;
}


QDataStream& operator>>(QDataStream &in, QRL_LogWindow(&d)){
	QSize s;QPoint p;bool b; QColor c; qint32 a,a2;QFont f; double dd;
	QString str; int Ncurve;
	in >> s;d.resize(s);
	in >> p; d.move(p);
	in >> b; d.setVisible(b);
	in >> b; d.setPlotting(b);
	in >> dd; d.changeRefreshRate(dd);
	in >> dd; d.setSaveTime(dd);
        in >> str; d.setFileName(str);
		  
	return in;
}

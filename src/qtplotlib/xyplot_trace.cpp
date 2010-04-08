#include "xyplot_trace.h"

QPL_XYPlotTrace::QPL_XYPlotTrace(QwtPlot *parent, unsigned int maxdatapoints, int j){
                qwtPlot=parent;
                MaxDataPoints=maxdatapoints;
                index = j;
                d_x = new double[MaxDataPoints+1];
                d_y = new double[MaxDataPoints+1];

                for (unsigned int i = 0; i< MaxDataPoints; i++)
              {
                  d_x[i] =0.;
                  d_y[i] = 0.;
              }
                cData = new QwtPlotCurve(QObject::tr("Trace %1").arg(j));
                cData->attach(qwtPlot);
                //cData->setPaintAttribute(QwtPlotCurve::PaintFiltered,true);
                cData->setPaintAttribute(QwtPlotCurve::ClipPolygons,true);
                oldStyle=QwtPlotCurve::Lines;
                cData->setStyle(QwtPlotCurve::Lines);
                setSymbolStyle(QwtSymbol::NoSymbol);
                oldSymbol=QwtSymbol::NoSymbol;
                xmax=1;
                lineWidth=3;
                dy=1.;
                dx=1.;


                visible=true;

                switch(j){
                        case 0:brush=QBrush(Qt::red);
                                break;
                        case 1:brush=QBrush(Qt::green);
                                break;
                        case 2:brush=QBrush(Qt::yellow);
                                break;
                        case 3:brush=QBrush(Qt::blue);
                                break;
                        case 4:brush=QBrush(Qt::black);
                                break;
                        case 5:brush=QBrush(Qt::magenta);
                                break;
                        case 6:brush=QBrush(Qt::darkRed);
                                break;
                        case 7:brush=QBrush(Qt::darkGreen);
                                break;
                        case 8:brush=QBrush(Qt::darkMagenta);
                                break;
                        case 9:brush=QBrush(Qt::darkYellow);
                                break;
                        case 10:brush=QBrush(Qt::cyan);
                                break;
                        case 11:brush=QBrush(Qt::darkCyan);
                                break;
                        case 12:brush=QBrush(Qt::gray);
                                break;
                        case 13:brush=QBrush(Qt::darkGray);
                                break;
                        case 14:brush=QBrush(Qt::lightGray);
                                break;
                }

                QPen pen;
                pen.setWidth(lineWidth);
                pen.setBrush(brush);
                cData->setPen(pen);
                //cData->setRawData(d_x, d_y, NDataSoll);

                traceName=QObject::tr("Trace %1").arg(j);

}

QPL_XYPlotTrace::~QPL_XYPlotTrace(){
//Plotting_Scope_Data_Thread->stopThread();
//Plotting_Scope_Data_Thread->wait();
//delete Plotting_Scope_Data_Thread;
//delete mY;
  delete[] d_x;
  delete[] d_y;
  delete cData;

}




   void QPL_XYPlotTrace::changeNDataSoll(int ds)
{
        NDataSoll=(int)ds;

    for (unsigned int i = 0; i< NDataSoll; i++)
    {
        //if (Scope->dt<=0.05)
        d_x[i] =0;     // time axis
            d_y[i] = 0;
    }
        cData->setRawData(d_x, d_y, NDataSoll);
}

   void QPL_XYPlotTrace::show(bool v){

        visible=v;
        if (!visible) {
                cData->setStyle(QwtPlotCurve::NoCurve);
                sym.setStyle(QwtSymbol::NoSymbol); cData->setSymbol(sym);

        }else{
                cData->setStyle(oldStyle);
                setSymbolStyle(oldSymbol);
        }
}

   void QPL_XYPlotTrace::setColor(const QColor& c)
{
                QPen pen;
                pen.setBrush(c);
                brush=QBrush(c);
                pen.setWidth(lineWidth);
                cData->setPen(pen);
               // zeroAxis.setLinePen(QPen(brush,2.,Qt::DashDotLine));
               // traceLabel.setLinePen(QPen(brush,2.,Qt::DashDotLine));
                QwtText ttext(traceName);
                ttext.setColor(QColor(c));
               // traceLabel.setLabel(ttext);
}

void QPL_XYPlotTrace::setWidth(int traceWidth)
{
                QPen pen;
                pen.setBrush(brush);
                pen.setWidth(traceWidth);
                lineWidth=traceWidth;
                cData->setPen(pen);
}

int  QPL_XYPlotTrace::getWidth()
{
                return cData->pen().width();
}

   void QPL_XYPlotTrace::setDx(double d){

                 for (unsigned int i = 0; i< NDataSoll; i++)
                  {
                        d_x[i]=(((d_x[i])*dx)/d);

                    }
                dx=d;
            //    zeroAxis.setYValue(offset);


}

double  QPL_XYPlotTrace::getDx()
{
                return dx;
}

   void QPL_XYPlotTrace::setDy(double d){

                 for (unsigned int i = 0; i< NDataSoll; i++)
                  {
                        d_y[i]=(((d_y[i])*dy)/d);

                    }
                dy=d;
               // zeroAxis.setYValue(offset);


}

double  QPL_XYPlotTrace::getDy()
{
                return dy;
}



void QPL_XYPlotTrace::setName(const QString &text){


        traceName=QString(text);

//        QwtText ttext(traceName);
//        ttext.setColor(QColor(brush.color()));
//        traceLabel.setLabel(ttext);


}


   void QPL_XYPlotTrace::setValue(int index, double x, double y){


   d_x[index]=(x)/getDx();
  d_y[index]=(y)/getDy();

   }

   void QPL_XYPlotTrace::moveDataToRight(int time){


                for (unsigned int i = NDataSoll - 1; i > time; i-- ){
                        d_x[i] = d_x[i-1-time];
                        d_y[i] = d_y[i-1-time];
                }


   }
   void QPL_XYPlotTrace::moveDataToLeft(int time){

                for (unsigned int i = 0; i < NDataSoll-time-1; i++ ){
                if ((i+time+1)<NDataSoll){
                        d_x[i] = d_x[i+time+1];
                        d_y[i] = d_y[i+time+1];
                    }
                }
   }

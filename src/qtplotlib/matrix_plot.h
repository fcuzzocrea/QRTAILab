#ifndef MATRIX_PLOT_H
#define MATRIX_PLOT_H

#include "qtplot_global.h"
#include <QtGui>
#include "matrix_view.h"

class  QPL_MatrixPlot : public QTableView
{
   Q_OBJECT

public:
     enum matrixDelegate {pixel,blackwhite,colorbar,text};
   QPL_MatrixPlot(QWidget *parent = 0);
   virtual ~QPL_MatrixPlot();
   void setValue(const QVector< QVector<float> > &v);
     void setMinScale(double min);
     void setMaxScale(double max);
     double getMinScale(){return minScale;}
     double getMaxScale(){return maxScale;}
     void setDelegate(matrixDelegate d);
     matrixDelegate getDelegate(){return actualDelegate;}
     void setPixelSize(int psize);
     int  getPixelSize(){return pixelSize;}
     void  setShowItemNumber(bool n);
     bool  getShowItemNumber(){return showItemNumber;}
   private:
     MatrixModel *model;
      PixelDelegate *pixelView;
       BlackWhiteDelegate *blackwhiteView;
       ColorBarDelegate *colorView;
       matrixDelegate actualDelegate;
      double minScale;
     double maxScale;
      int pixelSize;
     bool showItemNumber;

   friend QDataStream& operator<<(QDataStream &out, const QPL_MatrixPlot &d);
  friend QDataStream& operator>>(QDataStream &in, QPL_MatrixPlot(&d));
};
        QDataStream& operator<<(QDataStream &out, const QPL_MatrixPlot &d);
        QDataStream& operator>>(QDataStream &in, QPL_MatrixPlot(&d));





#endif // MATRIX_PLOT_H

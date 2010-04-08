#ifndef QTPLOT_GLOBAL_H
#define QTPLOT_GLOBAL_H

#include <QtCore/qglobal.h>

#include <QtGui>

#include <qwt_plot.h>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_math.h>

#define MAX_SCOPE_DATA		100000


#if defined(QTPLOT_LIBRARY)
#  define QTPLOTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTPLOTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTPLOT_GLOBAL_H

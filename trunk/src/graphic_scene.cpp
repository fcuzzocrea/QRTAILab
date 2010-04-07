#include "graphic_scene.h"




#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


QRL_OpenGLScene::QRL_OpenGLScene()
//:m_backgroundColor(0, 170, 255)
  :m_backgroundColor(Qt::gray)
{



//    QWidget *qwt = createDialog(tr("qwt"));
//
//    //DataPlot *plot = new DataPlot();
//    qwt->layout()->addWidget(plot);
//   // plot->setTimerInterval(20);
//
//  QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(0, Qt::Dialog);
//  proxy->setWidget(qwt);
//  addItem(proxy);
//
//
//
//    QPointF pos(10, 10);
//    foreach (QGraphicsItem *item, items()) {
//        item->setFlag(QGraphicsItem::ItemIsMovable);
//        //item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
//        item->setCacheMode(QGraphicsItem::ItemCoordinateCache);
//
//        const QRectF rect = item->boundingRect();
//        item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
//        pos += QPointF(0, 10 + rect.height());
//    }
//    m_time.start();


      scopeTimer = new QTimer(this);
         connect(scopeTimer, SIGNAL(timeout()), this, SLOT(update()));
         #ifdef _use_opengl_
          scopeTimer->start((int)(1./20*1000.));
         #endif
}

  void QRL_OpenGLScene::addDialog(QDialog * newDialog){

QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(0, Qt::Dialog);
  proxy->setWidget(newDialog);
  addItem(proxy);

    QPointF pos(10, 10);
    foreach (QGraphicsItem *item, items()) {
        item->setFlag(QGraphicsItem::ItemIsMovable);
        //item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        item->setCacheMode(QGraphicsItem::ItemCoordinateCache);

//        const QRectF rect = item->boundingRect();
//        item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
//        pos += QPointF(0, 10 + rect.height());
    }
    m_time.start();


  }

void QRL_OpenGLScene::drawBackground(QPainter *painter, const QRectF &)
{
    #ifdef _use_opengl_
    if (painter->paintEngine()->type() != QPaintEngine::OpenGL)
    //    && painter->paintEngine()->type() != QPaintEngine::OpenGL2)
    {
        qWarning("OpenGLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
        return;
    }
#endif
    //painter->beginNativePainting();
#ifdef _use_opengl_
    glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
   // painter->endNativePainting();

   // QTimer::singleShot(20, this, SLOT(update()));
}


void QRL_OpenGLScene::setBackgroundColor()
{
    const QColor color = QColorDialog::getColor(m_backgroundColor);
    if (color.isValid()) {
        m_backgroundColor = color;
        update();
    }
}


void QRL_OpenGLScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted())
        return;
    if (event->buttons() & Qt::LeftButton) {

    }
}

void QRL_OpenGLScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted())
        return;


    event->accept();
}

void QRL_OpenGLScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->isAccepted())
        return;


    event->accept();
    update();
}

void QRL_OpenGLScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsScene::wheelEvent(event);
    if (event->isAccepted())
        return;

    event->accept();
    update();
}


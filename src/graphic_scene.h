#ifndef GRAPHIC_SCENE_H
#define GRAPHIC_SCENE_H

#include <QtGui>
#ifdef _use_opengl_
#include <QtOpenGL>
#endif
#ifndef QT_NO_CONCURRENT
#include <QFutureWatcher>
#endif

class Model;
class QRL_OpenGLScene : public QGraphicsScene
{
    Q_OBJECT

public:
    QRL_OpenGLScene();

    void drawBackground(QPainter *painter, const QRectF &rect);
    void addDialog(QDialog *);
public slots:
    void setBackgroundColor();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent * wheelEvent);

private:
   //QDialog *createDialog(const QString &windowTitle) const;

    QColor m_backgroundColor;
    QTime m_time;
    int m_lastTime;
    int m_mouseEventTime;

   QTimer *scopeTimer;


#ifndef QT_NO_CONCURRENT
    QFutureWatcher<Model *> m_modelLoader;
#endif
};


#endif // GRAPHIC_SCENE_H

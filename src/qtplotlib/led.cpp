#include "led.h"


QPL_Led::QPL_Led(QWidget *parent)
    : QWidget(parent)
{
   m_value=false;
   m_color=Qt::red;
   setMinimumSize(QSize(20,20));
   //setFocusPolicy(Qt::StrongFocus);

}

void QPL_Led::paintEvent(QPaintEvent *)
{


        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setWindow( -50,-50,100,100);

        painter.setPen(Qt::white);
        painter.drawArc(-25,-25,50,50,0,5670);
        painter.drawArc(-32,-33,66,66,0,5670);
        painter.setPen(Qt::darkGray);
        painter.drawArc(-34,-33,66,66,3400,3000);

    if(m_value)
    {
      QRadialGradient radialGrad(QPointF(-8, -8), 20);
      radialGrad.setColorAt(0, Qt::white);

      radialGrad.setColorAt(1, m_color);
          QBrush brush(radialGrad);
      painter.setBrush(brush);
      painter.setPen(Qt::black);
          painter.drawEllipse(-25,-25,50,50);
    }
    else
    {
      QRadialGradient radialGrad(QPointF(-8, -8), 20);
      radialGrad.setColorAt(0, Qt::white);
      radialGrad.setColorAt(1, Qt::lightGray);
          QBrush brush(radialGrad);
      painter.setBrush(brush);
          painter.drawEllipse(-25,-25,50,50);
    }

}

void QPL_Led::setColor(QColor newColor)
{
   m_color=newColor;
   update();
}


void QPL_Led::setValue(bool value)
{
   m_value=value;
   update();
}


void QPL_Led::toggleValue()
{
        m_value=!m_value;
        update();
}

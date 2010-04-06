#ifndef LED_H
#define LED_H
#include "qtplot_global.h"
#include <QtGui>

class QColor;

/**
 * @brief QLed
 */
class  QPL_Led : public QWidget
{
 Q_OBJECT
        Q_PROPERTY(bool value READ value WRITE setValue);
        Q_PROPERTY(QColor color READ color WRITE setColor);

public:
    QPL_Led(QWidget *parent = 0);
    bool value() const { return m_value; }
    QColor color() const { return m_color; }
public slots:
        void setValue(bool);
        void setColor(QColor);
        void toggleValue();


protected:
    bool m_value;
    QColor m_color;
    void paintEvent(QPaintEvent *event);

};


#endif // LED_H

#include "dial_qwt.h"

QPL_DialQwt::QPL_DialQwt(QWidget *parent)
        :QwtDial(parent)
{
    // this->setObjectName(QString::fromUtf8("Thermo"));
    //this->setGeometry(QRect(50, 20, 52, 261));


        Min=-1.;
        Max=1.;

        //Dial->setObjectName(QString::fromUtf8("Dial"));
        //Dial->setGeometry(QRect(50, 20, 52, 50));
        this->setScaleArc(40.,320.);
        this->setRange(Min,Max);
        needle = new QwtDialSimpleNeedle(
        QwtDialSimpleNeedle::Arrow, true, Qt::red,
        QColor(Qt::gray).light(130));
        this->setNeedle(needle);
        this->scaleDraw()->setSpacing(2);
        //Dial->scaleDraw()->setRadius(5);
        //printf("radius %d\n",Dial->scaleDraw()->radius());
        this->setWrapping(false);
        this->setReadOnly(true);
        this->setScaleTicks(0, 4, 8);
        this->setScale(5,10);
        //Dial->setScale(1, 2, 0.25);
        this->setScaleOptions(QwtDial::ScaleTicks | QwtDial::ScaleLabel);
        this->setFrameShadow(QwtDial::Sunken);
        this->scaleDraw()->setPenWidth(2);
        //Dial->setLineWidth(1);
        this->setVisible(false);
    }

QPL_DialQwt::~QPL_DialQwt()
{

}

void QPL_DialQwt::setMax(double max)
{
        Max=max;
        this->setRange(Min,Max);
}


void QPL_DialQwt::setMin(double min)
{
        Min=min;
       this->setRange(Min,Max);
}

   void QPL_DialQwt::setNeedleColor(const QColor& c){
        QPalette p(c);
        // p.setColor(QPalette::Base,c); //knob
        needle->setPalette(p);
}


   QDataStream& operator<<(QDataStream &out, const QPL_DialQwt &d){


        out << (d.needle->palette().button().color());
        return out;
}


QDataStream& operator>>(QDataStream &in, QPL_DialQwt(&d)){
         QColor c;


        in >> c; d.setNeedleColor(c);


        return in;
}
/********************************************************************************
** Form generated from reading ui file 'qrl_scopes_manager.ui'
**
** Created: Sun Jan 6 12:42:45 2008
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QRL_SCOPES_MANAGER_H
#define UI_QRL_SCOPES_MANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "qwt_counter.h"
#include "qwt_wheel.h"

class Ui_QRL_ScopesManager
{
public:
    QGridLayout *gridLayout;
    QListWidget *scopeListWidget;
    QTabWidget *tabWidget;
    QWidget *Scope;
    QCheckBox *showCheckBox;
    QComboBox *optionComboBox;
    QComboBox *displayComboBox;
    QComboBox *directionComboBox;
    QCheckBox *oneshotCheckBox;
    QPushButton *triggerPushButton;
    QwtCounter *triggerCounter;
    QComboBox *traceComboBox_2;
    QLabel *label_2;
    QLineEdit *fileLineEdit;
    QwtCounter *timeCounter;
    QPushButton *savePushButton;
    QLabel *label;
    QwtCounter *dataCounter;
    QLabel *maxLabel;
    QwtCounter *rrCounter;
    QLabel *minLabel;
    QLabel *maxLabel_2;
    QComboBox *dxComboBox;
    QWidget *Trace;
    QPushButton *traceColorPushButton;
    QwtCounter *lineWidthCounter;
    QwtCounter *dyCounter;
    QwtCounter *offsetCounter;
    QLabel *widthLabel;
    QLabel *offsetLabel;
    QLabel *dyLabel;
    QComboBox *traceComboBox;
    QwtWheel *offsetWheel;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *helpButton;
    QSpacerItem *spacerItem1;
    QPushButton *closeButton;
    QSpacerItem *spacerItem2;

    void setupUi(QDialog *QRL_ScopesManager)
    {
    if (QRL_ScopesManager->objectName().isEmpty())
        QRL_ScopesManager->setObjectName(QString::fromUtf8("QRL_ScopesManager"));
    QRL_ScopesManager->resize(544, 356);
    gridLayout = new QGridLayout(QRL_ScopesManager);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    scopeListWidget = new QListWidget(QRL_ScopesManager);
    scopeListWidget->setObjectName(QString::fromUtf8("scopeListWidget"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(scopeListWidget->sizePolicy().hasHeightForWidth());
    scopeListWidget->setSizePolicy(sizePolicy);
    scopeListWidget->setBaseSize(QSize(100, 0));

    gridLayout->addWidget(scopeListWidget, 0, 0, 2, 1);

    tabWidget = new QTabWidget(QRL_ScopesManager);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    Scope = new QWidget();
    Scope->setObjectName(QString::fromUtf8("Scope"));
    showCheckBox = new QCheckBox(Scope);
    showCheckBox->setObjectName(QString::fromUtf8("showCheckBox"));
    showCheckBox->setGeometry(QRect(9, 9, 105, 22));
    optionComboBox = new QComboBox(Scope);
    optionComboBox->setObjectName(QString::fromUtf8("optionComboBox"));
    optionComboBox->setGeometry(QRect(10, 180, 115, 22));
    displayComboBox = new QComboBox(Scope);
    displayComboBox->setObjectName(QString::fromUtf8("displayComboBox"));
    displayComboBox->setGeometry(QRect(10, 240, 150, 22));
    directionComboBox = new QComboBox(Scope);
    directionComboBox->setObjectName(QString::fromUtf8("directionComboBox"));
    directionComboBox->setGeometry(QRect(10, 210, 105, 22));
    oneshotCheckBox = new QCheckBox(Scope);
    oneshotCheckBox->setObjectName(QString::fromUtf8("oneshotCheckBox"));
    oneshotCheckBox->setGeometry(QRect(10, 30, 105, 22));
    triggerPushButton = new QPushButton(Scope);
    triggerPushButton->setObjectName(QString::fromUtf8("triggerPushButton"));
    triggerPushButton->setGeometry(QRect(10, 50, 105, 27));
    triggerCounter = new QwtCounter(Scope);
    triggerCounter->setObjectName(QString::fromUtf8("triggerCounter"));
    triggerCounter->setGeometry(QRect(10, 80, 31, 23));
    triggerCounter->setNumButtons(0);
    triggerCounter->setProperty("basicstep", QVariant(0.1));
    triggerCounter->setMinValue(-1e+06);
    triggerCounter->setMaxValue(1e+06);
    traceComboBox_2 = new QComboBox(Scope);
    traceComboBox_2->setObjectName(QString::fromUtf8("traceComboBox_2"));
    traceComboBox_2->setGeometry(QRect(50, 80, 72, 22));
    label_2 = new QLabel(Scope);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(190, 150, 55, 21));
    fileLineEdit = new QLineEdit(Scope);
    fileLineEdit->setObjectName(QString::fromUtf8("fileLineEdit"));
    fileLineEdit->setGeometry(QRect(170, 170, 75, 23));
    timeCounter = new QwtCounter(Scope);
    timeCounter->setObjectName(QString::fromUtf8("timeCounter"));
    timeCounter->setGeometry(QRect(170, 210, 75, 23));
    timeCounter->setNumButtons(0);
    timeCounter->setMinValue(0);
    timeCounter->setMaxValue(1e+06);
    timeCounter->setValue(1);
    savePushButton = new QPushButton(Scope);
    savePushButton->setObjectName(QString::fromUtf8("savePushButton"));
    savePushButton->setGeometry(QRect(170, 240, 75, 27));
    savePushButton->setCheckable(true);
    savePushButton->setAutoDefault(false);
    label = new QLabel(Scope);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(190, 190, 51, 21));
    dataCounter = new QwtCounter(Scope);
    dataCounter->setObjectName(QString::fromUtf8("dataCounter"));
    dataCounter->setGeometry(QRect(10, 150, 105, 23));
    dataCounter->setNumButtons(1);
    dataCounter->setProperty("basicstep", QVariant(1));
    dataCounter->setMinValue(1);
    dataCounter->setMaxValue(10000);
    dataCounter->setStepButton1(100);
    dataCounter->setStepButton2(1000);
    dataCounter->setStepButton3(10000);
    dataCounter->setValue(100);
    maxLabel = new QLabel(Scope);
    maxLabel->setObjectName(QString::fromUtf8("maxLabel"));
    maxLabel->setGeometry(QRect(20, 120, 82, 23));
    rrCounter = new QwtCounter(Scope);
    rrCounter->setObjectName(QString::fromUtf8("rrCounter"));
    rrCounter->setGeometry(QRect(160, 90, 81, 23));
    rrCounter->setNumButtons(1);
    rrCounter->setProperty("basicstep", QVariant(0.1));
    rrCounter->setMinValue(0.01);
    rrCounter->setMaxValue(100);
    rrCounter->setStepButton1(1);
    rrCounter->setValue(20);
    minLabel = new QLabel(Scope);
    minLabel->setObjectName(QString::fromUtf8("minLabel"));
    minLabel->setGeometry(QRect(160, 60, 82, 42));
    maxLabel_2 = new QLabel(Scope);
    maxLabel_2->setObjectName(QString::fromUtf8("maxLabel_2"));
    maxLabel_2->setGeometry(QRect(180, 10, 55, 22));
    dxComboBox = new QComboBox(Scope);
    dxComboBox->setObjectName(QString::fromUtf8("dxComboBox"));
    dxComboBox->setGeometry(QRect(170, 40, 61, 22));
    dxComboBox->setEditable(true);
    dxComboBox->setMinimumContentsLength(1);
    tabWidget->addTab(Scope, QString());
    Trace = new QWidget();
    Trace->setObjectName(QString::fromUtf8("Trace"));
    traceColorPushButton = new QPushButton(Trace);
    traceColorPushButton->setObjectName(QString::fromUtf8("traceColorPushButton"));
    traceColorPushButton->setGeometry(QRect(180, 20, 83, 27));
    lineWidthCounter = new QwtCounter(Trace);
    lineWidthCounter->setObjectName(QString::fromUtf8("lineWidthCounter"));
    lineWidthCounter->setGeometry(QRect(180, 100, 74, 23));
    lineWidthCounter->setNumButtons(0);
    lineWidthCounter->setProperty("basicstep", QVariant(1));
    lineWidthCounter->setMinValue(1);
    lineWidthCounter->setMaxValue(50);
    dyCounter = new QwtCounter(Trace);
    dyCounter->setObjectName(QString::fromUtf8("dyCounter"));
    dyCounter->setGeometry(QRect(180, 170, 74, 23));
    dyCounter->setNumButtons(0);
    dyCounter->setProperty("basicstep", QVariant(0.0001));
    dyCounter->setMinValue(1e-06);
    dyCounter->setMaxValue(1e+06);
    dyCounter->setValue(1);
    offsetCounter = new QwtCounter(Trace);
    offsetCounter->setObjectName(QString::fromUtf8("offsetCounter"));
    offsetCounter->setGeometry(QRect(30, 150, 74, 23));
    offsetCounter->setNumButtons(0);
    offsetCounter->setProperty("basicstep", QVariant(0.1));
    offsetCounter->setMinValue(-1e+06);
    offsetCounter->setMaxValue(1e+06);
    widthLabel = new QLabel(Trace);
    widthLabel->setObjectName(QString::fromUtf8("widthLabel"));
    widthLabel->setGeometry(QRect(180, 80, 64, 18));
    offsetLabel = new QLabel(Trace);
    offsetLabel->setObjectName(QString::fromUtf8("offsetLabel"));
    offsetLabel->setGeometry(QRect(30, 130, 64, 18));
    dyLabel = new QLabel(Trace);
    dyLabel->setObjectName(QString::fromUtf8("dyLabel"));
    dyLabel->setGeometry(QRect(180, 150, 64, 18));
    traceComboBox = new QComboBox(Trace);
    traceComboBox->setObjectName(QString::fromUtf8("traceComboBox"));
    traceComboBox->setGeometry(QRect(20, 20, 72, 22));
    offsetWheel = new QwtWheel(Trace);
    offsetWheel->setObjectName(QString::fromUtf8("offsetWheel"));
    offsetWheel->setGeometry(QRect(30, 190, 64, 24));
    tabWidget->addTab(Trace, QString());

    gridLayout->addWidget(tabWidget, 0, 1, 1, 1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    helpButton = new QPushButton(QRL_ScopesManager);
    helpButton->setObjectName(QString::fromUtf8("helpButton"));

    hboxLayout->addWidget(helpButton);

    spacerItem1 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem1);

    closeButton = new QPushButton(QRL_ScopesManager);
    closeButton->setObjectName(QString::fromUtf8("closeButton"));

    hboxLayout->addWidget(closeButton);

    spacerItem2 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem2);


    gridLayout->addLayout(hboxLayout, 1, 1, 1, 1);


    retranslateUi(QRL_ScopesManager);
    QObject::connect(closeButton, SIGNAL(clicked()), QRL_ScopesManager, SLOT(hide()));

    tabWidget->setCurrentIndex(0);
    dxComboBox->setCurrentIndex(1);


    QMetaObject::connectSlotsByName(QRL_ScopesManager);
    } // setupUi

    void retranslateUi(QDialog *QRL_ScopesManager)
    {
    QRL_ScopesManager->setWindowTitle(QApplication::translate("QRL_ScopesManager", "Scopes Manager", 0, QApplication::UnicodeUTF8));
    showCheckBox->setText(QApplication::translate("QRL_ScopesManager", "Show/Hide", 0, QApplication::UnicodeUTF8));
    optionComboBox->clear();
    optionComboBox->insertItems(0, QStringList()
     << QApplication::translate("QRL_ScopesManager", "Show Grid", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QRL_ScopesManager", "set BG Color", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QRL_ScopesManager", "set Grid Color", 0, QApplication::UnicodeUTF8)
    );
    displayComboBox->clear();
    displayComboBox->insertItems(0, QStringList()
     << QApplication::translate("QRL_ScopesManager", "Continuous Roling", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QRL_ScopesManager", "Overwrite", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QRL_ScopesManager", "Trigger +to-", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QRL_ScopesManager", "Trigger -to+", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QRL_ScopesManager", "hold", 0, QApplication::UnicodeUTF8)
    );
    directionComboBox->clear();
    directionComboBox->insertItems(0, QStringList()
     << QApplication::translate("QRL_ScopesManager", "right to left", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QRL_ScopesManager", "left to right", 0, QApplication::UnicodeUTF8)
    );
    oneshotCheckBox->setText(QApplication::translate("QRL_ScopesManager", "One Shot", 0, QApplication::UnicodeUTF8));
    triggerPushButton->setText(QApplication::translate("QRL_ScopesManager", "Trigger", 0, QApplication::UnicodeUTF8));
    traceComboBox_2->clear();
    traceComboBox_2->insertItems(0, QStringList()
     << QApplication::translate("QRL_ScopesManager", "Trace 1", 0, QApplication::UnicodeUTF8)
    );
    label_2->setText(QApplication::translate("QRL_ScopesManager", "Filename", 0, QApplication::UnicodeUTF8));
    savePushButton->setText(QApplication::translate("QRL_ScopesManager", "Save", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("QRL_ScopesManager", "Time [s]", 0, QApplication::UnicodeUTF8));
    maxLabel->setText(QApplication::translate("QRL_ScopesManager", "Data points", 0, QApplication::UnicodeUTF8));
    minLabel->setText(QApplication::translate("QRL_ScopesManager", "Refresh rate", 0, QApplication::UnicodeUTF8));
    maxLabel_2->setText(QApplication::translate("QRL_ScopesManager", "sec/div", 0, QApplication::UnicodeUTF8));
    dxComboBox->clear();
    dxComboBox->insertItems(0, QStringList()
     << QApplication::translate("QRL_ScopesManager", "10", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QRL_ScopesManager", "1", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QRL_ScopesManager", "0.1", 0, QApplication::UnicodeUTF8)
    );
    tabWidget->setTabText(tabWidget->indexOf(Scope), QApplication::translate("QRL_ScopesManager", "Scope", 0, QApplication::UnicodeUTF8));
    traceColorPushButton->setText(QApplication::translate("QRL_ScopesManager", "Color", 0, QApplication::UnicodeUTF8));
    widthLabel->setText(QApplication::translate("QRL_ScopesManager", "Line Width", 0, QApplication::UnicodeUTF8));
    offsetLabel->setText(QApplication::translate("QRL_ScopesManager", "offset", 0, QApplication::UnicodeUTF8));
    dyLabel->setText(QApplication::translate("QRL_ScopesManager", "dy", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(Trace), QApplication::translate("QRL_ScopesManager", "Trace", 0, QApplication::UnicodeUTF8));
    helpButton->setText(QApplication::translate("QRL_ScopesManager", "Help", 0, QApplication::UnicodeUTF8));
    closeButton->setText(QApplication::translate("QRL_ScopesManager", "Close Manager", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(QRL_ScopesManager);
    } // retranslateUi

};

namespace Ui {
    class QRL_ScopesManager: public Ui_QRL_ScopesManager {};
} // namespace Ui

#endif // UI_QRL_SCOPES_MANAGER_H

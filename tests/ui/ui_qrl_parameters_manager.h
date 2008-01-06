/********************************************************************************
** Form generated from reading ui file 'qrl_parameters_manager.ui'
**
** Created: Sun Jan 6 12:42:45 2008
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QRL_PARAMETERS_MANAGER_H
#define UI_QRL_PARAMETERS_MANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

class Ui_QRL_ParametersManager
{
public:
    QGridLayout *gridLayout;
    QListWidget *blockListWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout1;
    QTableWidget *parameterTableWidget;
    QCheckBox *batchCheckBox;
    QPushButton *downloadPushButton;
    QPushButton *uploadPushButton;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *helpButton;
    QSpacerItem *spacerItem1;
    QPushButton *closeButton;
    QSpacerItem *spacerItem2;

    void setupUi(QDialog *QRL_ParametersManager)
    {
    if (QRL_ParametersManager->objectName().isEmpty())
        QRL_ParametersManager->setObjectName(QString::fromUtf8("QRL_ParametersManager"));
    QRL_ParametersManager->resize(479, 355);
    gridLayout = new QGridLayout(QRL_ParametersManager);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    blockListWidget = new QListWidget(QRL_ParametersManager);
    blockListWidget->setObjectName(QString::fromUtf8("blockListWidget"));
    blockListWidget->setBaseSize(QSize(100, 0));

    gridLayout->addWidget(blockListWidget, 0, 0, 3, 1);

    tabWidget = new QTabWidget(QRL_ParametersManager);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    gridLayout1 = new QGridLayout(tab);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    parameterTableWidget = new QTableWidget(tab);
    parameterTableWidget->setObjectName(QString::fromUtf8("parameterTableWidget"));

    gridLayout1->addWidget(parameterTableWidget, 0, 0, 1, 1);

    tabWidget->addTab(tab, QString());

    gridLayout->addWidget(tabWidget, 0, 1, 1, 3);

    batchCheckBox = new QCheckBox(QRL_ParametersManager);
    batchCheckBox->setObjectName(QString::fromUtf8("batchCheckBox"));

    gridLayout->addWidget(batchCheckBox, 1, 1, 1, 1);

    downloadPushButton = new QPushButton(QRL_ParametersManager);
    downloadPushButton->setObjectName(QString::fromUtf8("downloadPushButton"));
    downloadPushButton->setEnabled(false);

    gridLayout->addWidget(downloadPushButton, 1, 2, 1, 1);

    uploadPushButton = new QPushButton(QRL_ParametersManager);
    uploadPushButton->setObjectName(QString::fromUtf8("uploadPushButton"));
    uploadPushButton->setEnabled(true);

    gridLayout->addWidget(uploadPushButton, 1, 3, 1, 1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    helpButton = new QPushButton(QRL_ParametersManager);
    helpButton->setObjectName(QString::fromUtf8("helpButton"));

    hboxLayout->addWidget(helpButton);

    spacerItem1 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem1);

    closeButton = new QPushButton(QRL_ParametersManager);
    closeButton->setObjectName(QString::fromUtf8("closeButton"));

    hboxLayout->addWidget(closeButton);

    spacerItem2 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem2);


    gridLayout->addLayout(hboxLayout, 2, 1, 1, 3);


    retranslateUi(QRL_ParametersManager);
    QObject::connect(closeButton, SIGNAL(clicked()), QRL_ParametersManager, SLOT(hide()));

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(QRL_ParametersManager);
    } // setupUi

    void retranslateUi(QDialog *QRL_ParametersManager)
    {
    QRL_ParametersManager->setWindowTitle(QApplication::translate("QRL_ParametersManager", "Parameters Manager", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("QRL_ParametersManager", "Block Parameters", 0, QApplication::UnicodeUTF8));
    batchCheckBox->setText(QApplication::translate("QRL_ParametersManager", "batch mode", 0, QApplication::UnicodeUTF8));
    downloadPushButton->setText(QApplication::translate("QRL_ParametersManager", "download", 0, QApplication::UnicodeUTF8));
    uploadPushButton->setText(QApplication::translate("QRL_ParametersManager", "upload", 0, QApplication::UnicodeUTF8));
    helpButton->setText(QApplication::translate("QRL_ParametersManager", "Help", 0, QApplication::UnicodeUTF8));
    closeButton->setText(QApplication::translate("QRL_ParametersManager", "Close Manager", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(QRL_ParametersManager);
    } // retranslateUi

};

namespace Ui {
    class QRL_ParametersManager: public Ui_QRL_ParametersManager {};
} // namespace Ui

#endif // UI_QRL_PARAMETERS_MANAGER_H

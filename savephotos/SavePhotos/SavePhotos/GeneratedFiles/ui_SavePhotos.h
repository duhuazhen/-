/********************************************************************************
** Form generated from reading UI file 'SavePhotos.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEPHOTOS_H
#define UI_SAVEPHOTOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SavePhotosClass
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QPushButton *pushButton_3;
    QPushButton *pushButton;
    QPushButton *Btn_Cam2;
    QLabel *label_2;
    QLabel *C2;
    QPushButton *Btn_Cam1;
    QLabel *C1;
    QPushButton *pushButton_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SavePhotosClass)
    {
        if (SavePhotosClass->objectName().isEmpty())
            SavePhotosClass->setObjectName(QStringLiteral("SavePhotosClass"));
        SavePhotosClass->resize(1298, 871);
        centralWidget = new QWidget(SavePhotosClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(90, 610, 67, 17));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(320, 600, 151, 91));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(80, 560, 99, 27));
        Btn_Cam2 = new QPushButton(centralWidget);
        Btn_Cam2->setObjectName(QStringLiteral("Btn_Cam2"));
        Btn_Cam2->setGeometry(QRect(170, 660, 99, 27));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(90, 660, 67, 17));
        C2 = new QLabel(centralWidget);
        C2->setObjectName(QStringLiteral("C2"));
        C2->setGeometry(QRect(0, 0, 640, 480));
        C2->setStyleSheet(QStringLiteral("background-color: rgb(227, 201, 201);"));
        Btn_Cam1 = new QPushButton(centralWidget);
        Btn_Cam1->setObjectName(QStringLiteral("Btn_Cam1"));
        Btn_Cam1->setGeometry(QRect(170, 610, 99, 27));
        C1 = new QLabel(centralWidget);
        C1->setObjectName(QStringLiteral("C1"));
        C1->setGeometry(QRect(650, 0, 640, 480));
        C1->setStyleSheet(QStringLiteral("background-color: rgb(230, 199, 199);"));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(200, 560, 99, 27));
        SavePhotosClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SavePhotosClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1298, 23));
        SavePhotosClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SavePhotosClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SavePhotosClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SavePhotosClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SavePhotosClass->setStatusBar(statusBar);

        retranslateUi(SavePhotosClass);
        QObject::connect(pushButton, SIGNAL(clicked()), SavePhotosClass, SLOT(on_pushButton_clicked()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), SavePhotosClass, SLOT(on_pushButton_2_clicked()));
        QObject::connect(Btn_Cam1, SIGNAL(clicked()), SavePhotosClass, SLOT(on_Btn_Cam1_clicked()));
        QObject::connect(Btn_Cam2, SIGNAL(clicked()), SavePhotosClass, SLOT(on_Btn_Cam2_clicked()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), SavePhotosClass, SLOT(on_pushButton_3_clicked()));

        QMetaObject::connectSlotsByName(SavePhotosClass);
    } // setupUi

    void retranslateUi(QMainWindow *SavePhotosClass)
    {
        SavePhotosClass->setWindowTitle(QApplication::translate("SavePhotosClass", "SavePhotos", 0));
        label->setText(QApplication::translate("SavePhotosClass", "Camera1", 0));
        pushButton_3->setText(QApplication::translate("SavePhotosClass", "Save All", 0));
        pushButton->setText(QApplication::translate("SavePhotosClass", "Open ALL", 0));
        Btn_Cam2->setText(QApplication::translate("SavePhotosClass", "SavePhoto", 0));
        label_2->setText(QApplication::translate("SavePhotosClass", "Camera2", 0));
        C2->setText(QString());
        Btn_Cam1->setText(QApplication::translate("SavePhotosClass", "SavePhoto", 0));
        C1->setText(QString());
        pushButton_2->setText(QApplication::translate("SavePhotosClass", "Close ALL", 0));
    } // retranslateUi

};

namespace Ui {
    class SavePhotosClass: public Ui_SavePhotosClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEPHOTOS_H

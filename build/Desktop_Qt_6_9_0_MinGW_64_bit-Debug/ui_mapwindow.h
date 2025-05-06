/********************************************************************************
** Form generated from reading UI file 'mapwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPWINDOW_H
#define UI_MAPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_MapWindow
{
public:

    void setupUi(QDialog *MapWindow)
    {
        if (MapWindow->objectName().isEmpty())
            MapWindow->setObjectName("MapWindow");
        MapWindow->resize(800, 600);

        retranslateUi(MapWindow);

        QMetaObject::connectSlotsByName(MapWindow);
    } // setupUi

    void retranslateUi(QDialog *MapWindow)
    {
        MapWindow->setWindowTitle(QCoreApplication::translate("MapWindow", "Map Visualization", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MapWindow: public Ui_MapWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPWINDOW_H

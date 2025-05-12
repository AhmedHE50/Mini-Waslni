/********************************************************************************
** Form generated from reading UI file 'GraphMapWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHMAPWINDOW_H
#define UI_GRAPHMAPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GraphMapWindow
{
public:
    QVBoxLayout *verticalLayout;

    void setupUi(QDialog *GraphMapWindow)
    {
        if (GraphMapWindow->objectName().isEmpty())
            GraphMapWindow->setObjectName("GraphMapWindow");
        GraphMapWindow->resize(800, 600);
        verticalLayout = new QVBoxLayout(GraphMapWindow);
        verticalLayout->setObjectName("verticalLayout");

        retranslateUi(GraphMapWindow);

        QMetaObject::connectSlotsByName(GraphMapWindow);
    } // setupUi

    void retranslateUi(QDialog *GraphMapWindow)
    {
        GraphMapWindow->setWindowTitle(QCoreApplication::translate("GraphMapWindow", "Graph Visualization", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GraphMapWindow: public Ui_GraphMapWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHMAPWINDOW_H

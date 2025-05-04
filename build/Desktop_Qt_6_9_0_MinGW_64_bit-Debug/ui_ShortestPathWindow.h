/********************************************************************************
** Form generated from reading UI file 'ShortestPathWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHORTESTPATHWINDOW_H
#define UI_SHORTESTPATHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ShortestPathWindow
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *sourceCityComboBox;
    QLabel *label_2;
    QComboBox *destCityComboBox;
    QPushButton *btnFindPath;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *pathResultsTextEdit;

    void setupUi(QDialog *ShortestPathWindow)
    {
        if (ShortestPathWindow->objectName().isEmpty())
            ShortestPathWindow->setObjectName("ShortestPathWindow");
        ShortestPathWindow->resize(600, 500);
        verticalLayout = new QVBoxLayout(ShortestPathWindow);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(ShortestPathWindow);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        sourceCityComboBox = new QComboBox(groupBox);
        sourceCityComboBox->setObjectName("sourceCityComboBox");

        gridLayout->addWidget(sourceCityComboBox, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        destCityComboBox = new QComboBox(groupBox);
        destCityComboBox->setObjectName("destCityComboBox");

        gridLayout->addWidget(destCityComboBox, 1, 1, 1, 1);

        btnFindPath = new QPushButton(groupBox);
        btnFindPath->setObjectName("btnFindPath");

        gridLayout->addWidget(btnFindPath, 2, 1, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(ShortestPathWindow);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        pathResultsTextEdit = new QTextEdit(groupBox_2);
        pathResultsTextEdit->setObjectName("pathResultsTextEdit");
        pathResultsTextEdit->setReadOnly(true);

        verticalLayout_2->addWidget(pathResultsTextEdit);


        verticalLayout->addWidget(groupBox_2);


        retranslateUi(ShortestPathWindow);

        QMetaObject::connectSlotsByName(ShortestPathWindow);
    } // setupUi

    void retranslateUi(QDialog *ShortestPathWindow)
    {
        ShortestPathWindow->setWindowTitle(QCoreApplication::translate("ShortestPathWindow", "Shortest Path Finder", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ShortestPathWindow", "Find Shortest Path", nullptr));
        label->setText(QCoreApplication::translate("ShortestPathWindow", "Source City:", nullptr));
        label_2->setText(QCoreApplication::translate("ShortestPathWindow", "Destination City:", nullptr));
        btnFindPath->setText(QCoreApplication::translate("ShortestPathWindow", "Find Shortest Path", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("ShortestPathWindow", "Shortest Path Results", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShortestPathWindow: public Ui_ShortestPathWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHORTESTPATHWINDOW_H

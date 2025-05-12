/********************************************************************************
** Form generated from reading UI file 'GraphOperationsWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHOPERATIONSWINDOW_H
#define UI_GRAPHOPERATIONSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraphOperationsWindow
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *cityTab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *addCityGroup;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *cityNameLineEdit;
    QPushButton *btnAddCity;
    QGroupBox *deleteCityGroup;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QComboBox *cityListBox;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnDeleteCity;
    QSpacerItem *verticalSpacer;
    QWidget *roadTab;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *roadOperationsGroup;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QComboBox *fromCityComboBox;
    QLabel *label_4;
    QComboBox *toCityComboBox;
    QLabel *label_5;
    QLineEdit *distanceLineEdit;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnAddRoad;
    QPushButton *btnDeleteRoad;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnClose;

    void setupUi(QDialog *GraphOperationsWindow)
    {
        if (GraphOperationsWindow->objectName().isEmpty())
            GraphOperationsWindow->setObjectName("GraphOperationsWindow");
        GraphOperationsWindow->resize(600, 500);
        verticalLayout = new QVBoxLayout(GraphOperationsWindow);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(GraphOperationsWindow);
        tabWidget->setObjectName("tabWidget");
        cityTab = new QWidget();
        cityTab->setObjectName("cityTab");
        verticalLayout_2 = new QVBoxLayout(cityTab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        addCityGroup = new QGroupBox(cityTab);
        addCityGroup->setObjectName("addCityGroup");
        horizontalLayout = new QHBoxLayout(addCityGroup);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(addCityGroup);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        cityNameLineEdit = new QLineEdit(addCityGroup);
        cityNameLineEdit->setObjectName("cityNameLineEdit");

        horizontalLayout->addWidget(cityNameLineEdit);

        btnAddCity = new QPushButton(addCityGroup);
        btnAddCity->setObjectName("btnAddCity");

        horizontalLayout->addWidget(btnAddCity);


        verticalLayout_2->addWidget(addCityGroup);

        deleteCityGroup = new QGroupBox(cityTab);
        deleteCityGroup->setObjectName("deleteCityGroup");
        verticalLayout_3 = new QVBoxLayout(deleteCityGroup);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_2 = new QLabel(deleteCityGroup);
        label_2->setObjectName("label_2");

        verticalLayout_3->addWidget(label_2);

        cityListBox = new QComboBox(deleteCityGroup);
        cityListBox->setObjectName("cityListBox");

        verticalLayout_3->addWidget(cityListBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnDeleteCity = new QPushButton(deleteCityGroup);
        btnDeleteCity->setObjectName("btnDeleteCity");
        btnDeleteCity->setEnabled(false);

        horizontalLayout_2->addWidget(btnDeleteCity);


        verticalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(deleteCityGroup);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        tabWidget->addTab(cityTab, QString());
        roadTab = new QWidget();
        roadTab->setObjectName("roadTab");
        verticalLayout_4 = new QVBoxLayout(roadTab);
        verticalLayout_4->setObjectName("verticalLayout_4");
        roadOperationsGroup = new QGroupBox(roadTab);
        roadOperationsGroup->setObjectName("roadOperationsGroup");
        gridLayout = new QGridLayout(roadOperationsGroup);
        gridLayout->setObjectName("gridLayout");
        label_3 = new QLabel(roadOperationsGroup);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        fromCityComboBox = new QComboBox(roadOperationsGroup);
        fromCityComboBox->setObjectName("fromCityComboBox");

        gridLayout->addWidget(fromCityComboBox, 0, 1, 1, 1);

        label_4 = new QLabel(roadOperationsGroup);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        toCityComboBox = new QComboBox(roadOperationsGroup);
        toCityComboBox->setObjectName("toCityComboBox");

        gridLayout->addWidget(toCityComboBox, 1, 1, 1, 1);

        label_5 = new QLabel(roadOperationsGroup);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        distanceLineEdit = new QLineEdit(roadOperationsGroup);
        distanceLineEdit->setObjectName("distanceLineEdit");

        gridLayout->addWidget(distanceLineEdit, 2, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        btnAddRoad = new QPushButton(roadOperationsGroup);
        btnAddRoad->setObjectName("btnAddRoad");

        horizontalLayout_3->addWidget(btnAddRoad);

        btnDeleteRoad = new QPushButton(roadOperationsGroup);
        btnDeleteRoad->setObjectName("btnDeleteRoad");

        horizontalLayout_3->addWidget(btnDeleteRoad);


        gridLayout->addLayout(horizontalLayout_3, 3, 0, 1, 2);


        verticalLayout_4->addWidget(roadOperationsGroup);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        tabWidget->addTab(roadTab, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        btnClose = new QPushButton(GraphOperationsWindow);
        btnClose->setObjectName("btnClose");

        horizontalLayout_4->addWidget(btnClose);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(GraphOperationsWindow);
        QObject::connect(btnClose, &QPushButton::clicked, GraphOperationsWindow, qOverload<>(&QDialog::close));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(GraphOperationsWindow);
    } // setupUi

    void retranslateUi(QDialog *GraphOperationsWindow)
    {
        GraphOperationsWindow->setWindowTitle(QCoreApplication::translate("GraphOperationsWindow", "Graph Operations", nullptr));
        addCityGroup->setTitle(QCoreApplication::translate("GraphOperationsWindow", "Add City", nullptr));
        label->setText(QCoreApplication::translate("GraphOperationsWindow", "City Name:", nullptr));
        btnAddCity->setText(QCoreApplication::translate("GraphOperationsWindow", "Add City", nullptr));
        deleteCityGroup->setTitle(QCoreApplication::translate("GraphOperationsWindow", "Delete City", nullptr));
        label_2->setText(QCoreApplication::translate("GraphOperationsWindow", "Select City to Delete:", nullptr));
        btnDeleteCity->setText(QCoreApplication::translate("GraphOperationsWindow", "Delete City", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(cityTab), QCoreApplication::translate("GraphOperationsWindow", "City Operations", nullptr));
        roadOperationsGroup->setTitle(QCoreApplication::translate("GraphOperationsWindow", "Road Operations", nullptr));
        label_3->setText(QCoreApplication::translate("GraphOperationsWindow", "From City:", nullptr));
        label_4->setText(QCoreApplication::translate("GraphOperationsWindow", "To City:", nullptr));
        label_5->setText(QCoreApplication::translate("GraphOperationsWindow", "Distance:", nullptr));
        btnAddRoad->setText(QCoreApplication::translate("GraphOperationsWindow", "Add Road", nullptr));
        btnDeleteRoad->setText(QCoreApplication::translate("GraphOperationsWindow", "Delete Road", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(roadTab), QCoreApplication::translate("GraphOperationsWindow", "Road Operations", nullptr));
        btnClose->setText(QCoreApplication::translate("GraphOperationsWindow", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GraphOperationsWindow: public Ui_GraphOperationsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHOPERATIONSWINDOW_H

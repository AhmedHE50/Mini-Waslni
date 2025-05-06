/********************************************************************************
** Form generated from reading UI file 'citypositiondialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CITYPOSITIONDIALOG_H
#define UI_CITYPOSITIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CityPositionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QLabel *cityNameLabel;
    QFormLayout *formLayout;
    QLabel *xLabel;
    QDoubleSpinBox *xPositionSpinBox;
    QLabel *yLabel;
    QDoubleSpinBox *yPositionSpinBox;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CityPositionDialog)
    {
        if (CityPositionDialog->objectName().isEmpty())
            CityPositionDialog->setObjectName("CityPositionDialog");
        CityPositionDialog->resize(300, 170);
        verticalLayout = new QVBoxLayout(CityPositionDialog);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(CityPositionDialog);
        titleLabel->setObjectName("titleLabel");

        verticalLayout->addWidget(titleLabel);

        cityNameLabel = new QLabel(CityPositionDialog);
        cityNameLabel->setObjectName("cityNameLabel");
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        cityNameLabel->setFont(font);

        verticalLayout->addWidget(cityNameLabel);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        xLabel = new QLabel(CityPositionDialog);
        xLabel->setObjectName("xLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, xLabel);

        xPositionSpinBox = new QDoubleSpinBox(CityPositionDialog);
        xPositionSpinBox->setObjectName("xPositionSpinBox");
        xPositionSpinBox->setMinimum(-1.000000000000000);
        xPositionSpinBox->setMaximum(1.000000000000000);
        xPositionSpinBox->setSingleStep(0.050000000000000);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, xPositionSpinBox);

        yLabel = new QLabel(CityPositionDialog);
        yLabel->setObjectName("yLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, yLabel);

        yPositionSpinBox = new QDoubleSpinBox(CityPositionDialog);
        yPositionSpinBox->setObjectName("yPositionSpinBox");
        yPositionSpinBox->setMinimum(-1.000000000000000);
        yPositionSpinBox->setMaximum(1.000000000000000);
        yPositionSpinBox->setSingleStep(0.050000000000000);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, yPositionSpinBox);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(CityPositionDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(CityPositionDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, CityPositionDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, CityPositionDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(CityPositionDialog);
    } // setupUi

    void retranslateUi(QDialog *CityPositionDialog)
    {
        CityPositionDialog->setWindowTitle(QCoreApplication::translate("CityPositionDialog", "Set City Position", nullptr));
        titleLabel->setText(QCoreApplication::translate("CityPositionDialog", "Set Position for:", nullptr));
        cityNameLabel->setText(QCoreApplication::translate("CityPositionDialog", "City Name", nullptr));
        xLabel->setText(QCoreApplication::translate("CityPositionDialog", "X Position (-1.0 - 1.0):", nullptr));
        yLabel->setText(QCoreApplication::translate("CityPositionDialog", "Y Position (-1.0 - 1.0):", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CityPositionDialog: public Ui_CityPositionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CITYPOSITIONDIALOG_H

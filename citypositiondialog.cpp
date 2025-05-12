#include "CityPositionDialog.h"
#include "ui_citypositiondialog.h"

CityPositionDialog::CityPositionDialog(const QString& cityName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CityPositionDialog)
{
    ui->setupUi(this);
    setWindowTitle("Set City Position");

    // Set the city name in the dialog
    ui->cityNameLabel->setText(cityName);

    // Set default values (center of the map)
    ui->xPositionSpinBox->setValue(0.00);
    ui->yPositionSpinBox->setValue(0.00);
}

CityPositionDialog::~CityPositionDialog()
{
    delete ui;
}

double CityPositionDialog::getXPosition() const
{
    return ui->xPositionSpinBox->value();
}

double CityPositionDialog::getYPosition() const
{
    return ui->yPositionSpinBox->value();
}

void CityPositionDialog::setXPosition(double value)
{
    ui->xPositionSpinBox->setValue(value);
}

void CityPositionDialog::setYPosition(double value)
{
    ui->yPositionSpinBox->setValue(value);
}

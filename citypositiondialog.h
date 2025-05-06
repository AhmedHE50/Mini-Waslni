// This prevents the header file from being included multiple times in the same compilation unit, which can lead to errors
#ifndef CITYPOSITIONDIALOG_H
#define CITYPOSITIONDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class CityPositionDialog;
}

class CityPositionDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor and Destructor
    // explicit => Prevents implicit conversions
    explicit CityPositionDialog(const QString& cityName, QWidget *parent = nullptr);
    ~CityPositionDialog();

    // Getters and Setters
    double getXPosition() const;
    double getYPosition() const;
    void setXPosition(double value);
    void setYPosition(double value);

private:
    Ui::CityPositionDialog *ui;
};

#endif

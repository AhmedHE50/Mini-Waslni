#include "graphoperationswindow.h"
#include "ui_graphoperationswindow.h"
#include <QMessageBox>

GraphOperationsWindow::GraphOperationsWindow(Graph* graph, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphOperationsWindow),
    graph(graph)
{
    ui->setupUi(this);
    setWindowTitle("Manage Cities and Roads");

    // Initialize city list model
    cityListModel = new QStringListModel(this);

    // Update city list
    updateCityList();

    // Setup combo boxes
    ui->fromCityComboBox->setModel(cityListModel);
    ui->toCityComboBox->setModel(cityListModel);
    ui->cityListBox->setModel(cityListModel);

    setStyleSheet(R"(
        QDialog {
            background-color: #f0f0f0;
        }
        QLabel {
            font-size: 12px;
            color: #333;
        }
        QMessageBox {
            background-color: #fff;
            border: 1px solid #aaa;
            font-size = 12px;
        }
        QMessageBox QPushButton {
            background-color: #555;
            color: white;
            border: none;
            padding: 5px 10px;
            border-radius: 8px;
        }
        QMessageBox QPushButton:hover {
            background-color: #777;
        }
    )");
}

GraphOperationsWindow::~GraphOperationsWindow()
{
    delete ui;
}

void GraphOperationsWindow::updateCityList()
{
    cityList.clear();
    const CityGraph& cities = graph->getAllCities();

    for (auto it = cities.begin(); it != cities.end(); ++it) {
        cityList.append(QString::fromStdString(it->first));
    }

    cityList.sort();
    cityListModel->setStringList(cityList);
}

void GraphOperationsWindow::refreshCityList()
{
    updateCityList();
}

void GraphOperationsWindow::on_btnAddCity_clicked()
{
    QString cityName = ui->cityNameLineEdit->text().trimmed();

    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a city name.");
        return;
    }

    // Add city to graph
    graph->addCity(cityName.toStdString());

    // Update city list
    updateCityList();

    // Clear input field
    ui->cityNameLineEdit->clear();

    // Show success message
    QMessageBox::information(this, "Success", "City '" + cityName + "' added successfully.");
}

void GraphOperationsWindow::on_btnDeleteCity_clicked()
{
    int index = ui->cityListBox->currentIndex();
    if (index < 0 || index >= cityList.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a city to delete.");
        return;
    }

    QString cityName = cityList.at(index);

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Deletion",
                                  "Are you sure you want to delete " + cityName + "?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Delete city from graph
        graph->deleteCity(cityName.toStdString());

        // Update city list
        updateCityList();

        // Show success message
        QMessageBox::information(this, "Success", "City '" + cityName + "' deleted successfully.");
    }
}

void GraphOperationsWindow::on_btnAddRoad_clicked()
{
    int fromIndex = ui->fromCityComboBox->currentIndex();
    int toIndex = ui->toCityComboBox->currentIndex();

    if (fromIndex < 0 || toIndex < 0 || fromIndex >= cityList.size() || toIndex >= cityList.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select both cities.");
        return;
    }

    if (fromIndex == toIndex) {
        QMessageBox::warning(this, "Input Error", "Cannot create a road from a city to itself.");
        return;
    }

    bool ok;
    double distance = ui->distanceLineEdit->text().toDouble(&ok);

    if (!ok || distance <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid positive distance.");
        return;
    }

    QString fromCity = cityList.at(fromIndex);
    QString toCity = cityList.at(toIndex);

    // Add road to graph
    graph->addRoad(fromCity.toStdString(), toCity.toStdString(), distance);

    // Clear input
    ui->distanceLineEdit->clear();

    // Show success message
    QMessageBox::information(this, "Success", "Road added successfully between " + fromCity + " and " + toCity + ".");
}

void GraphOperationsWindow::on_btnDeleteRoad_clicked()
{
    int fromIndex = ui->fromCityComboBox->currentIndex();
    int toIndex = ui->toCityComboBox->currentIndex();

    if (fromIndex < 0 || toIndex < 0 || fromIndex >= cityList.size() || toIndex >= cityList.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select both cities.");
        return;
    }

    if (fromIndex == toIndex) {
        QMessageBox::warning(this, "Input Error", "Cannot delete a road from a city to itself.");
        return;
    }

    QString fromCity = cityList.at(fromIndex);
    QString toCity = cityList.at(toIndex);

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Road Deletion",
                                  "Are you sure you want to delete the road between " +
                                      fromCity + " and " + toCity + "?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Delete road from graph
        graph->deleteRoad(fromCity.toStdString(), toCity.toStdString());

        // Show success message
        QMessageBox::information(this, "Success", "Road deleted successfully between " + fromCity + " and " + toCity + ".");
    }
}

void GraphOperationsWindow::on_cityListBox_currentIndexChanged(int index)
{
    // Enable/disable delete button based on selection
    ui->btnDeleteCity->setEnabled(index >= 0);
}

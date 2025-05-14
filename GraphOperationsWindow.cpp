#include "graphoperationswindow.h"
#include "MapVisualization.h"
#include "MapWindow.h"
#include "ui_graphoperationswindow.h"
#include <QMessageBox>
#include <QRegularExpression>

GraphOperationsWindow::GraphOperationsWindow(Graph* graph, MapWindow* mapWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphOperationsWindow),
    graph(graph),
    mapWindow(mapWindow)
{
    ui->setupUi(this);
    setWindowTitle("Graph Operations");

    // Initialize city list model
    cityListModel = new QStringListModel(this);

    // Update city list
    updateCityList();

    // Setup combo boxes and list view with the city list model
    ui->fromCityComboBox->setModel(cityListModel);
    ui->toCityComboBox->setModel(cityListModel);
    ui->cityListBox->setModel(cityListModel);

    //  Styling the window (similar to CSS)
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

    // Iterate through the cities and add them to the list
    for (auto it = cities.begin(); it != cities.end(); ++it) {
        cityList.append(QString::fromStdString(it->first)); // city names <==> keys
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
    QString cityName = ui->cityNameLineEdit->text().trimmed(); // Get city name from input, remove extra spaces

    // Check 1: Empty name
    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a city name.");
        return;
    }

    // Check 2: Invalid characters
    QRegularExpression validCityName("^[a-zA-Z0-9\\s\\-']+$");
    if (!validCityName.match(cityName).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "City name contains invalid characters. Please use only letters, numbers, spaces, hyphens, or apostrophes.");
        ui->cityNameLineEdit->clear();
        return;
    }

    // Check 3: City already exists
    if (graph->getAllCities().count(cityName.toStdString())) {
        QMessageBox::warning(this, "Input Error", "City '" + cityName + "' already exists.");
        ui->cityNameLineEdit->clear();
        return;
    }

    graph->addCity(cityName.toStdString());

    if (!mapWindow) {
        mapWindow = new MapWindow(graph, this->parentWidget());
    }

    // Ensure mapVisualization is accessible
    MapVisualization* mapVis = mapWindow->getMapVisualization();

    if (mapVis) {
        // Show position dialog to set the position of the new city
        mapVis->showCityPositionDialog(cityName);
    } else {
        QMessageBox::warning(this, "Visualization Error", "Could not access map visualization to set city position.");
    }

    // Update city list
    updateCityList();

    // Clear input field
    ui->cityNameLineEdit->clear();

    // Show success message
    QMessageBox::information(this, "Success", "City '" + cityName + "' added successfully.");
}

void GraphOperationsWindow::on_btnDeleteCity_clicked()
{
    int index = ui->cityListBox->currentIndex(); // Get the index of the selected city

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

        // Also remove the city position from map visualization if mapWindow exists
        if (mapWindow) {
            MapVisualization* mapVis = mapWindow->getMapVisualization();
            if(mapVis) {
                mapVis->removeCityPosition(cityName);
                mapWindow->updateMap(); // Refresh map display
            }
        }

        // Clear input
        ui->distanceLineEdit->clear();

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

    QString fromCity = cityList.at(fromIndex);
    QString toCity = cityList.at(toIndex);

    const CityGraph& cities = graph->getAllCities();

    auto fromCityIt = cities.find(fromCity.toStdString());

    bool roadExists = false;
    if (fromCityIt != cities.end()) {
        const list<pair<string, double>>& neighbors = fromCityIt->second;

        for (const auto& neighborPair : neighbors) {
            if (neighborPair.first == toCity.toStdString()) {
                roadExists = true;
                break;
            }
        }
    }

    if (roadExists) {
        QMessageBox::warning(this, "Input Error", "A road already exists between " + fromCity + " and " + toCity + ".");
        return;
    }

    bool ok;
    double distance = ui->distanceLineEdit->text().toDouble(&ok); // Get distance, convert to double

    if (!ok || distance <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid positive distance.");
        return;
    }

    // Add road to graph
    graph->addRoad(fromCity.toStdString(), toCity.toStdString(), distance);

    // Update map visualization if mapWindow exists
    if (mapWindow) {
        mapWindow->updateMap();
    }

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
        // Delete road from graph success
        if(graph->deleteRoad(fromCity.toStdString(), toCity.toStdString())){
            // Update map visualization if mapWindow exists
            if (mapWindow) {
                mapWindow->updateMap();
            }

            // Show success message
            QMessageBox::information(this, "Success", "Road deleted successfully between " + fromCity + " and " + toCity + ".");
        }
        else{
            // Show success message
            QMessageBox::information(this, "Failed", "There is no road exist between " + fromCity + " and " + toCity + ".");
        }

        // Clear input
        ui->distanceLineEdit->clear();
    }
}

void GraphOperationsWindow::on_cityListBox_currentIndexChanged(int index)
{
    // Enable/disable delete button based on selection
    ui->btnDeleteCity->setEnabled(index >= 0);
}

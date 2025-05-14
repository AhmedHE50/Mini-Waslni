#include "graphtraversalwindow.h"
#include "ui_graphtraversalwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>

GraphTraversalWindow::GraphTraversalWindow(Graph* graph, MapWindow* mapWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphTraversalWindow),
    graph(graph),
    mapWindow(mapWindow),
    currentTraversalStep(0)
{
    ui->setupUi(this);
    setWindowTitle("Graph Traversal");

    // Initialize city list model
    cityListModel = new QStringListModel(this);

    // Update city list
    updateCityList();
    updateVisualizationSpeedList();

    // Setup combo box
    ui->startCityComboBox->setModel(cityListModel);

    // Clear results area
    ui->traversalResultsTextEdit->clear();

    // Initialize traversal timer
    traversalTimer = new QTimer(this);
    connect(traversalTimer, &QTimer::timeout, this, &GraphTraversalWindow::showNextTraversalStep);

    setStyleSheet(R"(
        QDialog {
            background-color: #f0f0f0;
        }
        QTextEdit {
            border: 1px solid #aaa;
            font-family: monospace;
            font-size: 12px;
            color: #222;
        }
    )");
}

GraphTraversalWindow::~GraphTraversalWindow()
{
    delete ui;
}

void GraphTraversalWindow::setMapWindow(MapWindow* mapWindow)
{
    this->mapWindow = mapWindow;
}

void GraphTraversalWindow::updateCityList()
{
    cityList.clear();
    const CityGraph& cities = graph->getAllCities();

    for (auto it = cities.begin(); it != cities.end(); ++it) {
        cityList.append(QString::fromStdString(it->first));
    }

    cityList.sort();
    cityListModel->setStringList(cityList);
}

void GraphTraversalWindow::refreshCityList()
{
    updateCityList();
}

void GraphTraversalWindow::on_btnBFS_clicked() {
    int startIndex = ui->startCityComboBox->currentIndex();

    if (startIndex < 0 || startIndex >= cityList.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a starting city.");
        return;
    }

    QString startCity = cityList.at(startIndex);
    std::vector<std::string> bfsResult = graph->BFS(startCity.toStdString()); // Perform BFS
    displayTraversalResult(bfsResult); // Display the BFS result

    if (mapWindow) {
        if (!mapWindow->isVisible()) {
            mapWindow->show();
        }
        mapWindow->raise();
        mapWindow->activateWindow();
        startTraversalVisualization(bfsResult);
    } else {
        // Create a new map window if it doesn't exist
        mapWindow = new MapWindow(graph, this);
        mapWindow->show();

        // Update the reference and try again
        this->setMapWindow(mapWindow);

        mapWindow->raise();
        mapWindow->activateWindow();
        startTraversalVisualization(bfsResult);
    }
}

void GraphTraversalWindow::on_btnDFS_clicked() {
    int startIndex = ui->startCityComboBox->currentIndex();

    if (startIndex < 0 || startIndex >= cityList.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a starting city.");
        return;
    }

    QString startCity = cityList.at(startIndex);
    std::vector<std::string> dfsResult = graph->DFS(startCity.toStdString());
    displayTraversalResult(dfsResult);

    if (mapWindow) {
        if (!mapWindow->isVisible()) {
            mapWindow->show();
        }
        mapWindow->raise();
        mapWindow->activateWindow();
        startTraversalVisualization(dfsResult);
    } else {
        // Create a new map window if it doesn't exist
        mapWindow = new MapWindow(graph, this);
        mapWindow->show();

        // Update the reference and try again
        this->setMapWindow(mapWindow);

        mapWindow->raise();
        mapWindow->activateWindow();
        startTraversalVisualization(dfsResult);
    }
}

void GraphTraversalWindow::displayTraversalResult(const std::vector<std::string>& path)
{
    QString result = "Traversal Order:\n";

    if (path.empty() && !graph->getAllCities().empty()) {
        // Handle case where the start city might not exist or has no connections
        result = "Traversal did not start or reach any cities from the selected start city.\n";
    } else {
        for (size_t i = 0; i < path.size(); ++i) {
            result += QString::number(i + 1) + ". " + QString::fromStdString(path[i]);

            if (i < path.size() - 1) {
                result += "\n";
            }
        }
    }

    // Check for disconnected graph
    if (graph->getAllCities().size() > 0 && path.size() < graph->getAllCities().size()) {
        result += "\n\nNote: The graph might be disconnected. The traversal did not reach all cities.";
    } else if (graph->getAllCities().empty()) {
        result += "\n\nNote: The graph is empty.";
    }


    ui->traversalResultsTextEdit->setText(result);
}

void GraphTraversalWindow::startTraversalVisualization(const std::vector<std::string>& path) {
    currentTraversalPath = path;
    currentTraversalStep = 0;
    resetVisualization();

    // Calculate the interval based on the selected speed
    int speedIndex = ui->visualizationSpeedComboBox->currentIndex();
    int baseInterval = 500; // Default 1x speed is 500ms
    int interval = baseInterval / (speedIndex + 1);

    traversalTimer->setInterval(interval);
    traversalTimer->start();
    ui->btnBFS->setEnabled(false);
    ui->btnDFS->setEnabled(false);
}

void GraphTraversalWindow::resetVisualization()
{
    if (mapWindow) {
        // Clear any selected city in the map
        mapWindow->getMapVisualization()->setProperty("selectedCity", "");
        mapWindow->getMapVisualization()->setProperty("visitedCities", QStringList());
        mapWindow->getMapVisualization()->setProperty("currentPath", QStringList());
        mapWindow->getMapVisualization()->update();
    }
}

void GraphTraversalWindow::updateVisualizationSpeedList()
{
    QStringList speeds;
    speeds << "1x" << "2x" << "3x" << "4x" << "5x";
    ui->visualizationSpeedComboBox->clear();
    ui->visualizationSpeedComboBox->addItems(speeds);
    ui->visualizationSpeedComboBox->setCurrentIndex(0); // Default to 1x
}

void GraphTraversalWindow::showNextTraversalStep()
{
    // Check if the traversal is complete
    if (currentTraversalStep >= static_cast<int>(currentTraversalPath.size())) {
        // End of traversal
        traversalTimer->stop();

        // Re-enable buttons
        ui->btnBFS->setEnabled(true);
        ui->btnDFS->setEnabled(true);
        return;
    }

    highlightCurrentStep(); // Highlight the current city
    currentTraversalStep++; // Move to the next step

    // Update the text display
    QTextCursor cursor = ui->traversalResultsTextEdit->textCursor();
    cursor.setPosition(0);
    ui->traversalResultsTextEdit->setTextCursor(cursor);
}

void GraphTraversalWindow::highlightCurrentStep()
{
    if (!mapWindow || currentTraversalStep >= static_cast<int>(currentTraversalPath.size())) {
        return;
    }

    MapVisualization* visualization = mapWindow->getMapVisualization();
    if (!visualization) {
        return;
    }

    // Get the current city to highlight
    QString currentCity = QString::fromStdString(currentTraversalPath[currentTraversalStep]);

    // Get the list of visited cities so far
    QStringList visitedCities;
    QVariant visitedVar = visualization->property("visitedCities");
    if (visitedVar.isValid()) {
        visitedCities = visitedVar.toStringList();
    }

    // Add current city to visited list
    if (!visitedCities.contains(currentCity)) {
        visitedCities.append(currentCity);
    }

    // Update the current path and highlighted roads
    QStringList currentPath;
    QStringList highlightedRoads;

    if (currentTraversalStep > 0) {
        QString prevCity = QString::fromStdString(currentTraversalPath[currentTraversalStep - 1]);

        // Get the route between the previous and current city
        const CityGraph& cities = graph->getAllCities();
        auto prevCityIt = cities.find(prevCity.toStdString());
        if (prevCityIt != cities.end()) {
            const auto& connections = prevCityIt->second;

            // Check if there's a direct connection to the current city
            auto connectionIt = std::find_if(connections.begin(), connections.end(),
                                             [&currentCity](const std::pair<std::string, double>& connection) {
                                                 return connection.first == currentCity.toStdString();
                                             });

            if (connectionIt != connections.end()) {
                // Create a road identifier that includes both cities
                QString roadId = prevCity + "|" + currentCity;
                highlightedRoads.append(roadId);

                // Reverse direction (since it's an undirected graph)
                QString reverseRoadId = currentCity + "|" + prevCity;
                highlightedRoads.append(reverseRoadId);
            }
        }

        // Add cities to the current path
        currentPath.append(prevCity);
        currentPath.append(currentCity);
    }

    // Set the selected city
    visualization->setProperty("selectedCity", currentCity);
    visualization->setProperty("visitedCities", visitedCities);
    visualization->setProperty("currentPath", currentPath);

    // Add a new property for highlighted roads
    visualization->setProperty("highlightedRoads", highlightedRoads);

    // Repaint the map
    visualization->update();
}

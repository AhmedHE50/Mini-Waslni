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
        mapWindow->show();
        mapWindow->raise();          // Bring it to the front
        mapWindow->activateWindow(); // Give it focus
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
        mapWindow->show();
        mapWindow->raise();
        mapWindow->activateWindow();
        startTraversalVisualization(dfsResult);
    }
}

void GraphTraversalWindow::displayTraversalResult(const std::vector<std::string>& path)
{
    QString result = "Traversal Order:\n";

    for (size_t i = 0; i < path.size(); ++i) {
        result += QString::number(i + 1) + ". " + QString::fromStdString(path[i]);

        if (i < path.size() - 1) {
            result += "\n";
        }
    }

    ui->traversalResultsTextEdit->setText(result);
}

void GraphTraversalWindow::startTraversalVisualization(const std::vector<std::string>& path) {
    currentTraversalPath = path;
    currentTraversalStep = 0;
    resetVisualization();
    traversalTimer->setInterval(500); // Set a fixed animation speed
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

    // Update the current path if there's a previous city
    if (currentTraversalStep > 0) {
        QString prevCity = QString::fromStdString(currentTraversalPath[currentTraversalStep - 1]);
        QStringList currentPath;
        QVariant pathVar = visualization->property("currentPath");
        if (pathVar.isValid()) {
            currentPath = pathVar.toStringList();
        }

        // Add the edge (two city names) to represent the path
        currentPath.append(prevCity);
        currentPath.append(currentCity);

        // Update the property
        visualization->setProperty("currentPath", currentPath);
    }

    // Set the selected city
    visualization->setProperty("selectedCity", currentCity);
    visualization->setProperty("visitedCities", visitedCities);

    // Repaint the map
    visualization->update();
}

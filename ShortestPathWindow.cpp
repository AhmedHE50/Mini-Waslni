#include "shortestpathwindow.h"
#include "ui_shortestpathwindow.h"
#include "FileManager.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>

ShortestPathWindow::ShortestPathWindow(Graph* graph, MapWindow* mapWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShortestPathWindow),
    graph(graph),
    mapWindow(mapWindow),
    currentPathStep(0)
{
    ui->setupUi(this);
    setWindowTitle("Shortest Path Finder");

    cityListModel = new QStringListModel(this);

    updateCityList();

    ui->sourceCityComboBox->setModel(cityListModel);
    ui->destCityComboBox->setModel(cityListModel);

    ui->pathResultsTextEdit->clear();

    pathAnimationTimer = new QTimer(this);
    connect(pathAnimationTimer, &QTimer::timeout, this, &ShortestPathWindow::showNextPathStep);

    setStyleSheet(R"(
        QDialog {
            background-color: #f0f0f0;
        }
        #btnFindPath {
            background-color: #ff9800; /* (orange) */
            color: white;
        }
        #btnFindPath:hover {
            background-color: #fb8c00; /* (dark orange) */
        }
    )");

    // Initialize the map window if it exists
    if (mapWindow) {
        setMapWindow(mapWindow);
    }

}

ShortestPathWindow::~ShortestPathWindow()
{
    delete ui;
}

void ShortestPathWindow::setMapWindow(MapWindow* mapWindow)
{
    this->mapWindow = mapWindow;
    if (mapWindow) {
        mapWindow->getMapVisualization()->updateMap();
        mapWindow->setWindowModality(Qt::NonModal);
    }
}

void ShortestPathWindow::updateCityList()
{
    cityList.clear();
    const CityGraph& cities = graph->getAllCities();

    for (auto it = cities.begin(); it != cities.end(); ++it) {
        cityList.append(QString::fromStdString(it->first));
    }

    cityList.sort();
    cityListModel->setStringList(cityList);
}

void ShortestPathWindow::refreshCityList()
{
    updateCityList();
}

void ShortestPathWindow::on_btnFindPath_clicked()
{
    int sourceIndex = ui->sourceCityComboBox->currentIndex();
    int destIndex = ui->destCityComboBox->currentIndex();

    if (sourceIndex < 0 || destIndex < 0 || sourceIndex >= cityList.size() || destIndex >= cityList.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select both source and destination cities.");
        return;
    }

    if (sourceIndex == destIndex) {
        QMessageBox::warning(this, "Input Error", "Source and destination cities must be different.");
        return;
    }

    QString sourceCity = cityList.at(sourceIndex);
    QString destCity = cityList.at(destIndex);

    currentSource = sourceCity.toStdString();
    currentDestination = destCity.toStdString();

    // Run Dijkstra's algorithm
    shortestPaths = graph->Dijkstra(currentSource);

    // Display the shortest path
    displayPath(currentDestination);

    // Start visual animation
    if (mapWindow) {
        if (!mapWindow->isVisible()) {
            mapWindow->show();
        }
        mapWindow->raise();
        mapWindow->activateWindow();

        MapVisualization* visualization = mapWindow->getMapVisualization();
        if (visualization) {
            visualization->updateMap();
            startPathVisualization();
        } else {
            QMessageBox::warning(this, "Visualization Error", "Cannot show path visualization - map component is missing.");
        }
    } else {
        QMessageBox::information(this, "Visualization", "Path found but no map window is available for visualization.");

        // Create a new map window if it doesn't exist
        mapWindow = new MapWindow(graph, this);
        mapWindow->show();

        // Update the reference and try again
        this->setMapWindow(mapWindow);

        // Recursive call now that we have a map window
        QTimer::singleShot(500, this, [this]() {
            MapVisualization* visualization = mapWindow->getMapVisualization();
            if (visualization) {
                visualization->updateMap();
                startPathVisualization();
            }
        });
    }
}

void ShortestPathWindow::displayPath(const std::string& destination)
{
    if (shortestPaths.find(destination) == shortestPaths.end()) {
        ui->pathResultsTextEdit->setText("No path found from " +
                                         QString::fromStdString(currentSource) + " to " +
                                         QString::fromStdString(destination));
        return;
    }

    double totalDistance = shortestPaths[destination].first;

    // Reconstruct the path
    pathCities = reconstructPath(currentSource, destination);

    QString result = "Shortest Path from " + QString::fromStdString(currentSource) +
                     " to " + QString::fromStdString(destination) + ":\n\n";

    // Display the path
    for (size_t i = 0; i < pathCities.size(); ++i) {
        result += QString::fromStdString(pathCities[i]);

        if (i < pathCities.size() - 1) {
            result += " → ";
        }
    }

    result += "\n\nTotal Distance: " + QString::number(totalDistance) + " km";

    ui->pathResultsTextEdit->setText(result);
}

std::vector<std::string> ShortestPathWindow::reconstructPath(const std::string& start, const std::string& end)
{
    std::vector<std::string> path;
    std::string current = end;

    // Reconstruct path backwards
    while (current != start && shortestPaths.count(current)) {
        path.push_back(current);
        current = shortestPaths.at(current).second;
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}

void ShortestPathWindow::startPathVisualization()
{
    if (!mapWindow) {
        return;
    }

    if (pathCities.empty()) {
        return;
    }

    // Reset visualization state
    resetVisualization();
    currentPathStep = 0;

    // Disable the find path button during animation
    ui->btnFindPath->setEnabled(false);

    // Start the animation timer
    pathAnimationTimer->setInterval(500);
    pathAnimationTimer->start();
}

void ShortestPathWindow::resetVisualization()
{
    // Reset any highlights in the map visualization
    if (mapWindow) {
        MapVisualization* visualization = mapWindow->getMapVisualization();
        if (visualization) {
            visualization->setSelectedCity("");
            visualization->setVisitedCities(QStringList());
            visualization->setCurrentPath(QStringList());
            visualization->update();
        }
    }
}

void ShortestPathWindow::showNextPathStep()
{
    if (currentPathStep >= static_cast<int>(pathCities.size())) {
        // End of path animation
        pathAnimationTimer->stop();

        // Re-enable find path button
        ui->btnFindPath->setEnabled(true);
        return;
    }

    MapVisualization* visualization = mapWindow->getMapVisualization();
    if (!visualization) {
        pathAnimationTimer->stop();
        ui->btnFindPath->setEnabled(true);
        return;
    }

    // Get the current city to highlight
    QString currentCity = QString::fromStdString(pathCities[currentPathStep]);

    // Get the list of visited cities so far
    QStringList visitedCities;
    for (int i = 0; i <= currentPathStep; i++) {
        visitedCities.append(QString::fromStdString(pathCities[i]));
    }

    // Update the current path if there's a previous city
    QStringList currentPath;
    if (currentPathStep > 0) {
        for (int i = 0; i < currentPathStep; i++) {
            currentPath.append(QString::fromStdString(pathCities[i]));
            currentPath.append(QString::fromStdString(pathCities[i + 1]));
        }
    }

    // Update the properties
    visualization->setSelectedCity(currentCity);
    visualization->setVisitedCities(visitedCities);
    visualization->setCurrentPath(currentPath);

    // Repaint the map
    visualization->update();

    // Highlight the corresponding text in the results
    highlightPathText(currentPathStep);

    // Move to next step
    currentPathStep++;
}

void ShortestPathWindow::highlightPathText(int step)
{
    // Calculate the position in the text to highlight
    QString resultText = ui->pathResultsTextEdit->toPlainText();
    int startPos = resultText.indexOf(QString::fromStdString(pathCities[0]));

    // Find the position of the current city in the text
    int currentPos = 0;
    int count = 0;

    while (count <= step && currentPos != -1) {
        QString cityName = QString::fromStdString(pathCities[count]);
        currentPos = resultText.indexOf(cityName, startPos);

        if (currentPos != -1) {
            startPos = currentPos + cityName.length();
            count++;
        }
    }

    if (currentPos != -1) {
        // Select the text of the current city
        QTextCursor cursor = ui->pathResultsTextEdit->textCursor();
        cursor.setPosition(currentPos);
        cursor.setPosition(currentPos + QString::fromStdString(pathCities[step]).length(), QTextCursor::KeepAnchor);
        ui->pathResultsTextEdit->setTextCursor(cursor);
    }
}

void ShortestPathWindow::on_sourceCityComboBox_currentIndexChanged(int index)
{
    ui->btnFindPath->setEnabled(index >= 0 && ui->destCityComboBox->currentIndex() >= 0);
}

void ShortestPathWindow::on_destCityComboBox_currentIndexChanged(int index)
{
    ui->btnFindPath->setEnabled(index >= 0 && ui->sourceCityComboBox->currentIndex() >= 0);
}

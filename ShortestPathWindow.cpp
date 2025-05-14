#include "shortestpathwindow.h"
#include "ui_shortestpathwindow.h"
#include "FileManager.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>
#include <unordered_map>
#include <queue>

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
    updateAlgorithmList();
    updateVisualizationSpeedList();

    ui->sourceCityComboBox->setModel(cityListModel);
    ui->destCityComboBox->setModel(cityListModel);

    ui->pathResultsTextEdit->clear();

    pathAnimationTimer = new QTimer(this);
    connect(pathAnimationTimer, &QTimer::timeout, this, &ShortestPathWindow::showNextPathStep);

    setStyleSheet(R"(
        QDialog {
            background-color: #f0f0f0;
        }
    )");

    // Initialize the map window if it exists
    if (mapWindow) {
        setMapWindow(mapWindow);
    }

    on_algorithmComboBox_currentIndexChanged(0);
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

void ShortestPathWindow::updateAlgorithmList()
{
    QStringList algorithms;
    algorithms << "Dijkstra" << "Kruskal MST";
    ui->algorithmComboBox->clear();
    ui->algorithmComboBox->addItems(algorithms);
}

void ShortestPathWindow::refreshCityList()
{
    updateCityList();
}

void ShortestPathWindow::on_algorithmComboBox_currentIndexChanged(int index)
{
    if (index == DIJKSTRA) {
        ui->btnFindPath->setText("Find Shortest Path");
    } else if (index == KRUSKAL_MST) {
        ui->btnFindPath->setText("Find MST");
    }
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

    // Determine which algorithm to use
    int algorithmIndex = ui->algorithmComboBox->currentIndex();

    if (algorithmIndex == DIJKSTRA) {
        runDijkstra();
    } else if (algorithmIndex == KRUSKAL_MST) {
        runKruskalMST();
    }

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

void ShortestPathWindow::runDijkstra()
{
    // Run Dijkstra's algorithm
    shortestPaths = graph->Dijkstra(currentSource);

    // Display the shortest path
    displayPath(currentDestination);
}

void ShortestPathWindow::runKruskalMST()
{
    // Get all cities for creating MST graph
    CityGraph cities = graph->getAllCities();

    // Helper function to extract edges for Kruskal's MST
    map<double, vector<pair<string, string>>> sortedEdges = graph->roadExtract(cities);

    // Create a set to store unique cities
    set<string> uniqueCities;
    for (auto it = cities.begin(); it != cities.end(); ++it) {
        string city = it->first;
        uniqueCities.insert(city);

        list<pair<string, double>>& neighbors = it->second;
        for (auto neighborIt = neighbors.begin(); neighborIt != neighbors.end(); ++neighborIt) {
            string neighbor = neighborIt->first;
            uniqueCities.insert(neighbor);
        }
    }
    int cityCount = uniqueCities.size();

    // Reset the MST paths
    mstPaths.clear();

    // Create a graph for MST
    unordered_map<string, vector<string>> mstGraph;

    // Initialize the graph with only nodes
    for (const string& city : uniqueCities) {
        mstGraph[city] = vector<string>();
    }

    // Iterate through edges in ascending order by weight
    for (auto edgeIt = sortedEdges.begin(); edgeIt != sortedEdges.end(); ++edgeIt) {
        double weight = edgeIt->first;
        vector<pair<string, string>>& edgeList = edgeIt->second;

        for (const auto& edge : edgeList) {
            string u = edge.first;
            string v = edge.second;

            // Simulate adding edge
            mstGraph[u].push_back(v);
            mstGraph[v].push_back(u);

            // Check for cycles
            unordered_map<string, bool> visited;
            bool cycleFound = false;

            for (const string& city : uniqueCities) {
                visited[city] = false;
            }

            if (graph->hasCycle(mstGraph, u, visited, "")) {
                cycleFound = true;
            }

            if (cycleFound) {
                // Remove edge if it creates a cycle
                vector<string>& uNeighbors = mstGraph[u];
                auto newEndU = remove(uNeighbors.begin(), uNeighbors.end(), v);
                uNeighbors.erase(newEndU, uNeighbors.end());

                vector<string>& vNeighbors = mstGraph[v];
                auto newEndV = remove(vNeighbors.begin(), vNeighbors.end(), u);
                vNeighbors.erase(newEndV, vNeighbors.end());
            } else {
                // Add edge to MST
                mstPaths.push_back(make_pair(make_pair(u, v), weight));

                // If we have n-1 edges, MST is complete
                if (mstPaths.size() == cityCount - 1) {
                    break;
                }
            }
        }

        // If MST is complete, break out of the loop
        if (mstPaths.size() == cityCount - 1) {
            break;
        }
    }

    // Find path in MST from source to destination
    pathCities = findPathInMST(currentSource, currentDestination);

    // Display MST path
    displayMSTPath();
}

vector<string> ShortestPathWindow::findPathInMST(const string& start, const string& end)
{
    // Create an adjacency list from MST edges
    unordered_map<string, vector<string>> mstAdjList;
    for (const auto& edge : mstPaths) {
        string u = edge.first.first;
        string v = edge.first.second;
        mstAdjList[u].push_back(v);
        mstAdjList[v].push_back(u);
    }

    // BFS to find the path
    queue<string> q;
    unordered_map<string, bool> visited;
    unordered_map<string, string> parent;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (current == end) {
            break;
        }

        for (const string& neighbor : mstAdjList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    // Reconstruct the path
    vector<string> path;
    string current = end;

    if (parent.find(end) != parent.end() || start == end) {
        while (current != start) {
            path.push_back(current);
            current = parent[current];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
    }

    return path;
}

void ShortestPathWindow::displayMSTPath()
{
    if (pathCities.empty()) {
        ui->pathResultsTextEdit->setText("No path found from " +
                                         QString::fromStdString(currentSource) + " to " +
                                         QString::fromStdString(currentDestination) + " in the MST.");
        return;
    }

    // Calculate total distance of the path
    double totalDistance = 0.0;
    for (size_t i = 0; i < pathCities.size() - 1; ++i) {
        for (const auto& edge : mstPaths) {
            string u = edge.first.first;
            string v = edge.first.second;
            double weight = edge.second;

            if ((u == pathCities[i] && v == pathCities[i+1]) ||
                (u == pathCities[i+1] && v == pathCities[i])) {
                totalDistance += weight;
                break;
            }
        }
    }

    QString result = "Shortest Path (MST) from " + QString::fromStdString(currentSource) +
                     " to " + QString::fromStdString(currentDestination) + ":\n\n";

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

    QString result = "Shortest Path (Dijkstra) from " + QString::fromStdString(currentSource) +
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

    // Calculate the interval based on the selected speed
    int speedIndex = ui->visualizationSpeedComboBox->currentIndex();
    int baseInterval = 500; // Default 1x speed is 500ms
    int interval = baseInterval / (speedIndex + 1);

    // Start the animation timer
    pathAnimationTimer->setInterval(interval);
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

void ShortestPathWindow::updateVisualizationSpeedList()
{
    QStringList speeds;
    speeds << "1x" << "2x" << "3x" << "4x" << "5x";
    ui->visualizationSpeedComboBox->clear();
    ui->visualizationSpeedComboBox->addItems(speeds);
    ui->visualizationSpeedComboBox->setCurrentIndex(0); // Default to 1x
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

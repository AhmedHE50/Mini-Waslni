#include "shortestpathwindow.h"
#include "ui_shortestpathwindow.h"
#include "FileManager.h"
#include <QMessageBox>
#include <QFileDialog>

ShortestPathWindow::ShortestPathWindow(Graph* graph, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShortestPathWindow),
    graph(graph)
{
    ui->setupUi(this);
    setWindowTitle("Shortest Path Finder");

    // Initialize city list model
    cityListModel = new QStringListModel(this);

    // Update city list
    updateCityList();

    // Setup combo boxes
    ui->sourceCityComboBox->setModel(cityListModel);
    ui->destCityComboBox->setModel(cityListModel);

    // Clear results area
    ui->pathResultsTextEdit->clear();

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
}

ShortestPathWindow::~ShortestPathWindow()
{
    delete ui;
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
    std::vector<std::string> path = reconstructPath(currentSource, destination);

    QString result = "Shortest Path from " + QString::fromStdString(currentSource) +
                     " to " + QString::fromStdString(destination) + ":\n\n";

    // Display the path
    for (size_t i = 0; i < path.size(); ++i) {
        result += QString::fromStdString(path[i]);

        if (i < path.size() - 1) {
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

void ShortestPathWindow::on_sourceCityComboBox_currentIndexChanged(int index)
{
    // Enable Find Path button if both source and destination are selected
    ui->btnFindPath->setEnabled(index >= 0 && ui->destCityComboBox->currentIndex() >= 0);
}

void ShortestPathWindow::on_destCityComboBox_currentIndexChanged(int index)
{
    // Enable Find Path button if both source and destination are selected
    ui->btnFindPath->setEnabled(index >= 0 && ui->sourceCityComboBox->currentIndex() >= 0);
}

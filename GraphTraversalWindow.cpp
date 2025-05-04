#include "graphtraversalwindow.h"
#include "ui_graphtraversalwindow.h"
#include "FileManager.h"
#include <QMessageBox>
#include <QFileDialog>

GraphTraversalWindow::GraphTraversalWindow(Graph* graph, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphTraversalWindow),
    graph(graph)
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

void GraphTraversalWindow::on_btnBFS_clicked()
{
    int startIndex = ui->startCityComboBox->currentIndex();

    if (startIndex < 0 || startIndex >= cityList.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a starting city.");
        return;
    }

    QString startCity = cityList.at(startIndex);

    // Perform BFS
    std::vector<std::string> bfsResult = graph->BFS(startCity.toStdString());

    // Display result
    displayTraversalResult(bfsResult);
}

void GraphTraversalWindow::on_btnDFS_clicked()
{
    int startIndex = ui->startCityComboBox->currentIndex();

    if (startIndex < 0 || startIndex >= cityList.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a starting city.");
        return;
    }

    QString startCity = cityList.at(startIndex);

    // Perform DFS
    std::vector<std::string> dfsResult = graph->DFS(startCity.toStdString());

    // Display result
    displayTraversalResult(dfsResult);
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

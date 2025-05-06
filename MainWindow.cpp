#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize windows
    graphOperationsWindow = nullptr;
    graphTraversalWindow = nullptr;
    shortestPathWindow = nullptr;
    mapWindow = nullptr;

    // Set default filename for saving
    graphDataFilename = "graph_data.json";
    cityPositionsFilename = "city_positions.json";

    setWindowTitle("Mini Waslni");

    setStyleSheet(R"(
        QMainWindow {
            background-color: #f0f0f0; /* (light gray) */
        }
        QPushButton {
            background-color: #4CAF50; /* (green) */ /* change to this #2196F3 (blue) */
            border: none;
            color: white;
            padding: 10px 20px;
            text-align: center;
            text-decoration: none;
            font-size: 14px;
            margin: 4px 2px;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #45a049; /* (dark green) */
        }
        QPushButton:pressed {
            background-color: #3e8e41;
        }
    )");
}

MainWindow::~MainWindow()
{
    delete ui;

    // Clean up
    if (graphOperationsWindow) delete graphOperationsWindow;
    if (graphTraversalWindow) delete graphTraversalWindow;
    if (shortestPathWindow) delete shortestPathWindow;
    if (mapWindow) delete mapWindow;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool graphSaved = false;
    bool positionsSaved = false;
    QString errorMessages;

    // Save the graph
    try {
        FileManager::saveGraph(graph, graphDataFilename.toStdString());
        graphSaved = true;
    }
    catch (const std::runtime_error& e) {
        errorMessages += QString("Failed to save graph: %1\n").arg(e.what());
    }

    // Save city positions
    try {
        mapWindow->getMapVisualization()->saveCityPositionsToFile(cityPositionsFilename);
        positionsSaved = true;
    }
    catch (const std::runtime_error& e) {
        errorMessages += QString("Failed to save city positions: %1").arg(e.what());
    }

    if (graphSaved && positionsSaved) {
        event->accept();
    } else {
        // Show error message if save fails
        QMessageBox::critical(this, "Error", errorMessages);

        // Ask if user wants to close anyway
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Close Application",
                                      "Some data could not be saved. Do you want to close anyway?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            event->accept();
        } else {
            event->ignore();
        }
    }
}

void MainWindow::on_btnGraphOperations_clicked()
{
    if (!graphOperationsWindow) {
        graphOperationsWindow = new GraphOperationsWindow(&graph, this);
        connect(graphOperationsWindow, &GraphOperationsWindow::destroyed, this, &MainWindow::onGraphChanged);
    } else {
        graphOperationsWindow->refreshCityList();
    }
    graphOperationsWindow->show();
    graphOperationsWindow->activateWindow();
}

void MainWindow::on_btnGraphTraversal_clicked()
{
    if (!graphTraversalWindow) {
        graphTraversalWindow = new GraphTraversalWindow(&graph, mapWindow, this);
    } else {
        graphTraversalWindow->refreshCityList();
    }
    graphTraversalWindow->show();
    graphTraversalWindow->activateWindow();
}

void MainWindow::on_btnShortestPath_clicked()
{
    if (!shortestPathWindow) {
        shortestPathWindow = new ShortestPathWindow(&graph, mapWindow, this);
    } else {
        shortestPathWindow->refreshCityList();
    }
    shortestPathWindow->show();
    shortestPathWindow->activateWindow();
}

void MainWindow::on_btnMapVisualization_clicked() {
    mapWindow = new MapWindow(&graph, this);

    /* if (!mapWindow) {
        // Load city positions from the file specified earlier
        if (!mapWindow->getMapVisualization()->loadCityPositionsFromFile(cityPositionsFilename)) {
            // If loading fails, it will use default positions
            QMessageBox::warning(this, "Warning",
                                 "City positions file could not be loaded. Using default positions.");
        }
    } else {
        mapWindow->refreshMap();
    } */

    // Reset visualization before showing the map
    mapWindow->getMapVisualization()->setSelectedCity("");
    mapWindow->getMapVisualization()->setVisitedCities(QStringList());
    mapWindow->getMapVisualization()->setCurrentPath(QStringList());
    mapWindow->getMapVisualization()->update();

    mapWindow->show();
    mapWindow->activateWindow();
}

void MainWindow::onGraphChanged()
{
    if (mapWindow) {
        mapWindow->updateMap();
    }
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "MapWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize windows to nullptr
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

    // Clean up child windows
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

    // Save city positions only if mapWindow exists and has mapVisualization
    if (mapWindow) {
        MapVisualization* mapVis = mapWindow->getMapVisualization();
        if (mapVis) {
            try {
                mapVis->saveCityPositionsToFile(cityPositionsFilename);
                positionsSaved = true;
            }
            catch (const std::runtime_error& e) {
                errorMessages += QString("Failed to save city positions: %1").arg(e.what());
            }
        } else {
            // If mapWindow exists but mapVisualization doesn't (shouldn't happen),
            // treat positions as not saved.
            positionsSaved = false;
            errorMessages += QString("Map visualization component not found for saving positions.\n");
        }
    } else {
        // If mapWindow didn't exist, consider positions saved (or not applicable)
        positionsSaved = true;
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
        // Pass the mapWindow instance to the GraphOperationsWindow constructor
        graphOperationsWindow = new GraphOperationsWindow(&graph, mapWindow, this);
        connect(graphOperationsWindow, &GraphOperationsWindow::destroyed, this, &MainWindow::onGraphChanged);
    } else {
        // If the window already exists, just refresh its city list
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
    // Create mapWindow only if it doesn't exist
    if (!mapWindow) {
        mapWindow = new MapWindow(&graph, this);

        // Load city positions from the file specified earlier
        if (!mapWindow->getMapVisualization()->loadCityPositionsFromFile(cityPositionsFilename)) {
            // If loading fails, it will use default positions
            QMessageBox::warning(this, "Warning",
                                 "City positions file could not be loaded. Using default positions.");
        }
    } else {
        // If mapWindow already exists, just refresh it
        mapWindow->refreshMap();
    }

    // Reset visualization state before showing/activating
    MapVisualization* mapVis = mapWindow->getMapVisualization();
    if (mapVis) {
        mapVis->setSelectedCity("");
        mapVis->setVisitedCities(QStringList());
        mapVis->setCurrentPath(QStringList());
        mapVis->update(); // Trigger a repaint
    }


    mapWindow->show();
    mapWindow->activateWindow();
}

void MainWindow::onGraphChanged()
{
    // This slot is connected to the destroyed signal of the operations window.
    // It should trigger updates in other windows that display graph data.

    // Update other windows that might be open and need graph changes reflected
    if (graphOperationsWindow) {
        graphOperationsWindow->refreshCityList();
    }
    if (graphTraversalWindow) {
        graphTraversalWindow->refreshCityList();
    }
    if (shortestPathWindow) {
        shortestPathWindow->refreshCityList();
    }
    if (mapWindow) {
        mapWindow->updateMap(); // Update map with changes (like added/deleted cities/roads)
    }
}

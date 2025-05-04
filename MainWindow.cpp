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

    // Set default filename for saving
    currentFilename = "D:/University/DS/Mini-Waslni/graph.json";

    setWindowTitle("Mini Waslni");

    // Apply style sheet to the main window
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

    // Clean up windows if they exist
    if (graphOperationsWindow) delete graphOperationsWindow;
    if (graphTraversalWindow) delete graphTraversalWindow;
    if (shortestPathWindow) delete shortestPathWindow;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    try {
        // Save the graph automatically when closing the application
        FileManager::saveGraph(graph, currentFilename.toStdString());
        event->accept();
    }
    catch (const std::runtime_error& e) {
        // Show error message if save fails
        QMessageBox::critical(this, "Error", QString("Failed to save graph: %1").arg(e.what()));

        // Ask if user wants to close anyway
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Close Application",
                                      "The graph could not be saved. Do you want to close anyway?",
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
    } else {
        graphOperationsWindow->refreshCityList();
    }
    graphOperationsWindow->show();
    graphOperationsWindow->activateWindow();
}

void MainWindow::on_btnGraphTraversal_clicked()
{
    if (!graphTraversalWindow) {
        graphTraversalWindow = new GraphTraversalWindow(&graph, this);
    } else {
        graphTraversalWindow->refreshCityList();
    }
    graphTraversalWindow->show();
    graphTraversalWindow->activateWindow();
}

void MainWindow::on_btnShortestPath_clicked()
{
    if (!shortestPathWindow) {
        shortestPathWindow = new ShortestPathWindow(&graph, this);
    } else {
        shortestPathWindow->refreshCityList();
    }
    shortestPathWindow->show();
    shortestPathWindow->activateWindow();
}

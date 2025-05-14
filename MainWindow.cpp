#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QPalette>
#include <QPixmap>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>
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
    backgroundFilename = "background.jpg";

    setBackgroundImage(backgroundFilename, 3.0);

    setWindowTitle("Mini Waslni");

    setStyleSheet(R"(
        #label {
            color: white;
        }
        QPushButton {
            background-color: #4D55CC;
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
            background-color: #3F47B2;
        }
        QPushButton:pressed {
            background-color: #2F3799;
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

QPixmap MainWindow::blurImage(const QPixmap& original, qreal blurRadius)
{
    // Create a graphics scene and item
    QGraphicsScene scene;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(original);
    scene.addItem(item);

    // Create and apply blur effect
    QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect();
    blurEffect->setBlurRadius(blurRadius);
    item->setGraphicsEffect(blurEffect);

    // Render the blurred image
    QPixmap blurredPixmap(original.size());
    blurredPixmap.fill(Qt::transparent);
    QPainter painter(&blurredPixmap);
    scene.render(&painter);

    return blurredPixmap;
}

void MainWindow::setBackgroundImage(const QString& imagePath, qreal blurRadius)
{
    // Create a pixmap from the image
    QPixmap originalImage(imagePath);

    // Check if image is valid
    if (originalImage.isNull()) {
        qWarning() << "Failed to load background image:" << imagePath;
        return;
    }

    // Blur the image
    QPixmap blurredImage = blurImage(originalImage, blurRadius);

    // Create a palette with the blurred background image
    QPalette palette;
    palette.setBrush(QPalette::Window, blurredImage);

    // Apply the palette to the main window
    this->setPalette(palette);

    // Ensure the background is drawn
    this->setAutoFillBackground(true);
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

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    // If you want to dynamically rescale background on resize
    if (!backgroundFilename.isEmpty()) {
        setBackgroundImage(backgroundFilename, 3.0);
    }
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

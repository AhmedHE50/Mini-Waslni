#include "MapWindow.h"
#include "ui_mapwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MapWindow::MapWindow(Graph* graph, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapWindow),
    graph(graph)
{
    ui->setupUi(this);
    setWindowTitle("Map Visualization");
    resize(800, 600);

    // Window stays on top
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mapVisualization = new MapVisualization(graph, this);
    mainLayout->addWidget(mapVisualization);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    // Add zoom controls
    QPushButton* zoomInBtn = new QPushButton("Zoom In", this);
    QPushButton* zoomOutBtn = new QPushButton("Zoom Out", this);
    QPushButton* resetViewBtn = new QPushButton("Reset View", this);

    buttonLayout->addWidget(zoomInBtn);
    buttonLayout->addWidget(zoomOutBtn);
    buttonLayout->addWidget(resetViewBtn);

    // Connect button signals
    connect(zoomInBtn, &QPushButton::clicked, mapVisualization, &MapVisualization::zoomIn);
    connect(zoomOutBtn, &QPushButton::clicked, mapVisualization, &MapVisualization::zoomOut);
    connect(resetViewBtn, &QPushButton::clicked, mapVisualization, &MapVisualization::resetZoom);

    // Add button layout to main layout
    mainLayout->addLayout(buttonLayout);

    // Set the main layout
    setLayout(mainLayout);

    // Initialize the map visualization
    mapVisualization->initializeCityPositions();
    mapVisualization->updateMap();
}

MapWindow::~MapWindow()
{
    delete ui;
}

void MapWindow::refreshMap()
{
    mapVisualization->updateMap();
}

void MapWindow::updateMap()
{
    mapVisualization->updateMap();
}

void MapWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    mapVisualization->updateMap();
}

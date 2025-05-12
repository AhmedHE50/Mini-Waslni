#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "Graph.h"
#include "FileManager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Create and initialize the main window
    MainWindow mainWindow;
    mainWindow.show();

    // Load graph data at the beginning
    Graph graph;
    try {
        FileManager::loadGraph(graph, "graph_data.json");
        mainWindow.setGraph(graph);
    } catch (const std::runtime_error& error) {
        QMessageBox::critical(nullptr, "File Error", QString::fromStdString(error.what()));
    }

    // Load city positions at the beginning
    try {
        mainWindow.setCityPositionsFile("city_positions.json");
    } catch (const std::runtime_error& error) {
        QMessageBox::critical(nullptr, "File Error", QString::fromStdString(error.what()));
    }

    int result = a.exec();

    return result;
}

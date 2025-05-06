#ifndef GRAPHTRAVERSALWINDOW_H
#define GRAPHTRAVERSALWINDOW_H

#include <QDialog>
#include <QStringListModel>
#include <QTimer>
#include "Graph.h"
#include "MapWindow.h"

namespace Ui {
class GraphTraversalWindow;
}

class GraphTraversalWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphTraversalWindow(Graph* graph, MapWindow* mapWindow = nullptr, QWidget *parent = nullptr);
    ~GraphTraversalWindow();

    void refreshCityList();
    void setMapWindow(MapWindow* mapWindow);

private slots:
    void on_btnBFS_clicked();
    void on_btnDFS_clicked();
    void showNextTraversalStep();

private:
    Ui::GraphTraversalWindow *ui;
    Graph* graph;
    MapWindow* mapWindow;
    QStringListModel* cityListModel;
    QStringList cityList;

    QTimer* traversalTimer;
    std::vector<std::string> currentTraversalPath;
    int currentTraversalStep;

    void updateCityList();

    // Traversal Visualization
    void displayTraversalResult(const std::vector<std::string>& path); // Text
    void startTraversalVisualization(const std::vector<std::string>& path); // Animation
    void resetVisualization();
    void highlightCurrentStep();
};

#endif

#ifndef SHORTESTPATHWINDOW_H
#define SHORTESTPATHWINDOW_H

#include <QDialog>
#include <QStringListModel>
#include <QTimer>
#include <QShowEvent>
#include "Graph.h"
#include "MapWindow.h"

namespace Ui {
class ShortestPathWindow;
}

class ShortestPathWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ShortestPathWindow(Graph* graph, MapWindow* mapWindow = nullptr, QWidget *parent = nullptr);
    ~ShortestPathWindow();
    void refreshCityList();
    void setMapWindow(MapWindow* mapWindow);

private slots:
    void on_btnFindPath_clicked();
    void on_sourceCityComboBox_currentIndexChanged(int index);
    void on_destCityComboBox_currentIndexChanged(int index);
    void showNextPathStep();

private:
    Ui::ShortestPathWindow *ui;
    Graph* graph;
    MapWindow* mapWindow;
    QStringListModel* cityListModel;
    QStringList cityList;
    std::map<std::string, std::pair<double, std::string>> shortestPaths;
    std::string currentSource;
    std::string currentDestination;
    std::vector<std::string> pathCities;
    int currentPathStep;
    QTimer* pathAnimationTimer;

    void updateCityList();

    // Traversal Visualization
    void displayPath(const std::string& destination); // Text
    std::vector<std::string> reconstructPath(const std::string& start, const std::string& end);
    void startPathVisualization(); // Animation
    void resetVisualization();
    void highlightPathText(int step);
};

#endif

#ifndef SHORTESTPATHWINDOW_H
#define SHORTESTPATHWINDOW_H

#include <QDialog>
#include <QStringListModel>
#include "Graph.h"

namespace Ui {
class ShortestPathWindow;
}

class ShortestPathWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ShortestPathWindow(Graph* graph, QWidget *parent = nullptr);
    ~ShortestPathWindow();
    void refreshCityList();

private slots:
    void on_btnFindPath_clicked();
    void on_sourceCityComboBox_currentIndexChanged(int index);
    void on_destCityComboBox_currentIndexChanged(int index);

private:
    Ui::ShortestPathWindow *ui;
    Graph* graph;
    QStringListModel* cityListModel;
    QStringList cityList;
    std::map<std::string, std::pair<double, std::string>> shortestPaths;
    std::string currentSource;
    std::string currentDestination;

    void updateCityList();
    void displayPath(const std::string& destination);
    std::vector<std::string> reconstructPath(const std::string& start, const std::string& end);
};

#endif // SHORTESTPATHWINDOW_H

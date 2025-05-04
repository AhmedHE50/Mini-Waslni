#ifndef GRAPHTRAVERSALWINDOW_H
#define GRAPHTRAVERSALWINDOW_H

#include <QDialog>
#include <QStringListModel>
#include "Graph.h"

namespace Ui {
class GraphTraversalWindow;
}

class GraphTraversalWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphTraversalWindow(Graph* graph, QWidget *parent = nullptr);
    ~GraphTraversalWindow();
    void refreshCityList();

private slots:
    void on_btnBFS_clicked();
    void on_btnDFS_clicked();

private:
    Ui::GraphTraversalWindow *ui;
    Graph* graph;
    QStringListModel* cityListModel;
    QStringList cityList;
    void updateCityList();
    void displayTraversalResult(const std::vector<std::string>& path);
};

#endif // GRAPHTRAVERSALWINDOW_H

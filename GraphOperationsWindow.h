#ifndef GRAPHOPERATIONSWINDOW_H
#define GRAPHOPERATIONSWINDOW_H

#include <QDialog>
#include <QStringListModel>
#include <QRegularExpression>
#include "Graph.h"

// Forward declaration
class MapWindow;

namespace Ui {
class GraphOperationsWindow;
}

class GraphOperationsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphOperationsWindow(Graph* graph, MapWindow* mapWindow, QWidget *parent = nullptr);
    ~GraphOperationsWindow();

    void refreshCityList();

private slots:
    void on_btnAddCity_clicked();
    void on_btnDeleteCity_clicked();
    void on_btnAddRoad_clicked();
    void on_btnDeleteRoad_clicked();

    void on_cityListBox_currentIndexChanged(int index);

private:
    Ui::GraphOperationsWindow *ui;
    Graph* graph;
    QStringList cityList;
    QStringListModel* cityListModel;
    MapWindow* mapWindow;

    void updateCityList();
};

#endif

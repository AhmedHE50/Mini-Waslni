#ifndef GRAPHOPERATIONSWINDOW_H
#define GRAPHOPERATIONSWINDOW_H

#include <QDialog>
#include <QStringListModel>
#include "Graph.h"

namespace Ui {
class GraphOperationsWindow;
}

class GraphOperationsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphOperationsWindow(Graph* graph, QWidget *parent = nullptr);
    ~GraphOperationsWindow();

    // Update the city list (Handle Graph Modification)
    void refreshCityList();

private slots: // slots => functions that can be connected to signals, like button clicks
    void on_btnAddCity_clicked();
    void on_btnDeleteCity_clicked();
    void on_btnAddRoad_clicked();
    void on_btnDeleteRoad_clicked();
    void on_cityListBox_currentIndexChanged(int index);

private:
    Ui::GraphOperationsWindow *ui;
    Graph* graph;
    QStringListModel* cityListModel;
    QStringList cityList;

    // Update the city list model
    void updateCityList();
};

#endif

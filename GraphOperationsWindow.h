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
    QStringListModel* cityListModel;
    QStringList cityList;
    void updateCityList();
};

#endif // GRAPHOPERATIONSWINDOW_H

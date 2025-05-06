#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QDialog>
#include <QStringListModel>
#include <QShowEvent>
#include "Graph.h"
#include "FileManager.h"
#include "MapVisualization.h"

namespace Ui {
class MapWindow;
}

class MapWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MapWindow(Graph* graph, QWidget *parent = nullptr);
    ~MapWindow();

    void refreshMap();
    MapVisualization* getMapVisualization() { return mapVisualization; }

public slots:
    void updateMap();

protected:
    void showEvent(QShowEvent* event) override;

private:
    Ui::MapWindow *ui;
    Graph* graph;
    MapVisualization* mapVisualization;
};

#endif

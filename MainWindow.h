#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Graph.h"
#include "graphoperationswindow.h"
#include "graphtraversalwindow.h"
#include "shortestpathwindow.h"
#include "MapWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setGraph(const Graph& newGraph) {
        graph = newGraph;

        if (graphOperationsWindow) graphOperationsWindow->refreshCityList();
        if (graphTraversalWindow) graphTraversalWindow->refreshCityList();
        if (shortestPathWindow) shortestPathWindow->refreshCityList();
        if (mapWindow) mapWindow->refreshMap();
    }

    // Sets the filename for saving city positions
    void setCityPositionsFile(const QString& filename) {
        cityPositionsFilename = filename;
    }

    QString getCityPositionsFile() const {
        return cityPositionsFilename;
    }

    void setBackgroundImage(const QString& imagePath, qreal blurRadius = 10.0);

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;
    QPixmap blurImage(const QPixmap& original, qreal blurRadius);

private slots:
    void on_btnGraphOperations_clicked();
    void on_btnGraphTraversal_clicked();
    void on_btnShortestPath_clicked();
    void on_btnMapVisualization_clicked();
    void onGraphChanged();

private:
    Ui::MainWindow *ui;
    Graph graph;
    QString graphDataFilename;
    QString cityPositionsFilename;
    QString backgroundFilename;

    // Pointers to child windows
    GraphOperationsWindow *graphOperationsWindow;
    GraphTraversalWindow *graphTraversalWindow;
    ShortestPathWindow *shortestPathWindow;
    MapWindow *mapWindow;
};
#endif

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Graph.h"
#include "FileManager.h"
#include "graphoperationswindow.h"
#include "graphtraversalwindow.h"
#include "shortestpathwindow.h"

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
        // Update any open windows
        if (graphOperationsWindow) graphOperationsWindow->refreshCityList();
        if (graphTraversalWindow) graphTraversalWindow->refreshCityList();
        if (shortestPathWindow) shortestPathWindow->refreshCityList();
    }

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_btnGraphOperations_clicked();
    void on_btnGraphTraversal_clicked();
    void on_btnShortestPath_clicked();

private:
    Ui::MainWindow *ui;
    Graph graph;
    QString currentFilename;

    GraphOperationsWindow *graphOperationsWindow;
    GraphTraversalWindow *graphTraversalWindow;
    ShortestPathWindow *shortestPathWindow;
};
#endif // MAINWINDOW_H

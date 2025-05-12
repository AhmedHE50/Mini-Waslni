#ifndef MAPVISUALIZATION_H
#define MAPVISUALIZATION_H

#include <QWidget>
#include <QPointF>
#include <QMap>
#include <QVariant>
#include "Graph.h"

// Forward declaration
class QWheelEvent;
class QMouseEvent;
class QResizeEvent;
class QPaintEvent;

// Define default city and road appearance
const int cityRadius = 8;
const int selectedCityRadius = 10;
const int visitedCityRadius = 9;

const QColor cityColor = Qt::green;
const QColor selectedCityColor = Qt::red;
const QColor visitedCityColor = Qt::darkBlue;
const QColor cityBorderColor = Qt::darkGray;

const int roadWidth = 2;
const int selectedRoadWidth = 3;

const QColor roadColor = Qt::gray;
const QColor traversedRoadColor = Qt::red;


class MapVisualization : public QWidget
{
    Q_OBJECT

    // Properties for visualization state
    Q_PROPERTY(QString selectedCity READ getSelectedCity WRITE setSelectedCity)
    Q_PROPERTY(QStringList visitedCities READ getVisitedCities WRITE setVisitedCities)
    Q_PROPERTY(QStringList currentPath READ getCurrentPath WRITE setCurrentPath)

public:
    explicit MapVisualization(Graph* graph, QWidget *parent = nullptr);

    void updateMap();
    void zoomIn();
    void zoomOut();
    void resetZoom();

    // City position handling
    bool loadCityPositionsFromFile(const QString& filePath);
    bool saveCityPositionsToFile(const QString& filePath);
    void showCityPositionDialog(const QString& cityName);
    void initializeCityPositions();
    QPointF calculateDefaultPosition(const QString& cityName);
    void removeCityPosition(const QString& cityName);

    // Getters for properties
    QString getSelectedCity() const { return selectedCity; }
    QStringList getVisitedCities() const { return visitedCities; }
    QStringList getCurrentPath() const { return currentPath; }

    // Setters for properties
    void setSelectedCity(const QString& city) { selectedCity = city; }
    void setVisitedCities(const QStringList& cities) { visitedCities = cities; }
    void setCurrentPath(const QStringList& path) { currentPath = path; }


protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override; // Added resizeEvent

private:
    Graph* graph;
    QMap<QString, QPointF> cityPositions; // Stores city positions in normalized coordinates (-1 to 1)
    QPixmap backgroundMap; // Optional background map
    qreal zoomFactor;
    QPointF panOffset;
    bool isPanning;
    QPointF lastPanPosition;

    QString selectedCity;
    QStringList visitedCities; // Cities that have been visited during traversal/pathfinding
    QStringList currentPath; // The current edges being highlighted in path/traversal


    // Helper functions for coordinate conversion
    QPointF mapToView(const QPointF& mapPoint);
    QPointF viewToMap(const QPointF& viewPoint);
    QPointF normalizedToScreen(const QPointF& normalizedPos); // Converts normalized (-1 to 1) to screen coordinates
    QPointF convertOldToNewCoordinates(const QPointF& oldCoords); // Converts 0-1 to -1-1 (if needed for compatibility)
    QPointF convertNewToOldCoordinates(const QPointF& newCoords); // Converts -1-1 to 0-1 (if needed for compatibility)

    // Drawing functions
    void drawCity(QPainter& painter, const QString& cityName, const QPointF& pos);
    void drawRoad(QPainter& painter, const QPointF& from, const QPointF& to, double distance);
    QString findCityAtPosition(const QPoint& pos);
    bool isEdgeInPath(const QString& fromCity, const QString& toCity) const; // Check if an edge is in the current path

};

#endif

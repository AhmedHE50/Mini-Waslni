#ifndef MAPVISUALIZATION_H
#define MAPVISUALIZATION_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QWheelEvent>
#include <QMouseEvent>
#include "Graph.h"

class MapVisualization : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString selectedCity READ getSelectedCity WRITE setSelectedCity)
    Q_PROPERTY(QStringList visitedCities READ getVisitedCities WRITE setVisitedCities)
    Q_PROPERTY(QStringList currentPath READ getCurrentPath WRITE setCurrentPath)

public:
    explicit MapVisualization(Graph* graph, QWidget *parent = nullptr);

    void updateMap();
    void setGraph(Graph* graph);

    bool loadCityPositionsFromFile(const QString& filePath);
    bool saveCityPositionsToFile(const QString& filePath);

    void showCityPositionDialog(const QString& cityName);

    void initializeCityPositions();

    QString getSelectedCity() const { return selectedCity; }
    void setSelectedCity(const QString& city) { selectedCity = city; }

    QStringList getVisitedCities() const { return visitedCities; }
    void setVisitedCities(const QStringList& cities) { visitedCities = cities; }

    QStringList getCurrentPath() const { return currentPath; }
    void setCurrentPath(const QStringList& path) { currentPath = path; }

public slots:
    void zoomIn();
    void zoomOut();
    void resetZoom();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Graph* graph;
    QMap<QString, QPointF> cityPositions;
    QPixmap backgroundMap;

    qreal zoomFactor;
    QPointF panOffset;
    bool isPanning;
    QPoint lastPanPosition;

    QString selectedCity;

    QStringList visitedCities;
    QStringList currentPath;

    const int cityRadius = 10;
    const int selectedCityRadius = 12;
    const int visitedCityRadius = 11;
    const QColor cityColor = QColor(50, 150, 200);
    const QColor selectedCityColor = QColor(220, 50, 50);
    const QColor visitedCityColor = QColor(100, 200, 100);
    const QColor cityBorderColor = QColor(20, 100, 150);
    const QColor roadColor = QColor(100, 100, 100, 180);
    const QColor selectedRoadColor = QColor(220, 50, 50);
    const QColor traversedRoadColor = QColor(100, 200, 100);
    const int roadWidth = 3;
    const int selectedRoadWidth = 5;

    void drawCity(QPainter& painter, const QString& cityName, const QPointF& position);
    void drawRoad(QPainter& painter, const QPointF& from, const QPointF& to, double distance);

    QPointF calculateDefaultPosition(const QString& cityName);
    QString findCityAtPosition(const QPoint& pos);

    QPointF normalizedToScreen(const QPointF& normalizedPos);

    QPointF mapToView(const QPointF& mapPoint);
    QPointF viewToMap(const QPointF& viewPoint);

    QPointF convertOldToNewCoordinates(const QPointF& oldCoords);
    QPointF convertNewToOldCoordinates(const QPointF& newCoords);

    bool isEdgeInPath(const QString& fromCity, const QString& toCity) const;
};

#endif

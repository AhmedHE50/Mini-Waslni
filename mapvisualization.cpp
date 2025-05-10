#include "MapVisualization.h"
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <cmath>
#include <QFont>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "CityPositionDialog.h"

MapVisualization::MapVisualization(Graph* graph, QWidget *parent)
    : QWidget(parent),
    graph(graph),
    zoomFactor(1.0),
    panOffset(0, 0),
    isPanning(false),
    selectedCity("")
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(600, 500);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void MapVisualization::updateMap()
{
    // Make sure all cities have positions
    const CityGraph& cities = graph->getAllCities();
    for (auto it = cities.begin(); it != cities.end(); ++it) {
        QString cityName = QString::fromStdString(it->first);
        if (!cityPositions.contains(cityName)) {
            cityPositions[cityName] = calculateDefaultPosition(cityName);
        }
    }

    // Remove positions for deleted cities
    QStringList keysToRemove;
    for (auto it = cityPositions.begin(); it != cityPositions.end(); ++it) {
        if (cities.find(it.key().toStdString()) == cities.end()) {
            keysToRemove.append(it.key());
        }
    }

    for (const QString& key : keysToRemove) {
        cityPositions.remove(key);
    }

    // Trigger a repaint
    update();
}

void MapVisualization::zoomIn()
{
    zoomFactor *= 1.2;
    update();
}

void MapVisualization::zoomOut()
{
    zoomFactor /= 1.2;
    if (zoomFactor < 0.1) zoomFactor = 0.1;
    update();
}

void MapVisualization::resetZoom()
{
    zoomFactor = 1.0;
    panOffset = QPointF(0, 0);
    update();
}

bool MapVisualization::loadCityPositionsFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        return false;
    }

    QJsonObject obj = doc.object();
    cityPositions.clear();

    for (auto it = obj.begin(); it != obj.end(); ++it) {
        if (it.value().isArray()) {
            QJsonArray posArray = it.value().toArray();
            if (posArray.size() == 2) {
                double x = posArray.at(0).toDouble();
                double y = posArray.at(1).toDouble();
                cityPositions[it.key()] = QPointF(x, y);
            }
        }
    }

    // For any cities in the graph that don't have loaded positions
    const CityGraph& cities = graph->getAllCities();
    for (auto it = cities.begin(); it != cities.end(); ++it) {
        QString cityName = QString::fromStdString(it->first);
        if (!cityPositions.contains(cityName)) {
            cityPositions[cityName] = calculateDefaultPosition(cityName);
        }
    }

    update();
    return true;
}

bool MapVisualization::saveCityPositionsToFile(const QString& filePath)
{
    QJsonObject obj;

    for (auto it = cityPositions.begin(); it != cityPositions.end(); ++it) {
        QJsonArray posArray;
        posArray.append(it.value().x());
        posArray.append(it.value().y());
        obj[it.key()] = posArray;
    }

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(data);
    file.close();
    return true;
}

void MapVisualization::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    // Scale the background map to the new size
    if (!backgroundMap.isNull()) {
        QPixmap scaledMap = backgroundMap.scaled(event->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        backgroundMap = scaledMap;
    }
}

void MapVisualization::initializeCityPositions()
{
    // Egypt governorates fixed positions - approximating real geographical locations
    // Converted coordinates from 0.0-1.0 range to -1.0 to 1.0 range
    cityPositions["Alexandria"] = QPointF(-0.6, -0.6);
    cityPositions["Beheira"] = QPointF(-0.5, -0.5);
    cityPositions["Cairo"] = QPointF(-0.1, -0.3);
    cityPositions["Giza"] = QPointF(-0.14, -0.26);
    cityPositions["Qalyubia"] = QPointF(-0.06, -0.36);
    cityPositions["Sharqia"] = QPointF(0.1, -0.4);
    cityPositions["Dakahlia"] = QPointF(0.0, -0.5);
    cityPositions["Kafr El Sheikh"] = QPointF(-0.3, -0.6);
    cityPositions["Gharbia"] = QPointF(-0.2, -0.5);
    cityPositions["Monufia"] = QPointF(-0.2, -0.4);
    cityPositions["Damietta"] = QPointF(0.1, -0.64);
    cityPositions["Port Said"] = QPointF(0.2, -0.64);
    cityPositions["Ismailia"] = QPointF(0.2, -0.5);
    cityPositions["Suez"] = QPointF(0.2, -0.3);
    cityPositions["North Sinai"] = QPointF(0.4, -0.5);
    cityPositions["South Sinai"] = QPointF(0.5, -0.2);
    cityPositions["Red Sea"] = QPointF(0.4, 0.0);
    cityPositions["Faiyum"] = QPointF(-0.3, -0.1);
    cityPositions["Beni Suef"] = QPointF(-0.2, 0.0);
    cityPositions["Minya"] = QPointF(-0.1, 0.1);
    cityPositions["Asyut"] = QPointF(-0.1, 0.3);
    cityPositions["Sohag"] = QPointF(-0.04, 0.4);
    cityPositions["Qena"] = QPointF(0.0, 0.5);
    cityPositions["Luxor"] = QPointF(0.0, 0.6);
    cityPositions["Aswan"] = QPointF(0.04, 0.7);
    cityPositions["New Valley"] = QPointF(-0.4, 0.3);
    cityPositions["Matrouh"] = QPointF(-0.8, -0.4);

    // For any cities in the graph that don't have predefined positions
    const CityGraph& cities = graph->getAllCities();
    for (auto it = cities.begin(); it != cities.end(); ++it) {
        QString cityName = QString::fromStdString(it->first);
        if (!cityPositions.contains(cityName)) {
            cityPositions[cityName] = calculateDefaultPosition(cityName);
        }
    }
}

QPointF MapVisualization::calculateDefaultPosition(const QString& cityName)
{
    // Use a simple hash of the city name to get a pseudo-random but fixed position
    int hash = 0;
    for (QChar c : cityName) {
        hash = (hash * 31 + c.unicode()) % 997;
    }

    // Generate a position directly in the -0.9 to 0.9 range to avoid edges
    // Using 90% of the range to keep cities away from the extreme edges
    qreal x = -0.9 + (hash % 180) / 100.0;
    qreal y = -0.9 + ((hash / 180) % 180) / 100.0;

    return QPointF(x, y);
}

void MapVisualization::showCityPositionDialog(const QString& cityName)
{
    CityPositionDialog dialog(cityName, this);

    // Set initial values if the city already has a position
    if (cityPositions.contains(cityName)) {
        // Set the spinboxes to the current position
        QPointF pos = cityPositions[cityName];
        // FIXED: Using setters instead of trying to assign to getters
        dialog.setXPosition(pos.x());
        dialog.setYPosition(pos.y());
    }

    if (dialog.exec() == QDialog::Accepted) {
        // Get the new position and update the map
        double x = dialog.getXPosition();
        double y = dialog.getYPosition();
        cityPositions[cityName] = QPointF(x, y);
        update();
    }
}

void MapVisualization::wheelEvent(QWheelEvent* event)
{
    // Zoom in or out based on wheel direction
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;

        // Get the position before zoom to maintain the point under cursor
        QPointF mousePos = event->position();

        // Calculate zoom factor change
        qreal oldZoom = zoomFactor;
        if (numSteps.y() > 0) {
            zoomFactor *= 1.1;
        } else {
            zoomFactor /= 1.1;
            if (zoomFactor < 0.1) zoomFactor = 0.1;
        }

        // Adjust pan offset to keep the point under the cursor stable
        QPointF center(width() / 2.0, height() / 2.0);
        QPointF relativePos = mousePos - center;
        panOffset += relativePos * (1.0/oldZoom - 1.0/zoomFactor);
    }

    event->accept();
    update();
}

void MapVisualization::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        // Check if a city was clicked
        QString clickedCity = findCityAtPosition(event->pos());
        if (!clickedCity.isEmpty()) {
            // Toggle selection or select a new city
            if (selectedCity == clickedCity) {
                selectedCity = "";
            } else {
                selectedCity = clickedCity;
            }
            update();
        } else {
            // Start panning
            isPanning = true;
            lastPanPosition = event->pos();
            setCursor(Qt::ClosedHandCursor);
        }
    }
}

void MapVisualization::mouseMoveEvent(QMouseEvent* event)
{
    if (isPanning) {
        // Calculate delta in screen coordinates
        QPointF delta = event->pos() - lastPanPosition;

        // Scale delta by a factor inverse to the zoom level
        // This makes panning slower when zoomed in and faster when zoomed out
        panOffset += delta / zoomFactor;

        // Update last position
        lastPanPosition = event->pos();
        update();
    }
}

void MapVisualization::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && isPanning) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
    }
}

QPointF MapVisualization::mapToView(const QPointF& mapPoint)
{
    // Convert from normalized map coordinates (-1 to 1) to view coordinates with zoom and pan
    QPointF center(width() / 2.0, height() / 2.0);

    // Scale directly from the -1,1 range to screen coordinates
    // This gives us the full screen area to work with
    QPointF scaled((mapPoint.x() * width() / 2.0), (mapPoint.y() * height() / 2.0));

    // Apply the center offset
    return center + scaled;
}

QPointF MapVisualization::viewToMap(const QPointF& viewPoint)
{
    // Convert from view coordinates to normalized map coordinates (-1 to 1)
    QPointF center(width() / 2.0, height() / 2.0);
    QPointF centered = viewPoint - center;

    // Scale to the -1,1 range
    return QPointF(centered.x() / (width() / 2.0), centered.y() / (height() / 2.0));
}

QPointF MapVisualization::convertOldToNewCoordinates(const QPointF& oldCoords)
{
    // Convert from 0-1 range to -1 to 1 range
    return QPointF(oldCoords.x() * 2.0 - 1.0, oldCoords.y() * 2.0 - 1.0);
}

QPointF MapVisualization::convertNewToOldCoordinates(const QPointF& newCoords)
{
    // Convert from -1 to 1 range to 0-1 range
    return QPointF((newCoords.x() + 1.0) / 2.0, (newCoords.y() + 1.0) / 2.0);
}

QString MapVisualization::findCityAtPosition(const QPoint& pos)
{
    // Find the nearest city within the click radius
    const double clickRadius = cityRadius * 1.5; // Slightly larger radius for easier clicking

    QString nearestCity;
    double minDistance = clickRadius;

    for (auto it = cityPositions.begin(); it != cityPositions.end(); ++it) {
        // Convert normalized position to screen position
        QPointF screenPos = normalizedToScreen(it.value());

        // Calculate distance
        double dx = pos.x() - screenPos.x();
        double dy = pos.y() - screenPos.y();
        double distance = std::sqrt(dx*dx + dy*dy);

        if (distance < minDistance) {
            minDistance = distance;
            nearestCity = it.key();
        }
    }

    return nearestCity;
}

void MapVisualization::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill with white background
    painter.fillRect(rect(), Qt::white);

    // Draw roads first (so they're under the cities)
    const CityGraph& cities = graph->getAllCities();
    for (auto it = cities.begin(); it != cities.end(); ++it) {
        QString fromCity = QString::fromStdString(it->first);
        if (!cityPositions.contains(fromCity)) continue;

        // Convert the normalized coordinates to screen coordinates
        QPointF fromPos = normalizedToScreen(cityPositions[fromCity]);

        // Draw roads to all neighbors
        for (const auto& neighbor : it->second) {
            QString toCity = QString::fromStdString(neighbor.first);
            if (!cityPositions.contains(toCity)) continue;

            // Convert the normalized coordinates to screen coordinates
            QPointF toPos = normalizedToScreen(cityPositions[toCity]);

            drawRoad(painter, fromPos, toPos, neighbor.second);
        }
    }

    // Draw cities
    for (auto it = cityPositions.begin(); it != cityPositions.end(); ++it) {
        // Convert the normalized coordinates to screen coordinates
        QPointF screenPos = normalizedToScreen(it.value());
        drawCity(painter, it.key(), screenPos);
    }
}

QPointF MapVisualization::normalizedToScreen(const QPointF& normalizedPos)
{
    // First convert to 0,1 range
    qreal screenX = (normalizedPos.x() + 1.0) / 2.0;
    qreal screenY = (normalizedPos.y() + 1.0) / 2.0;

    // Scale to widget size
    screenX = screenX * width();
    screenY = screenY * height();

    // Apply zoom and pan
    QPointF center(width() / 2.0, height() / 2.0);
    QPointF relativePos = QPointF(screenX, screenY) - center;
    return center + relativePos * zoomFactor + panOffset * zoomFactor;
}

void MapVisualization::drawCity(QPainter& painter, const QString& cityName, const QPointF& pos)
{
    // Draw city circle - change color based on city state
    painter.setPen(QPen(cityBorderColor, 2));

    if (cityName == selectedCity) {
        // Currently selected/active city in the traversal
        painter.setBrush(selectedCityColor);
        painter.drawEllipse(pos, selectedCityRadius, selectedCityRadius);
    }
    else if (visitedCities.contains(cityName)) {
        // City that has been visited in the traversal
        painter.setBrush(visitedCityColor);
        painter.drawEllipse(pos, visitedCityRadius, visitedCityRadius);
    }
    else {
        // Regular unvisited city
        painter.setBrush(cityColor);
        painter.drawEllipse(pos, cityRadius, cityRadius);
    }

    // Draw city name
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(8);
    painter.setFont(font);

    QRectF textRect(pos.x() - 50, pos.y() + cityRadius, 100, 20);
    painter.drawText(textRect, Qt::AlignHCenter, cityName);
}

void MapVisualization::drawRoad(QPainter& painter, const QPointF& from, const QPointF& to, double distance)
{
    QString fromCity = findCityAtPosition(QPoint(from.x(), from.y()));
    QString toCity = findCityAtPosition(QPoint(to.x(), to.y()));

    // Check if this road is part of the traversal path
    bool isTraversedRoad = isEdgeInPath(fromCity, toCity);

    // Draw the road with appropriate color and width
    if (isTraversedRoad) {
        painter.setPen(QPen(traversedRoadColor, selectedRoadWidth));
    } else {
        painter.setPen(QPen(roadColor, roadWidth));
    }

    painter.drawLine(from, to);

    // Draw distance label in the middle of the road
    QPointF midPoint = (from + to) / 2.0;

    // Create a subtle background for the text
    QString distanceText = QString::number(distance, 'f', 0);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(distanceText);
    int textHeight = fm.height();

    QRectF bgRect(midPoint.x() - textWidth/2 - 3, midPoint.y() - textHeight/2, textWidth + 6, textHeight);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255, 180));
    painter.drawRoundedRect(bgRect, 3, 3);

    // Draw the text
    painter.setPen(Qt::black);
    painter.drawText(midPoint.x() - textWidth/2, midPoint.y() + textHeight/3, distanceText);
}

bool MapVisualization::isEdgeInPath(const QString& fromCity, const QString& toCity) const
{
    // First check the explicit edge list in currentPath (original implementation)
    if (!currentPath.isEmpty() && currentPath.size() % 2 == 0) {
        // Check every pair of cities in the path
        for (int i = 0; i < currentPath.size() - 1; i += 2) {
            QString pathFrom = currentPath[i];
            QString pathTo = currentPath[i + 1];

            // Check both directions since the edge is undirected
            if ((pathFrom == fromCity && pathTo == toCity) ||
                (pathFrom == toCity && pathTo == fromCity)) {
                return true;
            }
        }

        // If currentPath is specified, only use that to determine edges
        // and don't fall through to the visitedCities check
        return false;
    }

    // Only if no explicit path is provided, check if these cities are consecutive in visitedCities
    if (visitedCities.contains(fromCity) && visitedCities.contains(toCity)) {
        // Check if they are consecutive in the visitedCities list
        int fromIndex = visitedCities.indexOf(fromCity);
        int toIndex = visitedCities.indexOf(toCity);

        // If they are adjacent in the path (abs difference of 1)
        if (abs(fromIndex - toIndex) == 1) {
            return true;
        }
    }

    return false;
}

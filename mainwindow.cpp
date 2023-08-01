#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "distancebearing.h"
#include "annotations.h"
#include "waypoints.h"

// LocalBase Map
#include "Basemap.h"
#include "Map.h"
#include "MapView.h"
#include "MapGraphicsView.h"

// Coordinate Points
#include "Graphic.h"
#include "CoordinateFormatter.h"
#include "Point.h"
#include <PictureMarkerSymbol.h>

// Annotations & WayPoints
#include "SimpleFillSymbol.h"
#include "SimpleMarkerSymbol.h"
#include "SketchEditor.h"

// Distance Bearing
#include "SimpleLineSymbol.h"
#include "GeometryEngine.h"
#include "SpatialReference.h"
#include "PolylineBuilder.h"

// Formatters
#include <QDebug>
#include <QMessageBox>
#include <QFormLayout>

// Raster Layer
#include "Raster.h"
#include <RasterLayer.h>

// Multicast Receiver
#include <QtWidgets>
#include <QtNetwork>

// Database
#include <QSql>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>


using namespace Esri::ArcGISRuntime;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      groupAddress4(QStringLiteral("192.168.122.1")),
      groupAddress6(QStringLiteral("ff12::2115"))
{
    ui->setupUi(this);

    localBaseMap();
    pointCoordinates();
    annotationSketch();
    waypointSketch();
    multicastPlatform();
    databaseManagement();
}

MainWindow::~MainWindow()
{
    if(m_map)
    {
        delete m_map;
        m_map = 0;
    }

    if(m_mapView)
    {
        delete m_mapView;
        m_mapView = 0;
    }

    if(m_mapGrid)
    {
        delete m_mapGrid;
        m_mapGrid = 0;
    }

    if(m_pointGraphic)
    {
        delete m_pointGraphic;
        m_pointGraphic = 0;
    }

    if(m_pointGraphicsOverlay)
    {
        delete m_pointGraphicsOverlay;
        m_pointGraphicsOverlay = 0;
    }

    if(m_antSketchGraphic)
    {
        delete m_antSketchGraphic;
        m_antSketchGraphic = 0;
    }

    if(m_antGraphicsOverlay)
    {
        delete m_antGraphicsOverlay;
        m_antGraphicsOverlay = 0;
    }

    if(m_antSketchEditor)
    {
        delete m_antSketchEditor;
        m_antSketchEditor = 0;
    }

    if(m_antMultiPointSymbol)
    {
        delete m_antMultiPointSymbol;
        m_antMultiPointSymbol = 0;
    }

    if(m_wptSketchGraphic)
    {
        delete m_wptSketchGraphic;
        m_wptSketchGraphic = 0;
    }

    if(m_wptPathGraphics)
    {
        delete m_wptPathGraphics;
        m_wptPathGraphics = 0;
    }

    if(m_wptGraphicsOverlay)
    {
        delete m_wptGraphicsOverlay;
        m_wptGraphicsOverlay = 0;
    }

    if(m_wptSketchEditor)
    {
        delete m_wptSketchEditor;
        m_wptSketchEditor = 0;
    }

    if(m_wptMultiPointSymbol)
    {
        delete m_wptMultiPointSymbol;
        m_wptMultiPointSymbol = 0;
    }

    if(m_wptLineSymbol)
    {
        delete m_wptLineSymbol;
        m_wptLineSymbol = 0;
    }

    if(pp_destinationGraphic)
    {
        delete pp_destinationGraphic;
        pp_destinationGraphic = 0;
    }

    if(pp_pathGraphic)
    {
        delete pp_pathGraphic;
        pp_pathGraphic = 0;
    }

    if(pp_graphicsOverlay)
    {
        delete pp_graphicsOverlay;
        pp_graphicsOverlay = 0;
    }

    if(tp_nycGraphic)
    {
        delete tp_nycGraphic;
        tp_nycGraphic = 0;
    }

    if(tp_destinationGraphic)
    {
        delete tp_destinationGraphic;
        tp_destinationGraphic = 0;
    }

    if(tp_pathGraphic)
    {
        delete tp_pathGraphic;
        tp_pathGraphic = 0;
    }

    if(tp_graphicsOverlay)
    {
        delete tp_graphicsOverlay;
        tp_graphicsOverlay = 0;
    }

    if(me_nycGraphic)
    {
        delete me_nycGraphic;
        me_nycGraphic = 0;
    }

    if(me_destinationGraphic)
    {
        delete me_destinationGraphic;
        me_destinationGraphic = 0;
    }

    if(me_pathGraphic)
    {
        delete me_pathGraphic;
        me_pathGraphic = 0;
    }

    if(me_graphicsOverlay)
    {
        delete me_graphicsOverlay;
        me_graphicsOverlay = 0;
    }

    if(p_platformGraphics)
    {
        delete p_platformGraphics;
        p_platformGraphics = 0;
    }

    if(p_platGraphicsOverlay)
    {
        delete p_platGraphicsOverlay;
        p_platGraphicsOverlay = 0;
    }

    if(p_plfPathGraphics)
    {
        delete p_plfPathGraphics;
        p_plfPathGraphics = 0;
    }

    if(p_plfPathGraphicsOverlay)
    {
        delete p_plfPathGraphicsOverlay;
        p_plfPathGraphicsOverlay = 0;
    }

    QSqlQuery query;
    query.exec("drop table platform_path");
    query.exec("drop table annotations");
    query.exec("drop table waypoints");

    delete ui;
}


/******************************************************************************/


/*** BASEMAP ***/

void MainWindow::localBaseMap()
{
    // Create a map Local BaseMap
    m_map = new Map();

    //Adding Raster Layers
    Raster *raster = new Raster("/home/niraj/MAP/MapApplication/Base_Map_India.tif");
    RasterLayer *rasterLayer = new RasterLayer(raster);
    m_map->operationalLayers()->append(rasterLayer);

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);

    // Set map to map view
    m_mapView->setMap(m_map);

    // set the mapView as the central widget
    ui->m_gridLayoutMap->addWidget(m_mapView);
    //    setCentralWidget(m_mapView);

    //Setting initial map scale
    m_mapView->setViewpointScale(10000000);

    // Create mapGrid for LatLong GridView
    m_mapGrid = new LatitudeLongitudeGrid(this);
}


/******************************************************************************/


/*** FORMAT COORDINATES ***/

void MainWindow::pointCoordinates()
{
    // Create a GraphicsOverlay for point
    m_pointGraphicsOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(m_pointGraphicsOverlay);

    // create a graphic
    SimpleMarkerSymbol* symbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), 5.0, this);

    //Create Graphic for point
    m_pointGraphic = new Graphic(this);
    m_pointGraphic->setSymbol(symbol);

    // and add insert the graphic
    m_pointGraphicsOverlay->graphics()->append(m_pointGraphic);

    // connect to the mouse clicked signal on the MapGraphicsView
    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
            // get the point from the mouse point
            Point qptPoint = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());

            m_pointGraphic->setVisible(true);

            // using the point, refresh the graphic and the text
            handleLocationUpdate(std::move(qptPoint));
    });
}

// handle case where user clicked on the map
void MainWindow::handleLocationUpdate(Point point)
{
    if (!point.isEmpty())
    {
        m_pointGraphic->setGeometry(point);
        setTextFromPoint(point);
    }
}

// Point To Text
void MainWindow::setTextFromPoint(Point point)
{
    // last parm = decimal places
    m_coordinatesInDD = CoordinateFormatter::toLatitudeLongitude(point, LatitudeLongitudeFormat::DecimalDegrees, 6);
    ui->m_leDegrees->setText(m_coordinatesInDD);

    // last parm = decimal places
    m_coordinatesInDMS = CoordinateFormatter::toLatitudeLongitude(point, LatitudeLongitudeFormat::DegreesMinutesSeconds, 1);
    ui->m_leDMS->setText(m_coordinatesInDMS);

    //! [MainWindow CoordinateFormatter point to USNG]
    int decimalPlaces = 5;
    bool addSpaces = true;
    m_coordinatesInUsng = CoordinateFormatter::toUsng(point, decimalPlaces, addSpaces);
    ui->m_leUsng->setText(m_coordinatesInUsng);

    // last parm = add spaces
    m_coordinatesInUtm = CoordinateFormatter::toUtm(point, UtmConversionMode::LatitudeBandIndicators, true);
    ui->m_leUtm->setText(m_coordinatesInUtm);
}

// Text To Point
void MainWindow::on_m_leDegrees_textEdited(const QString &)
{
    Point point = CoordinateFormatter::fromLatitudeLongitude(ui->m_leDegrees->text(), m_map->spatialReference());
    handleLocationUpdate(std::move(point));
}

void MainWindow::on_m_leDMS_textEdited(const QString &)
{
    Point point = CoordinateFormatter::fromLatitudeLongitude(ui->m_leDMS->text(), m_map->spatialReference());
    handleLocationUpdate(std::move(point));
}

void MainWindow::on_m_leUtm_textEdited(const QString &)
{
    Point point = CoordinateFormatter::fromUsng(ui->m_leUsng->text(), m_map->spatialReference());
    handleLocationUpdate(std::move(point));
}

void MainWindow::on_m_leUsng_textEdited(const QString &)
{
    Point point = CoordinateFormatter::fromUtm(ui->m_leUtm->text(), m_map->spatialReference(), UtmConversionMode::LatitudeBandIndicators);
    handleLocationUpdate(std::move(point));
}


/********************************************************************/


/*** ANNOTATIONS ***/

void MainWindow::annotationSketch()
{
    // Create a GraphicsOverlay to save the annotations to
    m_antGraphicsOverlay = new GraphicsOverlay(this);

    m_antMultiPointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Square, QColor(0, 0, 255), 10, this);

    m_mapView->graphicsOverlays()->append(m_antGraphicsOverlay);

    m_antSketchEditor = new SketchEditor(this);
}

int antCount = 0;
void MainWindow::on_actionAdd_Annotations_triggered()
{
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(true);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    resetTools();

    // Instructions
    QMessageBox::information(this, "Instructions", "Left Click : Add Annotations\nRight Click : Discard Annotations\nEnter Key : Save Annotations");

    // Set SketchEditor to sketch Annotations
    m_mapView->setSketchEditor(m_antSketchEditor);

    //Start annotation sketch editor
    m_antSketchEditor->start(SketchCreationMode::Multipoint);
    antCount++;

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::LeftButton)
        {
            if (m_antSketchEditor->isStarted())
            {
                // Storing Annotations into Database
                QSqlQuery query;
                query.exec("insert into annotations(ID, Coordinates) values(" + QString::number(antCount) + ", '" + m_coordinatesInDD + "')");
            }
        }
        if(mouseEvent.button() == Qt::RightButton)
        {
            if (m_antSketchEditor->isStarted())
            {
                m_antSketchEditor->stop();
                ui->AnnotationToolbar->setChecked(false);
                QSqlQuery query;
                query.exec("delete from annotations where ID = " + QString::number(antCount--));
                return;
            }
        }
    });

    connect(m_mapView, &MapGraphicsView::keyPressed, this, [this](QKeyEvent& keyEvent)
    {
        if(keyEvent.key() == Qt::Key_Enter)
        {
            if (!m_antSketchEditor->isSketchValid())
            {
                qWarning() << "Unable to save sketch. Sketch is not valid.";
                return;
             }

            // To save the sketch, create a graphic with the sketch's geometry before stopping the sketchEditor
            Geometry sketchGeometry = m_antSketchEditor->geometry();

            m_antSketchGraphic = new Graphic(sketchGeometry, m_antMultiPointSymbol, this);
            m_antGraphicsOverlay->graphics()->append(m_antSketchGraphic);
            m_antSketchEditor->stop();
            ui->AnnotationToolbar->setChecked(false);
        }
    });
}

void MainWindow::on_action_Show_All_triggered()
{
    // Create Annotations Table Model
    antTableModel = new QSqlTableModel();
    antTableModel->setTable("annotations");
    antTableModel->select();
    antTableModel->sort(0, Qt::AscendingOrder);

    QVariant Coordinate;
    int i;

    for(i=1; i<=antTableModel->rowCount(); i++)
    {
        // Create Graphic for Annotations
        m_antSketchGraphic = new Graphic();
        m_antSketchGraphic->setSymbol(m_antMultiPointSymbol);
        m_antGraphicsOverlay->graphics()->append(m_antSketchGraphic);

        Coordinate = antTableModel->data(antTableModel->index(i, 1));
        Point antPoint = CoordinateFormatter::fromLatitudeLongitude(Coordinate.toString(), m_map->spatialReference());
        m_antSketchGraphic->setGeometry(antPoint);
    }
}

void MainWindow::on_action_Hide_All_triggered()
{
    m_antGraphicsOverlay->graphics()->clear();
}

void MainWindow::on_action_Manage_triggered()
{
    Annotations antTableModel;
    antTableModel.exec();
}


/********************************************************************/


/*** WAYPOINTS ***/

void MainWindow::waypointSketch()
{
    // Create a GraphicsOverlay to save the waypoints to
    m_wptGraphicsOverlay = new GraphicsOverlay(this);

    m_wptMultiPointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor("red"), 10, this);

    m_wptLineSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor(144, 238, 144), 3, this);

    m_mapView->graphicsOverlays()->append(m_wptGraphicsOverlay);

    m_wptSketchEditor = new SketchEditor(this);
}

int wptCount = 0;
int wptPoint;
void MainWindow::on_actionAdd_Waypoints_triggered()
{
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(true);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    resetTools();

    // Instructions
    QMessageBox::information(this, "Instructions", "Left Click : Add Waypoints\nRight Click : Discard Waypoints\nEnter Key : Save Waypoints");

    // Set SketchEditor to sketch WayPoints
    m_mapView->setSketchEditor(m_wptSketchEditor);

    //Start waypoint sketch editor
    m_wptSketchEditor->start(SketchCreationMode::Polyline);
    wptCount++;
    wptPoint = 1;

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::LeftButton)
        {
            if (m_wptSketchEditor->isStarted())
            {
                // Storing Annotations into Database
                QSqlQuery query;
                query.exec("insert into waypoints(ID, Points, Coordinates) values(" + QString::number(wptCount) +  "," + QString::number(wptPoint++) + ", '" + m_coordinatesInDD + "')");
            }
        }
        if(mouseEvent.button() == Qt::RightButton)
        {
            if (m_wptSketchEditor->isStarted())
            {
                m_wptSketchEditor->stop();
                ui->WaypointsToolbar->setChecked(false);
                QSqlQuery query;
                query.exec("delete from waypoints where ID = " + QString::number(wptCount--));
                return;
            }
        }
    });

    connect(m_mapView, &MapGraphicsView::keyPressed, this, [this](QKeyEvent& keyEvent)
    {
        if(keyEvent.key() == Qt::Key_Enter)
        {
            if (!m_wptSketchEditor->isSketchValid())
            {
                qWarning() << "Unable to save sketch. Sketch is not valid.";
                return;
            }

            // To save the sketch, create a graphic with the sketch's geometry before stopping the sketchEditor
            Geometry sketchGeometry = m_wptSketchEditor->geometry();

            m_wptSketchGraphic = new Graphic(sketchGeometry, m_wptLineSymbol, this);
            m_wptGraphicsOverlay->graphics()->append(m_wptSketchGraphic);
            m_wptSketchEditor->stop();
            ui->WaypointsToolbar->setChecked(false);
        }
    });
}

void MainWindow::on_action_Show_All_2_triggered()
{
    // Create WayPoints Table Model
    wptTableModel = new QSqlTableModel();
    wptTableModel->setTable("waypoints");
    wptTableModel->select();
    wptTableModel->sort(0, Qt::AscendingOrder);

    QVariant Coordinate;
    int i, j, count=0;

    for(i=1; i<=wptCount; i++)
    {
        for(j=1; wptTableModel->data(wptTableModel->index(count, 0)).toInt() == i ; j++)
        {
            // Create Graphic for WayPoints
            m_wptSketchGraphic = new Graphic();
            m_wptSketchGraphic->setSymbol(m_wptMultiPointSymbol);
            m_wptGraphicsOverlay->graphics()->append(m_wptSketchGraphic);

            Coordinate = wptTableModel->data(wptTableModel->index(count, 2));
            Point wptPointA = CoordinateFormatter::fromLatitudeLongitude(Coordinate.toString(), m_map->spatialReference());
            m_wptSketchGraphic->setGeometry(wptPointA);

            if(j>1)
            {
                // Create path graphic
                m_wptPathGraphics = new Graphic(this);
                m_wptPathGraphics->setSymbol(m_wptLineSymbol);
                m_wptGraphicsOverlay->graphics()->append(m_wptPathGraphics);

                Coordinate = wptTableModel->data(wptTableModel->index(count-1, 2));
                Point wptPointB = CoordinateFormatter::fromLatitudeLongitude(Coordinate.toString(), m_map->spatialReference());
                // create line with start/end points
                const QList<Point> points = {wptPointA, wptPointB};
                const Polyline polyline = pointsToPolyline(points);

                // densify the path as a geodesic curve and show it with the path graphic
                constexpr double maxSegmentLength = 1.0;
                const LinearUnit unitOfMeasurement(LinearUnitId::Miles);
                constexpr GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
                const Geometry wptGeometry = GeometryEngine::densifyGeodetic(polyline, maxSegmentLength, unitOfMeasurement, curveType);

                // update the graphic
                m_wptPathGraphics->setGeometry(wptGeometry);
            }
            count++;
        }
    }
}

void MainWindow::on_action_Hide_All_2_triggered()
{
    m_wptGraphicsOverlay->graphics()->clear();
}

void MainWindow::on_action_Manage_2_triggered()
{
    Waypoints wptTableModel;
    wptTableModel.exec();
}


/*********************************************************************/


/*** DISTANCE & BEARING ***/

Point g_platform;
Point g_destination;

bool disUpdate = false;

void MainWindow::on_actionBetween_Platform_Point_triggered()
{
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(true);
    ui->DBTPToolButton->setChecked(false);

    resetTools();

    disUpdate = true;
    Point platform = g_platform;

    // Create a GraphicsOverlay
    pp_graphicsOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(pp_graphicsOverlay);

    // Create Graphic Symbols
    const QUrl pp_destinationSymbolUrl("qrc:/res/pin_circle_red.png");
    PictureMarkerSymbol* pp_destinationSymbol = new PictureMarkerSymbol(pp_destinationSymbolUrl, this);
    pp_destinationSymbol->setHeight(20);
    pp_destinationSymbol->setWidth(20);
    SimpleLineSymbol* pathSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Dash, QColor("blue"), 5.0f /*width*/, this);

    // Create destination graphic
    pp_destinationGraphic = new Graphic(this);
    pp_destinationGraphic->setSymbol(pp_destinationSymbol);
    pp_graphicsOverlay->graphics()->append(pp_destinationGraphic);

    // Create path graphic
    pp_pathGraphic = new Graphic(this);
    pp_pathGraphic->setSymbol(pathSymbol);
    pp_graphicsOverlay->graphics()->append(pp_pathGraphic);

    // connect to mouse clicked signal
    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this, platform](QMouseEvent& mouseEvent)
    {
        // re-project the point to match the NYC graphic
        const Point clickedPoint = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());
        const Point pp_destination = GeometryEngine::project(clickedPoint, p_platformGraphics->geometry().spatialReference());

        if(mouseEvent.button() == Qt::LeftButton && ui->DBPPToolButton->isChecked())
        {
            // update the destination graphic
            pp_destinationGraphic->setGeometry(pp_destination);
            g_destination = pp_destination;
        }

        if(mouseEvent.button() == Qt::RightButton)
        {
            pp_graphicsOverlay->graphics()->clear();
            disUpdate = false;
            ui->m_ledistance->setText(NULL);
            ui->DBPPToolButton->setChecked(false);
            return;
        }
    });
}

// helper function to create a new Polyline from a list of points
Polyline MainWindow::pointsToPolyline(const QList<Point>& points)
{
    PolylineBuilder polylineBuilder(SpatialReference::wgs84());
    for (const Point& point : points)
        polylineBuilder.addPoint(point);

    return polylineBuilder.toPolyline();
}

QString MainWindow::distanceText() const
{
    return m_distanceText;
}


void MainWindow::on_actionBetween_Two_Points_triggered()
{
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(true);

    resetTools();

    // Instructions
    QMessageBox::information(this, "Instructions", "Point A : Double Click\nPoint B : Single Click");

    // Create a GraphicsOverlay
    tp_graphicsOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(tp_graphicsOverlay);

    // Create Graphic Symbols
    const QUrl tp_markerSymbolUrl("qrc:/res/pinA.png");
    PictureMarkerSymbol* tp_markerSymbol = new PictureMarkerSymbol(tp_markerSymbolUrl, this);
    tp_markerSymbol->setHeight(40);
    tp_markerSymbol->setWidth(40);

    const QUrl tp_destinationSymbolUrl("qrc:/res/pinB.png");
    PictureMarkerSymbol* tp_destinationSymbol = new PictureMarkerSymbol(tp_destinationSymbolUrl, this);
    tp_destinationSymbol->setHeight(40);
    tp_destinationSymbol->setWidth(40);

    SimpleLineSymbol* pathSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Dash, QColor("blue"), 5.0f /*width*/, this);

    // Create nyc graphic
    tp_nycGraphic = new Graphic(this);
    tp_nycGraphic->setSymbol(tp_markerSymbol);
    tp_graphicsOverlay->graphics()->append(tp_nycGraphic);

    // Create destination graphic
    tp_destinationGraphic = new Graphic(this);
    tp_destinationGraphic->setSymbol(tp_destinationSymbol);
    tp_graphicsOverlay->graphics()->append(tp_destinationGraphic);

    // Create path graphic
    tp_pathGraphic = new Graphic(this);
    tp_pathGraphic->setSymbol(pathSymbol);
    tp_graphicsOverlay->graphics()->append(tp_pathGraphic);

    connect(m_mapView, &MapGraphicsView::mouseDoubleClicked, this, [this](QMouseEvent& mouseEvent)
    {
        // get the point from the mouse point
        Point tp_nycPoint = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());

        if(mouseEvent.button() == Qt::LeftButton && ui->DBTPToolButton->isChecked())
            tp_nycGraphic->setGeometry(tp_nycPoint);

        // connect to mouse clicked signal
        connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this, tp_nycPoint](QMouseEvent& mouseEvent)
        {
            // re-project the point to match the NYC graphic
            const Point clickedPoint = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());
            const Point tp_destination = GeometryEngine::project(clickedPoint, tp_nycGraphic->geometry().spatialReference());

            // create line with start/end points
            const QList<Point> points = {tp_nycPoint, tp_destination};
            const Polyline polyline = pointsToPolyline(points);

            // densify the path as a geodesic curve and show it with the path graphic
            constexpr double maxSegmentLength = 1.0;
            const LinearUnit unitOfMeasurement(LinearUnitId::Miles);
            constexpr GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
            const Geometry pathGeometry = GeometryEngine::densifyGeodetic(polyline, maxSegmentLength, unitOfMeasurement, curveType);

            if(mouseEvent.button() == Qt::LeftButton && ui->DBTPToolButton->isChecked())
            {
                // update the graphic
                tp_destinationGraphic->setGeometry(tp_destination);
                tp_pathGraphic->setGeometry(pathGeometry);

                // calculate the path's geodetic length
                m_distanceText = QString::number(GeometryEngine::lengthGeodetic(pathGeometry, unitOfMeasurement, curveType), 'f', 2);
                ui->m_ledistance->setText(m_distanceText + " Miles");
            }

            connect(m_mapView, &MapGraphicsView::mouseDoubleClicked, this, [this, tp_destination](QMouseEvent& mouseEvent)
            {
                // get the point from the mouse point
                Point nycPoint = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());

                // create line with start/end points
                const QList<Point> points = {nycPoint, tp_destination};
                const Polyline polyline = pointsToPolyline(points);

                // densify the path as a geodesic curve and show it with the path graphic
                constexpr double maxSegmentLength = 1.0;
                const LinearUnit unitOfMeasurement(LinearUnitId::Miles);
                constexpr GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
                const Geometry pathGeometry = GeometryEngine::densifyGeodetic(polyline, maxSegmentLength, unitOfMeasurement, curveType);
                if(mouseEvent.button() == Qt::LeftButton && ui->DBTPToolButton->isChecked())
                {
                    // update the graphic
                    tp_nycGraphic->setGeometry(nycPoint);
                    tp_pathGraphic->setGeometry(pathGeometry);

                    // calculate the path's geodetic length
                    m_distanceText = QString::number(GeometryEngine::lengthGeodetic(pathGeometry, unitOfMeasurement, curveType), 'f', 2);
                    ui->m_ledistance->setText(m_distanceText + " Miles");
                }
            });
        });
    });

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::RightButton)
        {
            tp_graphicsOverlay->graphics()->clear();
            ui->m_ledistance->setText(NULL);
            ui->DBTPToolButton->setChecked(false);
            return;
        }
    });
}

void MainWindow::on_action_Manual_Entry_triggered()
{
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    resetTools();

    DistanceBearing disBearing;
    disBearing.setModal(true);
    disBearing.exec();

    // Create a GraphicsOverlay
    me_graphicsOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(me_graphicsOverlay);

    // Create Graphic Symbols
    const QUrl me_markerSymbolUrl("qrc:/res/pinA.png");
    PictureMarkerSymbol* me_markerSymbol = new PictureMarkerSymbol(me_markerSymbolUrl, this);
    me_markerSymbol->setHeight(40);
    me_markerSymbol->setWidth(40);

    const QUrl destinationSymbolUrl("qrc:/res/pinB.png");
    PictureMarkerSymbol* destinationSymbol = new PictureMarkerSymbol(destinationSymbolUrl, this);
    destinationSymbol->setHeight(40);
    destinationSymbol->setWidth(40);

    SimpleLineSymbol* pathSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Dash, QColor("blue"), 5.0f /*width*/, this);

    // Create nyc graphic
    me_nycGraphic = new Graphic(this);
    me_nycGraphic->setSymbol(me_markerSymbol);
    me_graphicsOverlay->graphics()->append(me_nycGraphic);

    // Create destination graphic
    me_destinationGraphic = new Graphic(this);
    me_destinationGraphic->setSymbol(destinationSymbol);
    me_graphicsOverlay->graphics()->append(me_destinationGraphic);

    // Create path graphic
    me_pathGraphic = new Graphic(this);
    me_pathGraphic->setSymbol(pathSymbol);
    me_graphicsOverlay->graphics()->append(me_pathGraphic);

    // re-project the point to match the NYC graphic
    const Point me_PointA = CoordinateFormatter::fromLatitudeLongitude(disBearing.d_pointA, m_map->spatialReference());
    me_nycGraphic->setGeometry(me_PointA);
    const Point me_PointB = CoordinateFormatter::fromLatitudeLongitude(disBearing.d_pointB, m_map->spatialReference());
    me_destinationGraphic->setGeometry(me_PointB);

    // create line with start/end points
    const QList<Point> points = {me_PointA, me_PointB};
    const Polyline polyline = pointsToPolyline(points);

    // densify the path as a geodesic curve and show it with the path graphic
    constexpr double maxSegmentLength = 1.0;
    const LinearUnit unitOfMeasurement(LinearUnitId::Miles);
    constexpr GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
    const Geometry pathGeometry = GeometryEngine::densifyGeodetic(polyline, maxSegmentLength, unitOfMeasurement, curveType);

    // update the graphic
    me_pathGraphic->setGeometry(pathGeometry);

    // calculate the path's geodetic length
    m_distanceText = QString::number(GeometryEngine::lengthGeodetic(pathGeometry, unitOfMeasurement, curveType), 'f', 2);
    ui->m_ledistance->setText(m_distanceText + " Miles");

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::RightButton)
        {
            me_graphicsOverlay->graphics()->clear();
            ui->m_ledistance->setText(NULL);
            return;
        }
    });
}

void MainWindow::on_action_Remove_Line_triggered()
{
    if(pp_pathGraphic)
        pp_pathGraphic->setVisible(false);

    if(tp_pathGraphic)
        tp_pathGraphic->setVisible(false);

    if(me_pathGraphic)
        me_pathGraphic->setVisible(false);
}


/*****************************************************************/


/*** ZOOM LEVEL ***/

void MainWindow::on_Zoom_In_triggered()
{
    m_mapView->setViewpointScale(m_mapView->mapScale() - 10000000);
}

void MainWindow::on_Zoom_Out_triggered()
{
    m_mapView->setViewpointScale(m_mapView->mapScale() + 10000000);
}

void MainWindow::on_action_Reset_triggered()
{
    m_mapView->setViewpointScale(70000000);
}


/********************************************************************/


/*** LOAD MAPS ***/

void MainWindow::on_action_Load_Maps_triggered()
{
    QString m_filter = "Tif File(*.tif)";
    QString m_newlayer = QFileDialog::getOpenFileName(this, "Load Maps", QDir::homePath(), m_filter);

    //Adding Raster Layers
    Raster *raster = new Raster(m_newlayer);
    RasterLayer *rasterLayer = new RasterLayer(raster);
    m_map->operationalLayers()->append(rasterLayer);

    // Set map to mapView
    m_mapView->setMap(m_map);
}


/************************************************************************/


/*** MULTICASTING ***/

bool mm_triggered = false;
bool mp_triggered = false;
bool pm_triggered = false;

bool sp_triggered = false;

void MainWindow::multicastPlatform()
{
    // Platform Path
    p_plfPathGraphicsOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(p_plfPathGraphicsOverlay);

    // Multicasting Socket Program
    p_platGraphicsOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(p_platGraphicsOverlay);

    const QUrl platformPointUrl("qrc:/res/Navigation.png");
    PictureMarkerSymbol* platformPoint = new PictureMarkerSymbol(platformPointUrl, this);
    platformPoint->setHeight(20);
    platformPoint->setWidth(20);

    p_platformGraphics = new Graphic(this);
    p_platformGraphics->setSymbol(platformPoint);
    p_platGraphicsOverlay->graphics()->append(p_platformGraphics);

    udpSocket4.bind(QHostAddress::AnyIPv4, 37578, QUdpSocket::ShareAddress);
    udpSocket4.joinMulticastGroup(groupAddress4);

    if(!udpSocket6.bind(QHostAddress::AnyIPv6, 37578, QUdpSocket::ShareAddress) ||
       !udpSocket6.joinMulticastGroup(groupAddress6))
            connect(&udpSocket4, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    connect(&udpSocket6, &QUdpSocket::readyRead, this, &MainWindow::processPendingDatagrams);
}

void MainWindow::processPendingDatagrams()
{
    QByteArray datagram;

    SimpleMarkerSymbol* path = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::blue), 10.0, this);

    // Create Graphic for Platform Path
    p_plfPathGraphics = new Graphic();
    p_plfPathGraphics->setSymbol(path);
    p_plfPathGraphicsOverlay->graphics()->append(p_plfPathGraphics);

    // Storing Platform Data in Database
    QSqlQuery query;

    // using QUdpSocket::readDatagram (API since Qt 4)
    while (udpSocket4.hasPendingDatagrams())
    {
        datagram.resize(int(udpSocket4.pendingDatagramSize()));
        udpSocket4.readDatagram(datagram.data(), datagram.size());
        Point p_point = CoordinateFormatter::fromLatitudeLongitude(datagram.constData(), m_map->spatialReference());
        g_platform = p_point;
        p_platformGraphics->setGeometry(g_platform);
        setTextFromPoint(g_platform);

        QPointF m_platPoint = m_mapView->locationToScreen(g_platform);

        query.exec("insert into platform_path(Coordinates) values('" + m_coordinatesInDD + "')");

        if(sp_triggered)
            p_plfPathGraphics->setGeometry(g_platform);

        if(mm_triggered)
            m_mapView->setViewpointCenter(g_platform);

        if(mp_triggered && !m_mapView->rect().contains(QPoint(m_platPoint.x(),m_platPoint.y())))
            m_mapView->setViewpointCenter(g_platform);

        if(disUpdate)
        {
            // create line with start/end points
            const QList<Point> points = {g_platform, g_destination};
            const Polyline polyline = pointsToPolyline(points);

            // densify the path as a geodesic curve and show it with the path graphic
            constexpr double maxSegmentLength = 1.0;
            const LinearUnit unitOfMeasurement(LinearUnitId::Kilometers);
            constexpr GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
            const Geometry pathGeometry = GeometryEngine::densifyGeodetic(polyline, maxSegmentLength, unitOfMeasurement, curveType);

            // update the graphic
            pp_pathGraphic->setGeometry(pathGeometry);

            // calculate the path's geodetic length
            m_distanceText = QString::number(GeometryEngine::lengthGeodetic(pathGeometry, unitOfMeasurement, curveType), 'f', 2);
            ui->m_ledistance->setText(m_distanceText + " Miles");
        }
    }

    // using QUdpSocket::receiveDatagram (API since Qt 5.8)
    while (udpSocket6.hasPendingDatagrams())
    {
        QNetworkDatagram dgram = udpSocket6.receiveDatagram();
        Point p_point = CoordinateFormatter::fromLatitudeLongitude(dgram.data().constData(), m_map->spatialReference());
        g_platform = p_point;
        p_platformGraphics->setGeometry(g_platform);
        setTextFromPoint(g_platform);

        QPointF m_platPoint = m_mapView->locationToScreen(g_platform);

        query.exec("insert into platform_path(Coordinates) values('" + m_coordinatesInDD + "')");

        if(mm_triggered)
            m_mapView->setViewpointCenter(g_platform);

        if(mp_triggered && !m_mapView->rect().contains(QPoint(m_platPoint.x(),m_platPoint.y())))
            m_mapView->setViewpointCenter(g_platform);

        if(disUpdate)
        {
            // create line with start/end points
            const QList<Point> points = {g_platform, g_destination};
            const Polyline polyline = pointsToPolyline(points);

            // densify the path as a geodesic curve and show it with the path graphic
            constexpr double maxSegmentLength = 1.0;
            const LinearUnit unitOfMeasurement(LinearUnitId::Kilometers);
            constexpr GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
            const Geometry pathGeometry = GeometryEngine::densifyGeodetic(polyline, maxSegmentLength, unitOfMeasurement, curveType);

            // update the graphic
            pp_pathGraphic->setGeometry(pathGeometry);

            // calculate the path's geodetic length
            m_distanceText = QString::number(GeometryEngine::lengthGeodetic(pathGeometry, unitOfMeasurement, curveType), 'f', 2);
            ui->m_ledistance->setText(m_distanceText + " Miles");
        }
    }
}


/*********************************************************************************/


/*** DATABASE ***/

void MainWindow::databaseManagement()
{
    // Connecting Database
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setConnectOptions("Localhost via UNIX socket");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("map");
    if(!db.open())
    {
        QMessageBox::critical(this, "Database Error", "Couldn't Connect To Database!!");
    }

    QSqlQuery query;
    query.exec("create table platform_path(Coordinates varchar(30))");
    query.exec("create table annotations(ID int, Coordinates varchar(30) primary key)");
    query.exec("create table waypoints(ID int, Points int, Coordinates varchar(30) primary key)");
}


/*********************************************************************************/


/*** MAPVIEW MODE ***/

void MainWindow::on_actionMoving_Map_triggered()
{
    mp_triggered = false;
    pm_triggered = false;
    mm_triggered = true;

    m_mapView->unsetCursor();

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::RightButton)
        {
            mm_triggered = false;
            return;
        }
    });
}

void MainWindow::on_actionMoving_Platform_triggered()
{
    mm_triggered = false;
    pm_triggered = false;
    mp_triggered = true;

    m_mapView->unsetCursor();

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::RightButton)
        {
            mp_triggered = false;
            return;
        }
    });
}

void MainWindow::on_action_Pan_Map_triggered()
{
    mm_triggered = false;
    mp_triggered = false;
    pm_triggered = true;

    m_mapView->setCursor(Qt::OpenHandCursor);

    connect(m_mapView, &MapGraphicsView::mouseMoved, this, [this](QMouseEvent& )
    {
        if(pm_triggered)
        m_mapView->setCursor(Qt::ClosedHandCursor);

        connect(m_mapView, &MapGraphicsView::mouseReleased, this, [this](QMouseEvent& )
        {
            if(pm_triggered)
            m_mapView->setCursor(Qt::OpenHandCursor);
        });
    });

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::RightButton)
        {
            m_mapView->unsetCursor();
            pm_triggered = false;
            return;
        }
    });
}


/*********************************************************************/


/*** Platform Path ***/

void MainWindow::on_actionShow_Full_Path_triggered()
{
    p_plfPathGraphicsOverlay->graphics()->clear();
    sp_triggered = true;

    // Create platform_path TableModel
    plfTableModel = new QSqlTableModel();
    plfTableModel->setTable("platform_path");
    plfTableModel->select();

    SimpleMarkerSymbol* path = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::blue), 10.0, this);

    QVariant Coordinate;
    int i;

    for(i=1; i<=plfTableModel->rowCount(); i++)
    {
        // Create Graphic for Platform Path
        p_plfPathGraphics = new Graphic();
        p_plfPathGraphics->setSymbol(path);
        p_plfPathGraphicsOverlay->graphics()->append(p_plfPathGraphics);

        Coordinate = plfTableModel->data(plfTableModel->index(i, 0));
        Point p_point = CoordinateFormatter::fromLatitudeLongitude(Coordinate.toString(), m_map->spatialReference());
        p_plfPathGraphics->setGeometry(p_point);
    }
}

void MainWindow::on_actionShow_Latest_Path_triggered()
{
    p_plfPathGraphicsOverlay->graphics()->clear();
    sp_triggered = true;
}

void MainWindow::on_action_Hide_Path_triggered()
{
    sp_triggered = false;
    p_plfPathGraphicsOverlay->graphics()->clear();
}


/*********************************************************************/


/*** Reset To Default ***/

void MainWindow::on_actionReset_to_Default_triggered()
{
    m_pointGraphic->setVisible(false);
    ui->m_leDegrees->setText(NULL);
    ui->m_leDMS->setText(NULL);
    ui->m_leUsng->setText(NULL);
    ui->m_leUtm->setText(NULL);
    ui->m_ledistance->setText(NULL);

    ui->Display_Grid->setChecked(false);
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    if(m_mapGrid->isVisible())
        m_mapGrid->setVisible(false);

    if(m_antGraphicsOverlay)
        m_antGraphicsOverlay->graphics()->clear();

    if(m_wptGraphicsOverlay)
        m_wptGraphicsOverlay->graphics()->clear();

    if(p_plfPathGraphicsOverlay)
        p_plfPathGraphicsOverlay->graphics()->clear();

    if(pp_graphicsOverlay)
    {
        pp_graphicsOverlay->graphics()->clear();
        disUpdate = false;
    }

    if(tp_graphicsOverlay)
    {
        tp_graphicsOverlay->graphics()->clear();
    }

    if(me_graphicsOverlay)
        me_graphicsOverlay->graphics()->clear();

    if (m_antSketchEditor->isStarted())
    {
        m_antSketchEditor->stop();
        if (m_wptSketchEditor->isStarted())
        {
            m_wptSketchEditor->stop();
            return;
        }
        return;
    }

    if (m_wptSketchEditor->isStarted())
    {
        m_wptSketchEditor->stop();
        if (m_antSketchEditor->isStarted())
        {
            m_antSketchEditor->stop();
            return;
        }
        return;
    }
}

void MainWindow::resetTools()
{
    ui->m_ledistance->setText(NULL);

    if(pp_graphicsOverlay)
    {
        pp_graphicsOverlay->graphics()->clear();
        disUpdate = false;
    }

    if(tp_graphicsOverlay)
    {
        tp_graphicsOverlay->graphics()->clear();
    }

    if(me_graphicsOverlay)
        me_graphicsOverlay->graphics()->clear();

    if (m_antSketchEditor->isStarted())
    {
        m_antSketchEditor->stop();
        if (m_wptSketchEditor->isStarted())
        {
            m_wptSketchEditor->stop();
            return;
        }
        return;
    }

    if (m_wptSketchEditor->isStarted())
    {
        m_wptSketchEditor->stop();
        if (m_antSketchEditor->isStarted())
        {
            m_antSketchEditor->stop();
            return;
        }
        return;
    }
}


/*********************************************************************/


/*** TOOL BUTTONS ***/

void MainWindow::on_Display_Grid_clicked()
{
    //Set map to grid view
    m_mapView->setGrid(m_mapGrid);
    m_mapGrid->setVisible(true);

    if(!(ui->Display_Grid->isChecked()))
        m_mapGrid->setVisible(false);
}

void MainWindow::on_zoomInToolButton_clicked()
{
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::RightButton)
            ui->zoomInToolButton->setChecked(false);

        if(mouseEvent.button() == Qt::LeftButton && ui->zoomInToolButton->isChecked())
            m_mapView->setViewpointScale(m_mapView->mapScale() - 10000000);
    });
}

void MainWindow::on_zoomOutToolButton_clicked()
{
    ui->zoomInToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::RightButton)
            ui->zoomOutToolButton->setChecked(false);

        if(mouseEvent.button() == Qt::LeftButton && ui->zoomOutToolButton->isChecked())
            m_mapView->setViewpointScale(m_mapView->mapScale() + 10000000);
    });
}


void MainWindow::on_AnnotationToolbar_clicked()
{
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    resetTools();

    // Set SketchEditor to sketch Annotations
    m_mapView->setSketchEditor(m_antSketchEditor);

    if(ui->AnnotationToolbar->isChecked())
    {
        // Instructions
        QMessageBox::information(this, "Instructions", "Left Click : Add Annotations\nRight Click : Discard Annotations\nEnter Key : Save Annotations");
        m_antSketchEditor->start(SketchCreationMode::Multipoint);
        antCount++;
    }

    else
    {
        m_antSketchEditor->stop();
        return;
    }

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::LeftButton)
        {
            if (m_antSketchEditor->isStarted())
            {
                // Storing Annotations into Database
                QSqlQuery query;
                query.exec("insert into annotations(ID, Coordinates) values(" + QString::number(antCount) + ", '" + m_coordinatesInDD + "')");
            }
        }
        if(mouseEvent.button() == Qt::RightButton)
        {
            if (m_antSketchEditor->isStarted())
            {
                m_antSketchEditor->stop();
                ui->AnnotationToolbar->setChecked(false);
                QSqlQuery query;
                query.exec("delete from annotations where ID = " + QString::number(antCount--));
                return;
            }
        }
    });

    connect(m_mapView, &MapGraphicsView::keyPressed, this, [this](QKeyEvent& keyEvent)
    {
        if(keyEvent.key() == Qt::Key_Enter)
        {
            if (!m_antSketchEditor->isSketchValid())
            {
                qWarning() << "Unable to save sketch. Sketch is not valid.";
                return;
            }

            // To save the sketch, create a graphic with the sketch's geometry before stopping the sketchEditor
            Geometry sketchGeometry = m_antSketchEditor->geometry();

            m_antSketchGraphic = new Graphic(sketchGeometry, m_antMultiPointSymbol, this);
            m_antGraphicsOverlay->graphics()->append(m_antSketchGraphic);
            m_antSketchEditor->stop();
            ui->AnnotationToolbar->setChecked(false);
        }
    });
}

void MainWindow::on_WaypointsToolbar_clicked()
{
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    resetTools();

    // Set SketchEditor to sketch WayPoints
    m_mapView->setSketchEditor(m_wptSketchEditor);

    if(ui->WaypointsToolbar->isChecked())
    {
        // Instructions
        QMessageBox::information(this, "Instructions", "Left Click : Add Waypoints\nRight Click : Discard Waypoints\nEnter Key : Save Waypoints");
        m_wptSketchEditor->start(SketchCreationMode::Polyline);
        wptCount++;
        wptPoint = 1;
    }

    else
    {
        m_wptSketchEditor->stop();
        return;
    }

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::LeftButton)
        {
            if (m_wptSketchEditor->isStarted())
            {
                // Storing Annotations into Database
                QSqlQuery query;
                query.exec("insert into waypoints(ID, Points, Coordinates) values(" + QString::number(wptCount) +  "," + QString::number(wptPoint++) + ", '" + m_coordinatesInDD + "')");
            }
        }
        if(mouseEvent.button() == Qt::RightButton)
        {
            if (m_wptSketchEditor->isStarted())
            {
                m_wptSketchEditor->stop();
                ui->WaypointsToolbar->setChecked(false);
                QSqlQuery query;
                query.exec("delete from waypoints where ID = " + QString::number(wptCount--));
                return;
            }
        }
    });

    connect(m_mapView, &MapGraphicsView::keyPressed, this, [this](QKeyEvent& keyEvent)
    {
        if(keyEvent.key() == Qt::Key_Enter)
        {
            if (!m_wptSketchEditor->isSketchValid())
            {
                qWarning() << "Unable to save sketch. Sketch is not valid.";
                return;
            }

            // To save the sketch, create a graphic with the sketch's geometry before stopping the sketchEditor
            Geometry sketchGeometry = m_wptSketchEditor->geometry();

            m_wptSketchGraphic = new Graphic(sketchGeometry, m_wptLineSymbol, this);
            m_wptGraphicsOverlay->graphics()->append(m_wptSketchGraphic);
            m_wptSketchEditor->stop();
            ui->WaypointsToolbar->setChecked(false);
        }
    });
}

void MainWindow::on_DBPPToolButton_clicked()
{
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    resetTools();

    disUpdate = true;
    Point platform = g_platform;

    if(!ui->DBPPToolButton->isChecked())
        resetTools();

    // Create a GraphicsOverlay
    pp_graphicsOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(pp_graphicsOverlay);

    // Create Graphic Symbols
    const QUrl pp_destinationSymbolUrl("qrc:/res/pin_circle_red.png");
    PictureMarkerSymbol* pp_destinationSymbol = new PictureMarkerSymbol(pp_destinationSymbolUrl, this);
    pp_destinationSymbol->setHeight(20);
    pp_destinationSymbol->setWidth(20);
    SimpleLineSymbol* pathSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Dash, QColor("blue"), 5.0f /*width*/, this);

    // Create destination graphic
    pp_destinationGraphic = new Graphic(this);
    pp_destinationGraphic->setSymbol(pp_destinationSymbol);
    pp_graphicsOverlay->graphics()->append(pp_destinationGraphic);

    // Create path graphic
    pp_pathGraphic = new Graphic(this);
    pp_pathGraphic->setSymbol(pathSymbol);
    pp_graphicsOverlay->graphics()->append(pp_pathGraphic);

    // connect to mouse clicked signal
    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this, platform](QMouseEvent& mouseEvent)
    {
        // re-project the point to match the NYC graphic
        const Point clickedPoint = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());
        const Point pp_destination = GeometryEngine::project(clickedPoint, p_platformGraphics->geometry().spatialReference());

        if(mouseEvent.button() == Qt::LeftButton && ui->DBPPToolButton->isChecked())
        {
            // update the destination graphic
            pp_destinationGraphic->setGeometry(pp_destination);
            g_destination = pp_destination;
        }

        if(mouseEvent.button() == Qt::RightButton)
        {
            pp_graphicsOverlay->graphics()->clear();
            disUpdate = false;
            ui->m_ledistance->setText(NULL);
            ui->DBPPToolButton->setChecked(false);
            return;
        }
    });
}

void MainWindow::on_DBTPToolButton_clicked()
{
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);

    resetTools();

    if(ui->DBTPToolButton->isChecked())
    {
        // Instructions
        QMessageBox::information(this, "Instructions", "Point A : Double Click\nPoint B : Single Click");
    }

    // Create a GraphicsOverlay
    tp_graphicsOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(tp_graphicsOverlay);

    // Create Graphic Symbols
    const QUrl tp_markerSymbolUrl("qrc:/res/pinA.png");
    PictureMarkerSymbol* tp_markerSymbol = new PictureMarkerSymbol(tp_markerSymbolUrl, this);
    tp_markerSymbol->setHeight(40);
    tp_markerSymbol->setWidth(40);

    const QUrl tp_destinationSymbolUrl("qrc:/res/pinB.png");
    PictureMarkerSymbol* tp_destinationSymbol = new PictureMarkerSymbol(tp_destinationSymbolUrl, this);
    tp_destinationSymbol->setHeight(40);
    tp_destinationSymbol->setWidth(40);

    SimpleLineSymbol* pathSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Dash, QColor("blue"), 5.0f /*width*/, this);

    // Create nyc graphic
    tp_nycGraphic = new Graphic(this);
    tp_nycGraphic->setSymbol(tp_markerSymbol);
    tp_graphicsOverlay->graphics()->append(tp_nycGraphic);

    // Create destination graphic
    tp_destinationGraphic = new Graphic(this);
    tp_destinationGraphic->setSymbol(tp_destinationSymbol);
    tp_graphicsOverlay->graphics()->append(tp_destinationGraphic);

    // Create path graphic
    tp_pathGraphic = new Graphic(this);
    tp_pathGraphic->setSymbol(pathSymbol);
    tp_graphicsOverlay->graphics()->append(tp_pathGraphic);

    connect(m_mapView, &MapGraphicsView::mouseDoubleClicked, this, [this](QMouseEvent& mouseEvent)
    {
        // get the point from the mouse point
        Point tp_nycPoint = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());

        if(mouseEvent.button() == Qt::LeftButton && ui->DBTPToolButton->isChecked())
            tp_nycGraphic->setGeometry(tp_nycPoint);

        // connect to mouse clicked signal
        connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this, tp_nycPoint](QMouseEvent& mouseEvent)
        {
            // re-project the point to match the NYC graphic
            const Point clickedPoint = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());
            const Point tp_destination = GeometryEngine::project(clickedPoint, tp_nycGraphic->geometry().spatialReference());

            // create line with start/end points
            const QList<Point> points = {tp_nycPoint, tp_destination};
            const Polyline polyline = pointsToPolyline(points);

            // densify the path as a geodesic curve and show it with the path graphic
            constexpr double maxSegmentLength = 1.0;
            const LinearUnit unitOfMeasurement(LinearUnitId::Miles);
            constexpr GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
            const Geometry pathGeometry = GeometryEngine::densifyGeodetic(polyline, maxSegmentLength, unitOfMeasurement, curveType);

            if(mouseEvent.button() == Qt::LeftButton && ui->DBTPToolButton->isChecked())
            {
                // update the graphic
                tp_destinationGraphic->setGeometry(tp_destination);
                tp_pathGraphic->setGeometry(pathGeometry);

                // calculate the path's geodetic length
                m_distanceText = QString::number(GeometryEngine::lengthGeodetic(pathGeometry, unitOfMeasurement, curveType), 'f', 2);
                ui->m_ledistance->setText(m_distanceText + " Miles");
            }

            connect(m_mapView, &MapGraphicsView::mouseDoubleClicked, this, [this, tp_destination](QMouseEvent& mouseEvent)
            {
                // get the point from the mouse point
                Point nycPoint = m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());

                // create line with start/end points
                const QList<Point> points = {nycPoint, tp_destination};
                const Polyline polyline = pointsToPolyline(points);

                // densify the path as a geodesic curve and show it with the path graphic
                constexpr double maxSegmentLength = 1.0;
                const LinearUnit unitOfMeasurement(LinearUnitId::Miles);
                constexpr GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
                const Geometry pathGeometry = GeometryEngine::densifyGeodetic(polyline, maxSegmentLength, unitOfMeasurement, curveType);

                if(mouseEvent.button() == Qt::LeftButton && ui->DBTPToolButton->isChecked())
                {
                    // update the graphic
                    tp_nycGraphic->setGeometry(nycPoint);
                    tp_pathGraphic->setGeometry(pathGeometry);

                    // calculate the path's geodetic length
                    m_distanceText = QString::number(GeometryEngine::lengthGeodetic(pathGeometry, unitOfMeasurement, curveType), 'f', 2);
                    ui->m_ledistance->setText(m_distanceText + " Miles");
                }
            });
        });
    });

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::RightButton)
        {
            tp_graphicsOverlay->graphics()->clear();
            ui->m_ledistance->setText(NULL);
            ui->DBTPToolButton->setChecked(false);
            return;
        }
    });
}

void MainWindow::on_DBMEToolButton_clicked()
{
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    resetTools();

    DistanceBearing disBearing;
    disBearing.setModal(true);
    disBearing.exec();

    // Create a GraphicsOverlay
    me_graphicsOverlay = new GraphicsOverlay(this);
    m_mapView->graphicsOverlays()->append(me_graphicsOverlay);

    // Create Graphic Symbols
    const QUrl me_markerSymbolUrl("qrc:/res/pinA.png");
    PictureMarkerSymbol* me_markerSymbol = new PictureMarkerSymbol(me_markerSymbolUrl, this);
    me_markerSymbol->setHeight(40);
    me_markerSymbol->setWidth(40);

    const QUrl destinationSymbolUrl("qrc:/res/pinB.png");
    PictureMarkerSymbol* destinationSymbol = new PictureMarkerSymbol(destinationSymbolUrl, this);
    destinationSymbol->setHeight(40);
    destinationSymbol->setWidth(40);

    SimpleLineSymbol* pathSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Dash, QColor("blue"), 5.0f /*width*/, this);

    // Create nyc graphic
    me_nycGraphic = new Graphic(this);
    me_nycGraphic->setSymbol(me_markerSymbol);
    me_graphicsOverlay->graphics()->append(me_nycGraphic);

    // Create destination graphic
    me_destinationGraphic = new Graphic(this);
    me_destinationGraphic->setSymbol(destinationSymbol);
    me_graphicsOverlay->graphics()->append(me_destinationGraphic);

    // Create path graphic
    me_pathGraphic = new Graphic(this);
    me_pathGraphic->setSymbol(pathSymbol);
    me_graphicsOverlay->graphics()->append(me_pathGraphic);

    // re-project the point to match the NYC graphic
    const Point me_PointA = CoordinateFormatter::fromLatitudeLongitude(disBearing.d_pointA, m_map->spatialReference());
    me_nycGraphic->setGeometry(me_PointA);
    const Point me_PointB = CoordinateFormatter::fromLatitudeLongitude(disBearing.d_pointB, m_map->spatialReference());
    me_destinationGraphic->setGeometry(me_PointB);

    // create line with start/end points
    const QList<Point> points = {me_PointA, me_PointB};
    const Polyline polyline = pointsToPolyline(points);

    // densify the path as a geodesic curve and show it with the path graphic
    constexpr double maxSegmentLength = 1.0;
    const LinearUnit unitOfMeasurement(LinearUnitId::Miles);
    constexpr GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
    const Geometry pathGeometry = GeometryEngine::densifyGeodetic(polyline, maxSegmentLength, unitOfMeasurement, curveType);

    // update the graphic
    me_pathGraphic->setGeometry(pathGeometry);

    // calculate the path's geodetic length
    m_distanceText = QString::number(GeometryEngine::lengthGeodetic(pathGeometry, unitOfMeasurement, curveType), 'f', 2);
    ui->m_ledistance->setText(m_distanceText + " Miles");

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
    {
        if(mouseEvent.button() == Qt::RightButton)
        {
            me_graphicsOverlay->graphics()->clear();
            ui->m_ledistance->setText(NULL);
            return;
        }
    });
}

void MainWindow::on_Reset_to_Default_clicked()
{
    m_pointGraphic->setVisible(false);
    ui->m_leDegrees->setText(NULL);
    ui->m_leDMS->setText(NULL);
    ui->m_leUsng->setText(NULL);
    ui->m_leUtm->setText(NULL);
    ui->m_ledistance->setText(NULL);

    ui->Display_Grid->setChecked(false);
    ui->zoomInToolButton->setChecked(false);
    ui->zoomOutToolButton->setChecked(false);
    ui->AnnotationToolbar->setChecked(false);
    ui->WaypointsToolbar->setChecked(false);
    ui->DBPPToolButton->setChecked(false);
    ui->DBTPToolButton->setChecked(false);

    if(m_mapGrid->isVisible())
        m_mapGrid->setVisible(false);

    if(m_antGraphicsOverlay)
        m_antGraphicsOverlay->graphics()->clear();

    if(m_wptGraphicsOverlay)
        m_wptGraphicsOverlay->graphics()->clear();

    if(p_plfPathGraphicsOverlay)
        p_plfPathGraphicsOverlay->graphics()->clear();

    if(pp_graphicsOverlay)
    {
        pp_graphicsOverlay->graphics()->clear();
        disUpdate = false;
    }

    if(tp_graphicsOverlay)
    {
        tp_graphicsOverlay->graphics()->clear();
    }

    if(me_graphicsOverlay)
        me_graphicsOverlay->graphics()->clear();

    if (m_antSketchEditor->isStarted())
    {
        m_antSketchEditor->stop();
        if (m_wptSketchEditor->isStarted())
        {
            m_wptSketchEditor->stop();
            return;
        }
        return;
    }

    if (m_wptSketchEditor->isStarted())
    {
        m_wptSketchEditor->stop();
        if (m_antSketchEditor->isStarted())
        {
            m_antSketchEditor->stop();
            return;
        }
        return;
    }
}

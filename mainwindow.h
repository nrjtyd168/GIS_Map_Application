#ifndef MAINWINDOW_H
#define MAINWINDOW_H

namespace Esri
{
    namespace ArcGISRuntime
    {
        class Map;
        class MapGraphicsView;
        class Graphic;
        class GraphicsOverlay;
        class SketchEditor;
        class SimpleMarkerSymbol;
        class SimpleLineSymbol;
    }
}

#include <QMainWindow>
#include "Point.h"
#include "Polyline.h"
#include <LatitudeLongitudeGrid.h>
#include <QObject>
#include <QDialog>
#include <QHostAddress>
#include <QUdpSocket>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Q_INVOKABLE void handleLocationUpdate(Esri::ArcGISRuntime::Point point);

signals:
  void distanceTextChanged();

private slots:

    void on_actionMoving_Platform_triggered();

    void on_Zoom_In_triggered();

    void on_Zoom_Out_triggered();

    void on_actionBetween_Platform_Point_triggered();

    void on_actionBetween_Two_Points_triggered();

    void on_action_Load_Maps_triggered();

    void on_m_leDegrees_textEdited(const QString &);

    void on_m_leDMS_textEdited(const QString &);

    void on_m_leUtm_textEdited(const QString &);

    void on_m_leUsng_textEdited(const QString &);

    void on_action_Manual_Entry_triggered();

    void processPendingDatagrams();

    void on_actionMoving_Map_triggered();

    void on_action_Pan_Map_triggered();

    void on_Display_Grid_clicked();

    void on_action_Reset_triggered();

    void on_actionShow_Full_Path_triggered();

    void on_actionShow_Latest_Path_triggered();

    void on_action_Hide_Path_triggered();

    void on_action_Remove_Line_triggered();

    void on_actionReset_to_Default_triggered();

    void on_zoomInToolButton_clicked();

    void on_zoomOutToolButton_clicked();

    void on_actionAdd_Annotations_triggered();

    void on_actionAdd_Waypoints_triggered();

    void on_AnnotationToolbar_clicked();

    void on_WaypointsToolbar_clicked();

    void on_Reset_to_Default_clicked();

    void on_action_Hide_All_triggered();

    void on_action_Hide_All_2_triggered();

    void on_action_Show_All_triggered();

    void on_action_Manage_triggered();

    void on_action_Show_All_2_triggered();

    void on_action_Manage_2_triggered();

    void on_DBPPToolButton_clicked();

    void on_DBTPToolButton_clicked();

    void on_DBMEToolButton_clicked();

private:
    Ui::MainWindow *ui;

private:
    Esri::ArcGISRuntime::Map*                   m_map = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*       m_mapView = nullptr;
    Esri::ArcGISRuntime::LatitudeLongitudeGrid* m_mapGrid = nullptr;

    Esri::ArcGISRuntime::Graphic*               m_pointGraphic = nullptr;
    Esri::ArcGISRuntime::GraphicsOverlay*       m_pointGraphicsOverlay = nullptr;

    Esri::ArcGISRuntime::Graphic*               m_antSketchGraphic = nullptr;
    Esri::ArcGISRuntime::GraphicsOverlay*       m_antGraphicsOverlay = nullptr;
    Esri::ArcGISRuntime::SketchEditor*          m_antSketchEditor = nullptr;
    Esri::ArcGISRuntime::SimpleMarkerSymbol*    m_antMultiPointSymbol = nullptr;

    Esri::ArcGISRuntime::Graphic*               m_wptSketchGraphic = nullptr;
    Esri::ArcGISRuntime::Graphic*               m_wptPathGraphics = nullptr;
    Esri::ArcGISRuntime::GraphicsOverlay*       m_wptGraphicsOverlay = nullptr;
    Esri::ArcGISRuntime::SketchEditor*          m_wptSketchEditor = nullptr;
    Esri::ArcGISRuntime::SimpleMarkerSymbol*    m_wptMultiPointSymbol = nullptr;
    Esri::ArcGISRuntime::SimpleLineSymbol*      m_wptLineSymbol = nullptr;

    Esri::ArcGISRuntime::Graphic*               pp_destinationGraphic = nullptr;
    Esri::ArcGISRuntime::Graphic*               pp_pathGraphic = nullptr;
    Esri::ArcGISRuntime::GraphicsOverlay*       pp_graphicsOverlay = nullptr;

    Esri::ArcGISRuntime::Graphic*               tp_nycGraphic = nullptr;
    Esri::ArcGISRuntime::Graphic*               tp_destinationGraphic = nullptr;
    Esri::ArcGISRuntime::Graphic*               tp_pathGraphic = nullptr;
    Esri::ArcGISRuntime::GraphicsOverlay*       tp_graphicsOverlay = nullptr;

    Esri::ArcGISRuntime::Graphic*               me_nycGraphic = nullptr;
    Esri::ArcGISRuntime::Graphic*               me_destinationGraphic = nullptr;
    Esri::ArcGISRuntime::Graphic*               me_pathGraphic = nullptr;
    Esri::ArcGISRuntime::GraphicsOverlay*       me_graphicsOverlay = nullptr;

    Esri::ArcGISRuntime::Graphic*               p_platformGraphics = nullptr;
    Esri::ArcGISRuntime::GraphicsOverlay*       p_platGraphicsOverlay = nullptr;

    Esri::ArcGISRuntime::Graphic*               p_plfPathGraphics = nullptr;
    Esri::ArcGISRuntime::GraphicsOverlay*       p_plfPathGraphicsOverlay = nullptr;

private:
    QSqlTableModel* plfTableModel;
    QSqlTableModel* antTableModel;
    QSqlTableModel* wptTableModel;

    QString m_coordinatesInDD;
    QString m_coordinatesInDMS;
    QString m_coordinatesInUsng;
    QString m_coordinatesInUtm;
    QString m_distanceText;

    QUdpSocket udpSocket4;
    QUdpSocket udpSocket6;
    QHostAddress groupAddress4;
    QHostAddress groupAddress6;

private:
    void localBaseMap();
    void pointCoordinates();
    void annotationSketch();
    void waypointSketch();
    void multicastPlatform();
    void databaseManagement();
    void resetTools();

    void setTextFromPoint(Esri::ArcGISRuntime::Point point);
    Esri::ArcGISRuntime::Polyline pointsToPolyline(const QList<Esri::ArcGISRuntime::Point>& points);
    QString distanceText() const;

};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebChannel>
#include "gpsclient.h"
#include "scene.h"
#include "item.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public  slots:
  void getCoordinates(QString lon, QString lat);//接收页面消息

private slots:
  void location(double lng,double lat);
  void onCursorChanged(const QPointF &pos, const QPointF &scenePos, const QPointF &screenPos);

  void on_actionView_triggered();

  void on_actionEngine_triggered();

  void on_fitline_clicked();

private:
    GpsClient *MyGps;
    Ui::MainWindow *ui;
    Scene *my_scene;
    QString fit_point="";
    Item *item;





};
#endif // MAINWINDOW_H

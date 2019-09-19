#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWebChannel *channel = new QWebChannel;  //创建QWebchannel 对象
    channel->registerObject(QString("MainWindow"),this);  //向QWebchannel对象注册Qt对象
    ui->m_web->page()->setWebChannel(channel);            //将设置好的QWebChannel对象设置为当前页面的通道


    ui->m_web->page()->load(QUrl("qrc:/wx.html"));  //加载新建的wx.html百度地图文件
    ui->m_web->show();                              //显示

    MyGps = new GpsClient();
    MyGps->start();

    my_scene=new Scene(this);
    ui->graphicsView->setScene(my_scene);
    ui->graphicsView->setStyleSheet("background: transparent");


    QObject::connect(MyGps, SIGNAL(position(double,double)), SLOT(location(double,double)));
    QObject::connect(my_scene, SIGNAL(cursorChanged(QPointF, QPointF, QPointF)), this, SLOT(onCursorChanged(QPointF, QPointF, QPointF)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getCoordinates(QString lon, QString lat)
{  QString tempLon="Mouse Lontitude:"+lon+"°";
    QString tempLat="Mouse Lattitude:"+lat+"°";
    fit_point=fit_point+lon+","+lat+";";
//    ui->lng->setText(fit_point);

    ui->lng->setText(tempLon);
    ui->lat->setText(tempLat);
}


void MainWindow::location(double lng ,double lat)
{

      ui->m_web->page()->runJavaScript(QString("theLocation(%1,%2)").arg(lng, 0, 'g', 10).arg(lat, 0, 'g', 10));

//    ui->m_web->page()->runJavaScript(QString("theLocation(%1,%2)").arg(lng, 0, 'g', 10).arg(late, 0, 'g', 10));//c++向页面发送请求
}
void MainWindow::onCursorChanged(const QPointF &pos, const QPointF &scenePos, const QPointF &screenPos)
    {
        ui->statusBar->showMessage("pos : (" + QString::number(pos.x()) + ", " + QString::number(pos.y()) + "); " +
                                   "scenePos : (" + QString::number(scenePos.x()) + ", " + QString::number(scenePos.y()) + "); " +
                                   "screenPos : (" + QString::number(screenPos.x()) + ", " + QString::number(screenPos.y()) + ")" );

        fit_point=fit_point+QString::number(scenePos.x())+","+QString::number(scenePos.y()) + ";";
    }


void MainWindow::on_actionView_triggered()
{
    ui->graphicsView->setMinimumSize(900,400);
    ui->graphicsView->setMaximumSize(900,400);
}

void MainWindow::on_actionEngine_triggered()
{
    ui->graphicsView->setMinimumSize(0,0);
    ui->graphicsView->setMaximumSize(0,0);
}

void MainWindow::on_fitline_clicked()
{
    item->fit_line(fit_point);
    fit_point="";
}










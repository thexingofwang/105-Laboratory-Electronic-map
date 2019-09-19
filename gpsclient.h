#ifndef GPSCLIENT_H
#define GPSCLIENT_H
#include <QThread>
#include <QTcpSocket>

struct Loc
{
    double lon;
    double lat;
};

class GpsClient : public QThread
{
    Q_OBJECT
public:
    GpsClient(QObject *parent = nullptr);
signals:
    void position(double lng,double lat);
protected:
    void run();
private:
    QTcpSocket *tcpSocket;


    Loc wgs2gcj(Loc gps);
    Loc gcj2bd(Loc gg);
    double transformLon(double x, double y);
    double transformLat(double x, double y);
    const double pi = 3.14159265358979324;
    const double a = 6378245.0;
    const double ee = 0.00669342162296594323;
    const double x_pi = 3.14159265358979324 * 3000.0 / 180.0;


};

#endif // GPSCLIENT_H

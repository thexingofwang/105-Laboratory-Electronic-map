#include "gpsclient.h"

GpsClient::GpsClient(QObject *parent) :QThread (parent)
{
    tcpSocket = new QTcpSocket();

}
void GpsClient::run()
{
    tcpSocket->connectToHost("219.216.101.202", 8080);
    tcpSocket->write("GPS Client connected!");

    for (;;)
    {
        if (tcpSocket->waitForReadyRead())
        {

            QString  data = tcpSocket->readAll();
            if(data!="")

               {   QStringList array = data.split(',');//以,分割字符串data
                  Loc gpscoord;
                  gpscoord.lon = array[0].toDouble();
                  gpscoord.lat = array[1].toDouble();
                  gpscoord = wgs2gcj(gpscoord);
                  gpscoord= gcj2bd(gpscoord);
                  double lng;
                  double lat;
                  lng = gpscoord.lon;
                  lat = gpscoord.lat;
                emit position(lng,lat);
            }
        }
    }
}
double GpsClient::transformLon(double x, double y)
{
    double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(std::abs(x));
    ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
    ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
    ret += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;
    return ret;
}
double GpsClient::transformLat(double x, double y)
{
    double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(std::abs(x));
    ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
    ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
    ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
    return ret;
}
Loc GpsClient::wgs2gcj(Loc gps)
{
    Loc gcjcoord;
    double dLon = transformLon(gps.lon - 105.0, gps.lat - 35.0);
    double dLat = transformLat(gps.lon - 105.0, gps.lat - 35.0);
    double radLat = gps.lat / 180.0 * pi;
    double magic = sin(radLat);
    magic = 1 - ee * magic * magic;
    double sqrtMagic = sqrt(magic);
    dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
    gcjcoord.lon = gps.lon + dLon;
    gcjcoord.lat = gps.lat + dLat;
    return gcjcoord;

}
Loc GpsClient::gcj2bd(Loc gg)
{
    Loc bdcoord;
    double x = gg.lon;
    double y = gg.lat;
    double z = sqrt(x * x + y * y) + 0.00002 * sin(y * x_pi);
    double theta = atan2(y, x) + 0.000003 * cos(x * x_pi);
    bdcoord.lon = z * cos(theta) + 0.0065;
    bdcoord.lat = z * sin(theta) + 0.006;
    return bdcoord;
}

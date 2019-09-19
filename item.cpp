#include "item.h"

Item::Item(QObject *parent) : QObject(parent)
{
    m_boundingRect.setRect(0,0,898,398);

}
QRectF Item::boundingRect() const
{
    return m_boundingRect;

 }
void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::black);
    pen.setWidth(3);
    painter->setPen(pen);
//    painter->drawLine(0,0,898,398);
    painter->drawPath(dense_path_cp);
//    update();
}
void Item::fit_line(QString fit_position)
{

    int POINT_NUM=400;
    int i;
        float dist1, dist2;
    QList<QPointF> dense_waypoints;
    QList<coordinate> spare_points;  // store the way points which is struct type(spare points)
    QList<coordinate> dense_points;  // store the way points which is dense points
    QPainterPath path_cp;
    std::vector<double> dense_s;     // store the distance s which is the ith dense point away from origin
    float s;                      // the distance from zero point to (x[i], y[i])
    float distance;               // the distance between (x[i], y[i]) and (x[i+1], y[i+1])
    QStringList a;
    coordinate p_1,p_2;
    fit_position.replace(QRegExp(";$"), "");
    dense_waypoints.clear();
    spare_points.clear();
    dense_points.clear();
    dense_s.clear();
    s = 0.0;
    QStringList wayArr=fit_position.split(";");
    if(wayArr[0]==wayArr[1])
    {
       for(i=0;i<wayArr.length()-1;i++)
            wayArr[i]=wayArr[i+1];
        wayArr[wayArr.length()-1]=wayArr[0];
    }

    for (i = 0; i < wayArr.length(); i++)      //   data is GPS signal
    {
        coordinate my_point;
        my_point=bd2local(wayArr[i]);
        spare_points.append(my_point);
    }

//    for (i = 0; i < wayArr.length(); i++)    //data is local_site
//    {

//         a=wayArr[i].split(',');
//         x_site=a[0].toFloat();
//         y_site=a[1].toFloat();
//        coordinate wid_point={x_site,y_site};
//        spare_points.append(wid_point);
//    }
    // calculate the distance s in path and insert two average points between two spare points
    for (i = 0; i < spare_points.length()-1;i++)
    {
            coordinate point_first  =spare_points.at(i);
            coordinate point_second =spare_points.at(i+1);
            double x_relat = point_second.x - point_first.x;
            double y_relat = point_second.y - point_first.y;
            p_1.x = 1.0/3.0*x_relat+point_first.x;
            p_1.y = 1.0/3.0*y_relat+point_first.y;
            p_2.x = 2.0/3.0*x_relat+point_first.x;
            p_2.y = 2.0/3.0*y_relat+point_first.y;
            dense_points.append(point_first);
            dense_points.append(p_1);
            dense_points.append(p_2);
            if ( i == spare_points.length() - 2 )
                dense_points.append(point_second);

            distance = sqrt( (point_second.x - point_first.x)*(point_second.x - point_first.x)
                             + (point_second.y - point_first.y)*(point_second.y - point_first.y));

            dist1 = 1.0/3.0*distance;
            dist2 = 2.0/3.0*distance;
            dense_s.push_back(s);
            dense_s.push_back(s+dist1);
            dense_s.push_back(s+dist2);
            if (i==spare_points.length() - 2)
                dense_s.push_back(s+distance);
            s += distance;
    }
    // calculate the std::vector<double> (s, x) and (s, y)
    std::vector<double> x_points, y_points;
    for (i = 0; i < dense_points.length();i++)
    {
        x_points.push_back(dense_points.at(i).x);
        y_points.push_back(dense_points.at(i).y);
    }
    // cubic spline interpolation for (s, x) and (s, y) seperately
  //tk::spline s1, s2;
    s1.set_points(dense_s, x_points);
    s2.set_points(dense_s, y_points);

    for (i = 0; i <=POINT_NUM;i++)
    {
        coordinate xy;
        xy.x = s1(i*s/POINT_NUM);
        xy.y = s2(i*s/POINT_NUM);
        dense_waypoints.append(QPointF(xy.x, xy.y));

    }
    // set path
    QPainterPath dense_path(dense_waypoints[0]);
    for (int i = 0; i<dense_waypoints.size(); i++)
    {
        dense_path.lineTo(dense_waypoints[i]);
    }
    dense_path_cp = dense_path;
    //qDebug() << dense_path;
//    update();
}
coordinate Item::bd2local(QString bd_position)
{
    QStringList a;                           //   data type    1,2
    coordinate last_position;
    float referenceone[4];
    float referencetwo[4];
    float referencethree[4];
    referenceone[0]=123.424245;  //  baidu coordinate for fist point
    referenceone[1]=41.772123;
    referenceone[2]=123.427445;  //  baidu coordinate for second point
    referenceone[3]=41.771136;
    referencetwo[0]=53;            //Coordinates corresponding to the first point
    referencetwo[1]=74;
    referencetwo[2]=767;            // Coordinates corresponding to the second point
    referencetwo[3]=367;
    referencethree[0]=referenceone[2]-referenceone[0]; //lon difference
    referencethree[1]=referenceone[3]-referenceone[1]; //lat difference
    referencethree[2]=referencetwo[2]-referencetwo[0]; //x  difference
    referencethree[3]=referencetwo[3]-referencetwo[1]; //y  difference
    a=bd_position.split(",");
    last_position.x=(((a[0].toFloat()-referenceone[0])*referencethree[2])/referencethree[0])+referencetwo[0];
    last_position.y=(((a[1].toFloat()-referenceone[1])*referencethree[3])/referencethree[1])+referencetwo[1];
    return last_position;
}

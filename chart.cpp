#include "chart.h"

Chart::Chart(QWidget *parent, QString _chartname)
{
    setParent(parent);
        chartname = _chartname;
        series = new QSplineSeries(this);
        qchart = new QChart;
        chartview = new QChartView(qchart);
        layout = new QHBoxLayout(this);
        axisX = new QValueAxis(this);
        axisY = new QValueAxis(this);
        //��ui���������һ��Widget��������ͼ��ӽ�ȥ
        layout->addWidget(chartview);
        setLayout(layout);
        chartview->setRenderHint(QPainter::Antialiasing);//��ֹͼ������
}

void Chart::setAxis(QString _xname, qreal _xmin, qreal _xmax, int _xtickc, QString _yname, qreal _ymin, qreal _ymax, int _ytickc){

    xname = _xname; xmin = _xmin; xmax = _xmax; xtickc = _xtickc;
    yname = _yname; ymin = _ymin; ymax = _ymax; ytickc = _ytickc;

    axisX->setRange(xmin, xmax);    //���÷�Χ
    axisX->setLabelFormat("%u");   //���ÿ̶ȵĸ�ʽ
    /************************************
        %u �޷���ʮ��������
        %s �ַ���
        %c һ���ַ�
        %d �з���ʮ��������
        %e ��������e-������
        %f ��������ʮ���Ƽ�����
        %s �ַ���
    ****************************************/
        axisX->setGridLineVisible(true);   //�����߿ɼ�
        axisX->setTickCount(xtickc);       //���ö��ٸ����
        axisX->setMinorTickCount(1);   //����ÿ���������С�̶��ߵ���Ŀ
        axisX->setTitleText(xname);  //��������
        axisY->setRange(ymin, ymax);
        axisY->setLabelFormat("%u");
        axisY->setGridLineVisible(true);
        axisY->setTickCount(ytickc);
        axisY->setMinorTickCount(1);
        axisY->setTitleText(yname);
        qchart->addAxis(axisX, Qt::AlignBottom); //�£�Qt::AlignBottom  �ϣ�Qt::AlignTop
        qchart->addAxis(axisY, Qt::AlignLeft);   //��Qt::AlignLeft    �ң�Qt::AlignRight
}

void Chart::buildChart(QVector<QPointF> pointlist)
{
    //��������Դ
    series->setPen(QPen(Qt::blue,3,Qt::SolidLine));
    series->clear();
    for(int i=0; i<pointlist.size();i++)
        series->append(pointlist.at(i).x(), pointlist.at(i).y());

    qchart->setTitle(chartname);
    qchart->setAnimationOptions(QChart::SeriesAnimations);//�������߶���ģʽ
    qchart->legend()->hide(); //����ͼ��
    qchart->addSeries(series);//��������
    qchart->setAxisX(axisX, series);
    qchart->setAxisY(axisY, series);
}

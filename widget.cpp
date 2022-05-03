#include "widget.h"
#include "ui_widget.h"



widget::widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget)
{
    ui->setupUi(this);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(false);

//    connect(m_timer, &QTimer::timeout, this, &widget::slotTimeout);

    //��ȡ�����ļ�
    ReadFromFiles();

    //
    // �������������Ტ������ʾ��Χ
    //
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_axisX->setTitleText("X-label");
    m_axisY->setTitleText("Y-label");
    m_axisX->setMin(0);
    m_axisY->setMin(-AXIS_MAX_Y);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);

    m_lineSeries = new QSplineSeries();                             // �������߻��ƶ���
    m_lineSeries->setPointsVisible(true);                         // �������ݵ�ɼ�
    m_lineSeries->setName("Wave Plot");                            // ͼ������
    m_lineSeries->setPen(QPen(Qt::blue,2,Qt::SolidLine));
    m_lineSeries->clear();

    m_chart = new QChart();                                        // ����ͼ�����
    m_chart->addAxis(m_axisY, Qt::AlignLeft);                      // ��X����ӵ�ͼ����
    m_chart->addAxis(m_axisX, Qt::AlignBottom);                    // ��Y����ӵ�ͼ����
    m_chart->addSeries(m_lineSeries);                              // �����߶�����ӵ�ͼ����
    m_chart->setAnimationOptions(QChart::SeriesAnimations);        // ��������ʹ���߻�����ʾ�ĸ�ƽ��������Ч�����ÿ�

    m_lineSeries->attachAxis(m_axisX);                             // ���߶��������X�ᣬ�˲��������m_chart->addSeries֮��
    m_lineSeries->attachAxis(m_axisY);                             // ���߶��������Y�ᣬ�˲��������m_chart->addSeries֮��

    ui->graphicsView->setChart(m_chart);                           // ��ͼ��������õ�graphicsView�Ͻ�����ʾ
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);       // ������Ⱦ������ݣ������������ô���߾��Եò�ƽ��
    ui->graphicsView->setRubberBand(QChartView::RectangleRubberBand);   //XY����ͬʱ�Ŵ���껭���ľ��δ�С

}

widget::~widget()
{
    delete ui;
}

void widget::slotTimeout()
{
    m_lineSeries->clear();

    //ԭʼ����
    QVector<QPointF> pointsSeriesOld = pointsSeries;
    //���º������
    QVector<QPointF> pointsSeriesNew;

    //�������������ǰƽ��
    for(int i = 0; i < pointsSeriesOld.count()-256; i++)
    {
        pointsSeriesNew.append(QPointF(i,pointsSeriesOld[256 + i].y()));//����������ƽ�Ƶ���ͷ
    }

    //    //���������ݵ�pointsSeriesNew����
    //    for(int i = pointsSeriesOld.count()-256; i < pointsSeriesOld.count(); i++)
    //       {
    //           pointsSeriesNew.append(QPointF(i, pointsSeriesOld[i-pointsSeriesOld.count()+256].y()));
    //    }

    for(int i=0; i<pointsSeriesNew.size();i++)
        m_lineSeries->append(pointsSeriesNew.at(i).x(), pointsSeriesNew.at(i).y());

    pointsSeries = pointsSeriesNew;
}

void widget::ReadFromFiles()
{
    //1. ��ȡ�����ļ�
    saveFolder =  QString("F:/Desktop/UDPConnect/data/");

    saveFilenameAll = saveFolder+QString("[All][31]")+QString("20220502140910")+ QString(".bin");

    infileAll.open(saveFilenameAll.toStdString().data(), ifstream::binary);

    if(!infileAll.is_open()) return;

    infileAll.read(CHdata,READ_LENGTH);

    //2. four Bytes HEX to DEC >> CHdata_DEC_all[256]

    for(int i = 0; i<READ_LENGTH; i+=4){
        bool ok;
        int number_DEC = QString(CHdata[i]).toInt(&ok,16)*0 + QString(CHdata[i+1]).toInt(&ok,16)*256 + QString(CHdata[i+2]).toInt(&ok,16)*16 +QString(CHdata[i+3]).toInt(&ok,16)*1;
        if(number_DEC>2047)
            number_DEC = number_DEC-4096;
        int j = i/4;
        CHdata_DEC_all[j] = number_DEC;
    }

    //3. CHdata_DEC_all[256] split into 4 channels
    for(int k = 0; k<CHDATA_ALL_LENGTH; k+=4){
        int p = k/4;
        CHdata_DEC_1[p] = CHdata_DEC_all[k];
        CHdata_DEC_2[p] = CHdata_DEC_all[k+1];
        CHdata_DEC_3[p] = CHdata_DEC_all[k+2];
        CHdata_DEC_4[p] = CHdata_DEC_all[k+3];
    }
}


void widget::on_btnStartAndStop_clicked()
{

    //CHdata >> lineSeries
    for(int i = 0; i<CHDATA_LENGTH; i++){
        m_lineSeries->append(QPointF( i , CHdata_DEC_1[i] ) );
        pointsSeries.append(QPointF( i , CHdata_DEC_1[i] ) );
    }

    m_timer->start(1000);
}


void widget::on_btnReset_clicked()
{
    m_axisX->setMin(0);
    m_axisY->setMin(-AXIS_MAX_Y);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);
}


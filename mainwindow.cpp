#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initChart();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//��ʼ��ͼ��
void MainWindow::initChart()
{
    //���ñ�ͷ
    chart = new Chart(this,"Wave Display");

    //��������ϵ
    chart->setAxis("X Axis",0,CHDATA_LENGTH,10, "Y Axis",0,70000,10);

    /* ---- ��ȡ��ɢ������----- */
    ReadFromFiles();

    //4. ����pointSeries
    for(int i = 0; i<CHDATA_LENGTH; i++){

        pointsSeries.append(QPointF( i , CHdata_DEC_1[i] ) );

    }
    qDebug()<<"pointsSeries Length=" <<pointsSeries.count();

    //����
    chart->buildChart(pointsSeries);

    //ΪMainWindow��centralWidget���һ������
    QHBoxLayout *pHLayout = new QHBoxLayout(ui->centralwidget);

    //��chart��ӵ�������
    pHLayout->addWidget(chart);
}

void MainWindow::ReadFromFiles()
{
    //1. ��ȡ�����ļ�
    saveFolder =  QString("F:/Desktop/UDPConnect/data/");

    saveFilenameAll = saveFolder+QString("[All][31]")+QString("20220502140910")+ QString(".bin");

    infileAll.open(saveFilenameAll.toStdString().data(), ifstream::binary);

    if(!infileAll.is_open()) return;

    CHdata[0] = '\0';

    infileAll.read(CHdata,READ_LENGTH);

    //2. four Bytes HEX to DEC >> CHdata_DEC_all[256]
    CHdata_DEC_all[0] = '\0';

    for(int i = 0; i<READ_LENGTH; i+=4){
        bool ok;
        int number_DEC = QString(CHdata[i]).toInt(&ok,16)*0 + QString(CHdata[i+1]).toInt(&ok,16)*4096 + QString(CHdata[i+2]).toInt(&ok,16)*256 +QString(CHdata[i+3]).toInt(&ok,16)*1;
        if(number_DEC>2047)
            number_DEC = number_DEC-2047;
        int j = i/4;
        CHdata_DEC_all[j] = number_DEC;
    }

    //3. CHdata_DEC_all[256] split into 4 channels
    CHdata_DEC_1[0] = '\0';
    CHdata_DEC_2[0] = '\0';
    CHdata_DEC_3[0] = '\0';
    CHdata_DEC_4[0] = '\0';

    for(int k = 0; k<CHDATA_ALL_LENGTH; k+=4){
        int p = k/4;
        CHdata_DEC_1[p] = CHdata_DEC_all[k];
        CHdata_DEC_2[p] = CHdata_DEC_all[k+1];
        CHdata_DEC_3[p] = CHdata_DEC_all[k+2];
        CHdata_DEC_4[p] = CHdata_DEC_all[k+3];
    }
}

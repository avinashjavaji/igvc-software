#include "stereotuner.h"
#include "ui_stereotuner.h"

StereoTuner::StereoTuner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StereoTuner)
{
    ui->setupUi(this);
    signalMapper = new QSignalMapper(this);

    //connect all the sliders to the onSliderMove Slot
    connect(ui->SADWindowSizeSlider,SIGNAL(sliderReleased()),SLOT(onSliderMove()));
    connect(ui->numberOfDisparitiesSlider,SIGNAL(sliderReleased()),SLOT(onSliderMove()));
    connect(ui->preFilterSizeSlider,SIGNAL(sliderReleased()),SLOT(onSliderMove()));
    connect(ui->preFilterCapSlider,SIGNAL(sliderReleased()),SLOT(onSliderMove()));
    connect(ui->minDisparitySlider,SIGNAL(sliderReleased()),SLOT(onSliderMove()));
    connect(ui->textureThresholdSlider,SIGNAL(sliderReleased()),SLOT(onSliderMove()));
    connect(ui->uniquenessRatioSlider,SIGNAL(sliderReleased()),SLOT(onSliderMove()));
    connect(ui->speckleWindowSizeSlider,SIGNAL(sliderReleased()),SLOT(onSliderMove()));
    connect(ui->speckleRangeSlider,SIGNAL(sliderReleased()),SLOT(onSliderMove()));
}

StereoTuner::~StereoTuner()
{
    delete ui;
}

void StereoTuner::paintEvent(QPaintEvent *e)
{
    QSlider *slider = (QSlider *)sender();

    QWidget::paintEvent(e);
}

void StereoTuner::onSliderMove()
{

}

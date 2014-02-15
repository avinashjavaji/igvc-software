#ifndef STEREOTUNER_H
#define STEREOTUNER_H

#include <QWidget>
#include <QSignalMapper>

namespace Ui {
class StereoTuner;
}

/*!
 * \brief Widget for fine tuning stereo disparity image.
 * \author Idan Mintz
 */

class StereoTuner : public QWidget
{
    Q_OBJECT

public:
    explicit StereoTuner(QWidget *parent = 0);
    ~StereoTuner();

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::StereoTuner *ui;

    QSignalMapper* signalMapper;

    //The Variable Properties to be tuned set to defaults
    int SADWindowSize = 15;
    int numberOfDisparities = 16;
    int preFilterSize = 63;
    int preFilterCap = 63;
    int minDisparity = 0;
    int textureThreshold = 30;
    int uniquenessRatio = 3;
    int speckleWindowSize = 20;
    int speckleRange = 32;

    void onSliderMove();

};

#endif // STEREOTUNER_H

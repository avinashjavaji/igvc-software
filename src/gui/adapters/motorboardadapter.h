#ifndef MOTORBOARDADAPTER_H
#define MOTORBOARDADAPTER_H

#include <QWidget>
#include <hardware/actuators/motors/MotorDriver.hpp>
#include <memory>

namespace Ui {
class MotorBoardAdapter;
}

class MotorBoardAdapter : public QWidget
{
    Q_OBJECT

protected:
    typedef std::pair<double,double> VelocityPair;
    typedef std::pair<VelocityPair,VelocityPair> VelocityPairs;

public:
    explicit MotorBoardAdapter(std::shared_ptr<MotorDriver> driver, QWidget *parent = 0);
    ~MotorBoardAdapter();

public slots:
    void onNewCurrentVelocities(double left, double right);

private slots:
    void paintEvent(QPaintEvent *);

    void on_lineEdit_leftVel_returnPressed();

    void on_lineEdit_rightVel_returnPressed();

private:
    Ui::MotorBoardAdapter *ui;

    std::shared_ptr<MotorDriver> _driver;

    std::vector<VelocityPairs> _data;

    unsigned int _dataCount;
};

#endif // MOTORBOARDADAPTER_H

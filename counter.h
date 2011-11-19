#ifndef COUNTER_H
#define COUNTER_H

#include <sys/time.h>

#include <QObject>
#include <QAccelerometer>
#include <QTimer>
#include <QDebug>
#include <qplatformdefs.h>

#if defined(MEEGO_EDITION_HARMATTAN)
#include <qmsystem2/qmdisplaystate.h>
#endif

#include "ring.h"

QTM_USE_NAMESPACE

#define COUNTER_NUM_PLAYERS 2

/// Step counter.
class Counter: public QObject {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY step)
    Q_PROPERTY(int rawCount READ rawCount WRITE setRawCount NOTIFY rawCountChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(qreal calibration READ calibration WRITE setCalibration NOTIFY calibrationChanged)
    Q_PROPERTY(int sensitivity READ sensitivity WRITE setSensitivity NOTIFY sensitivityChanged)

public:
    explicit Counter(QObject *parent = 0);
    ~Counter();
    int count() {return stepCount;}
    bool running();
    qreal calibration() {return calibration_;}
    int rawCount() {return rawStepCount;}
    int sensitivity() {return sensitivity_;}

signals:
    void step(int count);
    void runningChanged();
    void calibrationChanged(qreal value);
    void rawCountChanged(int value);
    void sensitivityChanged(int value);

public slots:
    void measure();
    void pauseBlanking();
    Q_INVOKABLE void reset();
    void setRunning(bool running);
    void applicationActivated(bool active);
    void setCalibration(qreal value);
    void setRawCount(int value);
    void setSensitivity(int value);

protected:
    QTimer *timer;
    QTimer *blankingTimer;
    QAccelerometer *accelerometer;
#if defined(MEEGO_EDITION_HARMATTAN)
    MeeGo::QmDisplayState *displayState;
#endif
    Ring *ring;
    int peakCount;
    int rawStepCount; ///< Raw step count.
    int stepCount; ///< Calibrated step count.
    struct timeval lastPeakTime;
    long lastPeakTimeDiff;
    qreal calibration_; ///< Calibration coefficient. Default: 1.
    int sensitivity_; ///< Default: 100.
};

#endif // COUNTER_H
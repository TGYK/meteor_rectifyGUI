//============================================================================
// Name        : rectifythread.h
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This is the class definition of RectifyThread. Special note
//               is the signal used to notify the controller of work being
//               done.
//============================================================================

#ifndef RECTIFYTHREAD_H
#define RECTIFYTHREAD_H
#include <QThreadPool>
#include <QDebug>
#include <QImage>
#include <QMutex>
#include <vector>

using namespace std;

class RectifyThread: public QObject, public QRunnable{
Q_OBJECT

private:
    const QImage *original_pixels;
    QImage *rectified_pixels;
    int rectified_width;
    vector<long double> correction_factor;
    int end_row;
    int start_row;
    int *rows_completed;
    static QMutex mutex;
public:
//    explicit RectifyThread(QObject *parent = nullptr);
    RectifyThread(const QImage *original_pixels, QImage *rectified_pixels, int rectified_width, vector<long double> correction_factor, int end_row, int start_row, int *rows_completed):
        original_pixels(original_pixels), rectified_pixels(rectified_pixels), rectified_width(rectified_width), correction_factor(correction_factor), end_row(end_row), start_row(start_row), rows_completed(rows_completed){}
    virtual ~RectifyThread() {};
    void run() override;
signals:
    void rowCompleted();
};
#endif // RECTIFYTHREAD_H

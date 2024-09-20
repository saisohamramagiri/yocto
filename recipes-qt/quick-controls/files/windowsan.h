#ifndef WINDOWSAN_H
#define WINDOWSAN_H

#include <QQuickWindow>
#include <QObject>
#include <QQmlEngine>


class Windowsan : public QQuickWindow
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Windowsan(QQuickWindow *parent = nullptr);

signals:
};

#endif // WINDOWSAN_H

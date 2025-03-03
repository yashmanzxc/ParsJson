#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>

class QPushButton;
class QFrame;
class QVBoxLayout;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void selectFile();
    void startFile();

private:
    void creatingFolders(const QString &fileJ);

private:
    QPushButton *startButton;
    QPushButton *fileButton;
    QFrame *frame;
    QVBoxLayout *framelayout;
    QVBoxLayout *layout;
    QString jsonFile;
};

#endif // WIDGET_H

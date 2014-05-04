#ifndef WIDGET2_H
#define WIDGET2_H

#include <QWidget>
#include "widget.h"

class window : public QWidget
{
    Q_OBJECT
public:
    explicit window(QWidget *parent = 0);
    ~window();
    void addColor(std::vector<int> *color);
    void setColor(std::vector<std::vector<int> *> *arr_color);
    void clearColor();

signals:

public slots:
    void upColor();
    void downColor();
    void randomColor();

private:
    widget *w;
    QHBoxLayout *hl;
    QHBoxLayout *navLayout;
    QVBoxLayout *userLayout;
    QPushButton *next_click, *prev_click;
    QGroupBox *user_group;
    QLabel *listClicks;
    std::vector<std::vector<int> *> *graph;   //представление графа
    std::vector<std::vector<int> *> *_color;   //цвета вершин графа
    int currColor;
    QPushButton *addColorButton;

    void keyPressEvent(QKeyEvent *event);
};

#endif // WIDGET2_H

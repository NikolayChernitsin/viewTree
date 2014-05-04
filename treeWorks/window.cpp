#include "window.h"

window::window(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumHeight(522);
    this->setMinimumWidth(728);
    user_group = new QGroupBox();   //QGroupBox для расположения кнопок
    user_group->setFixedWidth(200);
    userLayout = new QVBoxLayout();

    //на эту кнопку следует повесить метод алгоритма
    addColorButton = new QPushButton();
    //имя на кнопке
    addColorButton->setText("add random color");
    //связь кнопки с методом организуется следующим образом
    connect(addColorButton,SIGNAL(clicked()),this,SLOT(randomColor()));
    //новые элементы и слои формы устанавливать на слой userLayout
    userLayout->addWidget(addColorButton);

    _color = new std::vector<std::vector<int> *>();
    currColor = -1;
    listClicks = new QLabel();
    listClicks->setText(QString::fromUtf8("%1 / %2").arg(currColor + 1).arg(_color->size()));
    navLayout = new QHBoxLayout();
    prev_click = new QPushButton();
    prev_click->setText("<");
    prev_click->setFixedWidth(20);
    prev_click->setEnabled(false);
    connect(prev_click,SIGNAL(clicked()),this,SLOT(downColor()));
    next_click = new QPushButton();
    next_click->setText(">");
    next_click->setFixedWidth(20);
    next_click->setEnabled(false);
    connect(next_click,SIGNAL(clicked()),this,SLOT(upColor()));
    navLayout->addWidget(prev_click);
    navLayout->addWidget(listClicks);
    navLayout->addWidget(next_click);
    userLayout->addLayout(navLayout);

    user_group->setLayout(userLayout);

    hl = new QHBoxLayout();
    w = new widget();
    //тестирование отображения графа//
    graph = new std::vector<std::vector<int> *>();
    for (int i = 0; i < 8; i++)
    {
        std::vector<int> *temp = new std::vector<int>();
        if (i%3 == 0)
            for (int i = 0; i < 8; i++)
                temp->push_back(i);
        graph->push_back(temp);
    }
    //конец//
    w->setGraph(graph); //передача графа на отображение
    hl->addWidget(user_group);
    hl->addWidget(w);
    this->setLayout(hl);
}

window::~window()
{
    //delete w;
    //delete hl;
    //delete but;
    //delete user_group;
}

void window::keyPressEvent(QKeyEvent *event)
{
    w->keyPressEvent(event);
}

/**
 * @brief window::addColor - добавление новой раскраски графа
 * @param color - раскраска графа
 *  представляется в виде:
 *      - номер элемента массива раскраски соответствует номеру вершины графа
 *      - цвета устанавливаются в интервале 0-15, 0(по-умолчанию)
 *      - элемент массива раскраски содержит цвет в который следует раскрасить вершину
 */
void window::addColor(std::vector<int> *color)
{
    _color->push_back(color);
    listClicks->setText(QString::fromUtf8("%1 / %2").arg(currColor + 1).arg(_color->size()));
    currColor = -1;
    next_click->setEnabled(true);
}

/**
* @brief window::setColor - функция для установки сразу всех вариантов раскраски графа
* @param arr_color - массив раскрасок графа
*/
void window::setColor(std::vector<std::vector<int> *> *arr_color)
{
    _color = arr_color;
    listClicks->setText(QString::fromUtf8("%1 / %2").arg(currColor + 1).arg(_color->size()));
    currColor = -1;
    if(_color->size() > 0)
        next_click->setEnabled(true);
}

/**
 * @brief window::clearColor - очистка раскрасок графа
 */
void window::clearColor()
{
    _color->clear();
    w->updateColor(0);
    listClicks->setText(QString::fromUtf8("%1 / %2").arg(currColor + 1).arg(_color->size()));
    currColor = -1;
    prev_click->setEnabled(false);
    next_click->setEnabled(false);
}

void window::upColor()
{
    currColor++;
    listClicks->setText(QString::fromUtf8("%1 / %2").arg(currColor + 1).arg(_color->size()));
    if((unsigned)(currColor + 1) == _color->size())
        next_click->setEnabled(false);
    prev_click->setEnabled(true);
    w->updateColor(_color->at(currColor));
}

void window::downColor()
{
    currColor--;
    listClicks->setText(QString::fromUtf8("%1 / %2").arg(currColor + 1).arg(_color->size()));
    if(currColor == -1)
        prev_click->setEnabled(false);
    next_click->setEnabled(true);
    w->updateColor((currColor == -1) ? 0 : _color->at(currColor));
}

void window::randomColor()
{
    std::vector<int> *nrcolor = new std::vector<int>();
    for(unsigned i = 0; i < graph->size(); i++)
        //if(i == rand() % graph->size())
            nrcolor->push_back(0);
    for(unsigned i = 0; i < graph->size(); i++)
        if(i == rand() % graph->size())
            nrcolor->at(i) = 1;
    addColor(nrcolor);
}

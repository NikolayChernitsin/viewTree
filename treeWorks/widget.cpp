#include "widget.h"
#include <math.h>
#include "geomvector.h"
#include <stdlib.h>

widget::widget(QGLWidget *parent) :
    QGLWidget(parent)
{
    width = 500;
    cwidth = 250;
    height = 500;
    cheight = 250;
    setFormat(QGLFormat(QGL::DoubleBuffer)); // Двойная буферизация
    glDepthFunc(GL_LEQUAL); // Буфер глубины
    offsetX = 0;
    offsetY = 0;
    mousePress = false;
    scale = 1;
    QColor tcolors[16] = {Qt::white,        Qt::red,        Qt::darkRed,    Qt::green,      Qt::darkGreen,
                         Qt::blue,         Qt::darkBlue,   Qt::cyan,       Qt::darkCyan,   Qt::magenta,
                         Qt::darkMagenta,  Qt::yellow,     Qt::darkYellow, Qt::gray,       Qt::darkGray, Qt::lightGray};
    for (int i = 0; i < 16; i++)
        colors[i] = tcolors[i];
}

widget::~widget()
{
}

void widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT); // чистим буфер
    glMatrixMode(GL_PROJECTION); // устанавливаем матрицу
    glLoadIdentity(); // загружаем матрицу
    glOrtho(0,width,height,0,1,0); // подготавливаем плоскости для матрицы
    glScalef(scale, scale, scale);
    if (_graph == 0)
    {
        qglColor(Qt::white);
        renderText(10, 10, 0, QString::fromUtf8("Graph is not initialize!"), QFont(), 2000);
        swapBuffers();
        return;
    }
    else
    {
        graphAnalyze();
        swapBuffers();
        return;
    }
    // BlendFunc позволяет работать в альфа режиме, например если нам нужно указывать прозрачность
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    qglColor(Qt::white); // Дальше рисуем белым цветом
    //  renderText позволяет писать текст на экран, так же можно указать различный стиль (читаем QFont)
    renderText(10, 10 , 0, QString::fromUtf8("Вы набрали %1 очков:").arg(_graph->size()), QFont() , 2000);

    // glBegin и glEnd - обозначают блок для рисования объекта(начало и конец), glBegin принимает параметр того, что нужно рисовать.
    glBegin(GL_POLYGON);
        glColor4f(0,1,0, 0.25);// Цвет которым рисовать
        glVertex2f(200, 300); // Точка 1 из 4 - отсчет по часовой стрелке
        glVertex2f(300, 300);
        glVertex2f(300, 400);
        glVertex2f(200, 400);
    glEnd();
    swapBuffers();
}

void widget::initializeGL()
{
    qglClearColor(Qt::black); // Черный цвет фона
}

void widget::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLint)w, (GLint)h);
    width = w;
    cwidth = w / 2;
    height = h;
    cheight = h / 2;
}

void widget::setGraph(const std::vector<std::vector<int> *> *graph, const std::vector<int> *color)
{
    _graph = graph;
    if (color != 0 && color->size() == graph->size())
        _color = color;
    updateGL();
}

void widget::updateColor(std::vector<int> *color)
{
    _color = 0;
    if (color != 0 && color->size() == _graph->size())
        _color = color;
    updateGL();
}

void widget::graphAnalyze()
{
    double rad = _graph->size() * (2 * RadVertex + Space) / (2 * M_PI);
    double dAlpha = 360 / _graph->size();

    geomVector vect(0, rad, 0, 0);
    geomVector vectText(0, rad + 2 * Space, 0, 0);
    for (unsigned int i = 0; i < _graph->size(); i++)
    {
        printVertex(vect.getVector()[X], vect.getVector()[Y], colors[(_color == 0) ? 0 : _color->at(i)]);
        printText(vectText.getVector()[X], vectText.getVector()[Y], QString::fromUtf8("%1").arg(i + 1), colors[(_color == 0) ? 0 : _color->at(i)]);
        if (!_graph[i].empty())
        {
            for (unsigned int j = 0; j < _graph->at(i)->size(); j++)
            {
                geomVector tempVect(0, rad, 0, 0);
                tempVect.rot((_graph->at(i)->at(j)) * dAlpha);
                printLine(vect.getVector()[X], vect.getVector()[Y], tempVect.getVector()[X], tempVect.getVector()[Y], colors[(_color != 0 && _color->at(i) == _color->at(_graph->at(i)->at(j))) ? _color->at(i) : 0]);
            }
        }
        vect.rot(dAlpha);
        vectText.rot(dAlpha);
    }
}

void widget::printVertex(int x, int y, QColor c)
{
    qglColor(c);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < RadVertex * 30; i++ )
    {
        float a = i / (RadVertex * 30) * M_PI * 2.0f;
        glVertex2f(offsetX + x + cwidth + cos(a) * RadVertex, offsetY + y + cheight + sin(a) * RadVertex);
    }
    glEnd();
}

void widget::printText(int x, int y, QString qstr, QColor c)
{
    qglColor(c); // Дальше рисуем белым цветом
    //  renderText позволяет писать текст на экран, так же можно указать различный стиль (читаем QFont)
    if (x < 0)
        x -= qstr.length() * 7;
    if (y > 0)
        y += 7;
    if (x == 0)
        x -= qstr.length() * 4;
    renderText(offsetX + x + cwidth, offsetY + y + cheight , 0, qstr, QFont() , 2000);
}

void widget::printLine(int x0, int y0, int x1, int y1, QColor c)
{
    qglColor(c);
    geomVector vect(x0,y0,x1,y1);
    vect.cut(RadVertex);
    glBegin(GL_LINES);
        glVertex2f(offsetX + vect.getVector()[X] + x0 + cwidth, offsetY + vect.getVector()[Y] + y0 + cheight);
        glVertex2f(offsetX - vect.getVector()[X] + x1 + cwidth, offsetY - vect.getVector()[Y] + y1 + cheight);
    glEnd();
}

void widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePress = true;
        mouseX = event->x();
        mouseY = event->y();
    }
}

void widget::mouseReleaseEvent(QMouseEvent *)
{
    mousePress = false;
}

void widget::mouseMoveEvent(QMouseEvent *event)
{
    if (!mousePress)
        return;
    offsetX -= (mouseX - event->x()) / scale;
    mouseX = event->x();
    offsetY -= (mouseY - event->y()) / scale;
    mouseY = event->y();
    updateGL();
}

void widget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        scale_up(event->x(), event->y());
    if (event->delta() < 0)
        scale_down(event->x(), event->y());
    updateGL();
}

void widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Plus:
        scale_up(cwidth, cheight);
        break;
    case Qt::Key_Minus:
        scale_down(cwidth, cheight);
        break;
    case Qt::Key_Up:
        offsetY -= dOffset;
        break;
    case Qt::Key_Down:
        offsetY += dOffset;
        break;
    case Qt::Key_Left:
        offsetX -= dOffset;
        break;
    case Qt::Key_Right:
        offsetX += dOffset;
        break;
    }
    updateGL();
}

void widget::scale_up(int mx, int my)
{
    scale *= dscale;
    offsetX -= (mx * dscale - mx) / scale;
    offsetY -= (my * dscale - my) / scale;
}

void widget::scale_down(int mx, int my)
{
    scale /= dscale;
    offsetX -= (mx / dscale - mx) / scale;
    offsetY -= (my / dscale - my) / scale;
}

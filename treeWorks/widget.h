#ifndef widget_H
#define widget_H

#include <QGLWidget>
#include <QtOpenGL>
#include <vector>

class widget : public QGLWidget
{
    Q_OBJECT

public:
    explicit widget(QGLWidget *parent = 0);
    ~widget();
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);
    //void updateGL();
    void setGraph(const std::vector<std::vector<int> *> *graph, const std::vector<int> *color = 0);
    void updateColor(std::vector<int> *color = 0);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    int width, cwidth;
    int height, cheight;
    const std::vector<std::vector<int> *> *_graph;
    const std::vector<int> *_color;
    static const double RadVertex = 20;
    static const int Space = 20;
    int offsetX, offsetY;
    bool mousePress;
    int mouseX, mouseY;
    double scale;
    static const double dscale = 1.1;
    QColor colors[16];
    static const int dOffset = 20;

    void printVertex(int x, int y, QColor c = Qt::white);
    void printLine(int x0, int y0, int x1, int y1, QColor c = Qt::white);
    void printText(int x, int y, QString qstr, QColor c = Qt::white);
    void graphAnalyze();
    void scale_up(int mx, int my);
    void scale_down(int mx, int my);
};

#endif // widget_H

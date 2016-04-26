
#ifndef KINGDOM_VIEWWIDGET_HPP
#define KINGDOM_VIEWWIDGET_HPP

#include <QGLWidget>
#include <QTimer>

class ViewWidget : public QGLWidget
{
    Q_OBJECT
protected:
    int32_t **p_desk;
    double m_ceil_width;
    double m_ceil_height;
    int32_t m_pressed_i;
    int32_t m_pressed_j;
    int32_t m_current_i;
    int32_t m_current_j;
    bool m_pressed;

    QTimer m_timer;

public:
    explicit ViewWidget(QWidget *parent = 0);

    void set_desk(int32_t **desk);
    int32_t **desk();

protected:
    virtual void mousePressEvent(QMouseEvent *mouseEvent) override;
    virtual void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent) override;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};


#endif //KINGDOM_VIEWWIDGET_HPP

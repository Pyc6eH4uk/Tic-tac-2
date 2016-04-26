//
// Created by gdhsnlvr on 08.04.16.
//

#include "ViewWidget.h"

#include <QMouseEvent>
#include <QDebug>
#include <QtCore/qtimer.h>

ViewWidget::ViewWidget(QWidget *parent)
        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    p_desk = nullptr;
    m_ceil_width = width() / 8.0;
    m_ceil_height = height() / 8.0;
    m_pressed_i = -1;
    m_pressed_j = -1;

    m_current_i = -1;
    m_current_j = -1;
    m_pressed = false;

    m_timer.start(1000/60);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(repaint()));

    setMouseTracking(true);
}

void ViewWidget::set_desk(int32_t **desk)
{
    p_desk = desk;
}

int32_t **ViewWidget::desk()
{
    return p_desk;
}

void ViewWidget::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    QWidget::mouseReleaseEvent(mouseEvent);

    if (p_desk == nullptr || !m_pressed) return;
    if (m_current_i != m_pressed_i || m_current_j != m_pressed_j) {
        p_desk[m_current_i][m_current_j] = p_desk[m_pressed_i][m_pressed_j];
        p_desk[m_pressed_i][m_pressed_j] = 0;
    }

    m_pressed_i = -1;
    m_pressed_j = -1;
    m_pressed = false;
}

void ViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if (m_pressed) {
        int32_t x = std::max(std::min(event->pos().x(), width()), 0);
        int32_t y = std::max(std::min(event->pos().y(), height()), 0);
        m_current_i = 8 - (int32_t) y / m_ceil_height;
        m_current_j = (int32_t) x / m_ceil_width;
        m_current_i = std::max(std::min(m_current_i, 7), 0);
        m_current_j = std::max(std::min(m_current_j, 7), 0);
    }
    //repaint();
}

void ViewWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    int32_t x = std::max(std::min(event->pos().x(), width()), 0);
    int32_t y = std::max(std::min(event->pos().y(), height()), 0);
    m_pressed_i = 8 - (int32_t) y / m_ceil_height;
    m_pressed_j = (int32_t) x / m_ceil_width;
    m_pressed_i = std::max(std::min(m_pressed_i, 7), 0);
    m_pressed_j = std::max(std::min(m_pressed_j, 7), 0);
    if (p_desk != nullptr && p_desk[m_pressed_i][m_pressed_j] != 0) {
        m_current_i = m_pressed_i;
        m_current_j = m_pressed_j;
        m_pressed = true;
    }
}

void ViewWidget::initializeGL()
{
    QGLWidget::initializeGL();
    qglClearColor(QColor(123, 23, 53));
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
}

void ViewWidget::resizeGL(int w, int h)
{
    QGLWidget::resizeGL(w, h);
    glViewport(0, 0, w, h);

    m_ceil_width = width() / 8.0;
    m_ceil_height = height() / 8.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

#ifdef QT_OPENGL_ES_1
    glOrthof(0, w, 0, h, 1e9, -1e9);
#else
    glOrtho(0, w, 0, h, 1e9, -1e9);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void ViewWidget::paintGL()
{
    QGLWidget::paintGL();
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (p_desk == nullptr)
        return;

    glLineWidth(2.0);
    for (int i = 0; i <= 8; i++) {
        glBegin(GL_LINES);
        glVertex2d(0, i * m_ceil_height);
        glVertex2d(width(), i * m_ceil_height);
        glEnd();
    }
    for (int i = 0; i <= 8; i++) {
        glBegin(GL_LINES);
        glVertex2d(i * m_ceil_width, 0);
        glVertex2d(i * m_ceil_width, height());
        glEnd();
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i == m_current_i && j == m_current_j && m_pressed) {
                glColor3d(1, 1, 1);
                glBegin(GL_LINE_LOOP);
                glVertex2d(j * m_ceil_width, i * m_ceil_height);
                glVertex2d((j+1) * m_ceil_width, i * m_ceil_height);
                glVertex2d((j+1) * m_ceil_width, (i+1) * m_ceil_height);
                glVertex2d(j * m_ceil_width, (i+1) * m_ceil_height);
                glEnd();
            }

            if (p_desk[i][j] == 0) continue;
            if (p_desk[i][j] == -1) glColor3d(1, 1, 1);
            if (p_desk[i][j] == 1) glColor3d(0, 0, 0);
            glBegin(GL_TRIANGLES);
                glVertex2d((j + 0.5) * m_ceil_width, (i + 0.9) * m_ceil_height);
                glVertex2d((j + 0.1) * m_ceil_width, (i + 0.1) * m_ceil_height);
                glVertex2d((j + 0.9) * m_ceil_width, (i + 0.1) * m_ceil_height);
            glEnd();
        }
    }
}

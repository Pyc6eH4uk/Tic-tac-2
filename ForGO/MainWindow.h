#ifndef KINGDOM_MAINWINDOW_HPP
#define KINGDOM_MAINWINDOW_HPP

#include <QMainWindow>
#include "ViewWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    ViewWidget *m_view;
    int32_t **m_desk;

public:
    MainWindow();
};


#endif //KINGDOM_MAINWINDOW_HPP

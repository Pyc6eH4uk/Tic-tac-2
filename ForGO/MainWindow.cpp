#include "MainWindow.h"

MainWindow::MainWindow()
    : QMainWindow(), m_view(new ViewWidget(this))
{
   m_desk = new int32_t*[8];
    for (int32_t i = 0; i < 8; i++) {
        m_desk[i] = new int32_t[8];
        for (int32_t j = 0; j < 8; j++) {
            m_desk[i][j] = (i + j) % 3 - 1;
        }
    }

    m_view->set_desk(m_desk);
    m_view->setMinimumSize(800, 500);

    setMinimumSize(800, 500);
    setCentralWidget(m_view);
}

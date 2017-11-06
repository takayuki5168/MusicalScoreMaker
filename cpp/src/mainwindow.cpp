#include <QPainter>
#include <mainwindow.hpp>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_timer(new QTimer), m_event(new Event)
{
    setGeometry(200, 0, 960, 720);
    setWindowTitle("MusicalScoreMaker");

    setMouseTracking(true);

    m_timer->start(m_time_step);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

void MainWindow::paintEvent(QPaintEvent* paint_event)
{
    //auto delta_time = 1000 * time
    m_note->update();
    QPainter painter(this);
    m_note->paint(painter);
}

void MainWindow::update()
{
}

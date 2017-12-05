#include <iostream>
#include <QPainter>
#include "mainwindow.hpp"
#include "basic_params.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_timer(new QTimer)
{
    m_note = std::make_unique<Note>();
    m_event = std::make_unique<Event>();

    setGeometry(200, 0, 960, 720);
    setWindowTitle("MusicalScoreMaker");

    setMouseTracking(true);

    {  // init button
        for (int i = 0; i < ButtonNum; i++) {
            m_button.at(i) = new QPushButton{m_button_params.name.at(i), this};
            m_button.at(i)->setGeometry(QRect(m_button_params.pos.at(i), m_button_params.size.at(i)));
            m_button.at(i)->setCheckable(m_button_params.toggle_or_not.at(i));
            if (m_button_params.toggle_or_not.at(i)) {
                if (i == 3) {
                    connect(m_button.at(i), SIGNAL(toggled(bool)), this, SLOT(setModePause()));
                } else if (i == 4) {
                    connect(m_button.at(i), SIGNAL(toggled(bool)), this, SLOT(setModeInput()));
                } else if (i == 5) {
                    connect(m_button.at(i), SIGNAL(toggled(bool)), this, SLOT(setModePlay()));
                }
            } else {
                connect(m_button.at(i), SIGNAL(clicked()), this, SLOT(po()));
            }
        }
    }

    m_timer->start(BasicParams::time_step);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

void MainWindow::paintEvent(QPaintEvent* paint_event)
{
    //auto delta_time = 1000 * time
    QPainter painter(this);
    m_note->update(m_mode);

    {  // paint Frame
        painter.setPen(QPen(m_color.blue, 2));
        painter.drawLine(50, BasicParams::frame_y_center - 200, 50, BasicParams::frame_y_center + 200);
    }
    {      // paint Note Class
        {  //paint vertical lines
            auto po = m_note->getOneXVector();
            painter.setPen(QPen(m_color.black, 2));
            for (const double& x : m_note->getOneXVector()) {
                painter.drawLine(x, 310 - 30, x, 310 + 30);
            }
            for (const double& x : m_note->getOneFourthXVector()) {
                painter.drawLine(x, 310 - 15, x, 310 + 15);
            }
        }
        {// paint notes
        }
        {  // paint piano
            painter.setPen(QPen(m_color.black, 2));
            painter.drawLine(60, 550, 900, 550);
            painter.drawLine(60, 700, 900, 700);
            painter.drawLine(60, 550, 60, 700);
            painter.drawLine(900, 550, 900, 700);

            painter.setBrush(m_color.white);
            double step_white = (900 - 60) / 21.0;
            for (int i = 0; i < 21; i++) {
                double x = (900 - 60) / 21.0 * i + 60;
                painter.drawRect(x, 550, step_white, 700 - 550);
            }

            painter.setBrush(m_color.black);
            double step_black = (900 - 60) / 21.0 * 0.4;
            for (int i = 0; i < (21 - 1); i++) {
                if (i % 7 == 2 or i % 7 == 6) {
                    continue;
                }
                double x = (900 - 60) / 21.0 * (i + 0.8) + 60;
                painter.drawRect(x, 550, step_black, 650 - 550);
            }
        }
    }

    {  // paint to delete unneccesary part
        painter.setPen(QPen(m_color.origin, 2));
        painter.setBrush(m_color.origin);
        painter.drawRect(0, BasicParams::frame_y_center - 200, 50 - 2, 400);
        painter.drawRect(900, BasicParams::frame_y_center - 200, 960 - 600, 400);
    }
}

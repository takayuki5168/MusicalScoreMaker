#include <iostream>
#include <chrono>
#include <thread>
#include <QPainter>
#include <QLabel>
#include "mainwindow.hpp"
#include "basic_params.hpp"
#include "mode.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_timer(new QTimer)
{
    m_note_manager = std::make_shared<NoteManager>();
    m_event_manager = std::make_unique<EventManager>(m_note_manager);

    // TODO ボタンから操作できるように
    //readCoord();
    //std::cout << m_coord.size() << std::endl;
    readNote();

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
                    connect(m_button.at(i), SIGNAL(toggled(bool)), this, SLOT(po()));  //setModePause()));
                } else if (i == 4) {
                    connect(m_button.at(i), SIGNAL(toggled(bool)), this, SLOT(po()));  //setModeInput()));
                } else if (i == 5) {
                    connect(m_button.at(i), SIGNAL(toggled(bool)), this, SLOT(po()));  //setModePlay()));
                }
            } else {
                connect(m_button.at(i), SIGNAL(clicked()), this, SLOT(po()));
            }
        }
    }

    m_timer->start(BasicParams::time_step);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_cnt = std::chrono::system_clock::now();
}

void MainWindow::paintEvent(QPaintEvent* /*paint_event*/)
{
    QPainter painter(this);

    m_note_manager->update();

    // 譜面のy座標の計算
    std::vector<int> y_pos;
    if (BasicParams::octet_num == 1) {
        y_pos = {0};
    } else if (BasicParams::octet_num == 2) {
        y_pos = {static_cast<int>(-BasicParams::frame_y_length / 8), static_cast<int>(BasicParams::frame_y_length / 8)};
    } else if (BasicParams::octet_num == 3) {
        y_pos = {static_cast<int>(-BasicParams::frame_y_length / 4), 0, static_cast<int>(BasicParams::frame_y_length / 4)};
    } else if (BasicParams::octet_num == 4) {
        y_pos = {
            static_cast<int>(-BasicParams::frame_y_length / 10 * 3), static_cast<int>(-BasicParams::frame_y_length / 10 * 1),
            static_cast<int>(BasicParams::frame_y_length / 10 * 1), static_cast<int>(BasicParams::frame_y_length / 10 * 3)};
    }

    {  // 枠の描画
        painter.setPen(QPen(m_color.blue, 2));
        painter.drawLine(50, BasicParams::frame_y_center - BasicParams::frame_y_length / 2, 50, BasicParams::frame_y_center + BasicParams::frame_y_length / 2);
    }

    {      // Noteクラスの描画
        {  // 縦線群の描画
            //auto po = m_note_manager->getOneXVector();
            painter.setPen(QPen(m_color.black, 2));
            m_coord_label.clear();
            for (int y : y_pos) {
                for (const double& x : m_note_manager->getOneXVector()) {
                    if (y == y_pos.at(0)) {  // 表示するコードの更新
                        unsigned int coord_idx = rangeInt((x + m_note_manager->getNowX() - 50) / 200.0);
                        if (coord_idx < m_coord.size()) {
                            std::shared_ptr<QLabel> coord_label = std::make_shared<QLabel>(m_coord.at(coord_idx), this);
                            coord_label->move(x + 5, y + 310 - 40);
                            coord_label->setFixedWidth(30);
                            m_coord_label.push_back(coord_label);
                        }
                    }

                    painter.drawLine(x, y + 310 - 30, x, y + 310 + 30);
                }
                for (const double& x : m_note_manager->getOneFourthXVector()) {
                    painter.drawLine(x, y + 310 - 15, x, y + 310 + 15);
                }
            }
            for (auto coord_label : m_coord_label) {  // コードの表示
                coord_label->show();
            }
        }

        {  // 音符の描画
            std::vector<std::vector<std::shared_ptr<NoteManager::Note>>> note = m_note_manager->getNote();
            m_sound_label.clear();
            for (unsigned int i = 0; i < m_note_manager->getNote().size(); i++) {
                for (unsigned int j = 0; j < m_note_manager->getNote().at(i).size(); j++) {

                    std::shared_ptr<NoteManager::Note> note = m_note_manager->getNote().at(i).at(j);

                    // 色の設定
                    painter.setPen(QPen(m_color.green, 4));
                    for (auto selected_note : m_event_manager->getSelectedNote()) {
                        if (note == selected_note) {
                            painter.setPen(QPen(m_color.blue, 4));
                        }
                    }
                    if (note == m_event_manager->getOnNote()) {
                        painter.setPen(QPen(m_color.skyblue, 4));
                    }

                    if (note->end_x < 0) {
                        continue;
                    }

                    double nx1 = BasicParams::min_score_x - m_note_manager->getNowX() + note->start_x;
                    double nx2 = BasicParams::min_score_x - m_note_manager->getNowX() + note->end_x;
                    if (nx1 < BasicParams::max_score_x and nx2 > BasicParams::min_score_x) {
                        if (nx2 < BasicParams::max_score_x and nx1 > BasicParams::min_score_x) {
                            painter.drawRect(nx1, y_pos.at(i) + BasicParams::frame_y_center - 10, nx2 - nx1, 20);
                            std::shared_ptr<QLabel> sound_label = std::make_shared<QLabel>(m_note_manager->getNoteSound(note->sound), this);
                            sound_label->move(nx1, y_pos.at(i) + BasicParams::frame_y_center - 16);
                            sound_label->setFixedWidth(30);
                            m_sound_label.push_back(sound_label);
                            //sound_label.setMouseTracking(True)
                        }
                    } else {
                    }
                }
            }
            for (auto sound_label : m_sound_label) {
                sound_label->show();
            }
        }

        {  // ピアノの描画
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

    auto diff_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_cnt).count();
    auto wait_time = static_cast<int>(BasicParams::time_step - diff_time);
    //std::cout << wait_time << " " << diff_time << " " << wait_time + diff_time << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(wait_time * 1000));

    m_cnt = std::chrono::system_clock::now();
}

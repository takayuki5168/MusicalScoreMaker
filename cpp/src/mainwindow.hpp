#pragma once

#include <memory>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <QMainWindow>
#include <QTimer>
#include <QEvent>
//#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include <QTextCodec>
#include "event_manager.hpp"
#include "note_manager.hpp"
#include "mode.hpp"

#define ButtonNum 6

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow& instance()
    {
        static MainWindow inst;
        return inst;
    }

public slots:

private slots:
    void po() { std::cout << "toggled" << std::endl; }

    // TODO
    // setChecked(true)をするとtoggle Slotが呼ばれて変に処理される
    void setModeInput()
    {
        if (Mode::mode == Mode::Mode::Input) {
            //m_mode = Mode::Pause;
            std::cout << "when input" << std::endl;
            //m_button.at(3)->setChecked(true);
            //m_button.at(4)->setChecked(false);
            //m_button.at(5)->setChecked(false);
        } else {
            Mode::mode = Mode::Mode::Input;
            std::cout << "when not input" << std::endl;
            //m_button.at(3)->setChecked(false);
            ////m_button.at(4)->setChecked(true);
            //m_button.at(5)->setChecked(false);
        }
    }

    void setModePlay()
    {
        if (Mode::mode == Mode::Mode::Play) {
            //m_mode = Mode::Pause;
            //m_button.at(3)->setChecked(true);
            //m_button.at(4)->setChecked(false);
            //m_button.at(5)->setChecked(false);
        } else {
            Mode::mode = Mode::Mode::Play;
            //m_button.at(3)->setChecked(false);
            //m_button.at(4)->setChecked(false);
            //m_button.at(5)->setChecked(true);
        }
    }

private:
    MainWindow(QWidget* parent = 0);
    ~MainWindow() override
    {
        delete m_timer;
        for (QPushButton* x : m_button) {
            delete x;
        }
    }

    void mouseMoveEvent(QMouseEvent* mouse_event) override { m_event_manager->mouseMove(mouse_event); }
    void mousePressEvent(QMouseEvent* mouse_event) override { m_event_manager->mousePress(mouse_event); }
    void keyPressEvent(QKeyEvent* key_event) override { m_event_manager->keyPress(key_event); }
    void keyReleaseEvent(QKeyEvent* key_event) override { m_event_manager->keyRelease(key_event); }
    void paintEvent(QPaintEvent* paint_event) override;

    void readNote()
    {
        m_note_manager->clearNote();
        std::cout << "po" << std::endl;
        m_coord.clear();
        std::ifstream note_log;
        note_log.open("note.log");

        std::string str;
        int all_cnt = 0;
        while (getline(note_log, str)) {
            int cnt = 0;
            int start_x, end_x, sound;
            std::stringstream ss(str);
            std::string item;
            if (all_cnt != 0) {
                m_note_manager->addOctet();
            }
            while (getline(ss, item, ' ') and !item.empty()) {
                if (all_cnt == 0) {
                    QTextCodec* tc = QTextCodec::codecForLocale();
                    m_coord.push_back(QString(tc->toUnicode(item.c_str())));
                } else {
                    if (cnt % 3 == 0) {
                        start_x = std::stod(item);
                    } else if (cnt % 3 == 1) {
                        end_x = std::stod(item);
                    } else {
                        sound = std::stoi(item);
                        std::shared_ptr<NoteManager::Note> note = std::make_shared<NoteManager::Note>(start_x, end_x, sound);
                        m_note_manager->addNote(all_cnt - 1, note);
                    }
                    cnt++;
                }
            }
            all_cnt++;
        }
        BasicParams::octet_num = all_cnt - 1;
    }

    void readCoord()
    {
        m_coord.clear();
        std::ifstream coord_log;
        coord_log.open("coord.log");

        std::string str;
        while (getline(coord_log, str)) {
            std::stringstream ss(str);
            std::string item;
            for (unsigned int i = 0; i < str.size(); i++) {
                while (getline(ss, item, ' ') and !item.empty()) {
                    QTextCodec* tc = QTextCodec::codecForLocale();
                    m_coord.push_back(QString(tc->toUnicode(item.c_str())));
                }
            }
        }
    }


    int rangeInt(double num)
    {
        int num_int = static_cast<int>(num);
        if (num - num_int > 0.5) {
            return num_int + 1;
        } else if (num_int - num > 0.5) {
            return num_int - 1;
        } else {
            return num_int;
        }
    }

    struct Color {
        QColor origin = QColor(240, 240, 240);
        QColor black = QColor(0, 0, 0);
        QColor white = QColor(255, 255, 255);
        QColor red = QColor(255, 0, 0);
        QColor green = QColor(0, 255, 0);
        QColor blue = QColor(0, 0, 255);
        QColor yellow = QColor(255, 255, 0);
        QColor purple = QColor(255, 0, 255);
        QColor skyblue = QColor(0, 200, 255);
    };

    struct ButtonParams {
        std::array<QString, ButtonNum> name = std::array<QString, ButtonNum>{"Open", "New", "Save", "Pause", "Input", "Play"};
        std::array<QPoint, ButtonNum> pos = std::array<QPoint, ButtonNum>{QPoint{10, 10}, QPoint{90, 10}, QPoint{170, 10},
            QPoint{260, 10}, QPoint{260, 40}, QPoint{260, 70}};
        std::array<QSize, ButtonNum> size = std::array<QSize, ButtonNum>{QSize{80, 90}, QSize{80, 90}, QSize{80, 90},
            QSize{50, 30}, QSize{50, 30}, QSize{50, 30}};
        std::array<bool, ButtonNum> toggle_or_not = std::array<bool, ButtonNum>{false, false, false, true, true, true};
    };

    QTimer* m_timer = nullptr;
    std::chrono::system_clock::time_point m_cnt;

    ButtonParams m_button_params = ButtonParams();
    std::array<QPushButton*, ButtonNum> m_button;

    Color m_color;

    std::unique_ptr<EventManager> m_event_manager = nullptr;
    std::shared_ptr<NoteManager> m_note_manager = nullptr;
    std::vector<std::shared_ptr<QLabel>> m_sound_label;
    std::vector<std::shared_ptr<QLabel>> m_coord_label;
    std::vector<QString> m_coord;
};

inline MainWindow& mainWindow() { return MainWindow::instance(); }

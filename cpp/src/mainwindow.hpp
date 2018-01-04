#pragma once

#include <memory>
#include <chrono>
#include <QMainWindow>
#include <QTimer>
#include <QEvent>
//#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
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
    void paintEvent(QPaintEvent* paint_event) override;

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
};

inline MainWindow& mainWindow() { return MainWindow::instance(); }

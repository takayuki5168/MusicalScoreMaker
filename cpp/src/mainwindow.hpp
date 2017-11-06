#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QEvent>
#include "event.hpp"
#include "note.hpp"

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
    void update();


private:
    struct Params {
        enum mode {
            pause = 0,
            input = 1,
            play = 2,
        };
        int octec_num = 1;         //!< 何重奏か
        double tempo = 120;        //!< テンポ
        double input_speed = 1.0;  //!< 何倍速で入力するか
        double beat_deno = 4;      //!< 拍子の分母
        double beat_nume = 4;      //!< 拍子の分子
    };

    MainWindow(QWidget* parent = 0);

  void mouseMoveEvent(QMouseEvent* mouse_event) override{ m_event->mouseMove(mouse_event); }
    void mousePressEvent(QMouseEvent* mouse_event) override{ m_event->mousePress(mouse_event); }
    void keyPressEvent(QKeyEvent* key_event) override{ m_event->keyPress(key_event); }
    void paintEvent(QPaintEvent* paint_event) override;

    QTimer* m_timer = nullptr;
    double m_time_step = 10;

    Params m_params;

    Event* m_event;
    Note* m_note;
};

inline MainWindow& mainWindow() { return MainWindow::instance(); }

#pragma once

#include <memory>
#include <fstream>
#include <QMainWindow>
#include <QEvent>
#include <QMouseEvent>
#include "../src/basic_params.hpp"
#include "../src/note_manager.hpp"

/*!
 * @brief   入力イベントを扱うクラス
 */
class EventManager
{
public:
    explicit EventManager(std::shared_ptr<NoteManager> note_manager)
        : m_note_manager(note_manager) {}

    void mouseMove(const QMouseEvent* mouse_event)
    {
        mouse_pos = mouse_event->pos();

        // カーソル上の音符を更新
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

        m_on_note = nullptr;
        for (unsigned int i = 0; i < m_note_manager->getNote().size(); i++) {
            for (unsigned int j = 0; j < m_note_manager->getNote().at(i).size(); j++) {
                std::shared_ptr<NoteManager::Note> note = m_note_manager->getNote().at(i).at(j);
                if (BasicParams::frame_y_center + y_pos.at(i) + BasicParams::note_height / 2 > mouse_pos.y()
                    and BasicParams::frame_y_center + y_pos.at(i) - BasicParams::note_height / 2 < mouse_pos.y()) {
                    double nx1 = BasicParams::min_score_x - m_note_manager->getNowX() + note->start_x;
                    double nx2 = BasicParams::min_score_x - m_note_manager->getNowX() + note->end_x;
                    if (nx2 > mouse_pos.x() and nx1 < mouse_pos.x()) {
                        m_on_note = note;
                        break;
                    }
                }
            }
        }
    }

    void mousePress(const QMouseEvent* mouse_event)
    {
        mouse_pos = mouse_event->pos();

        if (mouse_pos.y() > 550 and mouse_pos.y() < 700) {  //白鍵盤
            for (int i = 0; i < 21; i++) {
                double s_x = (900 - 60) / 21.0 * i + 60;
                double e_x = (900 - 60) / 21.0 * (i + 1) + 60;
                if (mouse_pos.x() > s_x and mouse_pos.x() < e_x) {
                    //w.play.playPiano(i);
                    for (unsigned int j = 0; j < m_selected_note.size(); j++) {
                        m_selected_note.at(j)->setSound(i);
                    }
                    break;
                }
            }
        }
        if (mouse_pos.y() > 550 and mouse_pos.y() < 650) {  // 黒鍵盤
            for (int i = 0; i < 21 - 1; i++) {
                if (i % 7 == 2 or i % 7 == 6) {
                    continue;
                }
                double s_x = (900 - 60) / 21.0 * (i + 0.8) + 60;
                double e_x = (900 - 60) / 21.0 * (i + 0.4 + 0.8) + 60;
                if (mouse_pos.x() > s_x and mouse_pos.x() < e_x) {
                    // w.play.playPiano(21 + i);
                    for (unsigned int j = 0; j < m_selected_note.size(); j++) {
                        m_selected_note.at(j)->setSound(21 + i);
                    }
                    break;
                }
            }
        }
        if (mouse_pos.y() < 550) {       // 押された点がピアノではない時
            if (m_on_note == nullptr) {  // カーソル上の音符が無いとき
                m_selected_is_note = false;
                m_selected_note.clear();
            } else {  // カーソル上の音符があるとき
                m_selected_is_note = true;

                bool is_already_selected = false;
                for (auto selected_note : m_selected_note) {
                    if (selected_note == m_on_note) {
                        is_already_selected = true;
                    }
                }
                if (is_already_selected) {  // 既に選択されていたら選択解除
                    for (unsigned int i = 0; i < m_selected_note.size(); i++) {
                        if (m_selected_note.at(i) == m_on_note) {
                            m_selected_note.erase(m_selected_note.begin() + i);
                            break;
                        }
                    }
                } else {  // まだ選択されていなかったら選択(他をすべて消すかはシフトを押しているか)
                    if (not m_shift_is_pressed) {
                        m_selected_note.clear();
                    }
                    m_selected_note.push_back(m_on_note);
                }
            }
        }
    }

    void keyPress(const QKeyEvent* key_event)
    {
        // Controlが押されているか判定
        m_control_is_pressed = false;
        if (key_event->modifiers().testFlag(Qt::ControlModifier)) {
            m_control_is_pressed = true;
        }
        // Shiftが押されているか判定
        m_shift_is_pressed = false;
        if (key_event->modifiers().testFlag(Qt::ShiftModifier)) {
            m_shift_is_pressed = true;
        }

        // エンターで音符生成
        if (key_event->key() == Qt::Key_Return) {
            if (m_note_manager->getNowX() > 0) {
                if (m_note_manager->getNote().at(m_selected_octet).size() > 0
                    and m_note_manager->getNote().at(m_selected_octet).back()->end_x < 0) {
                    m_note_manager->getNote().at(m_selected_octet).back()->end_x = m_note_manager->getNowX();
                }
                m_note_manager->addNote(m_selected_octet, std::make_shared<NoteManager::Note>(m_note_manager->getNowX(), -1, 0));
            }
        }

        // Zで再生か停止か
        if (key_event->key() == Qt::Key_Z) {
            m_selected_is_note = false;
            m_selected_note.clear();
            Mode::start_or_not = not Mode::start_or_not;
            if (not Mode::start_or_not) {  // 停止になった瞬間にend_xが負である音符を削除
                auto note = m_note_manager->getNote();
                for (unsigned int i = 0; i < note.size(); i++) {
                    for (unsigned int j = 0; j < note.at(i).size(); j++) {
                        if (note.at(i).at(j)->end_x < 0) {
                            m_note_manager->eraseNote(i, j);
                        }
                    }
                }
            }
        }

        // WASDで選択する音符または譜面の移動
        if (key_event->key() == Qt::Key_W) {  // 上
            std::cout << "up" << std::endl;
        } else if (key_event->key() == Qt::Key_S) {  // 下
        } else if (key_event->key() == Qt::Key_D) {  // 右
            if (Mode::mode == Mode::Mode::Input and Mode::start_or_not == false) {
                if (m_selected_is_note) {  // 音符の移動
                    if (m_selected_note.size() == 1) {
                        std::vector<std::vector<std::shared_ptr<NoteManager::Note>>> note = m_note_manager->getNote();
                        int selected_octet = -1;
                        for (unsigned int i = 0; i < note.size(); i++) {  // selected_octetの探索
                            for (unsigned int j = 0; j < note.at(i).size(); j++) {
                                if (note.at(i).at(j) == m_selected_note.at(0)) {
                                    selected_octet = i;
                                    break;
                                }
                            }
                            if (selected_octet >= 0) {
                                break;
                            }
                        }
                        for (unsigned int i = 0; i < note.at(selected_octet).size(); i++) {
                            if (m_selected_note.at(0) == note.at(selected_octet).at(i)) {
                                if (i != note.at(selected_octet).size() - 1) {
                                    m_selected_note.at(0) = note.at(selected_octet).at(i + 1);
                                    break;
                                }
                            }
                        }
                    }
                } else {  // 譜面の移動
                    m_note_manager->updateNowX(10);
                }
            }
        } else if (key_event->key() == Qt::Key_A) {  // 左
            if (Mode::mode == Mode::Mode::Input and Mode::start_or_not == false) {
                if (m_selected_is_note) {  // 音符の移動
                    if (m_selected_note.size() == 1) {
                        std::vector<std::vector<std::shared_ptr<NoteManager::Note>>> note = m_note_manager->getNote();
                        int selected_octet = -1;
                        for (unsigned int i = 0; i < note.size(); i++) {  // selected_octetの探索
                            for (unsigned int j = 0; j < note.at(i).size(); j++) {
                                if (note.at(i).at(j) == m_selected_note.at(0)) {
                                    selected_octet = i;
                                    break;
                                }
                            }
                            if (selected_octet >= 0) {
                                break;
                            }
                        }
                        for (unsigned int i = 0; i < note.at(selected_octet).size(); i++) {
                            if (m_selected_note.at(0) == note.at(selected_octet).at(i)) {
                                if (i != 0) {
                                    m_selected_note.at(0) = note.at(selected_octet).at(i - 1);
                                    break;
                                }
                            }
                        }
                    }
                } else {  // 譜面の移動
                    m_note_manager->updateNowX(-10);
                }
            }
        }

        if (m_control_is_pressed) {               // コントロールが押されている
            if (key_event->key() == Qt::Key_S) {  // 保存
                saveNoteLog();
            }
        }

        if (key_event->key() == Qt::Key_Delete) {
            std::vector<std::vector<std::shared_ptr<NoteManager::Note>>> note = m_note_manager->getNote();
            for (auto selected_note : m_selected_note) {
                for (unsigned int i = 0; i < note.size(); i++) {
                    for (unsigned int j = 0; j < note.at(i).size(); j++) {
                        if (note.at(i).at(j) == selected_note) {
                            m_note_manager->eraseNote(i, j);
                        }
                    }
                }
            }
            m_selected_note.clear();
        }
    }

    void
    keyRelease(const QKeyEvent* key_event)
    {
        // Controlの判定
        if (key_event->key() == Qt::Key_Control) {
            m_control_is_pressed = false;
        }
        // Shiftの判定
        if (key_event->key() == Qt::Key_Shift) {
            m_shift_is_pressed = false;
        }
    }

    // 音符のログを保存する
    void saveNoteLog()
    {
        std::ofstream note_log;
        note_log.open("note.log", std::fstream::out | std::fstream::trunc);
        auto note = m_note_manager->getNote();
        auto coord = m_note_manager->getCoord();
        for (auto c : coord) {
            note_log << c.toStdString() << " ";
        }
        note_log << std::endl;
        for (int i = 0; i < BasicParams::octet_num; i++) {
            for (auto n : note.at(i)) {
                note_log << n->start_x << " " << n->end_x << " " << n->sound << " ";
            }
            note_log << std::endl;
        }
    }

    // getter関数
    std::vector<std::shared_ptr<NoteManager::Note>> getSelectedNote() { return m_selected_note; }
    std::shared_ptr<NoteManager::Note> getOnNote() { return m_on_note; }

private:
    std::vector<std::shared_ptr<NoteManager::Note>> m_selected_note;  //!< 選択中の音符
    std::shared_ptr<NoteManager::Note> m_on_note;                     //!< カーソル上の音符

    bool m_selected_is_note = false;  //!< 選択中の音符
    int m_selected_octet = 0;         //!< 入力時に選択している奏(選択中の音符の奏ではないのに注意)

    bool m_control_is_pressed = false;
    bool m_shift_is_pressed = false;

    QPoint mouse_pos;
    std::shared_ptr<NoteManager> m_note_manager = nullptr;  //!< MainWindowのm_note_managerをshared_ptrとして持つ
};

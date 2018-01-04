#pragma once

#include <iostream>
#include <string>
#include <QPainter>
#include "mode.hpp"
#include "basic_params.hpp"

/*
 * 譜面を管理するクラス
 */
class NoteManager
{
public:
    struct Note {
        Note(double start_x, double end_x, int sound)
            : start_x(start_x), end_x(end_x), sound(sound) {}
        void setSound(int sound_) { sound = sound_; }
        double start_x;
        double end_x;
        int sound;
    };

    explicit NoteManager()
    {
        m_note.resize(BasicParams::octet_num);
        m_note_sound = {
            "C1", "D1", "E1", "F1", "G1", "A1", "B1", "C2", "D2", "E2", "F2", "G2", "A2", "B2", "C3", "D3", "E3", "F3", "G3", "A3", "B3",
            "C1s", "D1s", "E1s", "F1s", "G1s", "A1s", "B1s", "C2s", "D2s", "E2s", "F2s", "G2s", "A2s", "B2s", "C3s", "D3s", "E3s", "F3s", "G3s", "A3s", "B3s",
            "C1f", "D1f", "E1f", "F1f", "G1f", "A1f", "B1f", "C2f", "D2f", "E2f", "F2f", "G2f", "A2f", "B2f", "C3f", "D3f", "E3f", "F3f", "G3f", "A3f", "B3f"};
    }

    void update();

    // m_now_xの更新, getter関数
    void updateNowX(double x) { m_now_x += x; }
    double getNowX() { return m_now_x; }

    // 音符の追加, getter, setter関数
    void addNote(int octet, std::shared_ptr<Note> note) { m_note.at(octet).push_back(std::move(note)); }
    std::vector<std::vector<std::shared_ptr<Note>>> getNote() { return m_note; }
    void setSound(int octet, int idx, int sound) { m_note.at(octet).at(idx)->sound = sound; }

    // getter関数
    std::vector<double> getOneXVector() { return m_one_x_vector; }
    std::vector<double> getOneFourthXVector() { return m_one_fourth_x_vector; }
    QString getNoteSound(int note) { return m_note_sound.at(note); }

private:
    // 譜面に関するパラメータ
    struct Params {
        Params()
        {
            forward_x_step = BasicParams::tempo / 60.0 * one_fourth_x * BasicParams::time_step / 1000.0 * BasicParams::input_speed;
        }
        double one_x = 200.0;               //!< 一部音符の長さ
        double one_fourth_x = one_x / 4.0;  //!< 四分音符の長さ
        double one_eighth_x = one_x / 8.0;  //!< 八分音符の長さ
        double forward_x_step;
    };

    Params m_params;

    double m_pre_x = 0;
    double m_now_x = 0;  //!< 譜面のx座標

    std::vector<double> m_one_x_vector;
    std::vector<double> m_one_fourth_x_vector;
    std::vector<std::vector<std::shared_ptr<Note>>> m_note;  //!< 音符

    std::vector<QString> m_note_sound;
};

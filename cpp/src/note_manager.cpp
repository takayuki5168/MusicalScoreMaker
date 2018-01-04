#include "mainwindow.hpp"
#include "note_manager.hpp"
#include "math_util.hpp"

void NoteManager::update()
{
    // スクロール座標の更新
    //if (Mode::mode == Mode::Mode::Input or Mode::mode == Mode::Mode::Play) {
    if (Mode::start_or_not) {
        m_params.forward_x_step = BasicParams::tempo / 60.0 * m_params.one_fourth_x * BasicParams::time_step / 1000 * BasicParams::input_speed;
        m_pre_x = m_now_x;
        m_now_x += m_params.forward_x_step;
    }

    // メトロノーム
    {
        if (Mode::mode == Mode::Mode::Input) {  //!< 入力
            if (calcRemain(m_pre_x, m_params.one_x) > calcRemain(m_now_x, m_params.one_x)) {
                // メトロノーム
            } else if (calcRemain(m_pre_x, m_params.one_fourth_x) > calcRemain(m_now_x, m_params.one_fourth_x)) {
                // メトロノーム
            }
        } else if (Mode::mode == Mode::Mode::Play) {  //!< 再生
        }
    }

    {  // 縦棒の位置の更新
        m_one_x_vector.clear();
        double tmp_one_x = calcRemain(50 - m_now_x, m_params.one_x);
        while (true) {
            if (tmp_one_x > BasicParams::min_score_x) {
                m_one_x_vector.push_back(tmp_one_x);
            }
            if (tmp_one_x > BasicParams::max_score_x) {
                break;
            }
            tmp_one_x += m_params.one_x;
        }

        m_one_fourth_x_vector.clear();
        double tmp_one_fourth_x = calcRemain(50 - m_now_x, m_params.one_fourth_x);
        while (true) {
            if (tmp_one_fourth_x > BasicParams::min_score_x) {
                m_one_fourth_x_vector.push_back(tmp_one_fourth_x);
            }
            if (tmp_one_fourth_x > BasicParams::max_score_x) {
                break;
            }
            tmp_one_fourth_x += m_params.one_fourth_x;
        }
    }
}

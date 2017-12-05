#include "mainwindow.hpp"
#include "note.hpp"
#include "math_util.hpp"

void Note::update(Mode mode)
{
    // スクロール座標の更新
    if (mode == Mode::Input or mode == Mode::Play) {
        m_pre_x = m_now_x;
        m_now_x += m_params.forward_x_step;
    }

    if (mode == Mode::Pause) {         //!< 停止
    } else if (mode == Mode::Input) {  //!< 入力
        if (calcRemain(m_pre_x, m_params.one_x) > calcRemain(m_now_x, m_params.one_x)) {
            // メトロノーム
        } else if (calcRemain(m_pre_x, m_params.one_fourth_x) > calcRemain(m_now_x, m_params.one_fourth_x)) {
            // メトロノーム
        }
    } else if (mode == Mode::Play) {//!< 再生
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

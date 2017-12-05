#pragma once

#include <iostream>
#include <QPainter>
#include "mode.hpp"
#include "basic_params.hpp"

class Note
{
public:
    explicit Note() {}
    void update(Mode mode);
    void paint();

    std::vector<double> getOneXVector() { return m_one_x_vector; }
    std::vector<double> getOneFourthXVector() { return m_one_fourth_x_vector; }

private:
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
    double m_max_score_x = 900;
    double m_min_score_x = 50;

    double m_pre_x = 0;
    double m_now_x = 0;

    std::vector<double> m_one_x_vector;
    std::vector<double> m_one_fourth_x_vector;
    std::vector<std::vector<double>> m_note;
};

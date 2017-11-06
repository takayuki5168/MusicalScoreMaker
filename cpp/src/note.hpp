#pragma once
#include <QPainter>

class Note
{
public:
    explicit Note() {}

    void update();

    void paint(const QPainter& painter) { ; }
private:
    struct Params {
        double one_x = 200.0;               //!< 一部音符の長さ
        double one_fourth_x = one_x / 4.0;  //!< 四分音符の長さ
        double one_eighth_x = one_x / 8.0;  //!< 八分音符の長さ
        double forward_x_step;
    };

    Params m_params;
    double m_max_score_x = 900;
    double m_min_score_x = 50;
};

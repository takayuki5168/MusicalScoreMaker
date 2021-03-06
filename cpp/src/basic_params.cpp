#include "basic_params.hpp"

namespace BasicParams
{
int octet_num = 1;         //!< 何重奏か
double tempo = 120;        //!< テンポ
double input_speed = 1.0;  //!< 何倍速で入力するか
double beat_deno = 4;      //!< 拍子の分母
double beat_nume = 4;      //!< 拍子の分子

double time_step = 5;

double max_score_x = 900;
double min_score_x = 50;
double frame_y_center = 310;
double frame_y_length = 400;

double note_height = 20;
}

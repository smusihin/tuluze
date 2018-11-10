#include "tuluze.h"
#include <QPainter>
#include <set>

const uint32_t column_count = 60;
const uint32_t row_count = 10;
const double width_coef = 27.1/25.0;
const double height_coef = 6.9/8.8;
const double left_coef = 1.1/25.0;
const double top_coef = 0.9/8.8;


const std::vector<std::set<uint32_t>> vert =
{
    {1,3,8,13,17,19,24,28,31,35,39,46,48,53},
    {2,3,5,7,12,14,20,22,26,29,34,37,23,47,52,55,59},
    {1,7,10,16,20,23,27,28,32,37,44,51,54,58,60},
    {2,4,10,16,21,23,28,32,38,44,50,55,56,59},
    {3,7,12,14,15,22,24,26,33,35,42,46,53,58},
    {2,3,4,10,17,19,26,28,32,38,39,41,44,44,49,53,59},
    {1,3,8,11,15,21,23,30,33,39,41,46,50,52,56,60},
    {1,6,7,14,17,20,24,29,32,36,40,42,44,48,51,55,55,58},
    {2,3,8,12,15,19,22,24,26,27,29,33,25,37,45,49,53,56,60},
    {1,2,5,7,13,14,15,21,25,28,33,40,42,46,49,58}
};



tuluze::tuluze()
{

}

void tuluze::load_file(const QString &file)
{
    QImage srcImg(file);
    QPoint center = srcImg.rect().center();
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(-90);
    convertFromImage(srcImg.transformed(matrix));
   // load(file);
    reset();
}

void tuluze::rotate()
{
}

QPixmap tuluze::crop_image(int left, int top, int width, int height)
{
    recalc(left,top,width,height);
    return copy(left_,top_,width_,height_);
}

QPixmap tuluze::split_image()
{
    auto result = copy(left_,top_,width_,height_);
    QPainter painter(&result);
    for(uint32_t i=1;i< column_count; i++)
    {
       painter.drawLine(int(i* w_step_), 0, int(i*w_step_), height_);
    }
    for(uint32_t i=1;i< row_count; i++)
    {
       painter.drawLine(0,int(i*h_step_),width_, int(i*h_step_));
    }
    return result;
}

QPixmap tuluze::get_unit(uint32_t row,uint32_t column)
{
    auto pix = copy(left_+ int(w_step_*column),top_+int(h_step_ *row), int(w_step_),int(h_step_));
    if(vert[row].find(column+1) != vert[row].end())
    {
        QPainter painter(&pix);
        QPen pen;
        pen.setWidth(3);
        pen.setColor(Qt::yellow);
        painter.setPen(pen);
        painter.drawRect(1,1, int(w_step_-3),int(h_step_-3));
    }
    return pix;
}

QPixmap tuluze::select_unit(uint32_t row, uint32_t column)
{
    auto result = copy(left_,top_,width_,height_);
    QPainter painter(&result);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::green);
    painter.setPen(pen);
    painter.drawRect(int(w_step_*column),int(h_step_ *row), int(w_step_),int(h_step_));
    return result;
}

void tuluze::set_state(uint32_t row,uint32_t column, tuluze::State state)
{
    states_[row * column_count +column] = state;
}

std::pair<uint32_t, uint32_t> tuluze::row_results(uint32_t row)
{
    uint32_t ok_count = 0;
    uint32_t total_count = 0;
    uint32_t start = row * column_count;
    for(auto i = start; i < start + column_count; i++)
    {
        auto state = states_[i];
        if(state == Not)
        {
            continue;
        }
        else{
            if(state == Ok)
            {
                ok_count++;
            }
            total_count = i - start + 1;
        }
    }
    return std::pair<uint32_t, uint32_t>(total_count, total_count - ok_count);
}

std::pair<uint32_t, uint32_t> tuluze::results()
{
    std::pair<uint32_t, uint32_t> result(0,0);
    for (uint32_t i = 0; i< row_count;i++)
    {
        auto rr = row_results(i);
        result.first += rr.first;
        result.second += rr.second;
    }
    return result;
}

void tuluze::reset()
{
    recalc(0,0,0,0);
    states_ = std::vector<State>(column_count *row_count,State::Not);
}

uint32_t tuluze::rows()
{
    return row_count;
}

uint32_t tuluze::columns()
{
    return column_count;
}

void tuluze::recalc(int left, int top, int w, int h)
{
    left_ = left - int(w * left_coef);
    top_= top + int(h * top_coef);
    if(w)
    {
        width_ = int(w * width_coef);
    }
    else
    {
        width_ = width();
    }
    if(h != 0)
    {
        height_ = int(h * height_coef);
    }
    else
    {
        height_ = height();
    }
    w_step_ = float(width_)/column_count;
    h_step_ = float(height_)/row_count;
}

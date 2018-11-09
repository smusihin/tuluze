#include "tuluze.h"
#include <QPainter>

const uint32_t column_count = 60;
const uint32_t row_count = 10;

tuluze::tuluze()
{

}

void tuluze::load_file(const QString &file)
{
    load(file);
    reset();
}

QPixmap tuluze::crop_image(int left, int top, int width, int height)
{
    recalc(left,top,width,height);
    return copy(left,top,width,height);
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
    return copy(left_+ int(w_step_*column),top_+int(h_step_ *row), int(w_step_),int(h_step_));
}

QPixmap tuluze::select_unit(uint32_t row, uint32_t column)
{
    auto result = copy(left_,top_,width_,height_);
    QPainter painter(&result);
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::green);
    painter.setPen(pen);
    painter.drawRect(left_+ int(w_step_*column),top_+int(h_step_ *row), int(w_step_),int(h_step_));
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
    recalc(0,0,width(),height());
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

void tuluze::recalc(int left, int top, int width, int height)
{
    left_ = left;
    top_= top;
    width_ = width;
    height_ = height;
    w_step_ = float(width_)/column_count;
    h_step_ = float(height_)/row_count;
}

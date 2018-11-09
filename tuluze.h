#ifndef TULUZE_H
#define TULUZE_H
#include <QPixmap>


class tuluze : public QPixmap
{
    enum State
    {
        Not,
        Ok,
        Error
    };
public:
    tuluze();
    void load_file(const QString & file);
    QPixmap crop_image(int left, int top, int width, int height);
    QPixmap split_image();
    QPixmap get_unit(uint32_t row,uint32_t column);
    void set_state(uint32_t row,uint32_t column, State state);


private:
    int left_;
    int top_;
    int width_;
    int height_;
    float w_step_;
    float h_step_;
    int current_row_;
    int current_column_;
    std::vector<State> states_;

    void recalc(int left, int top, int width, int height);
};

#endif // TULUZE_H

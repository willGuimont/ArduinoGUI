#include "GUI.h"

// Button

Button::Button(int x, int y)
: Button(x, y, DEFAULT_BUTTON_COLOR)
{

}

Button::Button(int x, int y, long color)
{
    x_ = x;
    y_ = y;
    color_ = color;
    nextButton_ = NULL;
    callback_ = NULL;
    ornement_ = NULL;
}

Button::~Button()
{

}

bool Button::update(TSPoint p)
{
    bool value = false;
    bool nextValue = false;
    if (collision(p))
    {
        clicked();
        value = true;
    }
    if (nextButton_)
        nextValue = (nextButton_->update(p));
    return value || nextValue;
}

/* Abstract

void Button::draw(Adafruit_TFTLCD* tft)
{

}

*/

void Button::clicked()
{
    if (callback_)
        callback_->execute();
    if (ornement_)
        ornement_->setPosition(x_, y_);
}

void Button::addButton(Button* button)
{
    if (nextButton_)
        nextButton_->addButton(button);
    else
        nextButton_ = button;
}

Button* Button::setCallback(Callback* callback)
{
    if (callback)
        callback_ = callback;
    return this;
}

Button* Button::setOrnement(Ornement* ornement)
{
    if (ornement)
    {
        ornement_ = ornement;
        setOrnementVariables();
    }
    return this;
}

void Button::setOrnementVariables()
{
    if (ornement_)
        ornement_->setVariables(this);
}

// END Button

// RectButton

RectButton::RectButton(int x, int y, int w, int h)
:   RectButton(x, y, w, h, DEFAULT_BUTTON_COLOR)
{

}

RectButton::RectButton(int x, int y, int w, int h, long color)
: Button(x, y, color)
{
    w_ = w;
    h_ = h;
}

RectButton::~RectButton()
{

}

bool RectButton::collision(TSPoint p)
{
    return ((p.x >=x_ && p.x <= x_ + w_) && (p.y >= y_ && p.y <= y_ + h_));
}

void RectButton::draw(Adafruit_TFTLCD* tft)
{
    if (tft)
        tft->fillRoundRect(x_, y_ ,w_, h_, 1, color_);
    if (ornement_)
        ornement_->draw(tft);
    if (nextButton_)
        nextButton_->draw(tft);
}

void RectButton::setOrnementVariables()
{
    if (ornement_)
        ornement_->setVariables(this, RECT);
}

// END RectButton

// RoundButton

RoundButton::RoundButton(int x, int y, int r)
:   RoundButton(x, y, r, DEFAULT_BUTTON_COLOR)
{

}

RoundButton::RoundButton(int x, int y, int r, long color)
: Button(x, y, color)
{
    r_ = r;
}

RoundButton::~RoundButton()
{

}

bool RoundButton::collision(TSPoint p)
{
    return (sqrt(pow(x_ - p.x, 2) + pow(y_ - p.y, 2)) < r_);
}

void RoundButton::draw(Adafruit_TFTLCD* tft)
{
    if (tft)
        tft->fillCircle(x_, y_ , r_, color_);
    if (ornement_)
        ornement_->draw(tft);
    if (nextButton_)
        nextButton_->draw(tft);
}

void RoundButton::setOrnementVariables()
{
    if (ornement_)
        ornement_->setVariables(this, ROUND);
}

// END RoundButton

// Slider

Slider::Slider(int x, int y, int w, int h, int minVal, int maxVal)
: Slider(x, y, w, h, minVal, maxVal, DEFAULT_BUTTON_COLOR)
{

}

Slider::Slider(int x, int y, int w, int h, int minVal, int maxVal, long color)
: Slider(x, y, w, h, minVal, maxVal, color, DEFAULT_CURSOR_COLOR)
{

}

Slider::Slider(int x, int y, int w, int h, int minVal, int maxVal, long color, long cursorColor)
: RectButton(x, y, w, h, color)
{
    min_ = minVal;
    max_ = maxVal;
    val_ = minVal;
    delta = 0;
    cursorColor_ = cursorColor;
}

Slider::~Slider()
{

}

bool Slider::update(TSPoint p)
{
    updateValue(p);
    return RectButton::update(p);
}

void Slider::draw(Adafruit_TFTLCD* tft)
{
    if (tft)
    {
        RectButton::draw(tft);
        drawCursor(tft);
    }
    if (ornement_)
        ornement_->draw(tft);
    if (nextButton_)
        nextButton_->draw(tft);
}

int Slider::getVal() const
{
    return val_;
}

void Slider::setOrnementVariables()
{
    if (ornement_)
        ornement_->setVariables(this, SLIDER);
}

// END Slider

// HorizontalSlider

HorizontalSlider::HorizontalSlider(int x, int y, int w, int h, int minVal, int maxVal)
: HorizontalSlider(x, y, w, h, minVal, maxVal, DEFAULT_BUTTON_COLOR, DEFAULT_CURSOR_COLOR)
{
}

HorizontalSlider::HorizontalSlider(int x, int y, int w, int h, int minVal, int maxVal, long color)
: HorizontalSlider(x, y, w, h, minVal, maxVal, color, DEFAULT_CURSOR_COLOR)
{
}

HorizontalSlider::HorizontalSlider(int x, int y, int w, int h, int minVal, int maxVal, long color, long cursorColor)
: Slider(x, y, w, h, minVal, maxVal, color, cursorColor_)
{
}

HorizontalSlider::~HorizontalSlider()
{

}

void HorizontalSlider::drawCursor(Adafruit_TFTLCD* tft)
{
    /*
    if (tft)
        tft->fillCircle(x_ + delta, y_ + h_ / 2 , h_ / 2 + 2, cursorColor_);
    */
}

void HorizontalSlider::updateValue(TSPoint p)
{
    if (RectButton::collision(p))
    {
        delta = p.x - x_; //
        val_ = ((max_ - min_) * delta) / w_;
    }
}

// END HorizontalSlider

// VerticalSlider

VerticalSlider::VerticalSlider(int x, int y, int w, int h, int minVal, int maxVal)
: VerticalSlider(x, y, w, h, minVal, maxVal, DEFAULT_BUTTON_COLOR, DEFAULT_CURSOR_COLOR)
{
}

VerticalSlider::VerticalSlider(int x, int y, int w, int h, int minVal, int maxVal, long color)
: VerticalSlider(x, y, w, h, minVal, maxVal, color, DEFAULT_CURSOR_COLOR)
{
}

VerticalSlider::VerticalSlider(int x, int y, int w, int h, int minVal, int maxVal, long color, long cursorColor)
: Slider(x, y, w, h, minVal, maxVal, color, cursorColor_)
{
}

VerticalSlider::~VerticalSlider()
{

}

void VerticalSlider::drawCursor(Adafruit_TFTLCD* tft)
{
    if (tft)
        tft->fillCircle(x_ + w_ / 2, y_ + delta, w_ / 2, cursorColor_);
}

void VerticalSlider::updateValue(TSPoint p)
{
    if (RectButton::collision(p))
    {
        delta = p.y - y_;
        val_ = ((max_ - min_) * delta) / h_;
    }
}

// END VerticalSlider

// Ornement

void Ornement::setPosition(int x, int y)
{
    x_ = x;
    y_ = y;
}

void Ornement::setDimension(int w, int h)
{
    w_ = w;
    h_ = h;
}

void Ornement::setVariables(Button* parent, int type)
{
    if (!parent)
        return;
    type_ = type;
    x_ = parent->x_;
    y_ = parent->y_;
    switch (type_)
    {
    case RECT:
        {
        RectButton* tmp = (RectButton*)parent;
        w_ = tmp->w_;
        h_ = tmp->h_;
        break;
        }
    case ROUND:
        {
        RoundButton* tmp = (RoundButton*)parent;
        r_ = tmp->r_;
        break;
        }
    case SLIDER:
        {
        Slider* tmp = (Slider*)parent;
        w_ = tmp->w_;
        h_ = tmp->h_;
        break;
        }
    default:
        break;
    }
}

// END Ornement

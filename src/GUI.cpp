#include "GUI.h"

GUI::GUI(Adafruit_TFTLCD* tft)
{
    tft_ = tft;
	tft->setRotation(2);
    buttonHead_ = 0;
}

GUI::~GUI()
{
    //dtor
}

void GUI::addButton(Button* button)
{
    if (buttonHead_)
        buttonHead_->addButton(button);
    else
        buttonHead_ = button;
}

bool GUI::update(TSPoint p)
{
    bool value = false;
    if (buttonHead_)
        value = buttonHead_->update(p);
    return value;
}

void GUI::draw()
{
    if (buttonHead_ && tft_)
        buttonHead_->draw(tft_);
}

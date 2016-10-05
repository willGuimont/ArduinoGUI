#ifndef GUI_H
#define GUI_H

/*
    TFTLCD_GUI - Library to make GUI on TFT LCD screens.
    Created by William Guimont-Martin, 2015.
    Released into the public domain.
*/


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
/**/

#include <math.h>

// Colors

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define NAVY 0x000F
#define DARK_GREEN 0x03E0
#define DARK_CYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHT_GREY 0xC618
#define DARK_GREY 0x7BEF
#define ORANGE 0xFD20
#define GREEN_YELLOW 0xAFE5
#define PINK 0xF81F

// END Colors

#define DEFAULT_BUTTON_COLOR BLUE
#define DEFAULT_CURSOR_COLOR GREEN

/*
class Adafruit_TFTLCD;
class TSPoint;
*/

// Classes

enum Type {BUTTON, RECT, ROUND, SLIDER};

class Button;

class Ornement
{
    public:
        ~Ornement();
        virtual void draw(Adafruit_TFTLCD* tft) = 0;
        void setPosition(int x, int y);
        void setDimension(int w, int h);
        void setVariables(Button* parent, int type = 0);
        int getX()      {                                       return x_;}
        int getY()      {                                       return y_;}
        int getW()      {if (type_ == RECT || type_ == SLIDER)  return w_;  else    return 0;}
        int getH()      {if (type_ == RECT || type_ == SLIDER)  return h_;  else    return 0;}
        int getR()      {if (type_ == ROUND)                    return r_;  else    return 0;}
        int getType()   {return type_;}
    private:
        int x_;
        int y_;
        int w_;
        int h_;
        int r_;
        int type_;
};

class Callback
{
    public:
        ~Callback();
        virtual void execute() = 0;
};

class Button
{
    public:
        friend class Ornement;

        Button(int x, int y);
        Button(int x, int y, long color);
        ~Button();

        bool update(TSPoint p);
        virtual void draw(Adafruit_TFTLCD* tft) = 0;
        virtual bool collision(TSPoint p) = 0;
        virtual void clicked();

        void addButton(Button* button);
        Button* setCallback(Callback* callback);
        Button* setOrnement(Ornement* ornement);
        virtual void setOrnementVariables();
    protected:
        int x_;
        int y_;
        long color_;
        Button* nextButton_;
        Callback* callback_;
        Ornement* ornement_;
};

class RectButton : public Button
{
    public:
        friend class Ornement;

        RectButton(int x, int y, int w, int h);
        RectButton(int x, int y, int w, int h, long color);
        virtual ~RectButton();

        virtual void draw(Adafruit_TFTLCD* tft);
        virtual bool collision(TSPoint p);
        virtual void setOrnementVariables();
    protected:
        int w_;
        int h_;
};

class RoundButton : public Button
{
    public:
        friend class Ornement;

        RoundButton(int x, int y, int r);
        RoundButton(int x, int y, int r, long color);
        virtual ~RoundButton();

        virtual void draw(Adafruit_TFTLCD* tft);
        virtual bool collision(TSPoint p);
        virtual void setOrnementVariables();
    protected:
        int r_;
};

class GUI
{
    public:
        GUI(Adafruit_TFTLCD* tft);
        ~GUI();
        void addButton(Button* button);
        bool update(TSPoint p);
        void draw();
    private:
        Adafruit_TFTLCD* tft_;
        Button* buttonHead_;
};


#endif // GUI_H

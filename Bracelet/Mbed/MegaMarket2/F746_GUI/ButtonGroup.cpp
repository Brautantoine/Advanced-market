//-----------------------------------------------------------
//  ButtonGroup class
//
//  2017/01/25, Copyright (c) 2017 MIKAMI, Naoki
//-----------------------------------------------------------

#include "ButtonGroup.hpp"

namespace Mikami
{
    // Constructor
    ButtonGroup::ButtonGroup(
            uint16_t x0, uint16_t y0,
            uint16_t width, uint16_t height,
            uint16_t number, const string str[],
            uint16_t spaceX, uint16_t spaceY,
            uint16_t column, int touched,
            sFONT &fonts,
            uint32_t textColor, uint32_t backColor,
            uint32_t createdColor, uint32_t touchedColor,
            uint32_t inactiveColor, uint32_t inactiveTextColor)
        : TEXT_COLOR_(textColor), CREATED_COLOR_(createdColor),
          TOUCHED_COLOR_(touchedColor),
          NUMBER_(number), prevNum_(touched)
    {
        buttons_.SetSize(number);
        for (int n=0; n<number; n++)
        {
            div_t u1 = div(n, column);
            uint16_t x = x0 + u1.rem*(width + spaceX);
            uint16_t y = y0 + u1.quot*(height + spaceY);
            buttons_[n] =
                new Button(x, y, width, height, str[n], fonts,
                           textColor, backColor,
                           createdColor, touchedColor,
                           inactiveColor, inactiveTextColor);
        }
        // On created, set touched color as needed
        if (touched >= 0) TouchedColor(touched);
    }

    // Draw button
    bool ButtonGroup::Draw(int num, uint32_t color, uint32_t textColor)
    {
        if (!Range(num)) return false;
        buttons_[num]->Draw(color, textColor);
        return true;
    }

    // Change to touched color
    bool ButtonGroup::TouchedColor(int num)
    {
        if (prevNum_ != num) prevNum_ = num;
        return Draw(num, TOUCHED_COLOR_, TEXT_COLOR_);
    }

    // Erase button
    bool ButtonGroup::Erase(int num)
    {
        if (!Range(num)) return false;
        buttons_[num]->Erase();
        return true;
    }

    // Check touch detected for specified button
    bool ButtonGroup::Touched(int num)
    {
        if (!Range(num)) return false;
        if (!buttons_[num]->IsActive()) return false;
        int touched;
        if (!GetTouchedNumber(touched)) return false;
        bool rtn = (touched == num) ? true : false;
        return rtn;
    }

    // Get touched number, return value: true or false
    bool ButtonGroup::GetTouchedNumber(int &num)
    {
        bool rtn = false;
        if (GuiBase::PanelTouched())
        {
            for (int n=0; n<NUMBER_; n++)
                if (buttons_[n]->IsOnButton() &&
                    buttons_[n]->IsActive())
                {
                    num = n;
                    rtn = true;
                }
            
            if (!rtn) return false;
        }
        else
            return false;

        buttons_[num]->Draw(TOUCHED_COLOR_);
        if ((prevNum_ >= 0) && (prevNum_ != num))
            buttons_[prevNum_]->Draw();
        if (prevNum_ != num) prevNum_ = num;
        return true;
    }

    // Get touched number, return value: touched number
    int ButtonGroup::GetTouchedNumber()
    {
        int num;
        if (GetTouchedNumber(num))
            return num;
        else
            return -1;
    }

    // Wait until touched and get touched number
    int ButtonGroup::WaitTouchedAndGet()
    {
        int num;
        while (!GetTouchedNumber(num)) {}
        return num;
    }

    // Activate and inactivate button(s)
    bool ButtonGroup::Activate(int num)
    {
        if (!Range(num)) return false;
        buttons_[num]->Activate();
        return true;
    }
    bool ButtonGroup::Inactivate(int num)
    {
        if (!Range(num)) return false;
        buttons_[num]->Inactivate();
        return true;
    }
}


//-----------------------------------------------------------
//  GuiBase class (abstract base class) ---- Header
//      
//  2017/03/17, Copyright (c) 2017 MIKAMI, Naoki
//-----------------------------------------------------------

#ifndef F746_GUI_BASE_HPP
#define F746_GUI_BASE_HPP

#include "mbed.h"
#include <string>
#include "TS_DISCO_F746NG.h"
#include "LCD_DISCO_F746NG.h"

namespace Mikami
{
    class GuiBase
    {
    public:                    
        static LCD_DISCO_F746NG& GetLcd() { return lcd_; }
        static TS_DISCO_F746NG& GetTs() { return ts_; }

        // If panel touched, return true
        static bool PanelTouched();
        // Get touch panel state
        static TS_StateTypeDef GetTsState() { return state_; }

        enum { ENUM_TEXT = 0xFFFFFFFF, ENUM_BACK = 0xFF003538,
               ENUM_CREATED = 0xFF0068B7, ENUM_TOUCHED = 0xFF7F7FFF,
               ENUM_INACTIVE = 0xD0003538, ENUM_INACTIVE_TEXT = 0xFF808080};

    protected:
        static LCD_DISCO_F746NG lcd_;  // for LCD display
        static TS_DISCO_F746NG ts_;    // for touch pannel

        static TS_StateTypeDef state_;
        static bool multiTouch_;

        const uint16_t X_, Y_;
        sFONT *const FONTS_;

        const uint32_t TEXT_COLOR_;
        const uint32_t BACK_COLOR_;
        const uint32_t CREATED_COLOR_;
        const uint32_t TOUCHED_COLOR_;
        const uint32_t INACTIVE_COLOR_;
        const uint32_t INACTIVE_TEXT_COLOR_;

        // Constructor
        GuiBase(uint16_t x =0, uint16_t y =0,
                sFONT &fonts = Font12,
                uint32_t textColor         = ENUM_TEXT,
                uint32_t backColor         = ENUM_BACK,
                uint32_t createdColor      = ENUM_CREATED,
                uint32_t touchedColor      = ENUM_TOUCHED,
                uint32_t inactiveColor     = ENUM_INACTIVE,
                uint32_t inactiveTextColor = ENUM_INACTIVE_TEXT);

        void DrawString(uint16_t x, uint16_t y, const string str)
        { lcd_.DisplayStringAt(x, y, (uint8_t *)str.c_str(), LEFT_MODE); }

    private:
        static bool first_;
        
        // disallow copy constructor and assignment operator
        GuiBase(const GuiBase&);
        GuiBase& operator=(const GuiBase&);
    };
}
#endif  // F746_GUI_BASE_HPP

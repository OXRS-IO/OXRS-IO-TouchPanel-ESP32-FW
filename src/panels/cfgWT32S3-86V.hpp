#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <globalDefines.h>

#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <driver/i2c.h>

// Portrait
#define TFT_WIDTH SCREEN_WIDTH
#define TFT_HEIGHT SCREEN_HEIGHT

#define TFT_DC 0
#define TFT_RST 4
#define TFT_WR 47
#define TFT_RD -1

#define TFT_D0 10
#define TFT_D1 9
#define TFT_D2 3
#define TFT_D3 8
#define TFT_D4 18
#define TFT_D5 17
#define TFT_D6 16
#define TFT_D7 15

#define TFT_BL GPIO_NUM_45

#define TOUCH_INT -1
#define TOUCH_SDA GPIO_NUM_7
#define TOUCH_SCL GPIO_NUM_6

class LGFX : public lgfx::LGFX_Device
{
public:
  lgfx::Bus_RGB _bus_instance;
  lgfx::Panel_GC9503 _panel_instance;
  lgfx::Touch_FT5x06 _touch_instance;
  lgfx::Light_PWM   _light_instance;

  LGFX(void)
  {
    {
      auto cfg = _panel_instance.config();

      cfg.memory_width = TFT_WIDTH;
      cfg.memory_height = TFT_HEIGHT;
      cfg.panel_width = TFT_WIDTH;
      cfg.panel_height = TFT_HEIGHT;

      cfg.offset_x = 0;
      cfg.offset_y = 0;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _panel_instance.config_detail();

      cfg.pin_cs = 0;
      cfg.pin_sclk = 10; //5;
      cfg.pin_mosi = 9; // 4;

      _panel_instance.config_detail(cfg);
    }

    {
      auto cfg = _bus_instance.config();
      cfg.panel = &_panel_instance;
      cfg.pin_d0 = GPIO_NUM_10;   // B0
      cfg.pin_d1 = GPIO_NUM_9;   // B1
      cfg.pin_d2 = GPIO_NUM_40;  // B2
      cfg.pin_d3 = GPIO_NUM_20;  // B3
      cfg.pin_d4 = GPIO_NUM_19;   // B4
      cfg.pin_d5 = GPIO_NUM_41;   // G0
      cfg.pin_d6 = GPIO_NUM_46;   // G1
      cfg.pin_d7 = GPIO_NUM_3;  // G2
      cfg.pin_d8 = GPIO_NUM_42;  // G3
      cfg.pin_d9 = GPIO_NUM_8;  // G4
      cfg.pin_d10 = GPIO_NUM_18;  // G5
      cfg.pin_d11 = GPIO_NUM_2; // R0
      cfg.pin_d12 = GPIO_NUM_17; // R1
      cfg.pin_d13 = GPIO_NUM_16; // R2
      cfg.pin_d14 = GPIO_NUM_1; // R3
      cfg.pin_d15 = GPIO_NUM_15;  // R4

      cfg.pin_henable = GPIO_NUM_13;
      cfg.pin_vsync = GPIO_NUM_12; //5 #
      cfg.pin_hsync = GPIO_NUM_11; //4 #
      cfg.pin_pclk = GPIO_NUM_14;
      cfg.freq_write = 15000000;

      cfg.hsync_polarity = 1; // #
      cfg.hsync_front_porch = 10;
      cfg.hsync_pulse_width = 8;
      cfg.hsync_back_porch = 50;
      cfg.vsync_polarity = 1; // #
      cfg.vsync_front_porch = 10;
      cfg.vsync_pulse_width = 8;
      cfg.vsync_back_porch = 20;
      cfg.pclk_idle_high = 0; //0 #
      cfg.de_idle_high = 0; //1 #
      _bus_instance.config(cfg);
    }
    _panel_instance.setBus(&_bus_instance);

    {
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;
      cfg.x_max = TFT_WIDTH - 1;
      cfg.y_min = 0;
      cfg.y_max = TFT_HEIGHT - 1;
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;

      cfg.i2c_port = I2C_NUM_1;

      cfg.pin_int = GPIO_NUM_NC;
      cfg.pin_sda = TOUCH_SDA;
      cfg.pin_scl = TOUCH_SCL;
      cfg.pin_rst = GPIO_NUM_NC;

      cfg.freq = 400000;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = TFT_BL;
      _light_instance.config(cfg);
    }
    _panel_instance.light(&_light_instance);

    setPanel(&_panel_instance);
  }
};


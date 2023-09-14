#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <globalDefines.h>

#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <driver/i2c.h>

// Portrait
#define TFT_WIDTH SCREEN_WIDTH
#define TFT_HEIGHT SCREEN_HEIGHT

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

//      cfg.pin_rst = GPIO_NUM_41;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _panel_instance.config_detail();

      cfg.pin_cs = GPIO_NUM_38;
      cfg.pin_sclk = GPIO_NUM_45;
      cfg.pin_mosi = GPIO_NUM_48;
   
      _panel_instance.config_detail(cfg);
    }

    {
      auto cfg = _bus_instance.config();

      cfg.panel = &_panel_instance;
      cfg.pin_d0 = GPIO_NUM_45;   // B0
      cfg.pin_d1 = GPIO_NUM_48;   // B1
      cfg.pin_d2 = GPIO_NUM_47;   // B2
      cfg.pin_d3 = GPIO_NUM_0;    // B3
      cfg.pin_d4 = GPIO_NUM_21;   // B4
      cfg.pin_d5 = GPIO_NUM_14;   // G0
      cfg.pin_d6 = GPIO_NUM_13;   // G1
      cfg.pin_d7 = GPIO_NUM_12;   // G2
      cfg.pin_d8 = GPIO_NUM_11;   // G3
      cfg.pin_d9 = GPIO_NUM_16;   // G4
      cfg.pin_d10 = GPIO_NUM_17;  // G5
      cfg.pin_d11 = GPIO_NUM_18;  // R0
      cfg.pin_d12 = GPIO_NUM_8;   // R1
      cfg.pin_d13 = GPIO_NUM_3;   // R2
      cfg.pin_d14 = GPIO_NUM_46;  // R3
      cfg.pin_d15 = GPIO_NUM_10;  // R4

      cfg.pin_henable = GPIO_NUM_40;
      cfg.pin_vsync = GPIO_NUM_41;
      cfg.pin_hsync = GPIO_NUM_42;
      cfg.pin_pclk = GPIO_NUM_39;
      cfg.freq_write = 12000000;

      cfg.hsync_polarity = 1;
      cfg.hsync_front_porch = 10;
      cfg.hsync_pulse_width = 8;
      cfg.hsync_back_porch = 50;
      cfg.vsync_polarity = 1;
      cfg.vsync_front_porch = 10;
      cfg.vsync_pulse_width = 8;
      cfg.vsync_back_porch = 20;
      cfg.pclk_idle_high = 0;
      cfg.de_idle_high = 0;

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

      cfg.i2c_port = I2C_NUM_0;

      cfg.pin_int = GPIO_NUM_NC;
      cfg.pin_sda = I2C0_SDA;
      cfg.pin_scl = I2C0_SCL;
      cfg.pin_rst = GPIO_NUM_NC;

      cfg.freq = 400000;

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    {
      auto cfg = _light_instance.config();

      cfg.pin_bl = GPIO_NUM_5;

      _light_instance.config(cfg);
    }
    _panel_instance.light(&_light_instance);

    setPanel(&_panel_instance);
  }
};


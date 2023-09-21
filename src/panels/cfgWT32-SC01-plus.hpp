#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <driver/i2c.h>
#include <globalDefines.h>

// Portrait
#define TFT_WIDTH   SCREEN_WIDTH
#define TFT_HEIGHT  SCREEN_HEIGHT

#define TFT_DC    0
#define TFT_RST   4
#define TFT_WR    47
#define TFT_RD    -1

#define TFT_D0 9
#define TFT_D1 46
#define TFT_D2 3
#define TFT_D3 8
#define TFT_D4 18
#define TFT_D5 17
#define TFT_D6 16
#define TFT_D7 15

#define TFT_BL    GPIO_NUM_45

#define TOUCH_INT GPIO_NUM_7
#define TOUCH_SDA I2C0_SDA  // GPIO_NUM_6
#define TOUCH_SCL I2C0_SCL  // GPIO_NUM_5


// SD CARD - SPI
// SD card Working / enable it below
//#define SD_SUPPORTED
#define SDSPI_HOST_ID SPI3_HOST
#define SD_MISO       GPIO_NUM_38
#define SD_MOSI       GPIO_NUM_40
#define SD_SCLK       GPIO_NUM_39
#define SD_CS         GPIO_NUM_41

class LGFX : public lgfx::LGFX_Device
{
  // provide an instance that matches the type of panel you want to connect to.
  lgfx::Panel_ST7796 _panel_instance;

  // provide an instance that matches the type of bus to which the panel is connected.
  lgfx::Bus_Parallel8 _bus_instance; // MCU8080 8B

  // Prepare an instance that matches the type of touchscreen.
  lgfx::Touch_FT5x06 _touch_instance;

  lgfx::Light_PWM _light_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.freq_write = 40000000;
      cfg.pin_wr = TFT_WR;
      cfg.pin_rd = TFT_RD;
      cfg.pin_rs = TFT_DC;

      // LCD data interface, 8bit MCU (8080)
      cfg.pin_d0 = TFT_D0;
      cfg.pin_d1 = TFT_D1;
      cfg.pin_d2 = TFT_D2;
      cfg.pin_d3 = TFT_D3;
      cfg.pin_d4 = TFT_D4;
      cfg.pin_d5 = TFT_D5;
      cfg.pin_d6 = TFT_D6;
      cfg.pin_d7 = TFT_D7;

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();

      cfg.pin_cs = -1;
      cfg.pin_rst = 4;
      cfg.pin_busy = -1;

      cfg.panel_width = TFT_WIDTH;
      cfg.panel_height = TFT_HEIGHT;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 2;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = false;
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config();

      cfg.pin_bl = TFT_BL;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    {
      auto cfg = _touch_instance.config();

      cfg.x_min = 0;
      cfg.x_max = TFT_WIDTH - 1;
      cfg.y_min = 0;
      cfg.y_max = TFT_HEIGHT - 1;
      cfg.pin_int = TOUCH_INT;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;

      cfg.i2c_port = I2C_NUM_0;
      cfg.i2c_addr = 0x38;
      cfg.pin_sda = TOUCH_SDA;
      cfg.pin_scl = TOUCH_SCL;
      cfg.freq = 400000;

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    setPanel(&_panel_instance);
  }
};
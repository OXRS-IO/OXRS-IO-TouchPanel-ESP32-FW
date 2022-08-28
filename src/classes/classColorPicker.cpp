#include <classColorPicker.h>
#include <globalDefines.h>

#define CP_BRIGHTNESS_WHITE_Y 250
#define CP_KELVIN_Y           150

typedef struct
{
  float h;
  float s;
  float v;
} hsv_t;
hsv_t _colorWheelHSV;

extern lv_color_t colorOn;
extern lv_color_t colorBg;

// convert RGB to HSV with RGB888 and floating point calc
// h = [0,360], s = [0,1], v = [0,1]
hsv_t _RGBtoHSV(lv_color32_t rgb)
{
  float min, max, delta;
  float r = (float)rgb.ch.red / 255.0;
  float g = (float)rgb.ch.green / 255.0;
  float b = (float)rgb.ch.blue / 255.0;
  hsv_t hsv;

  min = r < g ? (r < b ? r : b) : (g < b ? g : b);
  max = r > g ? (r > b ? r : b) : (g > b ? g : b);

  hsv.v = max; 
  delta = max - min;
  if (max != 0)
    hsv.s = delta / max;
  else 
  // r = g = b = 0
  {
    hsv.s = 0;
    hsv.h = -1;
    return hsv;
  }
  if (max == min) 
  // all grey
  {
    hsv.h = 0;
    hsv.s = 0;
    return hsv;
  }
  if (r == max)
    hsv.h = (g - b) / delta; // between yello and magenta
  else if (g == max)
    hsv.h = 2 + (b - r) / delta; // between cyan and yello
  else
    hsv.h = 4 + (r - g) / delta; // between magenty and cyan
  hsv.h *= 60;                   // degrees
  if (hsv.h < 0)
    hsv.h += 360;

  return hsv;
}

// convert HSV to RGB with RGB888 and floating point calc
lv_color32_t _HSVtoRGB(hsv_t hsv)
{
  int i;
  float f, p, q, t;
  float r, g, b;
  lv_color32_t rgb;

  if (hsv.s == 0)
  // no color -> grey
  {
    r = g = b = hsv.v;
  }
  else
  {
    // sector 0 to 5
    hsv.h /= 60;
    i = floor(hsv.h);
    // factorial part of h
    f = hsv.h - i;
    p = hsv.v * (1 - hsv.s);
    q = hsv.v * (1 - hsv.s * f);
    t = hsv.v * (1 - hsv.s * (1 - f));
    switch (i)
    {
    case 0:
      r = hsv.v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = hsv.v;
      b = p;
      break;
    case 2:
      r = p;
      g = hsv.v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = hsv.v;
      break;
    case 4:
      r = t;
      g = p;
      b = hsv.v;
      break;
    default: // case 5:
      r = hsv.v;
      g = p;
      b = q;
      break;
    }
  }
  rgb.ch.red = r * 255.0;
  rgb.ch.green = g * 255.0;
  rgb.ch.blue = b * 255.0;
  return rgb;
}

// build the panels with all widgets
void classColorPicker::_createColorPicker(lv_img_dsc_t *imgCw)
{
  // full screen overlay / opaqe
  _ovlPanel = lv_obj_create(lv_scr_act());
  lv_obj_remove_style_all(_ovlPanel);
  lv_obj_set_size(_ovlPanel, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_align(_ovlPanel, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(_ovlPanel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_ovlPanel, colorBg, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  _btnColor = lv_btn_create(_ovlPanel);
  lv_obj_align(_btnColor, LV_ALIGN_TOP_LEFT, 5, 5);
  lv_obj_set_size(_btnColor, 153, 40);
  lv_obj_set_style_bg_color(_btnColor, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_btnColor, lv_color_hex(0xffffff), LV_STATE_CHECKED);
  lv_obj_set_style_bg_opa(_btnColor, WP_OPA_BG_OFF, LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnColor, WP_OPA_BG_ON, LV_STATE_CHECKED);
  lv_obj_t *labelColor = lv_label_create(_btnColor);
  lv_label_set_text(labelColor, "Color");
  lv_obj_set_style_text_color(_btnColor, lv_color_hex(0x000000), LV_STATE_CHECKED);
  lv_obj_center(labelColor);
  lv_obj_add_flag(_btnColor, LV_OBJ_FLAG_HIDDEN);

  _btnKelvin = lv_btn_create(_ovlPanel);
  lv_obj_align(_btnKelvin, LV_ALIGN_TOP_RIGHT, -5, 5);
  lv_obj_set_size(_btnKelvin, 153, 40);
  lv_obj_set_style_bg_color(_btnKelvin, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_btnKelvin, lv_color_hex(0xffffff), LV_STATE_CHECKED);
  lv_obj_set_style_bg_opa(_btnKelvin, WP_OPA_BG_OFF, LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnKelvin, WP_OPA_BG_ON, LV_STATE_CHECKED);
  lv_obj_t *labelKelvin = lv_label_create(_btnKelvin);
  lv_label_set_text(labelKelvin, "Temperature");
  lv_obj_set_style_text_color(_btnKelvin, lv_color_hex(0x000000), LV_STATE_CHECKED);
  lv_obj_center(labelKelvin);
  lv_obj_add_flag(_btnKelvin, LV_OBJ_FLAG_HIDDEN);

  // RGB panel
  _panelRGB = lv_obj_create(_ovlPanel);
  lv_obj_remove_style_all(_panelRGB);
  lv_obj_set_size(_panelRGB, SCREEN_WIDTH - 10, 480 - 40 - 40 - 4 * 5);
  lv_obj_align(_panelRGB, LV_ALIGN_TOP_MID, 0, 50);
  lv_obj_set_style_radius(_panelRGB, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(_panelRGB, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_panelRGB, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_panelRGB, WP_OPA_BG_OFF, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_flag(_panelRGB, LV_OBJ_FLAG_HIDDEN);

  // CCT panel
  _panelCCT = lv_obj_create(_ovlPanel);
  lv_obj_remove_style_all(_panelCCT);
  lv_obj_set_size(_panelCCT, SCREEN_WIDTH - 10, 480 - 40 - 40 - 4 * 5);
  lv_obj_align(_panelCCT, LV_ALIGN_TOP_MID, 0, 50);
  lv_obj_set_style_radius(_panelCCT, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(_panelCCT, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_panelCCT, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_panelCCT, WP_OPA_BG_OFF, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_flag(_panelCCT, LV_OBJ_FLAG_HIDDEN);

  // back button (closes pop up)
  _btnExit = lv_btn_create(_ovlPanel);
  lv_obj_set_size(_btnExit, 80, 40);
  lv_obj_align(_btnExit, LV_ALIGN_BOTTOM_LEFT, 5, -5);
  lv_obj_set_style_bg_color(_btnExit, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnExit, WP_OPA_BG_OFF, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *label = lv_label_create(_btnExit);
  lv_label_set_text(label, LV_SYMBOL_LEFT);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  lv_obj_add_event_cb(_btnExit, _exitButtonEventHandler, LV_EVENT_CLICKED, this);

  // frame for color wheel
  _panelCwFrame = lv_obj_create(_panelRGB);
  lv_obj_remove_style_all(_panelCwFrame);
  lv_obj_set_size(_panelCwFrame, 300, 280);
  lv_obj_align(_panelCwFrame, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_clear_flag(_panelCwFrame, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_opa(_panelCwFrame, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  // cw image
  _imgCw = lv_img_create(_panelCwFrame);
  lv_img_set_src(_imgCw, imgCw);
  lv_obj_align(_imgCw, LV_ALIGN_CENTER, 0, 0);

  // color wheel cursor (circle filled with selected color)
  _panelCursor = lv_obj_create(_panelCwFrame);
  lv_obj_set_size(_panelCursor, 30, 30);
  lv_obj_set_align(_panelCursor, LV_ALIGN_CENTER);
  lv_obj_clear_flag(_panelCursor, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_radius(_panelCursor, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_panelCursor, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_panelCursor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_panelCursor, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_panelCursor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_panelCursor, 3, LV_PART_MAIN | LV_STATE_DEFAULT);

  // _barBrightness color
  _barBrightnessColor = lv_bar_create(_panelRGB);
  lv_bar_set_range(_barBrightnessColor, 0, 100);
  lv_obj_set_size(_barBrightnessColor, 290, 40);
  lv_obj_align(_barBrightnessColor, LV_ALIGN_TOP_MID, 0, 310);
  lv_obj_set_style_radius(_barBrightnessColor, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_barBrightnessColor, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_barBrightnessColor, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_barBrightnessColor, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barBrightnessColor, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barBrightnessColor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barBrightnessColor, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_main_stop(_barBrightnessColor, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_stop(_barBrightnessColor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(_barBrightnessColor, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barBrightnessColor, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barBrightnessColor, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // _slider Brightness color
  _sliderBrightnessColor = lv_slider_create(_panelRGB);
  lv_slider_set_range(_sliderBrightnessColor, 0, 100);
  lv_obj_set_size(_sliderBrightnessColor, 260, 40);
  lv_obj_align(_sliderBrightnessColor, LV_ALIGN_TOP_MID, 0, 310);
  lv_obj_set_style_bg_opa(_sliderBrightnessColor, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderBrightnessColor, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_sliderBrightnessColor, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderBrightnessColor, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_sliderBrightnessColor, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_sliderBrightnessColor, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_sliderBrightnessColor, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_color(_sliderBrightnessColor, lv_color_hex(0x000000), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(_sliderBrightnessColor, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(_sliderBrightnessColor, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(_sliderBrightnessColor, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(_sliderBrightnessColor, -10, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(_sliderBrightnessColor, -10, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(_sliderBrightnessColor, -10, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(_sliderBrightnessColor, -10, LV_PART_KNOB | LV_STATE_DEFAULT);

  // label Brightness Color level
  _labelBrightnessColorValue = lv_label_create(_panelRGB);
  lv_obj_set_size(_labelBrightnessColorValue, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelBrightnessColorValue, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(_labelBrightnessColorValue, "100 %");
  lv_obj_align(_labelBrightnessColorValue, LV_ALIGN_TOP_RIGHT, -20, 290);

  // label Brightness Color text
  _labelBrightnessColor = lv_label_create(_panelRGB);
  lv_obj_set_size(_labelBrightnessColor, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelBrightnessColor, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(_labelBrightnessColor, "Brightness Color");
  lv_obj_align(_labelBrightnessColor, LV_ALIGN_TOP_LEFT, 20, 290);

  // _barBrightness white
  _barBrightnessWhite = lv_bar_create(_panelCCT);
  lv_bar_set_range(_barBrightnessWhite, 0, 100);
  lv_obj_set_size(_barBrightnessWhite, 290, 40);
  lv_obj_align(_barBrightnessWhite, LV_ALIGN_TOP_MID, 0, CP_BRIGHTNESS_WHITE_Y);
  lv_obj_set_style_radius(_barBrightnessWhite, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_barBrightnessWhite, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_barBrightnessWhite, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_barBrightnessWhite, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barBrightnessWhite, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barBrightnessWhite, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barBrightnessWhite, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_main_stop(_barBrightnessWhite, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_stop(_barBrightnessWhite, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(_barBrightnessWhite, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barBrightnessWhite, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barBrightnessWhite, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // _slider Brightness white
  _sliderBrightnessWhite = lv_slider_create(_panelCCT);
  lv_slider_set_range(_sliderBrightnessWhite, 0, 100);
  lv_obj_set_size(_sliderBrightnessWhite, 260, 40);
  lv_obj_align(_sliderBrightnessWhite, LV_ALIGN_TOP_MID, 0, CP_BRIGHTNESS_WHITE_Y);
  lv_obj_set_style_bg_opa(_sliderBrightnessWhite, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderBrightnessWhite, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_sliderBrightnessWhite, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderBrightnessWhite, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_sliderBrightnessWhite, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_sliderBrightnessWhite, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_sliderBrightnessWhite, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_color(_sliderBrightnessWhite, lv_color_hex(0x000000), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(_sliderBrightnessWhite, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(_sliderBrightnessWhite, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(_sliderBrightnessWhite, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(_sliderBrightnessWhite, -10, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(_sliderBrightnessWhite, -10, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(_sliderBrightnessWhite, -10, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(_sliderBrightnessWhite, -10, LV_PART_KNOB | LV_STATE_DEFAULT);

  // label Brightness white level
  _labelBrightnessWhiteValue = lv_label_create(_panelCCT);
  lv_obj_set_size(_labelBrightnessWhiteValue, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelBrightnessWhiteValue, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(_labelBrightnessWhiteValue, "100 %");
  lv_obj_align(_labelBrightnessWhiteValue, LV_ALIGN_TOP_RIGHT, -20, CP_BRIGHTNESS_WHITE_Y-20);  

  // label Brightness white text
  _labelBrightnessWhite = lv_label_create(_panelCCT);
  lv_obj_set_size(_labelBrightnessWhite, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelBrightnessWhite, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(_labelBrightnessWhite, "Brightness White");
  lv_obj_align(_labelBrightnessWhite, LV_ALIGN_TOP_LEFT, 20, CP_BRIGHTNESS_WHITE_Y-20);

  // panel for kelvin slider components
  _panelKelvin = lv_obj_create(_panelCCT);
  lv_obj_set_size(_panelKelvin, 292, 42);
  lv_obj_align(_panelKelvin, LV_ALIGN_TOP_MID, 0, CP_KELVIN_Y);
  lv_obj_clear_flag(_panelKelvin, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_radius(_panelKelvin, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_panelKelvin, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_panelKelvin, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_panelKelvin, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  // kelvin bar lower
  _barKelvinLow = lv_bar_create(_panelKelvin);
  lv_bar_set_range(_barKelvinLow, 0, 100);
  lv_obj_set_size(_barKelvinLow, 163, 40);
  lv_obj_align(_barKelvinLow, LV_ALIGN_LEFT_MID, -12, 0);
  lv_obj_set_style_radius(_barKelvinLow, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barKelvinLow, lv_color_hex(0xFA9C19), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barKelvinLow, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barKelvinLow, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_main_stop(_barKelvinLow, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_stop(_barKelvinLow, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(_barKelvinLow, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barKelvinLow, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barKelvinLow, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // kelvin bar upper
  _barKelvinHigh = lv_bar_create(_panelKelvin);
  lv_bar_set_range(_barKelvinHigh, 0, 100);
  lv_obj_set_size(_barKelvinHigh, 163, 40);
  lv_obj_align(_barKelvinHigh, LV_ALIGN_RIGHT_MID, 12, 0);
  lv_obj_set_style_radius(_barKelvinHigh, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barKelvinHigh, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barKelvinHigh, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barKelvinHigh, lv_color_hex(0x2493DD), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_main_stop(_barKelvinHigh, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_stop(_barKelvinHigh, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(_barKelvinHigh, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barKelvinHigh, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barKelvinHigh, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // _slider Kelvin
  _sliderKelvin = lv_slider_create(_panelKelvin);
  lv_slider_set_range(_sliderKelvin, 2000, 6000);
  lv_obj_set_size(_sliderKelvin, 260, 40);
  lv_obj_align(_sliderKelvin, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_bg_opa(_sliderKelvin, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderKelvin, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_sliderKelvin, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderKelvin, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_sliderKelvin, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_sliderKelvin, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_sliderKelvin, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_color(_sliderKelvin, lv_color_hex(0x000000), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(_sliderKelvin, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(_sliderKelvin, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(_sliderKelvin, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(_sliderKelvin, -10, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(_sliderKelvin, -10, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(_sliderKelvin, -10, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(_sliderKelvin, -10, LV_PART_KNOB | LV_STATE_DEFAULT);

  // label kelvin value
  _labelKelvinValue = lv_label_create(_panelCCT);
  lv_obj_set_size(_labelKelvinValue, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelKelvinValue, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(_labelKelvinValue, "4000 K");
  lv_obj_align(_labelKelvinValue, LV_ALIGN_TOP_RIGHT, -20, CP_KELVIN_Y-20);

  // label kelvin text
  _labelKelvin = lv_label_create(_panelCCT);
  lv_obj_set_size(_labelKelvin, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelKelvin, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(_labelKelvin, "Color Temperature");
  lv_obj_align(_labelKelvin, LV_ALIGN_TOP_LEFT, 20, CP_KELVIN_Y-20);
}

void classColorPicker::_exitButtonEventHandler(lv_event_t *e)
{
  lv_obj_t *btn = lv_event_get_target(e);
  lv_obj_t *panel1 = lv_obj_get_parent(btn);
  lv_obj_del(panel1);
}

classColorPicker::classColorPicker(classTile *tile, lv_event_cb_t colorPickerEventHandler, lv_event_cb_t ColorPickerCwEventHandler, lv_img_dsc_t *imgCw, int cpMode)
{
  // layout the color picker pop up
  _createColorPicker(imgCw);

  // update controls with stored values from calling tile
  _callingTile = tile;
  updatePanelRGB(_callingTile->getColorPickerRGB());
  lv_slider_set_value(_sliderKelvin, _callingTile->getColorPickerKelvin(), LV_ANIM_OFF);
  lv_slider_set_value(_sliderBrightnessWhite, _callingTile->getColorPickerBrightnessWhite(), LV_ANIM_OFF);
  lv_label_set_text_fmt(_labelKelvinValue, "%d K", lv_slider_get_value(_sliderKelvin));
  lv_label_set_text_fmt(_labelBrightnessWhiteValue, "%d %%", lv_slider_get_value(_sliderBrightnessWhite));

  // add event handler
  lv_obj_add_event_cb(_panelCwFrame, ColorPickerCwEventHandler, LV_EVENT_ALL, _callingTile);

  lv_obj_add_flag(_btnColor, LV_OBJ_FLAG_USER_3);
  lv_obj_add_event_cb(_btnColor, colorPickerEventHandler, LV_EVENT_ALL, _callingTile);

  lv_obj_add_flag(_btnKelvin, LV_OBJ_FLAG_USER_3);
  lv_obj_add_flag(_btnKelvin, LV_OBJ_FLAG_USER_4);
  lv_obj_add_event_cb(_btnKelvin, colorPickerEventHandler, LV_EVENT_ALL, _callingTile);

  lv_obj_add_flag(_sliderKelvin, LV_OBJ_FLAG_USER_2);
  lv_obj_add_event_cb(_sliderKelvin, colorPickerEventHandler, LV_EVENT_VALUE_CHANGED, _callingTile);

  lv_obj_add_flag(_sliderBrightnessWhite, LV_OBJ_FLAG_USER_2);
  lv_obj_add_event_cb(_sliderBrightnessWhite, colorPickerEventHandler, LV_EVENT_VALUE_CHANGED, _callingTile);

  lv_obj_add_flag(_sliderBrightnessColor, LV_OBJ_FLAG_USER_1);
  lv_obj_add_event_cb(_sliderBrightnessColor, colorPickerEventHandler, LV_EVENT_VALUE_CHANGED, _callingTile);

  // enable selected mode(s)
  if (cpMode & CP_MODE_COLOR)
    lv_obj_clear_flag(_btnColor, LV_OBJ_FLAG_HIDDEN);

  if (cpMode & CP_MODE_TEMP)
    lv_obj_clear_flag(_btnKelvin, LV_OBJ_FLAG_HIDDEN);

  // select desired mode 
  if ((cpMode & (CP_MODE_COLOR | CP_MODE_TEMP)) == CP_MODE_COLOR)
    switchMode(CP_MODE_COLOR);
  else if ((cpMode & (CP_MODE_COLOR | CP_MODE_TEMP)) == CP_MODE_TEMP)
    switchMode(CP_MODE_TEMP);
  else
    switchMode(_callingTile->getColorPickerMode());
}

// update variables from ui content
void classColorPicker::updateAll(void)
{
  int brightnessColor = lv_slider_get_value(_sliderBrightnessColor);
  lv_label_set_text_fmt(_labelBrightnessColorValue, "%d %%", brightnessColor);
  _colorWheelHSV.v = brightnessColor / 100.0;
  lv_color32_t color32 = _HSVtoRGB(_colorWheelHSV);
  _callingTile->setColorPickerRGB(color32);

  lv_label_set_text_fmt(_labelKelvinValue, "%d K", lv_slider_get_value(_sliderKelvin));
  lv_label_set_text_fmt(_labelBrightnessWhiteValue, "%d %%", lv_slider_get_value(_sliderBrightnessWhite));
  _callingTile->setColorPickerKelvin(lv_slider_get_value(_sliderKelvin));
  _callingTile->setColorPickerBrightnessWhite(lv_slider_get_value(_sliderBrightnessWhite));
}

// update color wheel when cursor moved
void classColorPicker::updateCw(lv_point_t point, int mode)
{
  float x, y, alphaRad, alpha, r;

  lv_area_t imgCwArea;
  lv_obj_get_coords(_imgCw, &imgCwArea);

  x = point.x - CW_RADIUS - imgCwArea.x1;
  y = point.y - CW_RADIUS - imgCwArea.y1;
  
  r = sqrt(x * x + y * y);
  alphaRad = acos(x / r);
  if (y < 0) alphaRad = (2 * PI) - alphaRad;
  alpha = alphaRad / (2 * PI) * 360.0;
  if (r > CW_RADIUS) r = CW_RADIUS;
  x = cos(alphaRad) * r;
  y = sin(alphaRad) * r;

  _colorWheelHSV.h = alpha;
  _colorWheelHSV.s = r / (float)CW_RADIUS;

  lv_color_t color = lv_color_hsv_to_rgb(_colorWheelHSV.h, _colorWheelHSV.s * 100, 100);
  lv_obj_align(_panelCursor, LV_ALIGN_CENTER, x, y);
  lv_obj_set_style_bg_color(_panelCursor, color, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barBrightnessColor, color, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// update panelRgb from stored values in callingTile
void classColorPicker::updatePanelRGB(lv_color32_t rgb)
{
  float x, y, alphaRad;

  _colorWheelHSV = _RGBtoHSV(rgb);
  alphaRad = _colorWheelHSV.h * (2.0 * PI) / 360.0;
  x = cos(alphaRad) * _colorWheelHSV.s * CW_RADIUS;
  y = sin(alphaRad) * _colorWheelHSV.s * CW_RADIUS;
  lv_obj_align(_panelCursor, LV_ALIGN_CENTER, x, y);
  lv_color_t color = lv_color_hsv_to_rgb(_colorWheelHSV.h, _colorWheelHSV.s * 100, 100);
  lv_obj_set_style_bg_color(_panelCursor, color, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barBrightnessColor, color, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_slider_set_value(_sliderBrightnessColor, (int)(_colorWheelHSV.v * 100.0), LV_ANIM_OFF);
  lv_label_set_text_fmt(_labelBrightnessColorValue, "%d %%", (int)(_colorWheelHSV.v * 100.0));
}

// switch panel between color / Temperature 
void classColorPicker::switchMode(int  cpMode)
{
  if (cpMode == CP_MODE_COLOR)
  {
    // switch to Color
    lv_obj_clear_state(_btnKelvin, LV_STATE_CHECKED);
    lv_obj_add_state(_btnColor, LV_STATE_CHECKED);
    lv_obj_clear_flag(_panelRGB, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(_panelCCT, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    //switch to Temperature
    lv_obj_clear_state(_btnColor, LV_STATE_CHECKED);
    lv_obj_add_state(_btnKelvin, LV_STATE_CHECKED);
    lv_obj_clear_flag(_panelCCT, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(_panelRGB, LV_OBJ_FLAG_HIDDEN);
  }

  _callingTile->setColorPickerMode(cpMode);
}

bool classColorPicker::isActive(void)
{
  return lv_obj_is_valid(_ovlPanel);
}

void classColorPicker::close(void)
{
  lv_obj_del_delayed(_ovlPanel, 50);
}

// get reference of calling tile
classTile *classColorPicker::getTile(void)
{
    return _callingTile;
}




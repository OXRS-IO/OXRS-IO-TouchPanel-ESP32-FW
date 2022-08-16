#include <classScreenSettings.h>
#include <globalDefines.h>

extern void _setBackLightLED(int val);
extern lv_color_t colorOn;
extern lv_color_t colorBg;

static void _callBackRestart(lv_event_t *e)
{
  _setBackLightLED(0);
  ESP.restart();
}

classScreenSettings::classScreenSettings(lv_obj_t *parent, const void *img)
{
  //    screens[8] = classScreen(8, 0);
  //    ui_InfoScreen = screens[8].screen; // lv_obj_create(NULL);

  _parent = parent;

  lv_obj_clear_flag(_parent, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_parent, colorBg, LV_PART_MAIN);

  _imgLogo = lv_img_create(_parent);
  lv_img_set_src(_imgLogo, img);
  lv_obj_set_align(_imgLogo, LV_ALIGN_TOP_MID);

  _infoTextArea = lv_table_create(_parent);
  lv_obj_set_size(_infoTextArea, 320, 190);
  lv_obj_align_to(_infoTextArea, _imgLogo, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  lv_obj_set_style_radius(_infoTextArea, 5, 0);
  lv_obj_set_style_bg_color(_infoTextArea, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(_infoTextArea, 50, LV_PART_MAIN);
  lv_obj_set_style_border_width(_infoTextArea, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(_infoTextArea, 5, LV_PART_MAIN);
  lv_obj_set_style_border_width(_infoTextArea, 0, LV_PART_ITEMS);
  lv_obj_set_style_pad_all(_infoTextArea, 2, LV_PART_ITEMS);
  lv_obj_set_style_bg_opa(_infoTextArea, 0, LV_PART_ITEMS);

  lv_table_set_col_width(_infoTextArea, 0, 70);
  lv_table_set_col_width(_infoTextArea, 1, 320 - 70 - 10);

  // backligth slider
  _panelSlider = lv_obj_create(_parent);
  lv_obj_set_size(_panelSlider, 320, 65);
  lv_obj_align_to(_panelSlider, _infoTextArea, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_obj_set_style_bg_color(_panelSlider, lv_color_hex(0xffffff), 0);
  lv_obj_set_style_bg_opa(_panelSlider, 40, 0);
  lv_obj_set_style_border_width(_panelSlider, 0, 0);

  // ui_Slider2
  _slider = lv_slider_create(_panelSlider);
  lv_slider_set_range(_slider, 1, 100);
  lv_slider_set_value(_slider, 50, LV_ANIM_OFF);
  // if (lv_slider_get_mode(_slider) == LV_SLIDER_MODE_RANGE)
  lv_slider_set_left_value(_slider, 2, LV_ANIM_OFF);
  lv_obj_set_size(_slider, 280, 15);
  lv_obj_set_align(_slider, LV_ALIGN_BOTTOM_MID);

  lv_obj_set_style_outline_color(_slider, lv_color_hex(0x070708), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(_slider, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(_slider, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(_slider, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_slider, lv_color_hex(0x0A0B0B), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_slider, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_slider, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_main_stop(_slider, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_stop(_slider, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(_slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // ui_Label1
  _sliderLabel1 = lv_label_create(_panelSlider);
  lv_obj_set_size(_sliderLabel1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_align(_sliderLabel1, LV_ALIGN_CENTER, -100, -19);
  lv_label_set_text(_sliderLabel1, "Backlight");

  // ui_Label2
  _sliderLabel2 = lv_label_create(_panelSlider);
  lv_obj_set_size(_sliderLabel2, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_align(_sliderLabel2, LV_ALIGN_CENTER, 125, -19);
  lv_label_set_text(_sliderLabel2, "50%");

  // reset button
  _btnRestart = lv_btn_create(_parent);

  lv_obj_set_style_bg_opa(_btnRestart, WP_OPA_BG_OFF, LV_PART_MAIN | LV_IMGBTN_STATE_RELEASED);
  lv_obj_set_style_img_recolor(_btnRestart, lv_color_hex(0xffffff), LV_PART_MAIN | LV_IMGBTN_STATE_RELEASED);
  lv_obj_set_style_img_recolor_opa(_btnRestart, 255, LV_PART_MAIN | LV_IMGBTN_STATE_RELEASED);
  lv_obj_set_size(_btnRestart, 150, 50);
  lv_obj_align(_btnRestart, LV_ALIGN_BOTTOM_RIGHT, -5, -40);

  lv_obj_t *_labelRestart = lv_label_create(_btnRestart);
  lv_obj_set_size(_labelRestart, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_center(_labelRestart);
  lv_label_set_text(_labelRestart, "Hold to restart...");

  lv_obj_add_event_cb(_btnRestart, _callBackRestart, LV_EVENT_LONG_PRESSED, NULL);
}

void classScreenSettings::addEventHandler(lv_event_cb_t callBack)
{
  lv_obj_add_event_cb(_slider, callBack, LV_EVENT_ALL, this);
}

lv_obj_t *classScreenSettings::getInfoPanel(void)
{
  return _infoTextArea;
}

void classScreenSettings::setSlider(int value)
{
  // lv_label_set_text(_sliderLabel2, text);
  lv_slider_set_value(_slider, value, LV_ANIM_OFF);
  lv_label_set_text_fmt(_sliderLabel2, "%d%%", value);
}

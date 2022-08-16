#pragma once
#include <lvgl.h>

class classScreenSettings
{
private:
  lv_obj_t *_parent = NULL;
  lv_obj_t *_imgLogo = NULL;
  lv_obj_t *_infoTextArea = NULL;
  lv_obj_t *_panelSlider = NULL;
  lv_obj_t *_slider = NULL;
  lv_obj_t *_sliderLabel1 = NULL;
  lv_obj_t *_sliderLabel2 = NULL;
  lv_obj_t *_btnRestart = NULL;

public:
  classScreenSettings(){};
  classScreenSettings(lv_obj_t *parent, const void *img);
  void addEventHandler(lv_event_cb_t callBack);
  lv_obj_t *getInfoPanel(void);
  void setSlider(int value);
};
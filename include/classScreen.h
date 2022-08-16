#pragma once
#include <lvgl.h>

class classScreen
{
private:
  lv_obj_t *_parent = NULL;
  lv_obj_t *_labelFooter = NULL;
  lv_obj_t *_labelWarning = NULL;
  lv_obj_t *_btnHome = NULL;
  lv_obj_t *_btnHomeImg;
  lv_obj_t *_btnSettings = NULL;
  lv_obj_t *_btnSettingsImg = NULL;
  lv_obj_t *_btnFooter = NULL;
  lv_obj_t *_labelLeft = NULL;
  lv_obj_t *_labelCenter = NULL;
  lv_obj_t *_labelRight = NULL;

  bool _hidden = false;

public:
  int screenIdx;

  lv_obj_t *screen = NULL;
  lv_obj_t *container = NULL;

  classScreen(void){};
  classScreen(int screenIdx, int style);
  
  void setHidden(bool hidden);
  bool isHidden();
  
  void setLabel(const char *labelText);
  const char *getLabel(void);
  
  void setFooter(const char *left, const char *center, const char *right);

  void updateBgColor(void);
  void createHomeButton(lv_event_cb_t callBack, const void *img);
  void createSettingsButton(lv_event_cb_t callBack, const void *img);
  void showConnectionStatus(bool connected);
  
  int getScreenNumber(void);
 
  void adWipeEventHandler(lv_event_cb_t callBack);
  void adScreenEventHandler(lv_event_cb_t callBack);
};

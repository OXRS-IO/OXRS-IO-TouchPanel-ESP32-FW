#pragma once
#include <lvgl.h>
#include <globalDefines.h>

#define TP_COLOR_BG_OVERWRITE LV_OBJ_FLAG_USER_1

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

  lv_color_t _screenBgColor = {0, 0, 0};
  bool _hidden = false;

  // grid definitions
  lv_coord_t colDsc[SCREEN_COLS_MAX + 1];
  lv_coord_t rowDsc[SCREEN_ROWS_MAX + 1];
  int _screenCols = SCREEN_COLS;
  int _screenRows = SCREEN_ROWS;

  void _makeScreenLayout(void);

public:
  int screenIdx;

  lv_obj_t *screen = NULL;
  lv_obj_t *container = NULL;

  classScreen(void){};
  ~classScreen();
  void begin(int screenIdx, int style, int cols, int rows);

  void setHidden(bool hidden);
  bool isHidden();
  
  void setLabel(const char *labelText);
  const char *getLabel(void);
  
  void setFooter(const char *left, const char *center, const char *right);

  void setBgColor(lv_color_t color);
  lv_color_t getBgColor(void);
  void updateBgColor(void);

  void createHomeButton(lv_event_cb_t callBack, const void *img);
  void createSettingsButton(lv_event_cb_t callBack, const void *img);
  void showConnectionStatus(bool connected);
  
  int getScreenNumber(void);
  int getScreenCols(void);
  int getScreenRows(void);

  void adScreenEventHandler(lv_event_cb_t callBack);
};

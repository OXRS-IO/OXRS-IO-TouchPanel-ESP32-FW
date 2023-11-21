#pragma once
#include <lvgl.h>
#include <classTile.h>

class classPopUpContainer

{
protected:
  classTile *_callingTile = NULL;
  lv_obj_t *_screen = NULL;
  lv_obj_t *_parentScreen = NULL;
  lv_obj_t *_ovlPanel = NULL;
  lv_obj_t *_panel = NULL;

  void _startUp(void);

public:
  classPopUpContainer(void){};
  classPopUpContainer(int start);
  void close(void);
  bool isActive(void);
  void addEventHandler(lv_event_cb_t callBack);
  classTile *getTile(void) { return _callingTile; };
  lv_obj_t *getScreen(void) { return _screen; };
  lv_obj_t *getParentScreen(void) { return _parentScreen; };
};

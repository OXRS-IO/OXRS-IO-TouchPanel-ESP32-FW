#pragma once
#include <lvgl.h>
#include <classTile.h>

class classPopUpContainer

{
protected:
  classTile *_callingTile = NULL;
  lv_obj_t *_parent = NULL;
  lv_obj_t *_ovlPanel = NULL;
  lv_obj_t *_panel = NULL;
  lv_obj_t *_btnExit = NULL;

  static void _exitButtonEventHandler(lv_event_t *e);

public:
  classPopUpContainer(void){};
  classPopUpContainer(int start);
  void close(void);
  bool isActive(void);
  classTile *getTile(void){return _callingTile;};
};

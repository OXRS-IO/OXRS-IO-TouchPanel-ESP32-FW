#pragma once
#include <lvgl.h>
#include <classTile.h>

class classSetPoint

{
private:
  classTile *_callingTile = NULL;
  lv_obj_t *_parent = NULL;
  lv_obj_t *_ovlPanel = NULL;
  lv_obj_t *_panel = NULL;
  lv_obj_t *_btnExit = NULL;
  lv_obj_t *_arcSetPoint = NULL;
  lv_obj_t *_labelSetPoint = NULL;
  lv_obj_t *_labelSetPointUnits = NULL;
  lv_obj_t *_labelRoomTemp = NULL;
  lv_obj_t *_labelCallingTile = NULL;
  lv_obj_t *_dropDown = NULL;
  lv_obj_t *_btnMode = NULL;
  lv_obj_t *_labelMode = NULL;

  int _setPointMin = 100;
  int _setPointMax = 300;

  void _createSetPoint(void);
  static void _exitButtonEventHandler(lv_event_t *e);

public:
  classSetPoint(void){};
  classSetPoint(classTile *tile, lv_event_cb_t setPointEventHandler);
  classTile *getTile(void);
  void close(void);
  bool isActive(void);
  void updateAll(void);
  void updateSetPoint(int setPoint);
  void showDropDown(void);
  void closeDropDown(void);
};

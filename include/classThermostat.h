#pragma once
#include <lvgl.h>
#include <classTile.h>

class classThermostat

{
private:
  classTile *_callingTile = NULL;
  lv_obj_t *_parent = NULL;
  lv_obj_t *_ovlPanel = NULL;
  lv_obj_t *_panel = NULL;
  lv_obj_t *_btnExit = NULL;
  lv_obj_t *_arcTarget = NULL;
  lv_obj_t *_labelTarget = NULL;
  lv_obj_t *_labelUnits = NULL;
  lv_obj_t *_labelCurrent = NULL;
  lv_obj_t *_labelCallingTile = NULL;
  lv_obj_t *_dropDown = NULL;
  lv_obj_t *_btnMode = NULL;
  lv_obj_t *_labelMode = NULL;

  int _targetMin = 100;
  int _targetMax = 300;

  void _createThermostat(void);
  static void _exitButtonEventHandler(lv_event_t *e);

public:
  classThermostat(void){};
  classThermostat(classTile *tile, lv_event_cb_t setPointEventHandler);
  classTile *getTile(void);
  void close(void);
  bool isActive(void);
  void updateAll(void);
  void updateTarget(int target);
  void showDropDown(void);
  void closeDropDown(void);
};

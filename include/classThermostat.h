#pragma once
#include <classPopUpContainer.h>

class classThermostat : public classPopUpContainer

{
private:
  lv_obj_t *_labelCallingTile = NULL;
  lv_obj_t *_arcTarget = NULL;
  lv_obj_t *_labelTarget = NULL;
  lv_obj_t *_labelUnits = NULL;
  lv_obj_t *_labelCurrent = NULL;
  lv_obj_t *_dropDown = NULL;
  lv_obj_t *_btnMode = NULL;
  lv_obj_t *_labelMode = NULL;

  int _targetMin = 100;
  int _targetMax = 300;

  void _createThermostat(void);

public:
  classThermostat(void){};
  classThermostat(classTile *tile, lv_event_cb_t setPointEventHandler);
  void updateAll(void);
  void updateTarget(int target);
  void showDropDown(void);
  void closeDropDown(void);
};

#pragma once
#include <lvgl.h>
#include <classTile.h>

class classDropDown

{
private:
  classTile* _callingTile = NULL;
  lv_obj_t* _parent = NULL;
  lv_obj_t *_ovlPanel = NULL;
  lv_obj_t *_ovlPanel2 = NULL;
  lv_obj_t* _btnBack = NULL;
  lv_obj_t* _btnUp = NULL;
  lv_obj_t* _btnDown = NULL;
  lv_obj_t* _label = NULL;
  lv_obj_t* _dropDown = NULL;
  lv_obj_t* _barLabel = NULL;

  void _createDropDown(void);

public:
  classDropDown(void){};
  classDropDown(classTile* tile, lv_event_cb_t dropDownEventHandler);
  void close(void);
  void open(void);
  classTile* getTile(void);
  bool isActive(void);

  void setDropDownList(const char *list);
  void setDropDownIndex(uint16_t index);
  void setDropDownLabel(const char* label);

};

#pragma once
#include <classPopUpContainer.h>

class classDropDown : public classPopUpContainer

{
private:
  lv_obj_t *_dropDown = NULL;

  void _createDropDown(void);

public:
  classDropDown(void){};
  classDropDown(classTile* tile, lv_event_cb_t dropDownEventHandler);
  void open(void);

  void setDropDownList(const char *list);
  void setDropDownIndex(uint16_t index);
  void setDropDownLabel(const char* label);

};

#pragma once
#include <classPopUpContainer.h>

class classRemote : public classPopUpContainer

{
private:
  lv_obj_t *_labelCallingTile = NULL;
  lv_obj_t *_panelRemote = NULL;
  lv_obj_t *_btnUp = NULL;
  lv_obj_t* _btnDown = NULL;
  lv_obj_t* _btnLeft = NULL;
  lv_obj_t* _btnRight = NULL;
  lv_obj_t* _btnOk = NULL;
  lv_obj_t* _btnInfo = NULL;
  lv_obj_t* _btnList = NULL;
  lv_obj_t* _btnBack = NULL;
  lv_obj_t* _btnHome = NULL;

  void _createRemote(void);
  void _setCommon(lv_obj_t *button, int btnIndex);
  void _setNavButtonProperties(lv_obj_t *button, int btnIndex);
  void _setExtraButtonProperties(lv_obj_t *button, int btnIndex);

public:
  classRemote(void){};
  classRemote(classTile* tile, lv_event_cb_t navigationButtonEventHandler);
};

#pragma once
#include <lvgl.h>
#include "classTile.h"
#include "classScreen.h"

class classRemote

{
private:
  classTile* _callingTile = NULL;
  lv_obj_t* _parent = NULL;
  lv_obj_t* _ovlPanel = NULL;
  lv_obj_t* _ovlPanel2 = NULL;
  lv_obj_t* _btnUp = NULL;
  lv_obj_t* _btnDown = NULL;
  lv_obj_t* _btnLeft = NULL;
  lv_obj_t* _btnRight = NULL;
  lv_obj_t* _btnOk = NULL;
  lv_obj_t* _btnInfo = NULL;
  lv_obj_t* _btnList = NULL;
  lv_obj_t* _btnBack = NULL;
  lv_obj_t* _btnHome = NULL;
  lv_obj_t* _btnExit = NULL;

  void _createRemote(void);
  void _setCommon(lv_obj_t *button, int btnIndex);
  void _setNavButtonProperties(lv_obj_t *button, int btnIndex);
  void _setExtraButtonProperties(lv_obj_t *button, int btnIndex);

public:
  classRemote(void){};
  classRemote(classTile* tile, lv_event_cb_t navigationButtonEventHandler);
  void close(void);
  bool isActive(void);

  classTile* getTile(void);
  static void closeEvt(lv_event_t* e);
 
};

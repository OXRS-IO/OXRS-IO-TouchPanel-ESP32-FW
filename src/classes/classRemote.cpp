#include "classRemote.h"
#include "globalDefines.h"

extern lv_color_t colorOn;
extern lv_color_t colorBg;
extern "C" const lv_img_dsc_t KodiStyleBlackAlpha_png;

void classRemote::_setCommon(lv_obj_t *button, int btnIndex)
{
  lv_obj_clear_flag(button, LV_OBJ_FLAG_PRESS_LOCK);
  lv_obj_set_style_bg_opa(button, 255, LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(button, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(button, colorOn, LV_STATE_PRESSED);
  lv_obj_set_style_outline_color(button, colorOn, LV_STATE_PRESSED);
  lv_obj_set_style_outline_opa(button, 255, LV_STATE_PRESSED);

  if (btnIndex & 0x01)    lv_obj_add_flag(button, LV_OBJ_FLAG_USER_1);
  if (btnIndex & 0x02)    lv_obj_add_flag(button, LV_OBJ_FLAG_USER_2);
  if (btnIndex & 0x04)    lv_obj_add_flag(button, LV_OBJ_FLAG_USER_3);
  if (btnIndex & 0x08)    lv_obj_add_flag(button, LV_OBJ_FLAG_USER_4);
}

void classRemote::_setNavButtonProperties(lv_obj_t *button, int btnIndex)
{
  lv_obj_set_style_radius(button, 60, LV_PART_MAIN);
  lv_obj_set_style_outline_width(button, 10, LV_STATE_PRESSED);

  _setCommon(button, btnIndex);
}

void classRemote::_setExtraButtonProperties(lv_obj_t *button, int btnIndex)
{
  lv_obj_set_style_radius(button, 10, LV_PART_MAIN);
  lv_obj_set_style_outline_width(button, 5, LV_STATE_PRESSED);

  _setCommon(button, btnIndex);
}

void classRemote::_createRemote(void)
{
  // label calling tile aat top
  _labelCallingTile = lv_label_create(_panel);
  lv_obj_set_size(_labelCallingTile, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelCallingTile, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(_labelCallingTile, "");
  lv_obj_align(_labelCallingTile, LV_ALIGN_TOP_MID, 00, 20);

  // container for remote control elements
  _panelRemote = lv_obj_create(_panel);
  lv_obj_remove_style_all(_panelRemote);
  lv_obj_set_size(_panelRemote, 320, 320);
  lv_obj_align(_panelRemote, LV_ALIGN_CENTER, 0, 0);
  lv_obj_clear_flag(_panelRemote, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_panelRemote, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_panelRemote, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  // set up all buttons for remote control
  _btnUp = lv_btn_create(_panelRemote);
  lv_obj_set_size(_btnUp, 100, 80);
  lv_obj_align(_btnUp, LV_ALIGN_TOP_MID, 0, 20);
  _setNavButtonProperties(_btnUp, 1);

  _btnDown = lv_btn_create(_panelRemote);
  lv_obj_set_size(_btnDown, 100, 80);
  lv_obj_align(_btnDown, LV_ALIGN_BOTTOM_MID, 0, -20);
  _setNavButtonProperties(_btnDown, 2);

  _btnLeft = lv_btn_create(_panelRemote);
  lv_obj_set_size(_btnLeft, 80, 100);
  lv_obj_align(_btnLeft, LV_ALIGN_LEFT_MID, 20, 0);
  _setNavButtonProperties(_btnLeft, 3);

  _btnRight = lv_btn_create(_panelRemote);
  lv_obj_set_size(_btnRight, 80, 100);
  lv_obj_align(_btnRight, LV_ALIGN_RIGHT_MID, -20, 0);
  _setNavButtonProperties(_btnRight, 4);

  _btnOk = lv_btn_create(_panelRemote);
  lv_obj_set_size(_btnOk, 100, 100);
  lv_obj_align(_btnOk, LV_ALIGN_CENTER, 0, 0);
  _setNavButtonProperties(_btnOk, 5);
  lv_obj_set_style_radius(_btnOk, 50, LV_PART_MAIN);

  _btnInfo = lv_btn_create(_panelRemote);
  lv_obj_set_size(_btnInfo, 60, 60);
  lv_obj_align(_btnInfo, LV_ALIGN_TOP_RIGHT, -11, 11);
  _setExtraButtonProperties(_btnInfo, 6);

  _btnList = lv_btn_create(_panelRemote);
  lv_obj_set_size(_btnList, 60, 60);
  lv_obj_align(_btnList, LV_ALIGN_BOTTOM_RIGHT, -11, -11);
  _setExtraButtonProperties(_btnList, 7);

  _btnBack = lv_btn_create(_panelRemote);
  lv_obj_set_size(_btnBack, 60, 60);
  lv_obj_align(_btnBack, LV_ALIGN_BOTTOM_LEFT, 11, -11);
  _setExtraButtonProperties(_btnBack, 8);

  _btnHome = lv_btn_create(_panelRemote);
  lv_obj_set_size(_btnHome, 60, 60);
  lv_obj_align(_btnHome, LV_ALIGN_TOP_LEFT, 11, 11);
  _setExtraButtonProperties(_btnHome, 9);

  // image on top of buttons
  // image is rendered at runtime from .png as array in progmem
  lv_obj_t *img = lv_img_create(_panelRemote);
  lv_img_set_src(img, &KodiStyleBlackAlpha_png);
  lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
}

classRemote::classRemote(classTile* tile, lv_event_cb_t remoteEventHandler) : classPopUpContainer(1)
{
  _createRemote();
  _callingTile = tile;

  lv_label_set_text(_labelCallingTile, _callingTile->getLabel());

  lv_obj_add_event_cb(_btnUp, remoteEventHandler, LV_EVENT_ALL, _callingTile);
  lv_obj_add_event_cb(_btnDown, remoteEventHandler, LV_EVENT_ALL, _callingTile);
  lv_obj_add_event_cb(_btnLeft, remoteEventHandler, LV_EVENT_ALL, _callingTile);
  lv_obj_add_event_cb(_btnRight, remoteEventHandler, LV_EVENT_ALL, _callingTile);
  lv_obj_add_event_cb(_btnOk, remoteEventHandler, LV_EVENT_ALL, _callingTile);
  lv_obj_add_event_cb(_btnInfo, remoteEventHandler, LV_EVENT_ALL, _callingTile);
  lv_obj_add_event_cb(_btnList, remoteEventHandler, LV_EVENT_ALL, _callingTile);
  lv_obj_add_event_cb(_btnBack, remoteEventHandler, LV_EVENT_ALL, _callingTile);
  lv_obj_add_event_cb(_btnHome, remoteEventHandler, LV_EVENT_ALL, _callingTile);
}





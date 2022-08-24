#include <classSetPoint.h>
#include <globalDefines.h>

extern lv_color_t colorOn;
extern lv_color_t colorBg;
extern "C" const lv_font_t number_OR_50;

// build the panels with all widgets
void classSetPoint::_createSetPoint(void)
{
  // full screen overlay / opaqe
  _ovlPanel = lv_obj_create(lv_scr_act());
  lv_obj_remove_style_all(_ovlPanel);
  lv_obj_set_size(_ovlPanel, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_align(_ovlPanel, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(_ovlPanel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_ovlPanel, colorBg, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);


  // base panel
  _panel = lv_obj_create(_ovlPanel);
  lv_obj_remove_style_all(_panel);
  lv_obj_set_size(_panel, SCREEN_WIDTH - 10, 480 - 40 - 5 * 3);
  lv_obj_align(_panel, LV_ALIGN_TOP_MID, 0, 5);
  lv_obj_set_style_radius(_panel, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(_panel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_panel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_panel, WP_OPA_BG_OFF, LV_PART_MAIN | LV_STATE_DEFAULT);

  // back button (closes pop up)
  _btnExit = lv_btn_create(_ovlPanel);
  lv_obj_set_size(_btnExit, 80, 40);
  lv_obj_align(_btnExit, LV_ALIGN_BOTTOM_LEFT, 5, -5);
  lv_obj_set_style_bg_color(_btnExit, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnExit, WP_OPA_BG_OFF, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *label = lv_label_create(_btnExit);
  lv_label_set_text(label, LV_SYMBOL_LEFT);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  lv_obj_add_event_cb(_btnExit, _exitButtonEventHandler, LV_EVENT_CLICKED, this);

  _arcSetPoint = lv_arc_create(_panel);

  lv_obj_set_size(_arcSetPoint, 250, 250);
  lv_obj_set_align(_arcSetPoint, LV_ALIGN_CENTER);
  lv_arc_set_bg_angles(_arcSetPoint, 135, 45);

 // lv_arc_set_range(_arcSetPoint, _setPointMin / 5, _setPointMax / 5);
  lv_arc_set_value(_arcSetPoint, 50);

  lv_obj_set_style_pad_all(_arcSetPoint, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_color(_arcSetPoint, lv_color_hex(0xC8C8C8), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(_arcSetPoint, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_width(_arcSetPoint, 3, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_arc_color(_arcSetPoint, lv_color_hex(0x0000FF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(_arcSetPoint, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_width(_arcSetPoint, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_arcSetPoint, lv_color_lighten(colorBg, WP_OPA_BG_OFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_arcSetPoint, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_arcSetPoint, lv_color_hex(0xC8C8C8), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_arcSetPoint, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_arcSetPoint, 3, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_side(_arcSetPoint, LV_BORDER_SIDE_FULL, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(_arcSetPoint, 7, LV_PART_KNOB | LV_STATE_DEFAULT);

  // label set point
  _labelSetPoint = lv_label_create(_panel);
  lv_obj_set_size(_labelSetPoint, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelSetPoint, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(_labelSetPoint, &number_OR_50, 0);
  // lv_obj_set_style_text_font(_labelSetPoint, &lv_font_montserrat_20, 0);
  lv_label_set_text(_labelSetPoint, "");
  lv_obj_align(_labelSetPoint, LV_ALIGN_CENTER, -10, -10);

  // label set point
  _labelSetPointUnits = lv_label_create(_panel);
  lv_obj_set_size(_labelSetPointUnits, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelSetPointUnits, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(_labelSetPointUnits, &lv_font_montserrat_20, 0);
  lv_label_set_text(_labelSetPointUnits, "°C");
  lv_obj_align_to(_labelSetPointUnits, _labelSetPoint, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, 5);
  // lv_obj_align(_labelSetPointUnits, LV_ALIGN_CENTER, 0, 0);

  // label room temp
  _labelRoomTemp = lv_label_create(_panel);
  lv_obj_set_size(_labelRoomTemp, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelRoomTemp, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_style_text_font(_labelSetPoint, &lv_font_montserrat_20, 0);
  lv_label_set_text(_labelRoomTemp, "");
  lv_obj_align(_labelRoomTemp, LV_ALIGN_CENTER, 00, 60);

  // label calling tile label
  _labelCallingTile = lv_label_create(_panel);
  lv_obj_set_size(_labelCallingTile, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_color(_labelCallingTile, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_style_text_font(_labelSetPoint, &lv_font_montserrat_20, 0);
  lv_label_set_text(_labelCallingTile, "");
  lv_obj_align(_labelCallingTile, LV_ALIGN_TOP_MID, 00, 20);

  // mode button 
  _btnMode = lv_btn_create(_panel);
  lv_obj_set_size(_btnMode, 150, 40);
  lv_obj_align(_btnMode, LV_ALIGN_BOTTOM_MID, 0, -40);
  lv_obj_set_style_bg_color(_btnMode, lv_color_lighten(colorBg, WP_OPA_BG_OFF * 2), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnMode, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  _labelMode = lv_label_create(_btnMode);
  lv_label_set_text(_labelMode, "");
  lv_obj_align(_labelMode, LV_ALIGN_CENTER, 0, 0);

  // configure the drop down
  _dropDown = lv_dropdown_create(_panel);
  lv_obj_set_style_text_font(_dropDown, &lv_font_montserrat_20, 0);
  lv_obj_set_style_pad_ver(_dropDown, 18, LV_PART_MAIN);
  lv_obj_set_size(_dropDown, 250, LV_SIZE_CONTENT);
  lv_obj_align(_dropDown, LV_ALIGN_BOTTOM_MID, 00, -40);
  lv_obj_set_style_border_width(_dropDown, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(_dropDown, lv_color_lighten(colorBg, WP_OPA_BG_OFF * 2), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(_dropDown, 255, LV_PART_MAIN);
  lv_dropdown_set_options(_dropDown, "empty");
  lv_obj_set_style_text_align(_dropDown, LV_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_t *list = lv_dropdown_get_list(_dropDown);
  lv_obj_set_style_max_height(list, 370, LV_PART_MAIN);
  lv_obj_set_style_max_width(list, 300, LV_PART_MAIN);
  lv_obj_set_style_border_width(list, 0, LV_PART_MAIN);
  lv_obj_set_style_text_line_space(list, 30, LV_PART_MAIN);
  lv_obj_set_style_text_font(list, &lv_font_montserrat_20, 0);
  lv_obj_set_style_bg_color(list, lv_color_lighten(colorBg, WP_OPA_BG_OFF * 2), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(list, 255, LV_PART_MAIN);
  lv_obj_set_style_radius(list, 5, LV_PART_SELECTED);
  lv_obj_set_style_bg_color(list, colorOn, LV_PART_SELECTED | LV_STATE_PRESSED);
  lv_obj_set_style_bg_color(list, colorOn, LV_PART_SELECTED | LV_STATE_CHECKED);
  lv_obj_add_flag(_dropDown, LV_OBJ_FLAG_HIDDEN);
}

void classSetPoint::_exitButtonEventHandler(lv_event_t *e)
{
  lv_obj_t *btn = lv_event_get_target(e);
  lv_obj_t *panel1 = lv_obj_get_parent(btn);
  lv_obj_del(panel1);
}

classSetPoint::classSetPoint(classTile *tile, lv_event_cb_t setPointEventHandler)
{
  // layout the color picker pop up
  _createSetPoint();

 _callingTile = tile;
  // get min/max from config
 if ((_callingTile->getLevelStart() != 0) || (_callingTile->getLevelStop() != 100))
 {
   _setPointMin = _callingTile->getLevelStart();
   _setPointMax = _callingTile->getLevelStop();
  }
  lv_arc_set_range(_arcSetPoint, _setPointMin / 5, _setPointMax / 5);

  // update controls with stored values from calling tile
  lv_label_set_text(_labelCallingTile, _callingTile->getLabel());
  updateSetPoint(_callingTile->getSetPointValue() / 5);
  updateAll();
  lv_label_set_text(_labelRoomTemp, _callingTile->getSetPointRoomTemp());
  lv_dropdown_set_text(_dropDown, _callingTile->getDropDownLabel());
  lv_dropdown_set_options(_dropDown, _callingTile->getDropDownList());
  int index = _callingTile->getDropDownIndex();
  if (index > 0)
    index--;
  lv_dropdown_set_selected(_dropDown, index);
  char buf[64];
  lv_dropdown_get_selected_str(_dropDown, buf, sizeof(buf));
  lv_label_set_text(_labelMode, buf);

  // add event handler
  lv_obj_add_flag(_arcSetPoint, LV_OBJ_FLAG_USER_1);
  lv_obj_add_event_cb(_arcSetPoint, setPointEventHandler, LV_EVENT_ALL, _callingTile);

  lv_obj_add_flag(_btnMode, LV_OBJ_FLAG_USER_2);
  lv_obj_add_event_cb(_btnMode, setPointEventHandler, LV_EVENT_ALL, _callingTile);

  lv_obj_add_flag(_dropDown, LV_OBJ_FLAG_USER_3);
  lv_obj_add_event_cb(_dropDown, setPointEventHandler, LV_EVENT_ALL, _callingTile);
}

// update variables from ui content
void classSetPoint::updateAll(void)
{
  int setPoint = lv_arc_get_value(_arcSetPoint) * 5;
  char buffer[10];
  sprintf(buffer, "%2.1f", (float)setPoint / 10.0);
  lv_label_set_text(_labelSetPoint, buffer);
  _callingTile->setSetPointValue(setPoint);
  lv_obj_align_to(_labelSetPointUnits, _labelSetPoint, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, 5);

  // calc hsv for coloring
  int range = (_setPointMax - _setPointMin) / 2;
  int mid = range + _setPointMin;
  int s = 50 + (abs(mid -setPoint) * 50) / range;
  // red(360) or blue(240)
  int h = 360;
  if (setPoint < mid)
    h = 240;
  lv_obj_set_style_arc_color(_arcSetPoint, lv_color_hsv_to_rgb(h, s, 100), LV_PART_INDICATOR | LV_STATE_DEFAULT);
}

// update panelRgb from stored values in callingTile
void classSetPoint::updateSetPoint(int setPointValue)
{
  lv_arc_set_value(_arcSetPoint, setPointValue);
}

bool classSetPoint::isActive(void)
{
  return lv_obj_is_valid(_ovlPanel);
}

void classSetPoint::close(void)
{
  lv_obj_del_delayed(_ovlPanel, 50);
}

// get reference of calling tile
classTile *classSetPoint::getTile(void)
{
  return _callingTile;
}

void classSetPoint::showDropDown(void)
{
  lv_obj_clear_flag(_dropDown, LV_OBJ_FLAG_HIDDEN);
  lv_dropdown_open(_dropDown);
}

void classSetPoint::closeDropDown(void)
{
  char buf[64];
  lv_dropdown_get_selected_str(_dropDown, buf, sizeof(buf));
  lv_label_set_text(_labelMode, buf);
  _callingTile->saveDropDownIndex(lv_dropdown_get_selected(_dropDown) + 1);
  lv_obj_add_flag(_dropDown, LV_OBJ_FLAG_HIDDEN);
}

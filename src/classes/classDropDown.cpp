#include <classDropDown.h>
#include <globalDefines.h>

extern lv_color_t colorOn;
extern lv_color_t colorBg;

void classDropDown::_createDropDown(void)
{
  // full screen overlay / semi transparent
  _ovlPanel = lv_obj_create(lv_scr_act());
  lv_obj_remove_style_all(_ovlPanel);
  lv_obj_set_size(_ovlPanel, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_align(_ovlPanel, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(_ovlPanel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_ovlPanel, colorBg, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

  // active upper part / colorBg
  _ovlPanel2 = lv_obj_create(_ovlPanel);
  lv_obj_remove_style_all(_ovlPanel2);
  lv_obj_set_size(_ovlPanel2, SCREEN_WIDTH, SCREEN_HEIGHT - 35);
  lv_obj_set_align(_ovlPanel2, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(_ovlPanel2, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_ovlPanel2, colorBg, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // panel for active controls
  lv_obj_t*  _panel = lv_obj_create(_ovlPanel2);
  lv_obj_remove_style_all(_panel);

  lv_obj_set_size(_panel, 310, SCREEN_HEIGHT - 35);
  lv_obj_align(_panel, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_clear_flag(_panel, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_bg_color(_panel, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_panel, WP_OPA_BG_OFF, LV_PART_MAIN | LV_STATE_DEFAULT);

  // configure the drop down 
  _dropDown = lv_dropdown_create(_panel);
  lv_obj_set_style_text_font(_dropDown, &lv_font_montserrat_20, 0);
  lv_obj_set_style_pad_ver(_dropDown, 18, LV_PART_MAIN);
  lv_obj_set_size(_dropDown, 300, LV_SIZE_CONTENT);
  lv_obj_align(_dropDown, LV_ALIGN_TOP_MID, 00, 10);
  lv_obj_set_style_border_width(_dropDown, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(_dropDown, colorBg, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(_dropDown, 150, LV_PART_MAIN);
  lv_dropdown_set_options(_dropDown, "empty");
  lv_obj_t* list = lv_dropdown_get_list(_dropDown);
  lv_obj_set_style_max_height(list, 370, LV_PART_MAIN);
  lv_obj_set_style_max_width(list, 300, LV_PART_MAIN);
  lv_obj_set_style_border_width(list, 0, LV_PART_MAIN);
  lv_obj_set_style_text_line_space(list, 30, LV_PART_MAIN);
  lv_obj_set_style_text_font(list, &lv_font_montserrat_20, 0);
  lv_obj_set_style_bg_color(list, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(list, WP_OPA_BG_OFF, LV_PART_MAIN);
  lv_obj_set_style_radius(list, 5, LV_PART_SELECTED);
  lv_obj_set_style_bg_color(list, colorOn, LV_PART_SELECTED | LV_STATE_PRESSED);
  lv_obj_set_style_bg_color(list, colorOn, LV_PART_SELECTED | LV_STATE_CHECKED);
}

classDropDown::classDropDown(classTile *tile, lv_event_cb_t dropDownEventHandler)
{
  // layout the drop down overlay
  _createDropDown();

  if(tile)
  {
    _callingTile = tile;
    // update with variables from calling tile
    lv_dropdown_set_text(_dropDown, _callingTile->getDropDownLabel());
    lv_dropdown_set_options(_dropDown, _callingTile->getDropDownList());
    int index = _callingTile->getDropDownIndex();
    if (index > 0) index--;
    lv_dropdown_set_selected(_dropDown, index);
  }
  // add event handler
  lv_obj_add_event_cb(_dropDown, dropDownEventHandler, LV_EVENT_ALL, _callingTile);
}

bool classDropDown::isActive(void)
{
  return lv_obj_is_valid(_ovlPanel);
}

// open the list
void classDropDown::open(void)
{
  lv_dropdown_open(_dropDown);
}

// close (destroy) the overlay panel
void classDropDown::close(void)
{
  lv_obj_del_delayed(_ovlPanel, 50);
}

// populate drop down list
void classDropDown::setDropDownList(const char* list)
{
  lv_dropdown_set_options(_dropDown, list);
}

// preset selected index
void classDropDown::setDropDownIndex(uint16_t index)
{
  lv_dropdown_set_selected(_dropDown, index - 1);
}

// set drop down label (selector)- replaces selectedItem
void classDropDown::setDropDownLabel(const char *label)
{
  lv_dropdown_set_text(_dropDown, label);
}

// get reference of calling tile
classTile* classDropDown::getTile(void)
{
  return _callingTile;
}




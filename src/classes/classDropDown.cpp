#include <classDropDown.h>
#include <globalDefines.h>

extern lv_color_t colorOn;
extern lv_color_t colorBg;

void classDropDown::_createDropDown(void)
{
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
  lv_obj_set_style_max_height(list, 350, LV_PART_MAIN);
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

classDropDown::classDropDown(classTile *tile, lv_event_cb_t dropDownEventHandler) : classPopUpContainer(1)
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

// open the list
void classDropDown::open(void)
{
  lv_dropdown_open(_dropDown);
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

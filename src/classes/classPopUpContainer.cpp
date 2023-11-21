#include <classPopUpContainer.h>
#include <globalDefines.h>

extern lv_color_t colorOn;
extern lv_color_t colorBg;
extern int opaBgOff;
extern int opaBgOn;

// build the panels with all widgets
void classPopUpContainer::_startUp(void)
{
  // new screen for pop-up 
  _screen = lv_obj_create(NULL);

  // full screen overlay / opaqe
  _ovlPanel = lv_obj_create(_screen);
  lv_obj_remove_style_all(_ovlPanel);
  lv_obj_set_size(_ovlPanel, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_align(_ovlPanel, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(_ovlPanel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_ovlPanel, colorBg, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // base panel
  _panel = lv_obj_create(_ovlPanel);
  lv_obj_remove_style_all(_panel);
  lv_obj_set_size(_panel, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
  lv_obj_align(_panel, LV_ALIGN_TOP_MID, 0, 5);
  lv_obj_set_style_radius(_panel, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(_panel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_panel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_panel, opaBgOff, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// build the panels with all widgets (constructor) and show on screen
classPopUpContainer::classPopUpContainer(int start)
{
  _parentScreen = lv_scr_act();
  _startUp();
  lv_disp_load_scr(_screen);
}

void classPopUpContainer::addEventHandler(lv_event_cb_t callBack)
{
  lv_obj_add_event_cb(_screen, callBack, LV_EVENT_GESTURE, this);
}

bool classPopUpContainer::isActive(void)
{
  return lv_obj_is_valid(_screen);
}

void classPopUpContainer::close(void)
{
  lv_disp_load_scr(_parentScreen);
  lv_obj_del_delayed(_screen, 50);
  _screen = NULL;
}

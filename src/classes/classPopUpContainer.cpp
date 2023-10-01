#include <classPopUpContainer.h>
#include <globalDefines.h>

extern lv_color_t colorOn;
extern lv_color_t colorBg;
extern int opaBgOff;
extern int opaBgOn;

// build the panels with all widgets
void classPopUpContainer::_startUp(void)
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
  lv_obj_set_style_bg_opa(_panel, opaBgOff, LV_PART_MAIN | LV_STATE_DEFAULT);

  // back button (closes pop up)
  _btnExit = lv_btn_create(_ovlPanel);
  lv_obj_set_size(_btnExit, 153, 40);
  lv_obj_align(_btnExit, LV_ALIGN_BOTTOM_LEFT, 5, -5);
  lv_obj_set_style_bg_color(_btnExit, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnExit, opaBgOff, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *label = lv_label_create(_btnExit);
  lv_label_set_text(label, LV_SYMBOL_LEFT);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  lv_obj_add_event_cb(_btnExit, _exitButtonEventHandler, LV_EVENT_CLICKED, this);
}

// build the panels with all widgets (constructor)
classPopUpContainer::classPopUpContainer(int start)
{
  _startUp();
}

void classPopUpContainer::_exitButtonEventHandler(lv_event_t *e)
{
  lv_obj_t *btn = lv_event_get_target(e);
  lv_obj_t *ovlPanel = lv_obj_get_parent(btn);
  lv_obj_del(ovlPanel);
}

bool classPopUpContainer::isActive(void)
{
  return lv_obj_is_valid(_ovlPanel);
}

void classPopUpContainer::close(void)
{
  lv_obj_del_delayed(_ovlPanel, 50);
  _ovlPanel = NULL;
}

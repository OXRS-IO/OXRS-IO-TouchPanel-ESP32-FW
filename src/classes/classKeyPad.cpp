#include "classKeyPad.h"
#include "globalDefines.h"

#define PASSWORD_SHOW_TIME 500

extern lv_color_t colorOn;
extern lv_color_t colorBg;

extern "C" const lv_font_t pwd_fond_15;

static const char *btnm_map[] = {"1", "2", "3", "\n",
                                 "4", "5", "6", "\n",
                                 "7", "8", "9", "\n",
                                 LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""};

// set keypad state from calling tile
// give tille a second image (UnLocked)

void classKeyPad::_createKeyPad(void)
{
  // full screen overlay / opaqe background
  _ovlPanel = lv_obj_create(lv_scr_act());
  lv_obj_remove_style_all(_ovlPanel);
  lv_obj_set_size(_ovlPanel, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_align(_ovlPanel, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(_ovlPanel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_ovlPanel, colorBg, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // active panel
  _ovlPanel2 = lv_obj_create(_ovlPanel);
  lv_obj_remove_style_all(_ovlPanel2);
  lv_obj_set_size(_ovlPanel2, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
  lv_obj_align(_ovlPanel2, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_radius(_ovlPanel2, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(_ovlPanel2, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_ovlPanel2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel2, WP_OPA_BG_OFF, LV_PART_MAIN | LV_STATE_DEFAULT);

  _btnm1 = lv_btnmatrix_create(_ovlPanel2);
  lv_obj_set_style_text_font(_btnm1, &lv_font_montserrat_20, LV_PART_ITEMS);
  lv_obj_set_style_height(_btnm1, 340, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(_btnm1, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(_btnm1, 80, LV_PART_ITEMS);
  lv_obj_set_style_bg_color(_btnm1, lv_color_hex(0xffffff), LV_PART_ITEMS);
  lv_obj_set_style_bg_opa(_btnm1, WP_OPA_BG_OFF, LV_PART_ITEMS);
  lv_obj_set_style_border_width(_btnm1, 0, LV_PART_MAIN);

  lv_obj_set_style_pad_row(_btnm1, 20, LV_PART_MAIN);
  lv_obj_set_style_pad_column(_btnm1, 20, LV_PART_MAIN);
  lv_obj_set_style_pad_all(_btnm1, 10, LV_PART_MAIN);

  lv_btnmatrix_set_map(_btnm1, btnm_map);
  lv_obj_align(_btnm1, LV_ALIGN_BOTTOM_MID, 0, -20);

  pwd_ta = lv_textarea_create(_ovlPanel2);
  lv_obj_set_style_border_width(pwd_ta, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(pwd_ta, 0, LV_PART_MAIN);
  lv_textarea_set_align(pwd_ta, LV_TEXT_ALIGN_CENTER);
  lv_textarea_set_password_mode(pwd_ta, true);
  lv_textarea_set_password_show_time(pwd_ta, PASSWORD_SHOW_TIME);
  lv_textarea_set_one_line(pwd_ta, true);
  lv_textarea_set_cursor_click_pos(pwd_ta, false);
  lv_textarea_set_max_length(pwd_ta, 10);
  lv_obj_set_width(pwd_ta, LV_SIZE_CONTENT);
  lv_obj_align(pwd_ta, LV_ALIGN_TOP_MID, 0, 80);

  _imgLockState = lv_img_create(_ovlPanel2);
  lv_obj_align(_imgLockState, LV_ALIGN_TOP_MID, 0, 0);
  lv_img_set_zoom(_imgLockState, 180);
  
  // main Label shows label from callingTile
  _label = lv_label_create(_ovlPanel2);
  lv_obj_set_size(_label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_align(_label, LV_ALIGN_TOP_MID, 0, 60);
  lv_obj_set_style_text_color(_label, lv_color_hex(0x000000), LV_STATE_CHECKED);
  lv_label_set_text(_label, _callingTile->getLabel());

  setState(_callingTile->getState() ? _imgLocked : _imgUnLocked, _callingTile->getColor(), "enter code");
}

classKeyPad::classKeyPad(classTile *tile, lv_event_cb_t keyPadEventHandler)
{
  _callingTile = tile;  
  _callingTile->getImages(_imgUnLocked, _imgLocked);
  _createKeyPad();

  lv_obj_add_event_cb(_btnm1, keyPadEventHandler, LV_EVENT_SHORT_CLICKED, _callingTile);
}

void classKeyPad::addChar(char value)
{
  lv_obj_set_style_text_font(pwd_ta, &pwd_fond_15, LV_PART_MAIN); // V
  lv_textarea_add_char(pwd_ta, value);
}

void classKeyPad::delChar(void)
{
  if (strlen(lv_textarea_get_text(pwd_ta)) <= 1)
    lv_obj_set_style_text_font(pwd_ta, lv_font_default(), LV_PART_MAIN); // V
  lv_textarea_del_char(pwd_ta);
}

void classKeyPad::setLocked(lv_color_t color, const char *text)
{
  setState(_imgLocked, color, text);
}

void classKeyPad::setUnlocked(lv_color_t color, const char *text)
{
  setState(_imgUnLocked, color, text);
}

void classKeyPad::setFailed(lv_color_t color, const char *text)
{
  setState(NULL, color, text);
}

void classKeyPad::setState(const void *img, lv_color_t color, const char *text)
{
  if (img != NULL)
  {
    lv_img_set_src(_imgLockState, img);
  }

  lv_obj_set_style_img_recolor(_imgLockState, color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(_imgLockState, 255, LV_PART_MAIN);

  lv_obj_set_style_text_font(pwd_ta, lv_font_default(), LV_PART_MAIN); // V
  lv_textarea_set_text(pwd_ta, "");
  lv_textarea_set_placeholder_text(pwd_ta, text);
}

bool classKeyPad::isActive(void)
{
  return lv_obj_is_valid(_ovlPanel);
}

void classKeyPad::close(void)
{
  lv_obj_del_delayed(_ovlPanel, 50);
}

const char *classKeyPad::getKey(void)
{
  return (lv_textarea_get_text(pwd_ta));
}

classTile *classKeyPad::getTile(void)
{
  return _callingTile;
}

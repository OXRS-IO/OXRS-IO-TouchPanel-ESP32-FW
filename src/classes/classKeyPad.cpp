#include "classKeyPad.h"

#define PASSWORD_SHOW_TIME 500

extern lv_color_t colorOn;
extern lv_color_t colorBg;
extern int opaBgOff;
extern int opaBgOn;

extern "C" const lv_font_t pwd_fond_15;

static const char *btnm_map[] = {"1", "2", "3", "\n",
                                 "4", "5", "6", "\n",
                                 "7", "8", "9", "\n",
                                 LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""};

// set keypad state from calling tile
// give tille a second image (UnLocked)

void classKeyPad::_createKeyPad(void)
{
  int heightOffset = (!_callingTile) ? 45 : 0;
  lv_obj_set_style_height(_panel, lv_obj_get_style_height(_panel, LV_PART_MAIN) + heightOffset, LV_PART_MAIN);
  _btnm1 = lv_btnmatrix_create(_panel);
  lv_obj_set_style_text_font(_btnm1, &lv_font_montserrat_20, LV_PART_ITEMS);
  lv_obj_set_style_height(_btnm1, 320 + heightOffset, LV_PART_MAIN);
  lv_obj_set_style_width(_btnm1, (320 + heightOffset) * 3 / 4, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(_btnm1, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(_btnm1, 80, LV_PART_ITEMS);
  lv_obj_set_style_bg_color(_btnm1, lv_color_hex(0xffffff), LV_PART_ITEMS);
  lv_obj_set_style_bg_opa(_btnm1, opaBgOff, LV_PART_ITEMS);
  lv_obj_set_style_border_width(_btnm1, 0, LV_PART_MAIN);

  lv_obj_set_style_pad_row(_btnm1, 15, LV_PART_MAIN);
  lv_obj_set_style_pad_column(_btnm1, 15, LV_PART_MAIN);
  lv_obj_set_style_pad_all(_btnm1, 10, LV_PART_MAIN);

  lv_btnmatrix_set_map(_btnm1, btnm_map);
  lv_obj_align(_btnm1, LV_ALIGN_BOTTOM_MID, 0, 0);

  pwd_ta = lv_textarea_create(_panel);
  lv_obj_set_style_border_width(pwd_ta, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(pwd_ta, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(pwd_ta, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(pwd_ta, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_textarea_set_align(pwd_ta, LV_TEXT_ALIGN_CENTER);
  lv_textarea_set_password_mode(pwd_ta, true);
  lv_textarea_set_password_show_time(pwd_ta, PASSWORD_SHOW_TIME);
  lv_textarea_set_one_line(pwd_ta, true);
  lv_textarea_set_cursor_click_pos(pwd_ta, false);
  lv_textarea_set_max_length(pwd_ta, 10);
  lv_obj_set_width(pwd_ta, LV_SIZE_CONTENT);
  lv_obj_align(pwd_ta, LV_ALIGN_TOP_MID, 0, 75);

  _imgLockState = lv_img_create(_panel);
  lv_obj_align(_imgLockState, LV_ALIGN_TOP_MID, 0, -3);
  lv_img_set_zoom(_imgLockState, 180);
  
  // main Label shows label from callingTile
  _label = lv_label_create(_panel);
  lv_obj_set_size(_label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_align(_label, LV_ALIGN_TOP_MID, 0, 57);
  lv_label_set_text(_label, "");
  lv_obj_set_style_text_color(_label, lv_color_hex(0x000000), LV_STATE_CHECKED);
  if(_callingTile)
  {
    lv_label_set_text(_label, _callingTile->getLabel());
    setState(_callingTile->getState() ? "locked" : "unlocked", NULL, _callingTile->getColor(), "enter code");
  }
}

classKeyPad::classKeyPad(classTile *tile, lv_event_cb_t keyPadEventHandler, keyPadType_t kpType) : classPopUpContainer(1)
{
  if (tile)
  {
    _callingTile = tile;  
    _callingTile->getImages(_imgUnLocked, _imgLocked);
  }
  _createKeyPad();
  _kpType = kpType;
  // hide exit button if called by direct command (no parent tile)
  if (!_callingTile)
    lv_obj_add_flag(_btnExit, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_event_cb(_btnm1, keyPadEventHandler, LV_EVENT_SHORT_CLICKED, _callingTile);
}

void classKeyPad::setLabel(const char *labelText)
{
  lv_label_set_text(_label, labelText);
}

keyPadType_t classKeyPad::getKeyPadType(void)
{
  return _kpType;
}

void classKeyPad::addChar(char value)
{
  lv_obj_set_style_text_font(pwd_ta, &pwd_fond_15, LV_PART_MAIN);
  lv_textarea_add_char(pwd_ta, value);
}

void classKeyPad::delChar(void)
{
  if (strlen(lv_textarea_get_text(pwd_ta)) <= 1)
    lv_obj_set_style_text_font(pwd_ta, lv_font_default(), LV_PART_MAIN);
  lv_textarea_del_char(pwd_ta);
}

void classKeyPad::setState(const char *state, const void *icon, lv_color_t color, const char *text)
{
  if (icon != NULL)
  {
    lv_img_set_src(_imgLockState, icon);
  }
  else 
  {
    if (strcmp(state, "locked") == 0)
    {
      lv_img_set_src(_imgLockState, _imgLocked);
    }
    else if (strcmp(state, "unlocked") == 0)
    {
      lv_img_set_src(_imgLockState, _imgUnLocked);
    }
  }

  if (color.full == 0)
  {
    if (strcmp(state, "locked") == 0)
    {
      color = colorOn;
    }
    else if (strcmp(state, "unlocked") == 0)
    {
      color = lv_color_hex(0xffffff);
    }
    else if (strcmp(state, "failed") == 0)
    {
      color = lv_color_hex(0xff0000);
    }
  }

  lv_obj_set_style_img_recolor(_imgLockState, color, LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(_imgLockState, 255, LV_PART_MAIN);

  lv_obj_set_style_text_font(pwd_ta, lv_font_default(), LV_PART_MAIN); // V
  lv_textarea_set_text(pwd_ta, "");
  lv_textarea_set_placeholder_text(pwd_ta, text);
}

const char *classKeyPad::getKey(void)
{
  return (lv_textarea_get_text(pwd_ta));
}

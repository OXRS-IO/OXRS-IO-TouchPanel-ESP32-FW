#include <classTile.h>
#include <globalDefines.h>

extern lv_color_t colorOn;
extern lv_color_t colorBg;
extern "C" const lv_font_t number_OR_50;

// create the tile
void classTile::_button(lv_obj_t *parent, const void *img)
{
  _parent = parent;
  _img = img;
  _imgOn = img;
  _imgConfig = img;
  _imgOnConfig = img;

  // image button
  _btn = lv_imgbtn_create(_parent);

  // placeholder for full image
  _imgBg = lv_img_create(_btn);

  lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_RELEASED, img, NULL, NULL);
  lv_obj_set_style_bg_opa(_btn, WP_OPA_BG_OFF, LV_PART_MAIN | LV_IMGBTN_STATE_RELEASED);
  lv_obj_set_style_img_recolor(_btn, lv_color_hex(0xffffff), LV_PART_MAIN | LV_IMGBTN_STATE_RELEASED);
  lv_obj_set_style_img_recolor_opa(_btn, 255, LV_PART_MAIN | LV_IMGBTN_STATE_RELEASED);

  lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, _imgOn, NULL, NULL);
  lv_obj_set_style_bg_opa(_btn, WP_OPA_BG_ON, LV_STATE_CHECKED);
  lv_obj_set_style_img_recolor(_btn, colorOn, LV_STATE_CHECKED);
  lv_obj_set_style_img_recolor_opa(_btn, 255, LV_STATE_CHECKED);

  lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, _imgOn, NULL, NULL);

  lv_obj_clear_flag(_btn, LV_OBJ_FLAG_PRESS_LOCK);

  // main Label (placeholder)
  _label = lv_label_create(_btn);
  lv_obj_set_size(_label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_label, "");
  lv_obj_align(_label, LV_ALIGN_BOTTOM_LEFT, 8, -22);
  lv_obj_set_style_text_color(_btn, lv_color_hex(0x000000), LV_STATE_CHECKED);

  // additional Label (show sybol ">" if button liks to new screen)
  _linkedLabel = lv_label_create(_btn);
  lv_obj_set_size(_linkedLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_linkedLabel, "");
  lv_obj_align(_linkedLabel, LV_ALIGN_TOP_RIGHT, -8, 5);
  lv_obj_set_style_text_color(_btn, lv_color_hex(0x000000), LV_STATE_CHECKED);

  // Sub Label (placeholder)
  _subLabel = lv_label_create(_btn);
  lv_obj_set_size(_label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_subLabel, "");
  lv_obj_align(_subLabel, LV_ALIGN_BOTTOM_LEFT, 8, -5);
  lv_obj_set_style_text_color(_subLabel, lv_color_hex(0x808080), 0);

  // additional Label (placeholder)
  _numLabel = lv_label_create(_btn);
  lv_obj_set_size(_numLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_numLabel, "");
  lv_obj_align(_numLabel, LV_ALIGN_TOP_LEFT, 8, 15);
  lv_obj_set_style_text_color(_numLabel, lv_color_hex(0x000000), LV_STATE_CHECKED);

  // additional Label (unit display)
  _unitLabel = lv_label_create(_btn);
  lv_obj_set_size(_unitLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_unitLabel, "");
  lv_obj_set_style_text_color(_unitLabel, lv_color_hex(0x000000), LV_STATE_CHECKED);

  // label for text to replace icon  (hide by default)
  _txtIconText = lv_label_create(_btn);
  lv_obj_align(_txtIconText, LV_ALIGN_TOP_LEFT, 8, 4);
  lv_obj_set_style_text_font(_txtIconText, &lv_font_montserrat_20, 0);
  lv_obj_set_style_text_color(_txtIconText, colorOn, LV_PART_MAIN | LV_STATE_CHECKED);
  lv_label_set_text(_txtIconText, "");
  lv_label_set_recolor(_txtIconText, true);
  lv_obj_add_flag(_txtIconText, LV_OBJ_FLAG_HIDDEN);
 
  // placeholders for drop down
  _dropDownList = lv_label_create(_btn);
  lv_obj_add_flag(_dropDownList, LV_OBJ_FLAG_HIDDEN);
  lv_label_set_text(_dropDownList, "");

  _dropDownLabel = lv_label_create(_btn);
  lv_obj_add_flag(_dropDownLabel, LV_OBJ_FLAG_HIDDEN);
  lv_label_set_text(_dropDownLabel, "");

  // set button and label size from grid
  int width = *lv_obj_get_style_grid_column_dsc_array(parent, 0) - 10;
  int height = *lv_obj_get_style_grid_row_dsc_array(parent, 0) - 10;

  lv_obj_set_width(_btn, width);
  lv_obj_set_height(_btn, height);

  lv_obj_set_size(_label, width - 10, LV_SIZE_CONTENT);
  lv_obj_set_size(_subLabel, width - 10, LV_SIZE_CONTENT);
  lv_obj_set_size(_txtIconText, width - 20, height - 4);

  btn = _btn;
}

void classTile::_setIconTextFromIndex()
{
  const char *text = lv_label_get_text(_dropDownList);
  if (strlen(text) > 0)
  {
    int index = _dropDownIndex;
    if (index > 0)
      index--;
    lv_obj_t *dd = lv_dropdown_create(_btn);
    lv_dropdown_set_options(dd, text);
    lv_dropdown_set_selected(dd, index);
    char buf[64];
    lv_dropdown_get_selected_str(dd, buf, sizeof(buf));
    setIconText(buf);
    lv_obj_del(dd);
  }
}

// recolor all effected objects
void classTile::_reColorAll(lv_color_t color, lv_style_selector_t selector)
{
  lv_obj_set_style_img_recolor(_btn, color, selector);
  lv_obj_set_style_text_color(_numLabel, color, selector);
  lv_obj_set_style_text_color(_unitLabel, color, selector);
  lv_obj_set_style_text_color(_txtIconText, color, selector);
}

// free ps_ram heap used by old image if exist
void classTile::_freeImageHeap(void)
{
  if (lv_obj_is_valid(_imgBg))
  {
    lv_img_dsc_t *oldImg = (lv_img_dsc_t *)lv_img_get_src(_imgBg);
    if (oldImg)
    {
      lv_img_cache_invalidate_src(oldImg);
      free((void *)oldImg->data);
      free(oldImg);
    }
  }
}

classTile::classTile(lv_obj_t *parent, const void *img)
{
  _button(parent, img);
}

classTile::classTile(lv_obj_t *parent, const void *img, const char *labelText)
{
  _button(parent, img);
  lv_label_set_text(_label, labelText);
}

classTile::~classTile()
{
  if (_btn)
  {
    _freeImageHeap();
    lv_obj_del(_btn);
  }
}

// initialise existing object
void classTile::begin(lv_obj_t *parent, const void *img, const char *labelText)
{
  _button(parent, img);
  lv_label_set_text(_label, labelText);
}

// supply bookkeeping information and align tile in grid
void classTile::registerTile(int screenIdx, int tileIdx, int style, const char* styleStr)
{
  _screenIdx = screenIdx;
  _tileIdx = tileIdx;
  tileId.idx.screen = screenIdx;
  tileId.idx.tile = tileIdx;
  _style = style;
  _styleStr = styleStr;

  // position tile in grid after tile and screen are known
  int row = (tileIdx - 1) / 2;
  int col = (tileIdx - 1) % 2;
  lv_obj_set_grid_cell(_btn, LV_GRID_ALIGN_CENTER, col, 1, LV_GRID_ALIGN_CENTER, row, 1);
}

void classTile::setLabel(const char *labelText)
{
  lv_label_set_text(_label, labelText);
}

void classTile::setSubLabel(const char *subLabelText)
{
  lv_label_set_text(_subLabel, subLabelText);
}

void classTile::setState(bool state)
{
  _state = state;
  state == false ? lv_obj_clear_state(_btn, LV_STATE_CHECKED) : lv_obj_add_state(_btn, LV_STATE_CHECKED);
  state == false ? lv_obj_clear_state(_txtIconText, LV_STATE_CHECKED) : lv_obj_add_state(_txtIconText, LV_STATE_CHECKED);
  if (_btnUp)
    state == false ? lv_obj_clear_state(_btnUp, LV_STATE_CHECKED) : lv_obj_add_state(_btnUp, LV_STATE_CHECKED);
  if (_btnDown)
    state == false ? lv_obj_clear_state(_btnDown, LV_STATE_CHECKED) : lv_obj_add_state(_btnDown, LV_STATE_CHECKED);
}

lv_color_t classTile::getColor()
{
  if (lv_obj_get_state(_btn) & LV_STATE_CHECKED)
  {
    return lv_obj_get_style_text_color(_txtIconText, LV_STATE_CHECKED);
  }
  else
  {
    return lv_obj_get_style_text_color(_txtIconText, LV_IMGBTN_STATE_RELEASED);
  }
}

void classTile::setColor(lv_color_t color)
{
  if (lv_obj_get_state(_btn) & LV_STATE_CHECKED)
  {
    _reColorAll(color, LV_STATE_CHECKED);
  }
  else
  {
    _reColorAll(color, LV_IMGBTN_STATE_RELEASED);
  }
}

void classTile::setColor(int r, int g, int b)
{
  if ((r + g + b) == 0)
  {
    // if all zero reset to default color
    if (lv_obj_get_state(_btn) & LV_STATE_CHECKED)
    {
      _reColorAll(colorOn, LV_STATE_CHECKED);
    }
    else
    {
      _reColorAll(lv_color_hex(0xffffff), LV_IMGBTN_STATE_RELEASED);
    }
  }
  else
  {
    setColor(lv_color_make(r, g, b));
  }
}

void classTile::setNumber(const char *number, const char *units)
{
  lv_obj_set_style_text_font(_numLabel, &number_OR_50, 0);
  lv_label_set_text(_numLabel, number);
  lv_obj_set_style_text_font(_unitLabel, &lv_font_montserrat_20, 0);
  lv_label_set_text(_unitLabel, units);
  lv_obj_align_to(_unitLabel, _numLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, 5);
}

// update the _imBg object and hide it, will be shown with alignBgImage()
void classTile::setBgImage(lv_img_dsc_t *img)
{
  // free old image if exist
  _freeImageHeap();

  lv_obj_add_flag(_imgBg, LV_OBJ_FLAG_HIDDEN);
  lv_img_set_src(_imgBg, img);
}

// align _imgBg
void classTile::alignBgImage(int zoom, int posOffsX, int posOffsY, int angle)
{
  // return if no valid bg image exists
  if (!lv_img_get_src(_imgBg))
    return;

  if (zoom == 0)   zoom = 100;
  if (zoom > 200)  zoom = 200;
  if (zoom < 50)   zoom = 50;

  lv_obj_clear_flag(_imgBg, LV_OBJ_FLAG_HIDDEN);
  lv_img_set_zoom(_imgBg, (256 * zoom) / 100);
  lv_img_set_angle(_imgBg, angle);
  lv_obj_set_size(_imgBg, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  // aling image to tile edge if posOffsX / -Y  >= abs(100)
  lv_area_t imgArea;
  lv_obj_get_coords(_imgBg, &imgArea);

  int imgW = (imgArea.x2 - imgArea.x1) + 1;
  int imgH = (imgArea.y2 - imgArea.y1) + 1;
  imgW = (imgW * zoom) / 100;
  imgH = (imgH * zoom) / 100;
  int tileW = lv_obj_get_width(_btn);
  int tileH = lv_obj_get_height(_btn);
  if (posOffsX <= -100)    posOffsX = -(tileW / 2 - imgW / 2) - 1;    // left
  if (posOffsX >= 100)     posOffsX = +(tileW / 2 - imgW / 2) + 1;    // right
  if (posOffsY <= -100)    posOffsY = -(tileH / 2 - imgH / 2) - 1;    // bottom
  if (posOffsY >= 100)     posOffsY = +(tileH / 2 - imgH / 2) + 1;    // top

  lv_obj_align(_imgBg, LV_ALIGN_CENTER, posOffsX, posOffsY * -1);
  lv_obj_set_style_radius(_imgBg, 5, 0);
  lv_obj_set_style_clip_corner(_imgBg, true, 0);
}

// this button calls a new screen (linkScreen)
void classTile::setLink(int linkScreen)
{
  _linkedScreen = linkScreen;
  lv_label_set_text(_linkedLabel, LV_SYMBOL_RIGHT);
}

int classTile::getLink(void)
{
  return _linkedScreen;
}

void classTile::setKeyPadEnable(bool enable)
{
  _keyPadEnable = enable;
}

bool classTile::getKeyPadEnable(void)
{
  return _keyPadEnable;
}

tileId_t classTile::getId(void)
{
  return tileId;
}

int classTile::getScreenIdx(void)
{
  return _screenIdx;
}

int classTile::getTileIdx(void)
{
  return _tileIdx;
}

int classTile::getStyle(void)
{
  return _style;
}

const char *classTile::getStyleStr(void)
{
  return _styleStr.c_str();
}

bool classTile::getState(void)
{
  return _state;
}

char *classTile::getLabel(void)
{
  return lv_label_get_text(_label);
}

// set 2nd icon to alternate state dependent
void classTile::setIconForStateOn(const void *imgStateOn)
{
  _imgOn = (imgStateOn != NULL) ? imgStateOn : _img;
  lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, _imgOn, NULL, NULL);
  lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, _imgOn, NULL, NULL);
}

// set icon for current state
void classTile::setIcon(const void *imgIcon)
{
  if (lv_obj_get_state(_btn) & LV_STATE_CHECKED)
  {
    _imgOn = (imgIcon != NULL) ? imgIcon : _imgOnConfig;
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, _imgOn, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, _imgOn, NULL, NULL);
  }
  else
  {
    _img = (imgIcon != NULL) ? imgIcon : _imgConfig;
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_RELEASED, _img, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_PRESSED, _img, NULL, NULL);
  }
  lv_obj_invalidate(_btn);
}

// replaces the existing icon by selected text, reverts to icon if text is empty
void classTile::setIconText(const char *iconText)
{
  if (strlen(iconText) == 0)
  {
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_RELEASED, _img, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, _imgOn, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, _imgOn, NULL, NULL);
    lv_obj_add_flag(_txtIconText, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_RELEASED, NULL, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, NULL, NULL);

    lv_label_set_text(_txtIconText, iconText);
    lv_obj_clear_flag(_txtIconText, LV_OBJ_FLAG_HIDDEN);
  }
}

void classTile::getImages(const void *&imgOff, const void *&imgOn)
{
  imgOff = _img;
  imgOn = _imgOn;
}

void classTile::addEventHandler(lv_event_cb_t callBack)
{
  // add click detecttion
  lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_PRESSED, _img, NULL, NULL);
  lv_obj_set_style_bg_opa(_btn, WP_OPA_BG_PRESSED, LV_STATE_PRESSED);
  lv_obj_set_style_img_recolor(_btn, lv_color_hex(0x404040), LV_STATE_PRESSED);
  lv_obj_set_style_img_recolor_opa(_btn, 255, LV_STATE_PRESSED);
  // add event handler
  lv_obj_add_event_cb(_btn, callBack, LV_EVENT_ALL, this);
}

// additional methods for on-tile level control

void classTile::setLevelStartStop(int start, int stop)
{
  _levelStart = start;
  _levelStop = stop;
  _levelLargeStep = ((stop - start) + 10) / 20;
  if (_levelLargeStep == 0) _levelLargeStep++;
  _level = _levelStart;
}

void classTile::setLevel(int level, bool force)
{
  // early exit if bar visualisation is activ and not forced
  if (lv_obj_is_valid(_ovlPanel) && !force)
    return;

  _level = level;
}

int classTile::getLevel(void)
{
  return _level;
}

int classTile::getLevelStart(void)
{
  return _levelStart;
}

int classTile::getLevelStop(void)
{
  return _levelStop;
}

int classTile::getLevelLargeStep(void)
{
  return _levelLargeStep;
}

void classTile::setTopDownMode(bool enable)
{
  _topDownMode = enable;
}

void classTile::addUpDownControl(lv_event_cb_t upDownEventHandler, const void *imgUpperButton, const void *imgLowerButton)
{
  // set button and label size from grid
  int width = (*lv_obj_get_style_grid_column_dsc_array(_parent, 0) - 10) / 2 + 1;
  int height = (*lv_obj_get_style_grid_row_dsc_array(_parent, 0) - 10) / 2 + 1;

  // up / down  buttons
  _btnUp = lv_btn_create(_btn);
  lv_obj_set_size(_btnUp, width, height);
  lv_obj_align(_btnUp, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_bg_img_src(_btnUp, imgUpperButton, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_btnUp, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnUp, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnUp, 0, LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_set_style_bg_img_recolor(_btnUp, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_set_style_bg_img_recolor_opa(_btnUp, 255, LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_set_style_bg_img_recolor_opa(_btnUp, 100, LV_PART_MAIN | LV_STATE_PRESSED);

  _btnDown = lv_btn_create(_btn);
  lv_obj_set_size(_btnDown, width, height);
  lv_obj_align(_btnDown, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_set_style_bg_img_src(_btnDown, imgLowerButton, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_btnDown, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnDown, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnDown, 0, LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_set_style_bg_img_recolor(_btnDown, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_set_style_bg_img_recolor_opa(_btnDown, 255, LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_set_style_bg_img_recolor_opa(_btnDown, 100, LV_PART_MAIN | LV_STATE_PRESSED);

  // select which button increments (USER_FLAG set = increment)
  if (!_topDownMode)
    lv_obj_add_flag(_btnUp, LV_OBJ_FLAG_USER_1);
  else
    lv_obj_add_flag(_btnDown, LV_OBJ_FLAG_USER_1);
  // add event handler
  lv_obj_add_event_cb(_btnUp, upDownEventHandler, LV_EVENT_ALL, this);
  lv_obj_add_event_cb(_btnDown, upDownEventHandler, LV_EVENT_ALL, this);

  // reduced width for main label
  lv_obj_set_size(_label, 80, LV_SIZE_CONTENT);
}

void classTile::showOvlBar(int level)
{
  if (lv_obj_is_valid(_ovlPanel))
  {
    lv_obj_del(_ovlPanel);
  }

  _ovlPanel = lv_obj_create(_btn);
  lv_obj_clear_flag(_ovlPanel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_border_width(_ovlPanel, 0, LV_PART_MAIN);
  lv_obj_set_size(_ovlPanel, 60, 70);
  lv_obj_align(_ovlPanel, LV_ALIGN_TOP_LEFT, 2, 2);
  lv_obj_set_style_bg_color(_ovlPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel, 255, LV_PART_MAIN | LV_STATE_CHECKED);

  _bar = lv_bar_create(_ovlPanel);
  if (lv_obj_get_state(_btn) & LV_STATE_CHECKED)
    lv_obj_add_state(_bar, LV_STATE_CHECKED);
  lv_bar_set_range(_bar, _levelStart, _levelStop);
  lv_obj_set_size(_bar, 10, 60);
  lv_obj_align(_bar, LV_ALIGN_CENTER, 10, 0);

  lv_obj_set_style_radius(_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(_bar, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // _barLabel
  lv_obj_t *_barLabel = lv_label_create(_ovlPanel);
  lv_obj_set_size(_barLabel, 40, LV_SIZE_CONTENT);
  lv_obj_align(_barLabel, LV_ALIGN_TOP_MID, -20, 0);
  lv_obj_set_style_text_align(_barLabel, LV_TEXT_ALIGN_RIGHT, 0);
  lv_obj_set_style_text_color(_barLabel, lv_color_hex(0x000000), 0);
  lv_label_set_text_fmt(_barLabel, "%d", level);

  // set mode to bottom-up(default)  or top-down
  if (!_topDownMode)
  {
    lv_obj_set_style_bg_color(_bar, lv_color_lighten(colorBg, 200), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_bar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(_bar, lv_color_lighten(colorBg, 50), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_bar, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(_bar, lv_color_lighten(colorOn, 150), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(_bar, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    lv_obj_set_style_bg_color(_bar, lv_color_lighten(colorOn, 0), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(_bar, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);

    lv_obj_set_y(_barLabel, 38 - (50 * 0 / 100));
    lv_bar_set_value(_bar, level, LV_ANIM_OFF);
  }
  else
  {
    lv_obj_set_style_bg_color(_bar, lv_color_lighten(colorBg, 50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_bar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(_bar, lv_color_lighten(colorBg, 200), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_bar, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(_bar, lv_color_lighten(colorOn, 0), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(_bar, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    lv_obj_set_style_bg_color(_bar, lv_color_lighten(colorOn, 150), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(_bar, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);

    lv_obj_set_y(_barLabel, -10);
    lv_bar_set_value(_bar, _levelStop - level, LV_ANIM_OFF);
  }

  lv_obj_del_delayed(_ovlPanel, 2000);
}

// additional methods for drop down (interface)

// store drop down list
void classTile::setDropDownList(const char *list)
{
  lv_label_set_text(_dropDownList, list);
  _setIconTextFromIndex();
}

// store selected item index
void classTile::setDropDownIndex(uint16_t index)
{
  _dropDownIndex = index;
  _setIconTextFromIndex();
}

// store drop down label
void classTile::setDropDownLabel(const char *label)
{
  lv_label_set_text(_dropDownLabel, label);
}

// get the stored list
const char *classTile::getDropDownList(void)
{
  return lv_label_get_text(_dropDownList);
}

// get the stored index
uint16_t classTile::getDropDownIndex(void)
{
  return _dropDownIndex;
}

// get the stored label (NULL if "")
const char *classTile::getDropDownLabel(void)
{
  if (strlen(lv_label_get_text(_dropDownLabel)) == 0)
    return NULL;
  else
    return lv_label_get_text(_dropDownLabel);
}

void classTile::setDropDownIndicator(void)
{
  lv_label_set_text(_linkedLabel, LV_SYMBOL_DOWN);
}

// methods for selector function
void classTile::showSelector(int index)
{
  if (lv_obj_is_valid(_roller))
  {
    lv_obj_del(_roller);
  }

  _roller = lv_roller_create(_btn);
  lv_obj_set_style_border_width(_roller, 0, LV_PART_MAIN);
  lv_roller_set_options(_roller, _selectorList.c_str(), LV_ROLLER_MODE_NORMAL);
  lv_roller_set_visible_row_count(_roller, 3);

  lv_obj_set_size(_roller, 70, 70);
  lv_obj_set_style_text_line_space(_roller, 8, LV_PART_MAIN);
  lv_obj_set_style_bg_color(_roller, lv_color_lighten(colorBg, 50), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(_roller, 255, LV_PART_MAIN);

  lv_obj_set_style_bg_color(_roller, colorOn, LV_PART_SELECTED);
  lv_obj_set_style_radius(_roller, 5, LV_PART_MAIN);
  lv_obj_align(_roller, LV_ALIGN_TOP_LEFT, 2, 2);
  lv_obj_clear_flag(_roller, LV_OBJ_FLAG_CLICKABLE);

  if (--index < 0) index = 0;
  lv_roller_set_selected(_roller, index, LV_ANIM_OFF);
  _selectorIndex = lv_roller_get_selected(_roller) + 1;

  lv_obj_del_delayed(_roller, 2000);
}

// set the selector list
void classTile::setSelectorList(const char *list)
{
  _selectorList = string(list);
}

// set index to selector list
void classTile::setSelectorIndex(int index)
{
  _selectorIndex = index;
}

// set index to selector list
int classTile::getSelectorIndex(void)
{
  return _selectorIndex;
}

// check if valid list exist
bool classTile::getSelectorValid(void)
{
  return (_selectorList.size() > 0);
}

// additional methods for color picker (interface)

void classTile::setColorPickerRGB(lv_color32_t rgb)
{
  _colorPickerRGB32 = rgb;
}

void classTile::setColorPickerRGB(int r, int g, int b)
{
  _colorPickerRGB32.ch.red = r;
  _colorPickerRGB32.ch.green = g;
  _colorPickerRGB32.ch.blue = b;
}

void classTile::setColorPickerKelvin(int kelvin)
{
  _colorPickerKelvin = kelvin;
}

void classTile::setColorPickerBrightnessWhite(int brightness)
{
  _colorPickerBrightnessWhite = brightness;
}

void classTile::setColorPickerMode(int mode)
{
  _colorPickerMode = mode;
}

lv_color32_t classTile::getColorPickerRGB(void)
{
  return _colorPickerRGB32;
}

int classTile::getColorPickerKelvin(void)
{
  return _colorPickerKelvin;
}

int classTile::getColorPickerBrightnessWhite(void)
{
  return _colorPickerBrightnessWhite;
}

int classTile::getColorPickerMode(void)
{
  return _colorPickerMode;
}

#include <classTile.h>
#include <globalDefines.h>

extern lv_color_t colorOn;
extern "C" const lv_font_t number_OR_50;
extern "C" const lv_font_t wp_symbol_font_15;

// create the tile
void classTile::_button(lv_obj_t *parent, const void *img)
{
  _parentContainer = parent;
  _tileBgColor = _parentScreen->getBgColor();
  // create a background layer to opional configle tile backgroung
  _tileBg = lv_obj_create(_parentContainer);
  lv_obj_remove_style_all(_tileBg);
  lv_obj_set_style_radius(_tileBg, 5, LV_PART_MAIN);
  lv_obj_set_style_bg_color(_tileBg, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(_tileBg, 0, LV_PART_MAIN);

  // create the image button as root parent for all content
  _btn = lv_imgbtn_create(_tileBg);
  // placeholder for full image
  _imgBg = lv_img_create(_btn);

  // check the icon image for special handling requirement
  if (img)
  {
    _img = img;
    _imgOn = img;
    _imgConfig = img;
    _imgOnConfig = img;
    // create an arc widget as dynamic thumbnail icon
    if (_isThumbNail(img))
    {
      _arcTarget = lv_arc_create(_btn);
      lv_obj_set_size(_arcTarget, 110, 110);
      lv_obj_set_align(_arcTarget, LV_ALIGN_TOP_LEFT);
      lv_arc_set_bg_angles(_arcTarget, 150, 30);
      lv_arc_set_range(_arcTarget, 100 / ARC_STEP, 300 / ARC_STEP);
      lv_arc_set_value(_arcTarget, 0);

      lv_obj_set_style_pad_all(_arcTarget, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_arc_color(_arcTarget, lv_color_hex(0xC8C8C8), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_arc_opa(_arcTarget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_arc_width(_arcTarget, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_arc_color(_arcTarget, lv_color_hex(0x707070), LV_PART_MAIN | LV_STATE_CHECKED);

      lv_obj_set_style_arc_color(_arcTarget, lv_color_hex(0x0000FF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
      lv_obj_set_style_arc_opa(_arcTarget, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
      lv_obj_set_style_arc_width(_arcTarget, 4, LV_PART_INDICATOR | LV_STATE_DEFAULT);

      lv_obj_set_style_bg_color(_arcTarget, lv_color_lighten(_tileBgColor, WP_OPA_BG_OFF), LV_PART_KNOB | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(_arcTarget, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
      lv_obj_set_style_border_color(_arcTarget, lv_color_hex(0xC8C8C8), LV_PART_KNOB | LV_STATE_DEFAULT);
      lv_obj_set_style_border_opa(_arcTarget, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
      lv_obj_set_style_border_width(_arcTarget, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
      lv_obj_set_style_border_side(_arcTarget, LV_BORDER_SIDE_FULL, LV_PART_KNOB | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_all(_arcTarget, 4, LV_PART_KNOB | LV_STATE_DEFAULT);
      lv_obj_set_style_border_color(_arcTarget, lv_color_hex(0x707070), LV_PART_KNOB | LV_STATE_CHECKED);
      lv_obj_set_style_bg_color(_arcTarget, colorOn, LV_PART_KNOB | LV_STATE_CHECKED);

      // label for target
      _labelArcValue = lv_label_create(_arcTarget);
      lv_obj_set_style_text_font(_labelArcValue, &lv_font_montserrat_20, 0);
      lv_label_set_text(_labelArcValue, "");
      lv_obj_align(_labelArcValue, LV_ALIGN_CENTER, 0, -5);
      lv_obj_set_style_text_color(_labelArcValue, colorOn, LV_STATE_CHECKED);

      // subTabel for current
      _labelArcSubValue = lv_label_create(_arcTarget);
      lv_label_set_text(_labelArcSubValue, "");
      lv_obj_align(_labelArcSubValue, LV_ALIGN_CENTER, 0, 20);
      lv_obj_set_style_text_color(_labelArcSubValue, lv_color_hex(0x808080), LV_STATE_DEFAULT);

      lv_obj_clear_flag(_arcTarget, LV_OBJ_FLAG_CLICKABLE);
    }
  }
  
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

  // Sub Label (placeholder)
  _subLabel = lv_label_create(_btn);
  lv_obj_set_size(_label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_subLabel, "");
  lv_obj_align(_subLabel, LV_ALIGN_BOTTOM_LEFT, 8, -5);
  lv_obj_set_style_text_color(_subLabel, lv_color_hex(0x808080), 0);

  // set tile bg, button and label size from grid
  int width = _tileWidth();
  int height = _tileHeight();

  lv_obj_set_size(_tileBg, width, height);
  lv_obj_set_size(_btn, width, height);

  lv_obj_set_size(_label, width - 10, LV_SIZE_CONTENT);
  lv_obj_set_size(_subLabel, width - 10, LV_SIZE_CONTENT);

  btn = _btn;
}

// tile width from grid
int classTile::_tileWidth()
{
  return *lv_obj_get_style_grid_column_dsc_array(_parentContainer, 0) - (TILE_PADDING * 2);
}

// tile width from grid
int classTile::_tileHeight()
{
  return *lv_obj_get_style_grid_row_dsc_array(_parentContainer, 0) - (TILE_PADDING * 2);
}

void classTile::_createValueLabels()
{
  lv_color_t colorChecked = lv_obj_get_style_img_recolor(_btn, LV_STATE_CHECKED);
  lv_color_t colorReleased = lv_obj_get_style_img_recolor(_btn, LV_IMGBTN_STATE_RELEASED);

  // additional Label (placeholder)
  _valueLabel = lv_label_create(_btn);
  lv_obj_set_size(_valueLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_font(_valueLabel, &number_OR_50, 0);
  lv_label_set_text(_valueLabel, "");
  lv_obj_align(_valueLabel, LV_ALIGN_TOP_LEFT, 8, 8);
  lv_obj_set_style_text_color(_valueLabel, colorChecked, LV_STATE_CHECKED);
  lv_obj_set_style_text_color(_valueLabel, colorReleased, LV_IMGBTN_STATE_RELEASED);

  // additional Label (unit display)
  _unitLabel = lv_label_create(_btn);
  lv_obj_set_size(_unitLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_font(_unitLabel, &lv_font_montserrat_20, 0);
  lv_label_set_text(_unitLabel, "");
  lv_obj_set_style_text_color(_unitLabel, colorChecked, LV_STATE_CHECKED);
  lv_obj_set_style_text_color(_unitLabel, colorReleased, LV_IMGBTN_STATE_RELEASED);

  // additional subLabel to value label
  _subValueLabel = lv_label_create(_btn);
  lv_obj_set_size(_subValueLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_font(_subValueLabel, &lv_font_montserrat_20, 0);
  lv_label_set_text(_subValueLabel, "");
  lv_obj_set_style_text_color(_subValueLabel, lv_color_hex(0x808080), LV_STATE_DEFAULT);
}

void classTile::_createLinkedLabel()
{
  // additional Label (show sybol ">", ":", "v"
  _linkedLabel = lv_label_create(_btn);
  lv_obj_set_size(_linkedLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_font(_linkedLabel, &wp_symbol_font_15, 0);
  lv_label_set_text(_linkedLabel, "");
  lv_obj_align(_linkedLabel, LV_ALIGN_TOP_RIGHT, -8, 5);
  lv_obj_set_style_text_color(_btn, lv_color_hex(0x000000), LV_STATE_CHECKED);
}

void classTile::_createIconText()
{
  lv_color_t colorChecked = lv_obj_get_style_img_recolor(_btn, LV_STATE_CHECKED);
  lv_color_t colorReleased = lv_obj_get_style_img_recolor(_btn, LV_IMGBTN_STATE_RELEASED);

  // label for text to replace icon  (hide by default)
  _txtIconText = lv_label_create(_btn);
  lv_obj_align(_txtIconText, LV_ALIGN_TOP_LEFT, 8, 4);
  lv_obj_set_style_text_font(_txtIconText, &lv_font_montserrat_20, 0);
  lv_label_set_text(_txtIconText, "");
  lv_label_set_recolor(_txtIconText, true);
  lv_obj_add_flag(_txtIconText, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_style_text_color(_txtIconText, colorChecked, LV_STATE_CHECKED);
  lv_obj_set_style_text_color(_txtIconText, colorReleased, LV_IMGBTN_STATE_RELEASED);

  // set size from grid
  lv_obj_set_size(_txtIconText, _tileWidth() - 20, _tileHeight() - 4);
}

  void classTile::_setIconTextFromIndex()
{
  if (_dropDownList.length() > 0)
  {
    int index = _dropDownIndex;
    if (index > 0)
      index--;
    lv_obj_t *dd = lv_dropdown_create(_btn);
    lv_dropdown_set_options(dd, _dropDownList.c_str());
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
  if (_txtIconText)
    lv_obj_set_style_text_color(_txtIconText, color, selector);
  if (_valueLabel)
  {
    lv_obj_set_style_text_color(_valueLabel, color, selector);
    lv_obj_set_style_text_color(_unitLabel, color, selector);
  }
  if (_arcTarget)
  {
    lv_obj_set_style_text_color(_labelArcValue, color, selector);
    lv_obj_set_style_bg_color(_arcTarget, color, LV_PART_KNOB | selector);
  }
}

// hide/unhide icon if to be replaced by text or number
void classTile::_hideIcon(bool hide)
{
  if (hide)
  {
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_RELEASED, NULL, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_PRESSED, NULL, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, NULL, NULL);
    _hideThumbNail(true);
  }
  else
  {
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_RELEASED, _img, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_PRESSED, _img, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, _imgOn, NULL, NULL);
    lv_imgbtn_set_src(_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, _imgOn, NULL, NULL);
    if (_isThumbNail(_state ? _imgOn : _img))
      _hideThumbNail(false);
  }
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

bool classTile::_isThumbNail(const void *img)
{
  if (img)
  {
    const lv_img_dsc_t *tImg = (lv_img_dsc_t *)img;
    if (tImg->data_size < 10)
      return true;
  }
  return false;
}

void classTile::_hideThumbNail(bool hide)
{
  if (_arcTarget)
    hide ? lv_obj_add_flag(_arcTarget, LV_OBJ_FLAG_HIDDEN) : lv_obj_clear_flag(_arcTarget, LV_OBJ_FLAG_HIDDEN);
}

classTile::~classTile()
{
  if (_tileBg)
  {
    _freeImageHeap();
    lv_obj_del(_tileBg);
  }
}

// initialise existing object
void classTile::begin(lv_obj_t * parent, classScreen * parentScreen, int tileIdx, const void *img, const char *labelText, int style, const char *styleStr)
{
  _parentScreen = parentScreen;
  tileId.idx.screen = _parentScreen->getScreenNumber();
  tileId.idx.tile = tileIdx;
  _style = style;
  _styleStr = styleStr;

  _button(parent, img);
  lv_label_set_text(_label, labelText);

  // position tile in grid after tile and screen are known
  int col = (tileIdx - 1) % SCREEN_COLS;
  int row = (tileIdx - 1) / SCREEN_COLS;
  lv_obj_set_grid_cell(_tileBg, LV_GRID_ALIGN_CENTER, col, 1, LV_GRID_ALIGN_CENTER, row, 1);
}

// cover tile with grey semi transparent layer to put into disabled state
void classTile::setTileDisabled(bool disable)
{
  if (disable)
  {
    if (!_tileFg)
    {
      int row = (tileId.idx.tile - 1) / 2;
      int col = (tileId.idx.tile - 1) % 2;

      _tileFg = lv_obj_create(_btn);
      lv_obj_remove_style_all(_tileFg);
      lv_obj_set_size(_tileFg, lv_obj_get_width(_btn), lv_obj_get_height(_btn));
      lv_obj_set_style_radius(_tileFg, 5, LV_PART_MAIN);
      lv_obj_set_grid_cell(_tileFg, LV_GRID_ALIGN_CENTER, col, 1, LV_GRID_ALIGN_CENTER, row, 1);
      lv_obj_set_style_bg_color(_tileFg, lv_color_hex(0x606060), LV_PART_MAIN);
      lv_obj_set_style_bg_opa(_tileFg, 160, LV_PART_MAIN);
      lv_obj_move_foreground(_tileFg);
    }
  }
  else
  {
    if (_tileFg)
    {
      lv_obj_del(_tileFg);
      _tileFg = NULL;
    }
  }
}

void classTile::setSubLabel(const char *subLabelText)
{
  lv_label_set_text(_subLabel, subLabelText);
}

void classTile::setState(bool state)
{
  if (_isThumbNail(_state ? _imgOn : _img))
    _hideThumbNail(true);

  _state = state;
  state == false ? lv_obj_clear_state(_btn, LV_STATE_CHECKED) : lv_obj_add_state(_btn, LV_STATE_CHECKED);
  if (_txtIconText)
    state == false ? lv_obj_clear_state(_txtIconText, LV_STATE_CHECKED) : lv_obj_add_state(_txtIconText, LV_STATE_CHECKED);
  if (_valueLabel)
  {
    state == false ? lv_obj_clear_state(_valueLabel, LV_STATE_CHECKED) : lv_obj_add_state(_valueLabel, LV_STATE_CHECKED);
    state == false ? lv_obj_clear_state(_unitLabel, LV_STATE_CHECKED) : lv_obj_add_state(_unitLabel, LV_STATE_CHECKED);
  }
  if (_btnUp)
    state == false ? lv_obj_clear_state(_btnUp, LV_STATE_CHECKED) : lv_obj_add_state(_btnUp, LV_STATE_CHECKED);
  if (_btnDown)
    state == false ? lv_obj_clear_state(_btnDown, LV_STATE_CHECKED) : lv_obj_add_state(_btnDown, LV_STATE_CHECKED);
  if (_arcTarget)
    state == false ? lv_obj_clear_state(_arcTarget, LV_STATE_CHECKED) : lv_obj_add_state(_arcTarget, LV_STATE_CHECKED);
  if (_labelArcValue)
    state == false ? lv_obj_clear_state(_labelArcValue, LV_STATE_CHECKED) : lv_obj_add_state(_labelArcValue, LV_STATE_CHECKED);

  if (_isThumbNail(state ? _imgOn : _img))
  {
    if (lv_imgbtn_get_src_left(_btn, _state ? LV_IMGBTN_STATE_CHECKED_RELEASED : LV_IMGBTN_STATE_RELEASED))
      _hideThumbNail(false);
}
}

lv_color_t classTile::getColor()
{
  if (lv_obj_get_state(_btn) & LV_STATE_CHECKED)
  {
    return lv_obj_get_style_img_recolor(_btn, LV_STATE_CHECKED);
  }
  else
  {
    return lv_obj_get_style_img_recolor(_btn, LV_IMGBTN_STATE_RELEASED);
  }
}

void classTile::setColor(lv_color_t color)
{
    // if all zero reset to default color
  if (color.full == 0)
  {
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
    if (lv_obj_get_state(_btn) & LV_STATE_CHECKED)
    {
      _reColorAll(color, LV_STATE_CHECKED);
    }
    else
    {
      _reColorAll(color, LV_IMGBTN_STATE_RELEASED);
    }
  }
}

void classTile::setBgColor(lv_color_t color)
{
  if (color.full == 0)
  {
    // if all zero reset to default background
    lv_obj_clear_flag(_tileBg, TP_COLOR_BG_OVERWRITE);
    _tileBgColor = _parentScreen->getBgColor();
  }
  else
  {
    lv_obj_add_flag(_tileBg, TP_COLOR_BG_OVERWRITE);
    _tileBgColor = color;
  }
  updateBgColor();
}

void classTile::updateBgColor(void)
{
  if (lv_obj_has_flag(_tileBg, TP_COLOR_BG_OVERWRITE))
  {
    lv_obj_set_style_bg_color(_tileBg, _tileBgColor, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(_tileBg, 255, LV_PART_MAIN);
  }
  else
  {
    _tileBgColor = _parentScreen->getBgColor();
    lv_obj_set_style_bg_color(_tileBg, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(_tileBg, 0, LV_PART_MAIN);
  }
  if (_arcTarget)
    lv_obj_set_style_bg_color(_arcTarget, lv_color_lighten(_tileBgColor, WP_OPA_BG_OFF), LV_PART_KNOB | LV_STATE_DEFAULT);
}

void classTile::setNumber(const char *value, const char *units, const char *subValue, const char *subUnits)
{
  // update number display
  _hideIcon(value || units || subValue || subUnits);

  if (!_valueLabel)
    _createValueLabels();

  lv_label_set_text(_valueLabel, !value ? "" : value);
  lv_label_set_text(_unitLabel, !units ? "" : units);
  lv_obj_align_to(_unitLabel, _valueLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, 5);

  lv_label_set_text_fmt(_subValueLabel, "%s %s", !subValue ? "" : subValue, !subUnits ? "" : subUnits);
  lv_obj_align_to(_subValueLabel, _valueLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
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
void classTile::alignBgImage(int zoom, int offsetX, int offsetY, int angle)
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
  if (offsetX <= -100)    offsetX = -(tileW / 2 - imgW / 2) - 1;    // left
  if (offsetX >= 100)     offsetX = +(tileW / 2 - imgW / 2) + 1;    // right
  if (offsetY <= -100)    offsetY = -(tileH / 2 - imgH / 2) - 1;    // bottom
  if (offsetY >= 100)     offsetY = +(tileH / 2 - imgH / 2) + 1;    // top

  lv_obj_align(_imgBg, LV_ALIGN_CENTER, offsetX, offsetY * -1);
  lv_obj_set_style_radius(_imgBg, 5, 0);
  lv_obj_set_style_clip_corner(_imgBg, true, 0);
}

// this button calls a new screen (linkScreen)
void classTile::setLink(int linkScreen)
{
  _linkedScreen = linkScreen;
}

int classTile::getLink(void)
{
  return _linkedScreen;
}

tileId_t classTile::getId(void)
{
  return tileId;
}

int classTile::getScreenIdx(void)
{
  return tileId.idx.screen;
}

int classTile::getTileIdx(void)
{
  return tileId.idx.tile;
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

  _hideThumbNail(_isThumbNail(imgIcon) ? false : true);
  lv_obj_invalidate(_btn);
}

// replaces the existing icon by selected text, reverts to icon if text is empty
void classTile::setIconText(const char *iconText)
{
  if (!_txtIconText)
    _createIconText();

  if (strlen(iconText) == 0)
  {
    _hideIcon(false);
    lv_obj_add_flag(_txtIconText, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    _hideIcon(true);
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

  if (_arcTarget)
  {
    if ((_levelStart != 0) || (_levelStop != 100))
    {
      lv_arc_set_range(_arcTarget, _levelStart / ARC_STEP, _levelStop / ARC_STEP);
      lv_arc_set_value(_arcTarget, _levelStart / ARC_STEP);
    }
  }
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
  int width = _tileWidth() / 2 + 1;
  int height = _tileHeight() / 2 + 1;

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
    lv_obj_set_style_bg_color(_bar, lv_color_lighten(_tileBgColor, 200), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_bar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(_bar, lv_color_lighten(_tileBgColor, 50), LV_PART_INDICATOR | LV_STATE_DEFAULT);
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
    lv_obj_set_style_bg_color(_bar, lv_color_lighten(_tileBgColor, 50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_bar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(_bar, lv_color_lighten(_tileBgColor, 200), LV_PART_INDICATOR | LV_STATE_DEFAULT);
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
void classTile::setDropDownList(string list)
{
  _dropDownList = list;
  _setIconTextFromIndex();
}

// store selected item index and show selected item as iconText
void classTile::setDropDownIndex(uint16_t index)
{
  _dropDownIndex = index;
  _setIconTextFromIndex();
}

// store drop down list for mode select with no further action
void classTile::saveDropDownList(string list)
{
  _dropDownList = list;
}

// store selected item index with no further action
void classTile::saveDropDownIndex(uint16_t index)
{
  _dropDownIndex = index;
}

// store drop down label
void classTile::setDropDownLabel(const char *label)
{
  _dropDownLabel = label;
}

// get the stored list
const char *classTile::getDropDownList(void)
{
  return _dropDownList.c_str();
}

// get the stored index
uint16_t classTile::getDropDownIndex(void)
{
  return _dropDownIndex;
}

// get the stored label (NULL if "")
const char *classTile::getDropDownLabel(void)
{
  return (_dropDownLabel.length() == 0) ? NULL : _dropDownLabel.c_str();
}

void classTile::setActionIndicator(const char *symbol)
{
  if (!_linkedLabel)
    _createLinkedLabel(); 

  lv_label_set_text(_linkedLabel, symbol);
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
  lv_roller_set_options(_roller, _dropDownList.c_str(), LV_ROLLER_MODE_NORMAL);
  lv_roller_set_visible_row_count(_roller, 3);

  lv_obj_set_size(_roller, 70, 70);
  lv_obj_set_style_text_line_space(_roller, 8, LV_PART_MAIN);
  lv_obj_set_style_bg_color(_roller, lv_color_lighten(_tileBgColor, 50), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(_roller, 255, LV_PART_MAIN);

  lv_obj_set_style_bg_color(_roller, colorOn, LV_PART_SELECTED);
  lv_obj_set_style_radius(_roller, 5, LV_PART_MAIN);
  lv_obj_align(_roller, LV_ALIGN_TOP_LEFT, 2, 2);
  lv_obj_clear_flag(_roller, LV_OBJ_FLAG_CLICKABLE);

  if (--index < 0) index = 0;
  lv_roller_set_selected(_roller, index, LV_ANIM_OFF);
  _dropDownIndex = lv_roller_get_selected(_roller) + 1;

  lv_obj_del_delayed(_roller, 2000);
}

// check if valid list exist
bool classTile::getSelectorValid(void)
{
  return (_dropDownList.size() > 0);
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

void classTile::setThermostatTarget(int target)
{
  _thermostatTarget = target;
  updateThermostatDisplay();
}

void classTile::setThermostatCurrent(int current)
{
  _thermostatCurrent = current;
  updateThermostatDisplay();
}

void classTile::setThermostatUnits(const char *units)
{
  _units = !units ? "" : units;
  updateThermostatDisplay();
}

int classTile::getThermostatTarget(void)
{
  return _thermostatTarget;
}

int classTile::getThermostatCurrent(void)
{
  return _thermostatCurrent;
}

const char *classTile::getThermostatUnits(void)
{
  return _units.c_str();
}

void classTile::updateThermostatDisplay(void)
{
  if (_arcTarget)
  {
    int target = _thermostatTarget / ARC_STEP;
    lv_arc_set_value(_arcTarget, target);

    // calc hsv for arc coloring
    int range = (lv_arc_get_max_value(_arcTarget) - lv_arc_get_min_value(_arcTarget)) / 2;
    int mid = range + lv_arc_get_min_value(_arcTarget);
    int s = 50 + (abs(mid - target) * 50) / range;
    // red(360) or blue(240)
    int h = (target < mid) ? 240 : 360;
    lv_obj_set_style_arc_color(_arcTarget, lv_color_hsv_to_rgb(h, s, 100), LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // update the target/current display values
    char buffer[10];
    sprintf(buffer, "%2.1f %s", (float)_thermostatTarget / 10.0, _units.c_str());
    lv_label_set_text(_labelArcValue, buffer);
    sprintf(buffer, "%2.1f %s", (float)_thermostatCurrent / 10.0, _units.c_str());
    lv_label_set_text(_labelArcSubValue, buffer);
  }
}

// add post to feed (max feed entries = 5)
void classTile::addPost(int id, const char *head, const char *body)
{
  if (head || body)
  {
    if (_feed.size() >= 5)
      _feed.pop_back();
    _feed.emplace_front(post(id, head ? (string)head : "", body ? (string)body : ""));
  }
}

// remove post(s) from feed
void classTile::removePost(int id)
{
  if (id == 0)
    _feed.clear();
  else
    _feed.remove_if([&](post const &p)
                    { return p.id == id; });
}

std::list<post>::iterator classTile::getFeedIterator(void)
{
  _feedIterator = _feed.begin();
  return _feedIterator;
}

int classTile::getFeedSize(void)
{
  return _feed.size();
}

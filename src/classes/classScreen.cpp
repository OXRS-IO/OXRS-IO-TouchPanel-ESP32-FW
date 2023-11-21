#include <classScreen.h>

extern lv_color_t colorOn;
extern lv_color_t colorBg;

// grid definitions
static lv_coord_t colDsc[SCREEN_COLS + 1];
static lv_coord_t rowDsc[SCREEN_ROWS + 1];

void classScreen::_makeScreenLayout()
{
  int colSize = (lv_obj_get_width(screen) / _screenCols);
  int rowSize = ((lv_obj_get_height(screen) - SCREEN_FOOTER_HEIGHT) / _screenRows);

  for (int c = 0; c < _screenCols; c++)
  {
    colDsc[c] = colSize;
  }
  colDsc[_screenCols] = LV_GRID_TEMPLATE_LAST;

  for (int r = 0; r < _screenRows; r++)
  {
    rowDsc[r] = rowSize;
  }
  rowDsc[_screenRows] = LV_GRID_TEMPLATE_LAST;
}

void classScreen::begin(int number, int style, int cols, int rows)
{
  screenIdx = number;

  _screenCols = cols;
  _screenRows = rows;

  screen = lv_obj_create(NULL);
  lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen, colorBg, LV_PART_MAIN);
  lv_obj_clear_flag(screen, TP_COLOR_BG_OVERWRITE);

  if (style == 1)
  {
    //*Create a container with grid
    lv_obj_t *cont = lv_obj_create(screen);

    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, lv_obj_get_width(screen), lv_obj_get_height(screen));
    lv_obj_set_align(cont, LV_ALIGN_TOP_MID);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);
    lv_obj_set_style_pad_top(cont, 0, 0);
    lv_obj_set_style_pad_left(cont, 0, 0);

    _makeScreenLayout();
    lv_obj_set_grid_dsc_array(cont, colDsc, rowDsc);

    container = cont;
  }

  _labelFooter = lv_label_create(screen);
  lv_obj_align(_labelFooter, LV_ALIGN_BOTTOM_MID, 0, -5);
  lv_obj_set_style_text_font(_labelFooter, &lv_font_montserrat_20, 0);
  lv_label_set_text_fmt(_labelFooter, "Screen %d", screenIdx);
  lv_label_set_recolor(_labelFooter, true);

  _labelWarning = lv_label_create(screen);
  lv_obj_align(_labelWarning, LV_ALIGN_BOTTOM_RIGHT, -45, -5);
  lv_label_set_text(_labelWarning, "");

  // placeholder for footer modifier
  _labelLeft = lv_label_create(screen);
  lv_obj_align(_labelLeft, LV_ALIGN_BOTTOM_LEFT, 10, -5);
  lv_obj_set_style_text_font(_labelLeft, &lv_font_montserrat_20, 0);
  lv_label_set_text(_labelLeft, "");
  lv_label_set_recolor(_labelLeft, true);
  lv_obj_add_flag(_labelLeft, LV_OBJ_FLAG_HIDDEN);

  _labelCenter = lv_label_create(screen);
  lv_obj_align(_labelCenter, LV_ALIGN_BOTTOM_MID, 0, -5);
  lv_obj_set_style_text_font(_labelCenter, &lv_font_montserrat_20, 0);
  lv_label_set_text(_labelCenter, "");
  lv_label_set_recolor(_labelCenter, true);
  lv_obj_add_flag(_labelCenter, LV_OBJ_FLAG_HIDDEN);

  _labelRight = lv_label_create(screen);
  lv_obj_align(_labelRight, LV_ALIGN_BOTTOM_RIGHT, -10, -5);
  lv_obj_set_style_text_font(_labelRight, &lv_font_montserrat_20, 0);
  lv_label_set_text(_labelRight, "");
  lv_label_set_recolor(_labelRight, true);
  lv_obj_add_flag(_labelRight, LV_OBJ_FLAG_HIDDEN);
}

classScreen::~classScreen()
{
  if (screen)
  {
    lv_obj_del(screen);
  }
}

int classScreen::getScreenNumber(void)
{
  return screenIdx;
}

int classScreen::getScreenCols(void)
{
  return _screenCols;
}

int classScreen::getScreenRows(void)
{
  return _screenRows;
}

void classScreen::setHidden(bool hidden)
{
  _hidden = hidden;
}

bool classScreen::isHidden(void)
{
  return _hidden;
}

void classScreen::setLabel(const char *labelText)
{
  lv_label_set_text(_labelFooter, labelText);
}

const char *classScreen::getLabel(void)
{
  return lv_label_get_text(_labelFooter);
}

void classScreen::setFooter(const char *left, const char *center, const char *right)
{
  if (!left)
  {
    lv_obj_add_flag(_labelLeft, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    lv_label_set_text(_labelLeft, left);
    lv_obj_clear_flag(_labelLeft, LV_OBJ_FLAG_HIDDEN);
  }

  if (!center)
  {
    lv_obj_add_flag(_labelCenter, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(_labelFooter, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    lv_label_set_text(_labelCenter, center);
    lv_obj_clear_flag(_labelCenter, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(_labelFooter, LV_OBJ_FLAG_HIDDEN);
  }

  // allow right footer updates only if no warning active
  if (strlen(lv_label_get_text(_labelWarning)) == 0)
  {
    if (!right)
    {
      lv_obj_add_flag(_labelRight, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(_labelRight, LV_OBJ_FLAG_USER_1);
    }
    else
    {
      lv_label_set_text(_labelRight, right);
      lv_obj_clear_flag(_labelRight, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(_labelRight, LV_OBJ_FLAG_USER_1);
    }
  }
}

void classScreen::setBgColor(lv_color_t color)
{
  if (color.full == 0)
  {
    // if all zero reset to default background
    lv_obj_clear_flag(screen, TP_COLOR_BG_OVERWRITE);
    _screenBgColor = colorBg;
  }
  else
  {
    lv_obj_add_flag(screen, TP_COLOR_BG_OVERWRITE);
    _screenBgColor = color;
  }
  updateBgColor();
}

void classScreen::updateBgColor(void)
{
  if (!lv_obj_has_flag(screen, TP_COLOR_BG_OVERWRITE))
    _screenBgColor = colorBg;
  lv_obj_set_style_bg_color(screen, _screenBgColor, LV_PART_MAIN);
}

lv_color_t classScreen::getBgColor(void)
{
  return _screenBgColor;
}


void classScreen::showConnectionStatus(bool connected)
{
  if (connected)
  {
    lv_label_set_text(_labelWarning, "");
    // show right footer label if enabled
    if (lv_obj_has_flag(_labelRight, LV_OBJ_FLAG_USER_1))
    {
      lv_obj_clear_flag(_labelRight, LV_OBJ_FLAG_HIDDEN);
    }
  }
  else
  {
    lv_obj_set_style_text_font(_labelWarning, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(_labelWarning, lv_color_make(180, 100, 10), 0);
    lv_label_set_text(_labelWarning, LV_SYMBOL_WARNING);
    // hide right footer label if enabled to show warning and settings button
    if (lv_obj_has_flag(_labelRight, LV_OBJ_FLAG_USER_1))
    {
      lv_obj_add_flag(_labelRight, LV_OBJ_FLAG_HIDDEN);
    }
  }
}

void classScreen::adScreenEventHandler(lv_event_cb_t callBack)
{
  lv_obj_add_event_cb(screen, callBack, LV_EVENT_ALL, this);
}

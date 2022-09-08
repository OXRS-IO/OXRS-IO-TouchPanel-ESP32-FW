#include <classScreen.h>
#include <classTile.h>
#include <globalDefines.h>

extern lv_color_t colorOn;
extern lv_color_t colorBg;
// extern classScreen *screenDsc[9];

// grid definitions
// sub screens 3 x 2 + home button
#define FOOTER_HEIGHT 25
#define COLS_2X3 2
#define ROWS_2X3 3
#define COL_SIZE_2X3 (SCREEN_WIDTH / COLS_2X3) - 2
#define ROW_SIZE_2X3 ((SCREEN_HEIGHT - FOOTER_HEIGHT) / ROWS_2X3) - 2
static lv_coord_t colDsc_2X3[] = {COL_SIZE_2X3, COL_SIZE_2X3, LV_GRID_TEMPLATE_LAST};
static lv_coord_t rowDsc_2X3[] = {ROW_SIZE_2X3, ROW_SIZE_2X3, ROW_SIZE_2X3, LV_GRID_TEMPLATE_LAST};

classScreen::classScreen(int number, int style)
{
  screenIdx = number;

  screen = lv_obj_create(NULL);
  lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

  // set background color to default
  setBgColor(0, 0, 0);

  if (style == 1)
  {
    //*Create a container with grid
    lv_obj_t *cont = lv_obj_create(screen);

    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_align(cont, LV_ALIGN_TOP_MID);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);
    lv_obj_set_style_pad_top(cont, 0, 0);
    lv_obj_set_style_pad_left(cont, 2, 0);
    lv_obj_set_grid_dsc_array(cont, colDsc_2X3, rowDsc_2X3);

    container = cont;
  }

  // middle "button" for screen selection drop down
  _btnFooter = lv_imgbtn_create(screen);
  lv_imgbtn_set_src(_btnFooter, LV_IMGBTN_STATE_RELEASED, NULL, NULL, NULL);
  lv_obj_set_size(_btnFooter, 200, 40);
  lv_obj_align(_btnFooter, LV_ALIGN_BOTTOM_MID, 0, 0);

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

int classScreen::getScreenNumber(void)
{
  return screenIdx;
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
    lv_obj_clear_flag(_btnHome, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(_btnHomeImg, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    lv_label_set_text(_labelLeft, left);
    lv_obj_clear_flag(_labelLeft, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(_btnHome, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(_btnHomeImg, LV_OBJ_FLAG_HIDDEN);
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
      lv_obj_clear_flag(_btnSettings, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(_btnSettingsImg, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
      lv_label_set_text(_labelRight, right);
      lv_obj_clear_flag(_labelRight, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(_labelRight, LV_OBJ_FLAG_USER_1);
      lv_obj_add_flag(_btnSettings, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(_btnSettingsImg, LV_OBJ_FLAG_HIDDEN);
    }
  }
}

void classScreen::setBgColor(int r, int g, int b)
{
  if ((r + g + b) == 0)
  {
    // if all zero reset to default background
    _screenBgColor = colorBg;
  }
  else
  {
    _screenBgColor = lv_color_make(r, g, b);
  }

  lv_obj_set_style_bg_color(screen, _screenBgColor, LV_PART_MAIN);
}

void classScreen::createHomeButton(lv_event_cb_t callBack, const void *img)
{
  _btnHome = lv_btn_create(screen);
  lv_obj_set_size(_btnHome, 45, 40);
  lv_obj_set_style_bg_opa(_btnHome, 0, LV_STATE_DEFAULT);
  lv_obj_align(_btnHome, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_clear_flag(_btnHome, LV_OBJ_FLAG_PRESS_LOCK);
  lv_obj_add_flag(_btnHome, LV_OBJ_FLAG_USER_1);
  lv_obj_add_event_cb(_btnHome, callBack, LV_EVENT_ALL, this);
  _btnHomeImg = lv_img_create(screen);
  lv_img_set_src(_btnHomeImg, img);
  lv_obj_align(_btnHomeImg, LV_ALIGN_BOTTOM_LEFT, 15, -6);

  // set call back for center button
  lv_obj_add_flag(_btnFooter, LV_OBJ_FLAG_USER_2);
  lv_obj_add_event_cb(_btnFooter, callBack, LV_EVENT_ALL, this);
}

void classScreen::createSettingsButton(lv_event_cb_t callBack, const void *img)
{
  _btnSettings = lv_btn_create(screen);
  lv_obj_set_size(_btnSettings, 45, 40);
  lv_obj_set_style_bg_opa(_btnSettings, 0, LV_STATE_DEFAULT);
  lv_obj_align(_btnSettings, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_clear_flag(_btnSettings, LV_OBJ_FLAG_PRESS_LOCK);
  lv_obj_add_flag(_btnSettings, LV_OBJ_FLAG_USER_3);
  lv_obj_add_event_cb(_btnSettings, callBack, LV_EVENT_ALL, this);
  _btnSettingsImg = lv_img_create(screen);
  lv_img_set_src(_btnSettingsImg, img);
  lv_obj_align(_btnSettingsImg, LV_ALIGN_BOTTOM_RIGHT, -15, -6);
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
      lv_obj_add_flag(_btnSettings, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(_btnSettingsImg, LV_OBJ_FLAG_HIDDEN);
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
      lv_obj_clear_flag(_btnSettings, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(_btnSettingsImg, LV_OBJ_FLAG_HIDDEN);
    }
  }
}

void classScreen::adScreenEventHandler(lv_event_cb_t callBack)
{
  lv_obj_add_event_cb(screen, callBack, LV_EVENT_ALL, this);
}

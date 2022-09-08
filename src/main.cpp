/**
  OXRS firmware for the WT32-SC01 touchscreen with support for POE enabled shields.

  See https://oxrs.io/docs/firmware/touch-panel-esp32.html for documentation.

  Compile options:
    ESP32

  Bugs/Features:
    See GitHub issues list

  Copyright 2019-2022 SuperHouse Automation Pty Ltd
*/

// Macro for converting env vars to strings
#define STRINGIFY(s) STRINGIFY1(s)
#define STRINGIFY1(s) #s

/*--------------------------- Defines -----------------------------------*/
// LCD backlight control
// TFT_BL GPIO pin defined in user_setup.h of tft_eSPI
// setting PWM properties
#define BL_PWM_FREQ 5000
#define BL_PWM_CHANNEL 0
#define BL_PWM_RESOLUTION 8

// I2C touch controller FT6336U
#define I2C_SDA 18
#define I2C_SCL 19
#define INT_N_PIN 39

/*--------------------------- Libraries ----------------------------------*/
#include <globalDefines.h>
#include <classTile.h>
#include <classTileList.h>
#include <classScreen.h>
#include <classScreenList.h>
#include <classScreenSettings.h>
#include <classDropDown.h>
#include <classRemote.h>
#include <classKeyPad.h>
#include <classIconList.h>
#include <classColorPicker.h>
#include <classThermostat.h>
#include <base64.hpp>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <classFT6336U.h>

#include <OXRS_WT32.h> // WT32 support

/*--------------------------- Constants ----------------------------------*/
// Serial
#define SERIAL_BAUD_RATE 115200

/*--------------------------- Global Variables ---------------------------*/
extern "C" const lv_img_dsc_t oxrs_splash_png;
extern "C" const lv_img_dsc_t oxrs_settings_png;

// iosicons
extern "C" const lv_img_dsc_t ios_room_60;
extern "C" const lv_img_dsc_t ios_up_nb_30;
extern "C" const lv_img_dsc_t ios_down_nb_30;
extern "C" const lv_img_dsc_t ios_bulb_60;
extern "C" const lv_img_dsc_t ios_door_60;
extern "C" const lv_img_dsc_t ios_coffee_60;
extern "C" const lv_img_dsc_t ios_window_60;
extern "C" const lv_img_dsc_t ios_blind_60;
extern "C" const lv_img_dsc_t ios_settings_25_l;
extern "C" const lv_img_dsc_t ios_back_25_l;
extern "C" const lv_img_dsc_t ios_home_25_l;
extern "C" const lv_img_dsc_t ios_thermometer_60;
extern "C" const lv_img_dsc_t ios_onoff_60;
extern "C" const lv_img_dsc_t ios_speaker_60;
extern "C" const lv_img_dsc_t ios_prev_40;
extern "C" const lv_img_dsc_t ios_next_40;
extern "C" const lv_img_dsc_t ios_play_60;
extern "C" const lv_img_dsc_t ios_pause_60;
extern "C" const lv_img_dsc_t ios_music_60;
extern "C" const lv_img_dsc_t ios_3dprint_60;
extern "C" const lv_img_dsc_t ios_remote_60;
extern "C" const lv_img_dsc_t ios_locked_60;
extern "C" const lv_img_dsc_t ios_unlocked_60;
extern "C" const lv_img_dsc_t ios_ceiling_fan_60;
extern "C" const lv_img_dsc_t ios_left_30;
extern "C" const lv_img_dsc_t ios_right_30;
extern "C" const lv_img_dsc_t ios_slider_60;

// pseudo icons
uint8_t nullImg[] = {0, 0, 0};
const lv_img_dsc_t wp_pseudo_thermostat =
{
  LV_IMG_CF_TRUE_COLOR_ALPHA, 0, 0, 1, 1, sizeof(nullImg), nullImg
};

// image pointer for further reference
const void *imgOxrsSplash = &oxrs_splash_png;
const void *imgOxrsSettings = &oxrs_settings_png;

const void *imgBlind = &ios_blind_60;
const void *imgBulb = &ios_bulb_60;
const void *imgWindow = &ios_window_60;
const void *imgDoor = &ios_door_60;
const void *imgCoffee = &ios_coffee_60;
const void *imgSettings = &ios_settings_25_l;
const void *imgUp = &ios_up_nb_30;
const void *imgDown = &ios_down_nb_30;
const void *imgBack = &ios_back_25_l;
const void *imgHome = &ios_home_25_l;
const void *imgRoom = &ios_room_60;
const void *imgThermo = &ios_thermometer_60;
const void *imgOnOff = &ios_onoff_60;
const void *imgSpeaker = &ios_speaker_60;
const void *imgPrev = &ios_prev_40;
const void *imgNext = &ios_next_40;
const void *imgPlay = &ios_play_60;
const void *imgPause = &ios_pause_60;
const void *imgMusic = &ios_music_60;
const void *img3dPrint = &ios_3dprint_60;
const void *imgRemote = &ios_remote_60;
const void *imgLocked = &ios_locked_60;
const void *imgUnLocked = &ios_unlocked_60;
const void *imgCeilingFan = &ios_ceiling_fan_60;
const void *imgLeft = &ios_left_30;
const void *imgRight = &ios_right_30;
const void *imgSlider = &ios_slider_60;
const void *imgPseudoThermostat = &wp_pseudo_thermostat;

int _actBackLight;
int _retainedBackLight;

connectionState_t _connectionState  = CONNECTED_NONE;
uint32_t _noActivityTimeOutToHome   = 0L;
uint32_t _noActivityTimeOutToSleep  = 0L;

#define DEFAULT_COLOR_ICON_ON_R     91
#define DEFAULT_COLOR_ICON_ON_G     190
#define DEFAULT_COLOR_ICON_ON_B     91

#define DEFAULT_COLOR_BACKGROUND_R  0
#define DEFAULT_COLOR_BACKGROUND_G  0
#define DEFAULT_COLOR_BACKGROUND_B  0

lv_color_t colorOn;
lv_color_t colorBg;

/*--------------------------- Global Objects -----------------------------*/
// WT32 handler
OXRS_WT32 wt32;

// the tile_style_LUT
styleLutEntry_t styleLut[TS_STYLE_COUNT] = {0};

// iconVault holds all icon image name and reference
classIconList iconVault = classIconList();

// screenVault holds all screens
classScreenList screenVault = classScreenList();

// tileVault holds all tiles
classTileList tileVault = classTileList();

// the Settings Screen
classScreenSettings screenSettings = classScreenSettings();

// drop down overlay
classDropDown dropDownOverlay = classDropDown();

// remote overlay
classRemote remoteControl = classRemote();

// key pad overlay
classKeyPad keyPad = classKeyPad();

// color picker overlay
classColorPicker colorPicker = classColorPicker();
lv_obj_t *_canvasCw = NULL;

// cset point overlay
classThermostat thermostat = classThermostat();

/*--------------------------- screen / lvgl relevant  -----------------------------*/

// Change to your screen resolution
static const uint16_t screenWidth = SCREEN_WIDTH;
static const uint16_t screenHeight = SCREEN_HEIGHT;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];
lv_indev_t *myInputDevice;

// TFT instance
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);
// touch controller instance
classFT6336U ft6336u = classFT6336U(I2C_SDA, I2C_SCL, INT_N_PIN);

#if LV_USE_LOG != 0
// Serial debugging if enabled
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

/*-----------------  create the color wheel to be used as an image   --------------*/

void createColorWheel(void)
{
  lv_draw_rect_dsc_t rect_dsc;
  lv_draw_rect_dsc_init(&rect_dsc);
  rect_dsc.radius = 5;
  rect_dsc.border_width = 0;

  int size = (CW_RADIUS * 2) + 2;
  static void *cbuf = ps_malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(size, size));

  _canvasCw = lv_canvas_create(NULL);
  lv_canvas_set_buffer(_canvasCw, cbuf, size, size, LV_IMG_CF_TRUE_COLOR_ALPHA);
  lv_obj_align(_canvasCw, LV_ALIGN_CENTER, 0, 0);
  lv_canvas_fill_bg(_canvasCw, lv_color_make(0, 0, 0), 0);

  int x, y, r;
  for (x = -CW_RADIUS; x <= CW_RADIUS; x++)
  {
    for (y = -CW_RADIUS; y <= CW_RADIUS; y++)
    {
      r = sqrt(x * x + y * y);
      if ((r <= CW_RADIUS) && (r > 0))
      {
        float alpha = acos((float)x / r) / (2 * PI) * 360.0;
        if (y < 0) alpha = 360 - alpha;
        lv_canvas_set_px_color(_canvasCw, x + CW_RADIUS, y + CW_RADIUS, lv_color_hsv_to_rgb(alpha, r / (float)CW_RADIUS * 100.0, 100));
        lv_canvas_set_px_opa(_canvasCw, x + CW_RADIUS, y + CW_RADIUS, 255);
      }
    }
  }
}


/*-----------------  Icon_Vault and tile_style_LUT handler  -----------------------*/
// initialise the Icon_Vault
void initIconVault(void)
{
  iconVault.add({string("_blind"), imgBlind});
  iconVault.add({string("_bulb"), imgBulb});
  iconVault.add({string("_ceilingfan"), imgCeilingFan});
  iconVault.add({string("_coffee"), imgCoffee});
  iconVault.add({string("_door"), imgDoor});
  iconVault.add({string("_locked"), imgLocked});
  iconVault.add({string("_unlocked"), imgUnLocked});
  iconVault.add({string("_music"), imgMusic});
  iconVault.add({string("_remote"), imgRemote});
  iconVault.add({string("_speaker"), imgSpeaker});
  iconVault.add({string("_window"), imgWindow});
  iconVault.add({string("_3dprint"), img3dPrint});
  iconVault.add({string("_onoff"), imgOnOff});
  iconVault.add({string("_play"), imgPlay});
  iconVault.add({string("_pause"), imgPause});
  iconVault.add({string("_thermometer"), imgThermo});
  iconVault.add({string("_slider"), imgSlider});
  iconVault.add({string("_thermostat"), imgPseudoThermostat});
}
// initialise the tile_style_LUT
void
initStyleLut(void)
{
  styleLut[TS_NONE] = {TS_NONE, ""};
  styleLut[TS_BUTTON] = {TS_BUTTON, "button"};
  styleLut[TS_BUTTON_LEVEL_UP] = {TS_BUTTON_LEVEL_UP, "buttonLevelUp"};
  styleLut[TS_BUTTON_LEVEL_DOWN] = {TS_BUTTON_LEVEL_DOWN, "buttonLevelDown"};
  styleLut[TS_BUTTON_UP_DOWN] = {TS_BUTTON_UP_DOWN, "buttonUpDown"};
  styleLut[TS_BUTTON_LEFT_RIGHT] = {TS_BUTTON_LEFT_RIGHT, "buttonLeftRight"};
  styleLut[TS_BUTTON_PREV_NEXT] = {TS_BUTTON_PREV_NEXT, "buttonPrevNext"};
  styleLut[TS_BUTTON_SELECTOR] = {TS_BUTTON_SELECTOR, "buttonSelector"};
  styleLut[TS_INDICATOR] = {TS_INDICATOR, "indicator"};
  styleLut[TS_COLOR_PICKER_RGB_CCT] = {TS_COLOR_PICKER_RGB_CCT, "colorPickerRgbCct"};
  styleLut[TS_COLOR_PICKER_RGB] = {TS_COLOR_PICKER_RGB, "colorPickerRgb"};
  styleLut[TS_COLOR_PICKER_CCT] = {TS_COLOR_PICKER_CCT, "colorPickerCct"};
  styleLut[TS_DROPDOWN] = {TS_DROPDOWN, "dropDown"};
  styleLut[TS_KEYPAD] = {TS_KEYPAD, "keyPad"};
  styleLut[TS_KEYPAD_BLOCKING] = {TS_KEYPAD_BLOCKING, "keyPadBlocking"};
  styleLut[TS_REMOTE] = {TS_REMOTE, "remote"};
  styleLut[TS_THERMOSTAT] = {TS_THERMOSTAT, "thermostat"};
  styleLut[TS_LINK] = {TS_LINK, "link"};
}

// converts a style string to its enum
int parseInputStyle(const char *inputStyle)
{
  int i;

  if (!inputStyle) return(TS_NONE);

  for (i = 0; i < TS_STYLE_COUNT; i++)
  {
    if (styleLut[i].styleStr)
      if (strcmp(styleLut[i].styleStr, inputStyle) == 0)
        break;
  }
  // in range check
  if (i >= TS_STYLE_COUNT) i = 0;
  return (styleLut[i].style);
}

// converts a style enum to its string
const char *styleEnum2Str(int styleEnum)
{
  // in range check
  if (styleEnum >= TS_STYLE_COUNT) styleEnum = 0;
  return styleLut[styleEnum].styleStr;
}

/*--------------------------- publish with MQTT  -----------------------------*/

// publish Tile Event
// {"screen":1, "tile":1, "type":"button", "event":"single" , "state":"on"}
void publishTileEvent(classTile *tPtr, const char *event)
{
  StaticJsonDocument<128> json;
  json["screen"] = tPtr->getScreenIdx();
  json["tile"] = tPtr->getTileIdx();
  json["style"] = tPtr->getStyleStr();
  json["type"] = "button";
  json["event"] = event;
  json["state"] = (tPtr->getState() == true) ? "on" : "off";

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish remote button Event
// {"screen":1, "tile":1, "style":remote, "type":"up", "event":"single" }
void publishRemoteEvent(classTile *tPtr, int btnIndex, const char *event)
{
  StaticJsonDocument<128> json;
  json["screen"] = tPtr->getScreenIdx();
  json["tile"] = tPtr->getTileIdx();
  json["style"] = tPtr->getStyleStr();
  switch (btnIndex)
  {
  case 1:    json["type"] = "up";    break;
  case 2:    json["type"] = "down";  break;
  case 3:    json["type"] = "left";  break;
  case 4:    json["type"] = "right"; break;
  case 5:    json["type"] = "ok";    break;
  case 6:    json["type"] = "info";  break;
  case 7:    json["type"] = "list";  break;
  case 8:    json["type"] = "back";  break;
  case 9:    json["type"] = "home";  break;
  }
  json["event"] = event;

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish dropdown change Event
// {"screen":1, "tile":1, "type":"dropdown", "event":"change" , "state":1}
void publishDropDownEvent(classTile *tPtr, int listIndex)
{
  StaticJsonDocument<128> json;
  json["screen"] = tPtr->getScreenIdx();
  json["tile"] = tPtr->getTileIdx();
  json["style"] = tPtr->getStyleStr();
  json["type"] = "dropDown";
  json["event"] = "selection";
  json["state"] = listIndex;

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish key pad change Event
// {"screen":1, "tile":1, "style":"light", "type":"button", "event":"key", "state":"off", "keycode":"1234" }
void publishKeyPadEvent(classTile *tPtr, const char *key)
{
  StaticJsonDocument<128> json;
  json["screen"] = tPtr->getScreenIdx();
  json["tile"] = tPtr->getTileIdx();
  json["style"] = tPtr->getStyleStr();
  json["type"] = "button";
  json["event"] = "key";
  json["state"] = (tPtr->getState() == true) ? "on" : "off";
  json["keyCode"] = key;

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish Level change Event
// {"screen":1, "tile":1, "type":"level", "event":"change" , "state":50}
void publishLevelEvent(classTile *tPtr, const char *event, int value)
{
  StaticJsonDocument<128> json;
  json["screen"] = tPtr->getScreenIdx();
  json["tile"] = tPtr->getTileIdx();
  json["style"] = tPtr->getStyleStr();
  json["type"] = "level";
  json["event"] = event;
  json["state"] = value;

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish color picker change Event
// {"screen":<number>, "tile":<number>, "style":"<style>", "type":"colorPicker", “event”:“change”, 
// “state”:{"colorRgb":{"r":<val>, "g":<val>, "b":<val>}, "colorKelvin":<val>, "brightness":<val>}}

void publishColorPickerEvent(classTile *tPtr, const char *event, lv_color32_t rgb, int kelvin, int brightness)
{
  StaticJsonDocument<256> json;
  json["screen"] = tPtr->getScreenIdx();
  json["tile"] = tPtr->getTileIdx();
  json["style"] = tPtr->getStyleStr();
  json["type"] = "colorPicker";
  json["event"] = "change";
  json["state"]["colorRgb"]["r"]= rgb.ch.red;
  json["state"]["colorRgb"]["g"] = rgb.ch.green;
  json["state"]["colorRgb"]["b"] = rgb.ch.blue;
  json["state"]["colorKelvin"] = kelvin;
  json["state"]["brightness"] = brightness;

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish prev / next Event
// {"screen":1, "tile":1, "type":"prev"|"next", "event":"single"|"hold" }
void publishPrevNextEvent(classTile *tPtr, const char *type, const char *event)
{
  StaticJsonDocument<128> json;
  json["screen"] = tPtr->getScreenIdx();
  json["tile"] = tPtr->getTileIdx();
  json["style"] = tPtr->getStyleStr();
  json["type"] = type;
  json["event"] = event;

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish selector Event
// {"screen":1, "tile":1, "type":"prev"|"next", "event":"single"|"hold" }
void publishSelectorEvent(classTile *tPtr, int index)
{
  StaticJsonDocument<128> json;
  json["screen"] = tPtr->getScreenIdx();
  json["tile"] = tPtr->getTileIdx();
  json["style"] = tPtr->getStyleStr();
  json["type"] = "selector";
  json["event"] = "selection";
  json["state"] = index;

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish thermostat (mode) change Event
// {"screen":1, "tile":1, "style":"thermostat","type":"thermostat" | "mode", "event":"change" , "state":205}
void publishThermostatEvent(classTile *tPtr, int mode, int target)
{
  StaticJsonDocument<128> json;
  json["screen"] = tPtr->getScreenIdx();
  json["tile"] = tPtr->getTileIdx();
  json["style"] = tPtr->getStyleStr();
  json["type"] = "thermostat";
  json["event"] = "change";
  json["state"]["mode"] = mode;
  json["state"]["targetTemperature"] = target;

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish Screen Event
// {"screen":1, "type":"screen", "event":"change" , "state":"unloaded"}
void publishScreenEvent(int screenIdx, const char *state)
{
  StaticJsonDocument<128> json;
  json["screen"] = screenIdx;
  json["type"] = "screen";
  json["event"] = "change";
  json["state"] = state;

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish backlight change event
//   “type” : “backlight”, “event” : “change”, “state” : “sleep” |”awake”, “brightness” : <number>
void publishBacklightEvent(int brightness)
{
  StaticJsonDocument<128> json;
  json["type"] = "backlight";
  json["event"] = "change";
  json["state"] = (brightness != 0) ? "awake" : "sleep" ;
  json["brightness"] = brightness;

  wt32.publishStatus(json.as<JsonVariant>());
}

// publish message box closed Event
// {"screen":0, "type":"message", "event":"open" , "state":"open"}
void publishMessageEvent(const char *event, const char *state)
{
  StaticJsonDocument<128> json;
  json["type"] = "message";
  json["event"] = event;
  json["state"] = state;

  wt32.publishStatus(json.as<JsonVariant>());
}

/*
    backlight of LCD handling
*/

// low level setting
void _setBackLightLED(int val)
{
  if (val > 100)   val = 100;
  if (val < 0)     val = 0;

  ledcWrite(BL_PWM_CHANNEL, 255 * val / 100);
  _actBackLight = val;
}

void _setBackLight(int val)
{
  _setBackLightLED(val);
  screenSettings.setSlider(val);

  publishBacklightEvent(val);
}

/*
    lcd interface
    transfer pixel data range to lcd
*/
void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);

  lv_disp_flush_ready(disp);
}

/*
    touch pad interface
    test for touch and report RELEASED / or PRESSED + x/y back
*/
void my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
  typePoint ts;
  bool touched = ft6336u.readTouchPoint(&ts);

  // no touch detected
  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
    return;
  }
  // touch detected while backlight = 0
  if (_actBackLight == 0)
  {
    _setBackLight(_retainedBackLight);
    delay(200);
    // mimic keypress in top/left corner (no sensitive area)
    ts.x = 0;
    ts.y = 0;
  }
  // get coordinates and write into point structure
  data->point.x = ts.x;
  data->point.y = ts.y;
  data->state = LV_INDEV_STATE_PR;

#if defined(DEBUG_TOUCH)
  wt32.print(F("[tp32] touch data (x,y): "));
  wt32.print(data->point.x);
  wt32.print(F(","));
  wt32.println(data->point.y);
#endif
}

// check for timeout inactivity timeout
void checkNoActivity(void)
{
  // observer disabled
  if (_noActivityTimeOutToHome != 0)
  {
    if (lv_disp_get_inactive_time(NULL) > _noActivityTimeOutToHome)
    {
      // check and close pop-ups first
      if (dropDownOverlay.isActive()) dropDownOverlay.close();
      if (colorPicker.isActive()) colorPicker.close();
      if (thermostat.isActive()) thermostat.close();
      if (remoteControl.isActive()) remoteControl.close();
     // return to HomeScreen if keyPad is NOT active
      if (!keyPad.isActive())
      {
        if (lv_scr_act() != screenVault.get(SCREEN_HOME)->screen) screenVault.show(SCREEN_HOME);
      }
    }
  }
  if (_noActivityTimeOutToSleep != 0)
  {
    // is in sleep allready ?
    if (_actBackLight > 0)
    {
      // time elapsed, jump to HomeScreen
      if (lv_disp_get_inactive_time(NULL) > _noActivityTimeOutToSleep)
      {
        _retainedBackLight = _actBackLight;
        _setBackLight(0);
      }
    }
  }
}

/*
  * ui helper functions
  */

void setIconOnColor(int r, int g, int b)
{
  // all zero is defined as unset, so set default
  if ((r + g + b) == 0)
  {
    colorOn = lv_color_make(
      DEFAULT_COLOR_ICON_ON_R, 
      DEFAULT_COLOR_ICON_ON_G, 
      DEFAULT_COLOR_ICON_ON_B);
  }
  else
  {
    colorOn = lv_color_make(r, g, b);
  }
}

void setBackgroundColor(int r, int g, int b)
{
  // all zero is defined as unset, so set default
  if ((r + g + b) == 0)
  {
    colorBg = lv_color_make(
      DEFAULT_COLOR_BACKGROUND_R,
      DEFAULT_COLOR_BACKGROUND_G,
      DEFAULT_COLOR_BACKGROUND_B);
  }
  else
  {
    colorBg = lv_color_make(r, g, b);
  }
}

// update info text panel on screenSettings
void updateInfoText(void)
{
  char buffer[40];

  lv_obj_t *table = screenSettings.getInfoPanel();
  lv_table_set_row_cnt(table, 9);
  lv_table_set_col_cnt(table, 2);

  lv_table_set_cell_value(table, 0, 0, "Name:");
  lv_table_set_cell_value(table, 0, 1, FW_NAME);
  lv_table_set_cell_value(table, 1, 1, FW_SHORT_NAME);
  lv_table_set_cell_value(table, 2, 0, "Maker:");
  lv_table_set_cell_value(table, 2, 1, FW_MAKER);
  lv_table_set_cell_value(table, 3, 0, "Version:");
  lv_table_set_cell_value(table, 3, 1, STRINGIFY(FW_VERSION));

  lv_table_set_cell_value(table, 5, 0, "MAC:");
  wt32.getMACAddressTxt(buffer);
  lv_table_set_cell_value(table, 5, 1, buffer);

  lv_table_set_cell_value(table, 6, 0, "IP:");
  wt32.getIPAddressTxt(buffer);
  lv_table_set_cell_value(table, 6, 1, buffer);

  lv_table_set_cell_value(table, 7, 0, "MODE:");
  #if defined(ETH_MODE)
    lv_table_set_cell_value(table, 7, 1, "Ethernet");
  #else
    lv_table_set_cell_value(table, 7, 1, "WiFi");
  #endif

    lv_table_set_cell_value(table, 8, 0, "MQTT:");
    wt32.getMQTTTopicTxt(buffer);
    lv_table_set_cell_value(table, 8, 1, buffer);
}

// check for changes in IP/MQTT connection and update warning sign in footer
void updateConnectionStatus(void)
{
  connectionState_t connectionState = wt32.getConnectionState();
  if (_connectionState != connectionState)
  {
    _connectionState = connectionState;
    // update footers in all screens
    classScreen *sPtr = screenVault.getStart();
    do
    {
      sPtr->showConnectionStatus(_connectionState == CONNECTED_MQTT);
    } while ((sPtr = screenVault.getNext(sPtr->screenIdx)));

    // update info text to reflect actual status
    updateInfoText();
  }
}

/*--------------------------- Event Handler ------------------------------------*/

// screen event handler
// detects unload and load
void screenEventHandler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_SCREEN_UNLOAD_START)
  {
    classScreen *sPtr = (classScreen *)lv_event_get_user_data(e);
    publishScreenEvent(sPtr->screenIdx, "unloaded");
  }
  if (code == LV_EVENT_SCREEN_LOADED)
  {
    classScreen *sPtr = (classScreen *)lv_event_get_user_data(e);
    publishScreenEvent(sPtr->screenIdx, "loaded");
  }
}

// message box closed event handler
void messageClosedEventHandler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_DELETE)
  {
    publishMessageEvent("close", "closed");
  }
}

// Up / Down Button Event Handler
static void upDownEventHandler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = lv_event_get_target(e);

  if ((code == LV_EVENT_SHORT_CLICKED) || (code == LV_EVENT_LONG_PRESSED) || (code == LV_EVENT_LONG_PRESSED_REPEAT))
  {
    // get tile* of clicked tile from USER_DATA
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    int levelStart = tPtr->getLevelStart();
    int levelStop = tPtr->getLevelStop();
    int levelLargeStep = tPtr->getLevelLargeStep();
    int level = tPtr->getLevel();
    // short increments 1; long increments 5
    int increment = (code == LV_EVENT_SHORT_CLICKED) ? 1 : levelLargeStep;
    int direction = lv_obj_has_flag(btn, LV_OBJ_FLAG_USER_1) ? 1 : -1;
    increment *= direction;
    // calc new value and limit to 0...100
    level += increment;
    if (level > levelStop)
      level = levelStop;
    if (level < levelStart)
      level = levelStart;
    tPtr->setLevel(level, true);
    tPtr->showOvlBar(level);
    // send event
    publishLevelEvent(tPtr, (direction == 1) ? "up" : "down", level);
  }
}

// Up / Down Button with selector Event Handler
static void selectorEventHandler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = lv_event_get_target(e);

  if (code == LV_EVENT_SHORT_CLICKED)
  {
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    int index = tPtr->getDropDownIndex();
    lv_obj_has_flag(btn, LV_OBJ_FLAG_USER_1) ? index++ : index--;
    tPtr->showSelector(index);
    publishSelectorEvent(tPtr, tPtr->getDropDownIndex());
  }
}

// previous/next event handler
static void prevNextEventHandler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = lv_event_get_target(e);
  const char *type = NULL;

  if ((code == LV_EVENT_SHORT_CLICKED) || (code == LV_EVENT_LONG_PRESSED) || (code == LV_EVENT_LONG_PRESSED_REPEAT))
  {
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    // up / down events only
    if (tPtr->getStyle() == TS_BUTTON_PREV_NEXT)
      type = lv_obj_has_flag(btn, LV_OBJ_FLAG_USER_1) ? "prev" : "next";
    if (tPtr->getStyle() == TS_BUTTON_LEFT_RIGHT)
      type = lv_obj_has_flag(btn, LV_OBJ_FLAG_USER_1) ? "left" : "right";
    if (tPtr->getStyle() == TS_BUTTON_UP_DOWN)
      type = lv_obj_has_flag(btn, LV_OBJ_FLAG_USER_1) ? "up" : "down";
    const char *event = (code == LV_EVENT_SHORT_CLICKED) ? "single" : "hold";
    publishPrevNextEvent(tPtr, type, event);
  }
}

// key pad event handler
static void keyPadEventHandler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_SHORT_CLICKED)
  {
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    const char *txt = lv_btnmatrix_get_btn_text(obj, id);

    LV_LOG_USER("%s was pressed\n", txt);
    if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)
    {
      keyPad.delChar();
    }
    else if (strcmp(txt, LV_SYMBOL_NEW_LINE) == 0)
    {
      if ((strlen(keyPad.getKey()) == 0) && !(tPtr->getStyle() == TS_KEYPAD_BLOCKING))
        keyPad.close();
      if (strlen(keyPad.getKey()) > 0)
        publishKeyPadEvent(tPtr, keyPad.getKey());
    }
    else
    {
      keyPad.addChar(txt[0]);
    }
  }
}

// remote control
static void navigationButtonEventHandler(lv_event_t * e)
{
  int btnIndex = 0;
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if ((code == LV_EVENT_SHORT_CLICKED) || (code == LV_EVENT_LONG_PRESSED))
  {
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_1))
      btnIndex += 1;
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_2))
      btnIndex += 2;
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_3))
      btnIndex += 4;
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_4))
      btnIndex += 8;
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    if (code == LV_EVENT_SHORT_CLICKED)
      publishRemoteEvent(tPtr, btnIndex, "single");
    else
      publishRemoteEvent(tPtr, btnIndex, "hold");
  }
}

// drop down event handler
static void dropDownEventHandler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  // CANCEL fired when drop down list closed
  if (code == LV_EVENT_CANCEL)
  {
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    char buf[64];
    lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
    int listIndex = lv_dropdown_get_selected(obj) + 1;
    publishDropDownEvent(tPtr, listIndex);
    tPtr->setIconText(buf);
    tPtr->setDropDownIndex(listIndex);
    dropDownOverlay.close();
  }
}

// screen select drop down Event Handler
static void screenDropDownEventHandler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_CANCEL)
  {
    int listIndex = lv_dropdown_get_selected(obj);
    screenVault.showByIndex(listIndex);
    dropDownOverlay.close();
  }
}

// color picker color wheel
static void colorPickerCwEventHandler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_PRESSING) 
  {
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    lv_point_t point;
    lv_indev_t *myInputDevice = lv_indev_get_act();
    lv_indev_get_point(myInputDevice, &point);
    colorPicker.updateCw(point, 0);
    colorPicker.updateAll();
  }
  if (code == LV_EVENT_RELEASED)
  {
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    lv_color32_t color32 = tPtr->getColorPickerRGB();
    publishColorPickerEvent(tPtr, "change", color32, 0, 0);
  }
}

// color picker event handler
static void colorPickerEventHandler(lv_event_t *e)
{
  lv_color32_t color32;
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_VALUE_CHANGED) 
  {
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_1))
    {
      colorPicker.updateAll();
      color32 = tPtr->getColorPickerRGB();
      publishColorPickerEvent(tPtr, "change", color32, 0, 0);
    }
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_2))
    {
      colorPicker.updateAll();
      color32.full = 0;
      publishColorPickerEvent(tPtr, "change", color32, tPtr->getColorPickerKelvin(), tPtr->getColorPickerBrightnessWhite());
    }
  }
  if (code == LV_EVENT_SHORT_CLICKED)
  {
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_3))
    {
      colorPicker.switchMode(lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_4) ? CP_MODE_TEMP : CP_MODE_COLOR);
    }
    // must have been the button -> send tile event
    else if (!lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_1) && !lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_2))
    {
      publishTileEvent(tPtr, "single");
    }
  }
}

// thermostat  arc event handler
static void thermostatEventHandler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);

  if (code == LV_EVENT_VALUE_CHANGED)
  {
    thermostat.updateAll();
  }
  if (code == LV_EVENT_RELEASED)
  {
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_1))
    // thermostat arc released
    {
      classTile *tPtr = (classTile *)lv_event_get_user_data(e);
      publishThermostatEvent(tPtr, tPtr->getDropDownIndex(), tPtr->getThermostatTarget());
    }
  }
  if (code == LV_EVENT_CLICKED)
  {
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_2))
      // mode button
      thermostat.showDropDown();
  }
  if (code == LV_EVENT_CANCEL)
  {
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_USER_3))
    // drop down 
    {
      classTile *tPtr = (classTile *)lv_event_get_user_data(e);
      thermostat.closeDropDown();
      publishThermostatEvent(tPtr, tPtr->getDropDownIndex(), tPtr->getThermostatTarget());
    }
  }
}

// general Tile Event Handler
static void tileEventHandler(lv_event_t * e)
{
  static uint32_t tilePressStarted;
  lv_event_code_t code = lv_event_get_code(e);

  if ((code == LV_EVENT_SHORT_CLICKED) || (code == LV_EVENT_LONG_PRESSED))
  {
    // get tile* of clicked tile from USER_DATA
    classTile *tPtr = (classTile *)lv_event_get_user_data(e);
    if (code == LV_EVENT_SHORT_CLICKED)
    {
      // handle the different button styles
      switch (tPtr->getStyle())
      {
      case TS_LINK:
        // button is style LINK -> switch to linked screen
        screenVault.show(tPtr->getLink());
        break;

      case TS_DROPDOWN:
        // button is style DROPDOWN -> show drop down overlay
        dropDownOverlay = classDropDown(tPtr, dropDownEventHandler);
        dropDownOverlay.open();
        break;

      case TS_REMOTE:
        // button is style REMOTE -> show remote overlay
        remoteControl = classRemote(tPtr, navigationButtonEventHandler);
        break;

      case TS_KEYPAD:
      case TS_KEYPAD_BLOCKING:
        // button is style keypad
        keyPad = classKeyPad(tPtr, keyPadEventHandler);
        break;

      case TS_COLOR_PICKER_RGB_CCT:
        // style colorpicker (RGB + CCT)
        colorPicker = classColorPicker(tPtr, colorPickerEventHandler, colorPickerCwEventHandler, lv_canvas_get_img(_canvasCw), CP_MODE_COLOR | CP_MODE_TEMP);
        break;

      case TS_COLOR_PICKER_RGB:
        // style colorpicker (RGB only)
        colorPicker = classColorPicker(tPtr, colorPickerEventHandler, colorPickerCwEventHandler, lv_canvas_get_img(_canvasCw), CP_MODE_COLOR);
        break;

      case TS_COLOR_PICKER_CCT:
        // style colorpicker (CCT only)
        colorPicker = classColorPicker(tPtr, colorPickerEventHandler, colorPickerCwEventHandler, lv_canvas_get_img(_canvasCw), CP_MODE_TEMP);
        break;

      case TS_THERMOSTAT:
        // button is style thermostat
        thermostat = classThermostat(tPtr, thermostatEventHandler);
        break;

      default:
        // no special action -> publish click event
        publishTileEvent(tPtr, "single");
        break;
      }
    }
    else
    {
      // publish long press event
      publishTileEvent(tPtr, "hold");
    }
  }
}

// screen footer button Event handler
//    HomeButton            -> displays Home screen
//    SettingsButton        -> displays Settings
//    Center Button (label) -> show screen select drop down
static void footerButtonEventHandler(lv_event_t * e)
{
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  if (event == LV_EVENT_SHORT_CLICKED)
  {
    if (lv_obj_has_flag(ta, LV_OBJ_FLAG_USER_1))
      screenVault.show(SCREEN_HOME);
    if (lv_obj_has_flag(ta, LV_OBJ_FLAG_USER_3))
      screenVault.show(SCREEN_SETTINGS);
    if (lv_obj_has_flag(ta, LV_OBJ_FLAG_USER_2))
    {
      dropDownOverlay = classDropDown(NULL, screenDropDownEventHandler);
      char buf[SCREEN_END * 32];
      int index = screenVault.makeDropDownList(buf, lv_scr_act()) + 1;
      dropDownOverlay.setDropDownList(buf);
      dropDownOverlay.setDropDownIndex(index);
      dropDownOverlay.setDropDownLabel("Select Screen");
      dropDownOverlay.open();
    }
  }
  if (event == LV_EVENT_LONG_PRESSED)
  {
    // long press of footer center loads hom screen
    if (lv_obj_has_flag(ta, LV_OBJ_FLAG_USER_2))
      screenVault.show(SCREEN_HOME);
  }
}

// BackLight slider event handler
static void backLightSliderEventHandler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *slider = lv_event_get_target(e);

  if (code == LV_EVENT_VALUE_CHANGED)
  {
    classScreenSettings *sPtr = (classScreenSettings *)lv_event_get_user_data(e);
    sPtr->setSlider((int)lv_slider_get_value(slider));
    _setBackLightLED(lv_slider_get_value(slider));
  }
  if (code == LV_EVENT_RELEASED)
  {
    _setBackLight(lv_slider_get_value(slider));
  }
}

// create screen for tiles in screenVault if not exists
void createScreen(int screenIdx)
{
  // exit if screenIdx exits
  if (screenVault.exist(screenIdx))
    return;
  // create new screen with grid container
  classScreen &ref = screenVault.add(screenIdx, 1);
  ref.createHomeButton(footerButtonEventHandler, imgHome);
  ref.createSettingsButton(footerButtonEventHandler, imgSettings);
  ref.adScreenEventHandler(screenEventHandler);
  // sort screenIdx in ascending order
  screenVault.sort();
}

/**
  Config handler
  */

// icon list for config
void createIconEnum(JsonObject parent)
{
  JsonArray styleEnum = parent.createNestedArray("enum");

  string iconStr;
  iconVault.setIteratorStart();
  while ((iconStr = iconVault.getNextStr()) != "")
  {
    styleEnum.add(iconStr);
  }
}

// style list for config
void createInputStyleEnum(JsonObject parent)
{
  JsonArray styleEnum = parent.createNestedArray("enum");

  for (int i = 1; i < TS_STYLE_COUNT; i++)
  {
    styleEnum.add(styleLut[i].styleStr);
  }
}

// r/g/b color properties for config
void createRgbProperties(JsonVariant json)
{
  JsonObject properties = json.createNestedObject("properties");

  JsonObject r = properties.createNestedObject("r");
  r["title"] = "Red";
  r["type"] = "integer";
  r["minimum"] = 0;
  r["maximum"] = 255;

  JsonObject g = properties.createNestedObject("g");
  g["title"] = "Green";
  g["type"] = "integer";
  g["minimum"] = 0;
  g["maximum"] = 255;

  JsonObject b = properties.createNestedObject("b");
  b["title"] = "Blue";
  b["type"] = "integer";
  b["minimum"] = 0;
  b["maximum"] = 255;
}

// Create any tile on any screen
void createTile(const char *styleStr, int screenIdx, int tileIdx, const char *iconStr, const char *label, int linkedScreen, int levelStart, int levelStop)
{
  const void *img = NULL;
  int style;

  // get the tile_style
  style = parseInputStyle(styleStr);
  if (!style)
  {
    wt32.print(F("[tp32] invalid style for screen/tile : "));
    wt32.print(screenIdx);
    wt32.print(F("/"));
    wt32.println(tileIdx);
    return;
  }
  
  // create screen if not exist
  createScreen(screenIdx);

  // delete tile reference if exist
  tileVault.remove(screenIdx, tileIdx);

  // get the icon image
  if (iconStr)
    img = iconVault.getIcon(string(iconStr));

  // create new Tile
  classTile &ref = tileVault.add();
  ref.begin(screenVault.get(screenIdx)->container, img, label);
  ref.registerTile(screenIdx, tileIdx, style, styleStr);

  // handle tiles depending on style capabilities
  if ((style == TS_LINK) && linkedScreen)
  {
    ref.setLink(linkedScreen);
    // create screen if not exist
    createScreen(linkedScreen);
  }

  // set the event handler if NOT (INDICATOR_*)
  if (style != TS_INDICATOR)
  {
    ref.addEventHandler(tileEventHandler);
  }

  // enable on-tile level control (bottom-up)
  if (style == TS_BUTTON_LEVEL_UP)
  {
    ref.addUpDownControl(upDownEventHandler, imgUp, imgDown);
  }

  // enable on-tile level control (top-down)
  if (style == TS_BUTTON_LEVEL_DOWN)
  {
    ref.setTopDownMode(true);
    ref.addUpDownControl(upDownEventHandler, imgUp, imgDown);
  }

  // enable on-tile selector control (roller)
  if (style == TS_BUTTON_SELECTOR)
  {
    ref.addUpDownControl(selectorEventHandler, imgUp, imgDown);
  }

  // set levelrange
  {
    if ((levelStart != 0) || (levelStop != 0))
    {
      // allow increasing only (Stop > Start)
      if(levelStop < levelStart)
      {
        wt32.println(F("[tp32] invalid level range."));
      }
      else
      {
        ref.setLevelStartStop(levelStart, levelStop);
      }
    }
  }

  // enable prev/next control (button events only)
  if (style == TS_BUTTON_PREV_NEXT)
  {
    ref.addUpDownControl(prevNextEventHandler, imgPrev, imgNext);
  }

  // enable up/down control (button events only)
  if (style == TS_BUTTON_UP_DOWN)
  {
    ref.addUpDownControl(prevNextEventHandler, imgUp, imgDown);
  }

  // enable left/right control (button events only)
  if (style == TS_BUTTON_LEFT_RIGHT)
  {
    ref.addUpDownControl(prevNextEventHandler, imgLeft, imgRight);
  }

  // set action indicator if required (depends on tile style)
  switch (style)
  {
    case TS_LINK: ref.setActionIndicator(WP_SYMBOL_CHEV_RIGHT); break;
    case TS_DROPDOWN: ref.setActionIndicator(WP_SYMBOL_CHEV_DOWN); break;
    case TS_COLOR_PICKER_CCT:
    case TS_COLOR_PICKER_RGB:
    case TS_COLOR_PICKER_RGB_CCT:
    case TS_KEYPAD:
    case TS_KEYPAD_BLOCKING:
    case TS_REMOTE:
    case TS_THERMOSTAT: ref.setActionIndicator(WP_SYMBOL_DOTS); break;
  }
}

// Snapshot API
void getApiSnapshot(Request &req, Response &res)
{
  // get the query from request ?tile=n
  char requestedTile[8];
  req.query("tile", requestedTile, sizeof(requestedTile));
  int tileIdx = atoi(requestedTile);

  // take a snapshot
  lv_img_dsc_t *snapshot = lv_snapshot_take(lv_scr_act(), LV_IMG_CF_TRUE_COLOR_ALPHA);
  uint8_t *bufferPtr = (uint8_t *)snapshot->data;
  size_t bufferSize = WT32_SCREEN_WIDTH * WT32_SCREEN_HEIGHT * 3;

  // convert to RGB888
  lv_color_t color;
  for (int i = 0; i < bufferSize; i += 3)
  {
    color.full = bufferPtr[i] + bufferPtr[i + 1] * 0x100;
    bufferPtr[i] = (color.ch.blue * 255) / 31;
    bufferPtr[i + 1] = (color.ch.green * 255) / 63;
    bufferPtr[i + 2] = (color.ch.red * 255) / 31;
  }

  // build header for .bmp file (full screen)
  struct bmpHeader_t
  {
    uint8_t magic[2] = {'B', 'M'};
    uint32_t bfSize = (uint32_t)(WT32_SCREEN_WIDTH * WT32_SCREEN_HEIGHT * 3);
    uint32_t bfReserved = 0;
    uint32_t bfOffBits = sizeof(bmpHeader_t);

    uint32_t biSize = 40;
    int32_t biWidth = WT32_SCREEN_WIDTH;
    int32_t biHeight = -WT32_SCREEN_HEIGHT;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 24;
    uint32_t biCompression = 0;
    uint32_t biSizeImage = bfSize;
    int32_t biXPelsPerMeter = 2836;
    int32_t biYPelsPerMeter = 2836;
    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;

    uint32_t bdMask[3] = {0x0, 0x0, 0x0};
  } __attribute__((packed)) bmpHeader;

  // preset for full screen
  int rowStart = 0;
  int colStart = 0;
  int rows = WT32_SCREEN_HEIGHT;
  int cols = WT32_SCREEN_WIDTH;

  // handle one tile only request
  if ((tileIdx >= 1) && (tileIdx <= 6))
  {
    rows = 139;
    cols = 148;
    tileIdx--;
    int rowColStarts[6][2] = {{5, 7},  {5, 165},  {154, 7},  {154, 165},  {303, 7},  {303, 165}};
    rowStart = rowColStarts[tileIdx][0];
    colStart = rowColStarts[tileIdx][1];
    uint32_t size = rows * cols * 3;

    // update header with recent values for tile only mode
    bmpHeader.bfSize = size;
    bmpHeader.biSizeImage = bmpHeader.bfSize;
    bmpHeader.biWidth = cols;
    bmpHeader.biHeight = -rows;
  }

  // return the snapshot image to the caller
  res.set("Content-Type", "image/bmp");
  res.write((uint8_t *)&bmpHeader, sizeof(bmpHeader));

  // send bmp data row by row to limit buffer size
  for (bufferPtr += (rowStart * WT32_SCREEN_WIDTH * 3 + colStart * 3); rows > 0; bufferPtr += (WT32_SCREEN_WIDTH * 3), rows--)
  {
    res.write(bufferPtr, cols * 3);
  }

  // free used memory
  lv_snapshot_free(snapshot);
}

/**
 * Config Handler
 */

void jsonIconOnColorConfig(JsonVariant json)
{
  uint8_t r, g, b;

  r = (uint8_t)json["r"].as<int>();
  g = (uint8_t)json["g"].as<int>();
  b = (uint8_t)json["b"].as<int>();

  setIconOnColor(r, g, b);
}

void jsonBackgroundColorConfig(JsonVariant json)
{
  uint8_t r, g, b;

  r = (uint8_t)json["r"].as<int>();
  g = (uint8_t)json["g"].as<int>();
  b = (uint8_t)json["b"].as<int>();

  setBackgroundColor(r, g, b);

  // set all screens to the new background color
  classScreen *sPtr = screenVault.getStart();
  do
  {
    if (sPtr) sPtr->setBgColor(0, 0, 0);
  } while ((sPtr = screenVault.getNext(sPtr->screenIdx)));
}

void jsonTilesConfig(int screenIdx, JsonVariant json)
{
  if ((screenIdx < SCREEN_START) || (screenIdx > SCREEN_END))
  {
    wt32.print(F("[tp32] invalid screen: "));
    wt32.println(screenIdx);
    return;
  }

  int tileIdx = json["tile"].as<int>();
  if ((tileIdx < TILE_START) || (tileIdx > TILE_END))
  {
    wt32.print(F("[tp32] invalid tile: "));
    wt32.println(tileIdx);
    return;
  }

  createTile(json["style"], screenIdx, tileIdx, json["icon"], json["label"], json["link"], json["levelStart"], json["levelStop"]);
}

void jsonConfig(JsonVariant json)
{
  if (json.containsKey("iconOnColorRgb"))
  {
    jsonIconOnColorConfig(json["iconOnColorRgb"]);
  }

  if (json.containsKey("backgroundColorRgb"))
  {
    jsonBackgroundColorConfig(json["backgroundColorRgb"]);
  }

  if (json.containsKey("noActivitySecondsToHome"))
  {
    _noActivityTimeOutToHome = json["noActivitySecondsToHome"].as<int>() * 1000;
  }

  if (json.containsKey("noActivitySecondsToSleep"))
  {
    _noActivityTimeOutToSleep = json["noActivitySecondsToSleep"].as<int>() * 1000;
  }

  if (json.containsKey("screens"))
  {
    for (JsonVariant screenJson : json["screens"].as<JsonArray>())
    {
      int screenIdx = screenJson["screen"].as<int>();
      createScreen(screenIdx);

      classScreen *screen = screenVault.get(screenIdx);
      screen->setLabel(screenJson["label"]);
      if (screenIdx != SCREEN_START)
        screen->setHidden(screenJson["hidden"].as<bool>());

      for (JsonVariant tileJson : screenJson["tiles"].as<JsonArray>())
      {
        jsonTilesConfig(screenIdx, tileJson);
      }
    }
  }
}

void screenConfigSchema(JsonVariant json)
{
  // screens
  JsonObject screens = json.createNestedObject("screens");
  screens["title"] = "Screens";
  screens["description"] = "Add one or more screens to your panel. Screen 1 is the home screen and is mandatory and can't be hidden. Index must be between 1-32.";
  screens["type"] = "array";

  JsonObject screensItems = screens.createNestedObject("items");
  screensItems["type"] = "object";

  JsonObject screensProperties = screensItems.createNestedObject("properties");

  JsonObject screen = screensProperties.createNestedObject("screen");
  screen["title"] = "Index";
  screen["type"] = "integer";
  screen["minimum"] = SCREEN_START;
  screen["maximum"] = SCREEN_END;

  JsonObject screenLabel = screensProperties.createNestedObject("label");
  screenLabel["title"] = "Label";
  screenLabel["type"] = "string";

  JsonObject screenHidden = screensProperties.createNestedObject("hidden");
  screenHidden["title"] = "Hidden";
  screenHidden["type"] = "boolean";

  JsonArray screensRequired = screensItems.createNestedArray("required");
  screensRequired.add("screen");
  screensRequired.add("label");

  // tiles on screen
  JsonObject tiles = screensProperties.createNestedObject("tiles");
  tiles["title"] = "Tiles";
  tiles["description"] = "Add one or more tiles to your screen. Index must be between 1-6. 'Linked Screen Index' required for 'link' tiles. 'Level Start/Stop' optional for 'buttonLevelXxx' tiles (defaults to 0/100).";
  tiles["type"] = "array";

  JsonObject tilesItems = tiles.createNestedObject("items");
  tilesItems["type"] = "object";

  JsonObject tilesProperties = tilesItems.createNestedObject("properties");

  JsonObject tile = tilesProperties.createNestedObject("tile");
  tile["title"] = "Index";
  tile["type"] = "integer";
  tile["minimum"] = TILE_START;
  tile["maximum"] = TILE_END;

  JsonObject style = tilesProperties.createNestedObject("style");
  style["title"] = "Style";
  createInputStyleEnum(style);

  JsonObject icon = tilesProperties.createNestedObject("icon");
  icon["title"] = "Icon";
  createIconEnum(icon);

  JsonObject tileLabel = tilesProperties.createNestedObject("label");
  tileLabel["title"] = "Label";
  tileLabel["type"] = "string";

  JsonObject link = tilesProperties.createNestedObject("link");
  link["title"] = "Linked Screen Index";
  link["type"] = "integer";
  link["minimum"] = SCREEN_START;
  link["maximum"] = SCREEN_END;

  JsonObject levelStart = tilesProperties.createNestedObject("levelStart");
  levelStart["title"] = "Level Start";
  levelStart["type"] = "integer";

  JsonObject levelStop = tilesProperties.createNestedObject("levelStop");
  levelStop["title"] = "Level Stop";
  levelStop["type"] = "integer";

  JsonArray tilesRequired = tilesItems.createNestedArray("required");
  tilesRequired.add("tile");
  tilesRequired.add("style");

  // background color
  JsonObject colorBackground = json.createNestedObject("colorBackground");
  colorBackground["title"] = "Background Color";
  colorBackground["description"] = "RGB color of screen background (defaults to black - R0, G0, B0).";
  createRgbProperties(colorBackground);

  // icon 'ON' color
  JsonObject colorIconOn = json.createNestedObject("colorIconOn");
  colorIconOn["title"] = "Icon Color";
  colorIconOn["description"] = "RGB color of icon when 'on' (defaults to light green - R91, G190, B91).";
  createRgbProperties(colorIconOn);

  // noActivity timeout
  JsonObject noActivitySecondsToHome = json.createNestedObject("noActivitySecondsToHome");
  noActivitySecondsToHome["title"] = "Home Screen Timeout (seconds)";
  noActivitySecondsToHome["description"] = "Return to home screen after a period of in-activity (defaults to 0 which disables the timeout). Must be a number between 0 and 600 (i.e. 10 minutes).";
  noActivitySecondsToHome["type"] = "integer";
  noActivitySecondsToHome["minimum"] = 0;
  noActivitySecondsToHome["maximum"] = 600;

  // noActivity timeout
  JsonObject noActivitySecondsToSleep = json.createNestedObject("noActivitySecondsToSleep");
  noActivitySecondsToSleep["title"] = "Screen Sleep Timeout (seconds)";
  noActivitySecondsToSleep["description"] = "Turn off screen backlight after a period of in-activity (defaults to 0 which disables the timeout). Must be a number between 0 and 3600 (i.e. 1 hour).";
  noActivitySecondsToSleep["type"] = "integer";
  noActivitySecondsToSleep["minimum"] = 0;
  noActivitySecondsToSleep["maximum"] = 3600;
}

void setConfigSchema()
{
  // Define our config schema
  StaticJsonDocument<4096> json;
  JsonVariant config = json.as<JsonVariant>();

  screenConfigSchema(config);

  // Pass our config schema down to the WT32 library
  wt32.setConfigSchema(config);
}

void addCustomApis()
{
  // Custom endpoint for downloading a snapshot of the WT32 display
  wt32.apiGet("/snapshot.bmp", &getApiSnapshot);
}

// decode base64 encoded png image to ps_ram
lv_img_dsc_t *decodeBase64ToImg(const char *imageBase64)
{
  // decode image into ps_ram
  // TODO :
  //    check if ps_alloc successful
  size_t inLen = strlen(imageBase64);
  // exit if no data to decode
  if (inLen == 0)
    return NULL;

  size_t outLen = BASE64::decodeLength(imageBase64);
  uint8_t *raw = (uint8_t *)ps_malloc(outLen);
  BASE64::decode(imageBase64, raw);

  // calc width and height from image file (start @ pos [16])
  uint32_t size[2];
  memcpy(&size[0], raw + 16, 8);

  // prepaare image descriptor
  lv_img_dsc_t *imgPng = (lv_img_dsc_t *)ps_malloc(sizeof(lv_img_dsc_t));
  imgPng->header.cf = LV_IMG_CF_RAW_ALPHA;
  imgPng->header.always_zero = 0;
  imgPng->header.reserved = 0;
  imgPng->header.w = (lv_coord_t)((size[0] & 0xff000000) >> 24) + ((size[0] & 0x00ff0000) >> 8);
  imgPng->header.h = (lv_coord_t)((size[1] & 0xff000000) >> 24) + ((size[1] & 0x00ff0000) >> 8);
  imgPng->data_size = outLen;
  imgPng->data = raw;

  return imgPng;
}

// convert json array of strings to single string
void jsonArrayToString(JsonArray array, string *longString)
{
  for (JsonVariant s : array)
  {
    *longString += s.as<const char *>();
    *longString += "\n";
  }
  longString->pop_back();
}

void jsonSetBackLightCommand(JsonVariant json)
{
  int blValue = -1;
  if (json.containsKey("brightness"))
  {
    blValue = json["brightness"].as<int>();
    if (blValue < 1)    blValue = 1;
    if (blValue > 100)  blValue = 100;
  }

  if (json.containsKey("state"))
  {
    if (strcmp(json["state"], "sleep") == 0)
    {
      _retainedBackLight = _actBackLight;
      blValue = 0;
    }
    if (strcmp(json["state"], "awake") == 0)
    {
      blValue = _retainedBackLight;
    }
  }
  
  if (blValue > -1)
  {
    lv_disp_trig_activity(NULL);
    _setBackLight(blValue);
  }
}

void jsonShowMessage(JsonVariant json)
{
  lv_obj_t *mbox1 = lv_msgbox_create(NULL, json["title"], json["text"], NULL, true);

  lv_obj_t *cbtn = lv_msgbox_get_close_btn(mbox1);
  lv_obj_set_style_bg_color(cbtn, lv_color_make(128, 30, 0), 0);
  lv_obj_set_style_bg_opa(cbtn, 255, 0);
  lv_obj_center(mbox1);

  lv_obj_add_event_cb(mbox1, messageClosedEventHandler, LV_EVENT_ALL, NULL);
  publishMessageEvent("open", "open");
}

void jsonScreenCommand(JsonVariant json)
{
  uint8_t r, g, b;

  int screenIdx = json["screen"].as<int>();
  if ((screenIdx < SCREEN_START) || (screenIdx > SCREEN_END))
  {
    wt32.print(F("[tp32] invalid screen: "));
    wt32.println(screenIdx);
    return;
  }

  classScreen *screen = screenVault.get(screenIdx);
  if (!screen)
  {
    wt32.print(F("[tp32] screen not found: "));
    wt32.println(screenIdx);
    return;
  }

  if (json.containsKey("backgroundColorRgb"))
  {
    r = (uint8_t)json["backgroundColorRgb"]["r"].as<int>();
    g = (uint8_t)json["backgroundColorRgb"]["g"].as<int>();
    b = (uint8_t)json["backgroundColorRgb"]["b"].as<int>();

    screen->setBgColor(r, g, b);
  }

  if (json.containsKey("footer"))
  {
    screen->setFooter(json["footer"]["left"], json["footer"]["center"], json["footer"]["right"]);
  }
}

void jsonTileCommand(JsonVariant json)
{
  uint8_t r, g, b;

  int screenIdx = json["screen"].as<int>();
  if ((screenIdx < SCREEN_START) || (screenIdx > SCREEN_END))
  {
    wt32.print(F("[tp32] invalid screen: "));
    wt32.println(screenIdx);
    return;
  }

  int tileIdx = json["tile"].as<int>();
  if ((tileIdx < TILE_START) || (tileIdx > TILE_END))
  {
    wt32.print(F("[tp32] invalid tile: "));
    wt32.println(tileIdx);
    return;
  }

  classTile *tile = tileVault.get(screenIdx, tileIdx);
  if (!tile)
  {
    wt32.print(F("[tp32] screen/tile not found: "));
    wt32.print(screenIdx);
    wt32.print(F("/"));
    wt32.println(tileIdx);
    return;
  }

  if (json.containsKey("state"))
  {
    const char *state = json["state"];
    int iState = -1;
    // decode state
    if (strcmp(state, "off") == 0)
    {
      iState = 0;
    }
    else if (strcmp(state, "on") == 0)
    {
      iState = 1;
    }
    // send if valid
    if(iState >= 0)
    {
      tile->setState(iState == 0 ? false : true);
      // send to colorpicker if active and belongs to tile which the state is sent to
      if(colorPicker.isActive() && (colorPicker.getTile()->tileId.id == tile->tileId.id))
      {
        colorPicker.setState(iState == 0 ? false : true);
      }
    }
    // throw error
    else
    {
      wt32.print(F("[tp32] invalid state: "));
      wt32.println(state);
    }
  }

  if (json.containsKey("subLabel"))
  {
    tile->setSubLabel(json["subLabel"]);
  }

  if (json.containsKey("level"))
  {
    tile->setLevel(json["level"].as<int>(), false);
  }

  if (json.containsKey("iconColorRgb"))
  {
    r = (uint8_t)json["iconColorRgb"]["r"].as<int>();
    g = (uint8_t)json["iconColorRgb"]["g"].as<int>();
    b = (uint8_t)json["iconColorRgb"]["b"].as<int>();

    tile->setColor(r, g, b);
  }

  if (json.containsKey("backgroundColorRgb"))
  {
    r = (uint8_t)json["backgroundColorRgb"]["r"].as<int>();
    g = (uint8_t)json["backgroundColorRgb"]["g"].as<int>();
    b = (uint8_t)json["backgroundColorRgb"]["b"].as<int>();

    tile->setBgColor(r, g, b);
  }

  if (json.containsKey("icon"))
  {
    tile->setIcon(iconVault.getIcon(json["icon"]));
  }

  if (json.containsKey("number"))
  {
    JsonVariant jsonNumber = json["number"];
    tile->setNumber(jsonNumber["value"], jsonNumber["units"], jsonNumber["subValue"], jsonNumber["subUnits"]);
  }

  if (json.containsKey("text"))
  {
    tile->setIconText(json["text"]);
  }

  if (json.containsKey("disable"))
  {
    tile->setTileDisabled(json["disable"]);
  }

  if (json.containsKey("backgroundImage"))
  {
    JsonVariant jsonBgImage = json["backgroundImage"];

    if (jsonBgImage.size() == 0)
    {
      tile->setBgImage(NULL);
    }
    else
    {
      if (jsonBgImage.containsKey("imageBase64"))
      {
        tile->setBgImage(decodeBase64ToImg(jsonBgImage["imageBase64"]));
      }
      tile->alignBgImage(jsonBgImage["zoom"], jsonBgImage["offset"][0], jsonBgImage["offset"][1], jsonBgImage["angle"]);
    }
  }

  if (json.containsKey("dropDownList"))
  {
    string dropDownList = "";
    jsonArrayToString(json["dropDownList"].as<JsonArray>(), &dropDownList);
    tile->setDropDownList(dropDownList);
  }

  if (json.containsKey("dropDownSelect"))
  {
    tile->setDropDownIndex(json["dropDownSelect"].as<uint>());
  }

  if (json.containsKey("dropDownLabel"))
  {
    tile->setDropDownLabel(json["dropDownLabel"]);
  }

  if (json.containsKey("selectorList"))
  {
    string selectorList = "";
    jsonArrayToString(json["selectorList"].as<JsonArray>(), &selectorList);
    tile->saveDropDownList(selectorList);
  }

  if (json.containsKey("selectorSelect"))
  {
    tile->saveDropDownIndex(json["selectorSelect"].as<uint>());
  }

  if (json.containsKey("colorPicker"))
  {
    JsonVariant jsonColorPicker = json["colorPicker"];

    if (jsonColorPicker.containsKey("colorRgb"))
    {
      r = (uint8_t)jsonColorPicker["colorRgb"]["r"].as<int>();
      g = (uint8_t)jsonColorPicker["colorRgb"]["g"].as<int>();
      b = (uint8_t)jsonColorPicker["colorRgb"]["b"].as<int>();

      tile->setColorPickerRGB(r, g, b);
    }

    if (jsonColorPicker.containsKey("colorKelvin"))
    {
      tile->setColorPickerKelvin(jsonColorPicker["colorKelvin"]);
    }

    if (jsonColorPicker.containsKey("brightness"))
    {
      tile->setColorPickerBrightnessWhite(jsonColorPicker["brightness"]);
    }

    if (jsonColorPicker.containsKey("mode"))
    {
      const char *mode = jsonColorPicker["mode"];
      if (strcmp(mode, "colorKelvin") == 0)
      {
        tile->setColorPickerMode(CP_MODE_TEMP);
      }
      else if (strcmp(mode, "colorRgb") == 0)
      {
        tile->setColorPickerMode(CP_MODE_COLOR);
      }
      else
      {
        wt32.print(F("[tp32] invalid mode: "));
        wt32.println(mode);
      }
    }
  }

  if (json.containsKey("keyPad"))
  {
    // exit early if no active keypad
    if (!keyPad.isActive())
      return;

    // exit early if not addressed to the active keypad
    if (keyPad.getTile()->tileId.id != tile->tileId.id)
      return;

    JsonVariant jsonKeyPad = json["keyPad"];

    const char * state = jsonKeyPad["state"];

    // close keypad and exit early
    if (strcmp(state, "close") == 0)
    {
      keyPad.close();
    }
    else
    {
      const void * icon = jsonKeyPad.containsKey("icon") ? iconVault.getIcon(jsonKeyPad["icon"]) : NULL;
      const char * text = jsonKeyPad.containsKey("text") ? jsonKeyPad["text"] : jsonKeyPad["state"];

      uint8_t r, g, b;

      if (jsonKeyPad.containsKey("iconColorRgb"))
      {
        r = (uint8_t)jsonKeyPad["iconColorRgb"]["r"].as<int>();
        g = (uint8_t)jsonKeyPad["iconColorRgb"]["g"].as<int>();
        b = (uint8_t)jsonKeyPad["iconColorRgb"]["b"].as<int>();
      }
      
      lv_color_t color = lv_color_make(r, g, b);

      keyPad.setState(state, icon, color, text);
    }
  }

  if (json.containsKey("thermostat"))
  {
    JsonVariant jsonThermostat = json["thermostat"];

    if (jsonThermostat.containsKey("modeList"))
    {
      string modeList = "";
      jsonArrayToString(jsonThermostat["modeList"].as<JsonArray>(), &modeList);
      tile->saveDropDownList(modeList);
      tile->setDropDownLabel("Select mode ...");
    }

    if (jsonThermostat.containsKey("mode"))
    {
      tile->saveDropDownIndex(jsonThermostat["mode"]);
    }

    if (jsonThermostat.containsKey("currentTemperature"))
    {
      tile->setThermostatCurrent(jsonThermostat["currentTemperature"]);
    }

    if (jsonThermostat.containsKey("targetTemperature"))
    {
      tile->setThermostatTarget(jsonThermostat["targetTemperature"]);
    }

    if (jsonThermostat.containsKey("units"))
    {
      tile->setThermostatUnits(jsonThermostat["units"]);
    }
  }
}

// add icon from bas64 coded .png image
void jsonAddIcon(JsonVariant json)
{
  // decode image into ps_ram
  // TODO :
  //    check if ps_alloc successful

  if (!json["name"] || !json["imageBase64"]) return;
  
  // check if named icon exist, if yes -> get descriptor
  lv_img_dsc_t *oldIcon = (lv_img_dsc_t *)iconVault.getIcon(json["name"]);

  // decode new icon
  lv_img_dsc_t *iconPng = decodeBase64ToImg(json["imageBase64"]);

  // add custom icon to iconVault (deletes possible existing one)
  string iconStr = json["name"];
  iconVault.add({iconStr, iconPng});

  // free ps_ram heap if named icon existed allready
  if (oldIcon)
  {
    free((void *)oldIcon->data);
    free(oldIcon);
  }

  // update configutation
  setConfigSchema();
}

void jsonCommand(JsonVariant json)
{
  if (json.containsKey("backlight"))
  {
    jsonSetBackLightCommand(json["backlight"]);
  }

  if (json.containsKey("message"))
  {
    jsonShowMessage(json["message"]);
  }

  if (json.containsKey("screen"))
  {
    screenVault.show(json["screen"]["load"].as<int>());
  }

  if (json.containsKey("screens"))
  {
    for (JsonVariant screen : json["screens"].as<JsonArray>())
    {
      jsonScreenCommand(screen);
    }
  }

  if (json.containsKey("tiles"))
  {
    for (JsonVariant tile : json["tiles"].as<JsonArray>())
    {
      jsonTileCommand(tile);
    }
  }

  if (json.containsKey("addIcon"))
  {
    jsonAddIcon(json["addIcon"]);
  }
}

/**
  init and start LVGL pages
  */

// define the defaults for lvgl objects
static lv_style_t style_my_btn;

static void new_theme_apply_cb(lv_theme_t * th, lv_obj_t * obj)
{
  LV_UNUSED(th);
  if ((lv_obj_check_type(obj, &lv_btn_class)) || (lv_obj_check_type(obj, &lv_imgbtn_class)))
  {
    lv_obj_add_style(obj, &style_my_btn, 0);
  }
}

static void new_theme_init_and_set(void)
{
  /*Initialize the styles*/
  lv_style_init(&style_my_btn);
  lv_style_set_bg_color(&style_my_btn, lv_color_hex(0xffffff));
  lv_style_set_radius(&style_my_btn, 5);
  /*Initialize the new theme from the current theme*/
  lv_theme_t *th_act = lv_disp_get_theme(NULL);
  static lv_theme_t th_new;
  th_new = *th_act;
  /*Set the parent theme and the style apply callback for the new theme*/
  lv_theme_set_parent(&th_new, th_act);
  lv_theme_set_apply_cb(&th_new, new_theme_apply_cb);
  /*Assign the new theme to the current display*/
  lv_disp_set_theme(NULL, &th_new);
}

// initialize ui
void ui_init(void)
{
  new_theme_init_and_set();

  // HomeScreen
  createScreen(SCREEN_HOME);

  // setup Settings Screen as screen[SCREEN_SETTINGS]
  classScreen &ref = screenVault.add(SCREEN_SETTINGS, 0);
  screenSettings = classScreenSettings(ref.screen, imgOxrsSettings);
  screenSettings.addEventHandler(backLightSliderEventHandler);
  ref.createHomeButton(footerButtonEventHandler, imgHome);
  ref.adScreenEventHandler(screenEventHandler);
  ref.setLabel("Settings");
}

/**
  Setup
*/
void setup()
{
  // start serial and let settle
  Serial.begin(SERIAL_BAUD_RATE);
  delay(2000);
  Serial.println(F("[tp32] starting up..."));

  // initialise the Tile_Style_LUT and Img_LUT for later use
  initStyleLut();
  initIconVault();

  // set up for backlight dimming (PWM)
  ledcSetup(BL_PWM_CHANNEL, BL_PWM_FREQ, BL_PWM_RESOLUTION);
  ledcAttachPin(TFT_BL, BL_PWM_CHANNEL);
  ledcWrite(BL_PWM_CHANNEL, 0);

  // start lvgl
  lv_init();
  lv_img_cache_set_size(10);
  Serial.print(F("[tp32] lvgl starting v"));
  Serial.print(lv_version_major());
  Serial.print(F("."));
  Serial.print(lv_version_minor());
  Serial.print(F("."));
  Serial.println(lv_version_patch());
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); // register print function for debugging
#endif

  // start tft library
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // touch pad
  ft6336u.begin();
  pinMode(39, INPUT);

  // initialise draw buffer
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);
  // initialise the display
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  // settings for display driver
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // initialise the input device driver (touch panel)
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  myInputDevice = lv_indev_drv_register(&indev_drv);
  // set timings for LongPress ,RepeatTime and gesture detect
  indev_drv.long_press_time = 500;
  indev_drv.long_press_repeat_time = 200;
  indev_drv.gesture_limit = 40;
  indev_drv.scroll_limit = 3;

  // set colors to default, may be updated later from config handler
  setIconOnColor(0, 0, 0);
  setBackgroundColor(0, 0, 0);

  // show splash screen
  _setBackLightLED(20);
  lv_obj_t *img1 = lv_img_create(lv_scr_act());
  lv_img_set_src(img1, imgOxrsSplash);
  lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
  lv_timer_handler();
  _actBackLight = 50;
  _setBackLightLED(_actBackLight);
  _retainedBackLight = _actBackLight;

  // start the screen to make sure everything is initialised
  ui_init();

  // draw the color wheel to draw only once and save time when used
  createColorWheel();

  // start WT32 hardware
  wt32.begin(jsonConfig, jsonCommand);

  // set up config/command schema (for self-discovery and adoption)
  setConfigSchema();

  // add any custom API endpoints
  addCustomApis();

  // prepare info text
  updateInfoText();

  // show HomeScreen
  screenVault.show(SCREEN_HOME);
}

/**
  Main processing loop
*/
void loop()
{
  // let WT32 hardware handle any events etc
  wt32.loop();

  // update our connection status
  updateConnectionStatus();

  // check any internal timers
  checkNoActivity();

  // let the GUI do its work
  lv_timer_handler();

  // TODO: is this needed?
  delay(3);
}

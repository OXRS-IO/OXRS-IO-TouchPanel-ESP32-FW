#pragma once
#include <list>
#include <lvgl.h>
#include <globalDefines.h>
#include <classScreen.h>

#define ARC_STEP 5

// object to handle feed posts
class post
{
public:
  string head = "";
  string body = "";
  int id = 0;

  post(int _id, string h, string b)
  {
    id = _id;
    head = h;
    body = b;
  };
};

class classTile
{
protected:
  lv_obj_t *_parentContainer = NULL;
  lv_obj_t *_tileBg = NULL;
  lv_obj_t *_tileFg = NULL;
  lv_obj_t *_btn = NULL;
  lv_obj_t *_label = NULL;
  lv_obj_t *_subLabel = NULL;
  lv_obj_t *_linkedLabel = NULL;
  lv_obj_t *_unitLabel = NULL;
  lv_obj_t *_valueLabel = NULL;
  lv_obj_t *_subValueLabel = NULL;
  lv_obj_t *_btnUp = NULL;
  lv_obj_t *_btnDown = NULL;
  lv_obj_t *_ovlPanel = NULL;
  lv_obj_t *_bar = NULL;
  lv_obj_t *_txtIconText = NULL;
  lv_obj_t *_imgBg = NULL;
  lv_obj_t *_roller = NULL;
  lv_obj_t *_arcTarget = NULL;
  lv_obj_t *_labelArcValue = NULL;
  lv_obj_t *_labelArcSubValue = NULL;

  classScreen *_parentScreen = NULL;
  int _style = 0;
  string _styleStr;
  int _linkedScreen = 0;
  bool _state = false;
  int _levelStart = 0;
  int _levelStop = 100;
  int _levelLargeStep = 5;
  int _level = 0;
  bool _topDownMode = false;
  const void *_img = NULL;
  const void *_imgOn = NULL;
  const void *_imgConfig = NULL;
  const void *_imgOnConfig = NULL;
  uint16_t _dropDownIndex = 0;
  string _dropDownList;
  string _dropDownLabel;
  lv_color32_t _colorPickerRGB32 = {255, 255, 255};
  int _colorPickerKelvin = 4000;
  int _colorPickerBrightnessWhite = 50;
  int _colorPickerMode = CP_MODE_COLOR;
  int _thermostatTarget = 0;
  int _thermostatCurrent = 0;
  string _units = "";
  lv_color_t _tileBgColor = {0, 0, 0};

  std::list<post> _feed;
  std::list<post>::iterator _feedIterator;

  void _button(lv_obj_t *parent, const void *img);
  void _reColorAll(lv_color_t color, lv_style_selector_t selector);
  void _setIconTextFromIndex(void);
  void _freeImageHeap();
  void _hideIcon(bool hide);
  bool _isThumbNail(const void *img);
  void _hideThumbNail(bool hide);

public : 
  tileId_t tileId;
  lv_obj_t *btn = NULL;

  classTile(void){};
  ~classTile();

  void begin(lv_obj_t *parent, classScreen *parentScreen, int tileIdx, const void *img, const char *labelText, int style, const char* styleStr);
  void setSubLabel(const char *subLabelText);
  void setState(bool state);
  void setColor(lv_color_t color);
  lv_color_t getColor();
  void setBgColor(lv_color_t color);
  void updateBgColor(void);
  void setIcon(const void *imgIcon);
  void setNumber(const char *value, const char *units, const char *subValue, const char *subUnits);
  void setBgImage(lv_img_dsc_t *img);
  void alignBgImage(int zoom, int posOffsX, int posOffsY, int angle);
  void setIconForStateOn(const void* imgStateOn);
  void setIconText(const char *iconText);
  void getImages(const void* &imgOff, const void* &imgOn);
  void setActionIndicator(const char* symbol);
  void setTileDisabled(bool disable);

  void setLink(int linkedScreen);
  int getLink(void);
  tileId_t getId(void);
  int getScreenIdx(void);
  int getTileIdx(void);
  int getStyle(void);
  const char* getStyleStr(void);
  bool getState(void);
  char *getLabel(void);
  
  void addEventHandler(lv_event_cb_t callBack);

  void setLevelStartStop(int start, int stop);
  void setLevel(int level, bool force);
  int getLevel(void);
  int getLevelStart(void);
  int getLevelStop(void);
  int getLevelLargeStep(void);
  void setTopDownMode(bool enable);
  void showOvlBar(int level);
  void addUpDownControl(lv_event_cb_t upDownEventHandler, const void* imgUpperButton, const void* imgLowerButton);
 
  void setDropDownList(string list);
  void setDropDownIndex(uint16_t index);
  void setDropDownLabel(const char *label);
  void saveDropDownList(string list);
  void saveDropDownIndex(uint16_t index);
  const char *getDropDownList(void);
  uint16_t getDropDownIndex(void);
  const char *getDropDownLabel(void);

  void showSelector(int index);
  bool getSelectorValid(void);

  void setColorPickerRGB(int r, int g, int b);
  void setColorPickerRGB(lv_color32_t rgb);
  void setColorPickerKelvin(int kelvin);
  void setColorPickerBrightnessWhite(int brightness);
  void setColorPickerMode(int mode);
  lv_color32_t getColorPickerRGB(void);
  int getColorPickerKelvin(void);
  int getColorPickerBrightnessWhite(void);
  int getColorPickerMode(void);

  void setThermostatTarget(int target);
  int getThermostatTarget(void);
  void setThermostatCurrent(int current);
  int getThermostatCurrent(void);
  void setThermostatUnits(const char *units);
  const char *getThermostatUnits(void);
  void updateThermostatDisplay(void);

  void addPost(int id, const char *head, const char *body);
  void removePost(int id);
  std::list<post>::iterator getFeedIterator(void);
  int getFeedSize(void);
};
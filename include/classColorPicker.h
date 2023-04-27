#pragma once
#include <classPopUpContainer.h>

class classColorPicker : public classPopUpContainer

{
private:
  lv_obj_t *_labelCallingTileRGB = NULL;
  lv_obj_t *_labelCallingTileCCT = NULL;
  lv_obj_t *_panelRGB = NULL;
  lv_obj_t *_panelCCT = NULL;
  lv_obj_t *_colorWheel = NULL;
  lv_obj_t *_labelKelvin = NULL;
  lv_obj_t *_labelKelvinValue = NULL;
  lv_obj_t *_labelBrightnessColor = NULL;
  lv_obj_t *_labelBrightnessColorValue = NULL;
  lv_obj_t *_labelBrightnessWhite = NULL;
  lv_obj_t *_labelBrightnessWhiteValue = NULL;
  lv_obj_t *_panelKelvin = NULL;
  lv_obj_t *_barKelvinLow = NULL;
  lv_obj_t *_barKelvinHigh = NULL;
  lv_obj_t *_sliderKelvin = NULL;
  lv_obj_t *_barBrightnessColor = NULL;
  lv_obj_t *_sliderBrightnessColor = NULL;
  lv_obj_t *_barBrightnessWhite = NULL;
  lv_obj_t *_sliderBrightnessWhite = NULL;
  lv_obj_t *_panelPreview = NULL;
  lv_obj_t *_labelCursor = NULL;
  lv_obj_t *_imgCursor = NULL;
  lv_obj_t *_btnColor = NULL;
  lv_obj_t *_btnKelvin = NULL;
  lv_obj_t *_panelCursor = NULL;
  lv_obj_t *_imgCw = NULL;
  lv_obj_t *_panelCwFrame = NULL;
  lv_obj_t *_btn = NULL;
  lv_obj_t *_imgBtn = NULL;

  void _createColorPicker(lv_img_dsc_t *imgCw);

public:
  classColorPicker(void) : classPopUpContainer() {};
  classColorPicker(classTile *tile, lv_event_cb_t colorPickerEventHandler, lv_event_cb_t ColorPickerCwEventHandler, lv_img_dsc_t *imgCw, int cpMode);
  void updateAll(void);
  void updateCw(lv_point_t point, int mode);
  void updatePanelRGB(lv_color32_t rgb);
  void switchMode(int cpMode);
  void setState(bool state);
};

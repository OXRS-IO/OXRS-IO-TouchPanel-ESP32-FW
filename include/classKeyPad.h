#pragma once
#include <classPopUpContainer.h>

class classKeyPad : public classPopUpContainer

{
private:
  lv_obj_t *_btnm1 = NULL;
  lv_obj_t *_imgLockState = NULL;
  lv_obj_t *_label = NULL;
  lv_obj_t *pwd_ta = NULL;
  const void *_imgLocked;
  const void *_imgUnLocked;

  void _createKeyPad(void);

public:
  classKeyPad(void){};
  classKeyPad(classTile *tile, lv_event_cb_t keyPadEventHandler);

  void addChar(char value);
  void delChar(void);

  void setLocked(lv_color_t color, const char *text);
  void setUnlocked(lv_color_t color, const char *text);
  void setFailed(lv_color_t color, const char *text);
  void setState(const void *img, lv_color_t color, const char *text);

  const char *getKey(void);
};

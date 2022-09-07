#pragma once
#include <classPopUpContainer.h>

class classKeyPad : public classPopUpContainer

{
private:
  lv_obj_t *_btnm1 = NULL;
  lv_obj_t *_imgLockState = NULL;
  lv_obj_t *_label = NULL;
  lv_obj_t *pwd_ta = NULL;

  void _createKeyPad(void);
  const void *_imgUnLocked, *_imgLocked;

public:
  classKeyPad(void){};
  classKeyPad(classTile *tile, lv_event_cb_t keyPadEventHandler);

  void addChar(char value);
  void delChar(void);

  void setState(const char *state, const void *icon, lv_color_t color, const char *text);

  const char *getKey(void);
};

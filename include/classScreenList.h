﻿#pragma once
#include <lvgl.h>
#include <classScreen.h>
#include <list>
#include <algorithm>
class classScreenList
{
private:
  // THE list that holds all screens
  std::list<classScreen> _listScreens;
  // common iterator
  std::list<classScreen>::iterator it;
  // next iterator
  std::list<classScreen>::iterator _itNext;

public:
  classScreenList(void){};
  classScreen &add(int screenIdx, int style, int cols = SCREEN_COLS, int rows = SCREEN_ROWS);
  classScreen *get(int screenIdx);

  void setIteratorStart(void);
  classScreen *getNextScreen(void);
  classScreen* getNextScreen(int screenIdx);
  classScreen* getPreviousScreen(int screenIdx);

  void remove(int screenIdx);
  bool exist(int screenIdx);
  void sort(void);
  bool show(int screenIdx);
  int makeDropDownList(char *list, lv_obj_t *screenObj);
  void showByIndex(int index);
  int getSize(void);
};

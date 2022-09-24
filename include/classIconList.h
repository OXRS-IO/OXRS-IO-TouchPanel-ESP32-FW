#pragma once
#include <lvgl.h>
#include "globalDefines.h"

class classIconList
{
private:
  // THE list that holds all icons
  std::list<imgListElement_t> _listIcons;
  // next iterator
  std::list<imgListElement_t>::iterator _itNext;

public:
  classIconList(void){};
  void add(imgListElement_t element);
  void remove(string iconStr);
  void sort(void);
  const void *getIcon(string iconStr);
  void setIteratorStart(void);
  string getNextIconStr(void);
  int size(void);
};

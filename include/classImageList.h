#pragma once
#include <lvgl.h>
#include "globalDefines.h"

class classImageList
{
private:
  // THE list that holds all icons
  std::list<imgListElement_t> _listImages;
  // next iterator
  std::list<imgListElement_t>::iterator _itNext;

public:
  classImageList(void){};
  void add(imgListElement_t element);
  void remove(string imageStr);
  void sort(void);
  const void *get(string imageStr);
  void setIteratorStart(void);
  string getNextImageStr(void);
  int size(void);
};

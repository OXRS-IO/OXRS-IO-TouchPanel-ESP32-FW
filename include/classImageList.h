#pragma once
#include <lvgl.h>
#include "globalDefines.h"

class classImageList
{
private:
  // THE list that holds all icons
  std::list<tp32Image> _listImages;
  // next iterator
  std::list<tp32Image>::iterator _itNext;

public:
  classImageList(void){};
  void add(tp32Image element);
  void remove(string imageStr);
  void sort(void);
  tp32Image get(string imageStr);
  void setIteratorStart(void);
  string getNextImageStr(void);
  int size(void);
};

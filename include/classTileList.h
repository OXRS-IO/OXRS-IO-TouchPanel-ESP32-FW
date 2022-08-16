#pragma once
#include <lvgl.h>
#include <classTile.h>

class classTileList
{
private:
public:
  classTileList(void){};
 
  classTile &add(void);
  classTile *get(int screenIdx, int tileIdx);
  void remove(int screenIdx, int tileIdx);
 
  int getSize(void);
};
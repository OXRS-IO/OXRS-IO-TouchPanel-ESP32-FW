#pragma once
#include <lvgl.h>
#include <classTile.h>
#include <list>
#include <algorithm>

class classTileList
{
private:
  // THE list that holds all tiles
  std::list<classTile> _listTiles;
  // next iterator
  std::list<classTile>::iterator _itNext;

public:
  classTileList(void){};
 
  classTile &add(void);
  classTile *get(int screenIdx, int tileIdx);
  void remove(int screenIdx, int tileIdx);
  void setIteratorStart(void);
  classTile *getNextTile(void);
 
  int getSize(void);
};
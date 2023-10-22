#include "classScreenList.h"
#include <stdio.h>

using std::list;


// helper for find unhidden screens
bool _isNotHidden(classScreen &screen)
{
  return !screen.isHidden();
}

classScreen &classScreenList::add(int screenIdx, int style, int cols, int rows)
{
  _listScreens.emplace_back(classScreen());
  _listScreens.back().begin(screenIdx, style, cols, rows);
  return _listScreens.back();
}

classScreen *classScreenList::get(int screenIdx)
{
  it = std::find_if(std::begin(_listScreens), std::end(_listScreens),
                    [&](classScreen const &p)
                    { return p.screenIdx == screenIdx; });
  if (it == _listScreens.end())
  {
    return NULL;
  }
  else
  {
    return it.operator->();
  }
}

bool classScreenList::exist(int screenIdx)
{
  it = std::find_if(std::begin(_listScreens), std::end(_listScreens),
                    [&](classScreen const &p)
                    { return p.screenIdx == screenIdx; });
  if (it == _listScreens.end())
  {
      return false;
  }
  else
  {
      return true;
  }
}

// remove tile[screen][tile] from list (if exist)
void classScreenList::remove(int screenIdx)
{
  _listScreens.remove_if([&](classScreen const &p)
                         { return p.screenIdx == screenIdx; });
}

// sort screen list in ascending order
void classScreenList::sort(void)
{
_listScreens.sort(
    [&](classScreen const &p1, classScreen const &p2)
    { return p1.screenIdx < p2.screenIdx; });
}

// show selected screen if available
bool classScreenList::show(int screenIdx)
{
  if (classScreen *sPtr = get(screenIdx))
  {
    lv_disp_load_scr(sPtr->screen);
    return true;
  }
  else
  {
    return false;
  }
}

// make list to show available screens in drop down
// return drop down list index of current screen
int classScreenList::makeDropDownList(char *list, lv_obj_t *screenObj)
{
  int listIndex = 0;
  int retVal = 0;
  list[0] = 0;
  it = std::find_if(std::begin(_listScreens), std::end(_listScreens), _isNotHidden);
  strcat(list, it->getLabel());
  while ((it = std::find_if(++it, std::end(_listScreens), _isNotHidden)) != _listScreens.end())
  {
    strcat(list, "\n");
    strcat(list, it->getLabel());
    listIndex++;
    if (it->screen == screenObj)
      retVal = listIndex;
  }

  return retVal;
}

// show screen from index of drop down list
void classScreenList::showByIndex(int index)
{
  it = std::find_if(std::begin(_listScreens), std::end(_listScreens), _isNotHidden);
  while (index--)
  {
    it = std::find_if(++it, std::end(_listScreens), _isNotHidden);
  }
  lv_disp_load_scr(it->screen);
}

// set the iterator to start
void classScreenList::setIteratorStart(void)
{
  _itNext = _listScreens.begin();
}

// return iterator and advance iterator to next
classScreen *classScreenList::getNextScreen(void)
{
  if (_itNext == _listScreens.end())
  {
    return NULL;
  }
  else
  {
    return _itNext++.operator->();
  }
}

// return size from list
int classScreenList::getSize(void)
{
  return _listScreens.size();
}


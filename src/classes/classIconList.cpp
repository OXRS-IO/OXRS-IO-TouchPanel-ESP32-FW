#include <list>
#include <algorithm>
#include <string>
#include "classIconList.h"
#include "globalDefines.h"

using std::list;
using std::string;

// add icon to list, replaces if exist, sorts list in accending iconStr order
void classIconList::add(imgListElement_t element)
{
  remove(element.iconStr);
  _listIcons.push_back(element);
  sort();
}

// get icom image for iconStr from list
const void *classIconList::getIcon(string iconStr)
{
  std::list<imgListElement_t>::iterator it;
  it = std::find_if(std::begin(_listIcons), std::end(_listIcons),
                    [&](imgListElement_t const &p)
                    { return p.iconStr == iconStr; });
  if (it == _listIcons.end())
    return NULL;
  else
    return it->img;
}

// remove icon from list (if exist)
void classIconList::remove(string iconStr)
{
  _listIcons.remove_if([&](imgListElement_t const &p)
                       { return p.iconStr == iconStr; });
}

// set iterator to 1st element to prepare for getNext()
void classIconList::setIteratorStart(void)
{
  _iterator = begin(_listIcons);
}

// get next iconStr
string classIconList::getNextStr(void)
{
  if (_iterator == end(_listIcons))
    return "";
  else
    return _iterator++->iconStr;
}

// sort list by iconStr
void classIconList::sort(void)
{
  _listIcons.sort([](const imgListElement_t &p1, const imgListElement_t &p2)
                  { return p1.iconStr < p2.iconStr; });
}

// return size from list
int classIconList::size(void)
{
  return _listIcons.size();
}

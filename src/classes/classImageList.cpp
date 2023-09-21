#include <list>
#include <algorithm>
#include <string>
#include "classImageList.h"
#include "globalDefines.h"

using std::list;
using std::string;

// add icon to list, replaces if exist, sorts list in accending imageStr order
void classImageList::add(imgListElement_t element)
{
  remove(element.imageStr);
  _listImages.push_back(element);
  sort();
}

// get icom image for imageStr from list
const void *classImageList::get(string imageStr)
{
  std::list<imgListElement_t>::iterator it;
  it = std::find_if(std::begin(_listImages), std::end(_listImages),
                    [&](imgListElement_t const &p)
                    { return p.imageStr == imageStr; });
  if (it == _listImages.end())
    return NULL;
  else
    return it->img;
}

// remove icon from list (if exist)
void classImageList::remove(string imageStr)
{
  _listImages.remove_if([&](imgListElement_t const &p)
                       { return p.imageStr == imageStr; });
}

// set iterator to 1st element to prepare for getNext()
void classImageList::setIteratorStart(void)
{
  _itNext = _listImages.begin();
}

// get next imageStr
string classImageList::getNextImageStr(void)
{
  if (_itNext == _listImages.end())
    return "";
  else
    return _itNext++->imageStr;
}

// sort list by imageStr
void classImageList::sort(void)
{
  _listImages.sort([](const imgListElement_t &p1, const imgListElement_t &p2)
                  { return p1.imageStr < p2.imageStr; });
}

// return size from list
int classImageList::size(void)
{
  return _listImages.size();
}

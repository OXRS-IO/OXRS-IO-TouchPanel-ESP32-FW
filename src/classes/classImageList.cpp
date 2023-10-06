#include <list>
#include <algorithm>
#include <string>
#include "classImageList.h"
#include "globalDefines.h"

using std::list;
using std::string;

// add icon to list, replaces if exist, sorts list in accending imageStr order
void classImageList::add(tp32Image element)
{
  remove(element.imageStr);
  _listImages.push_back(element);
  sort();
}

// get icom image for imageStr from list
tp32Image classImageList::get(string imageStr)
{
  tp32Image notFound = { "", NULL };
  std::list<tp32Image>::iterator it;
  it = std::find_if(std::begin(_listImages), std::end(_listImages),
                    [&](tp32Image const &p)
                    { return p.imageStr == imageStr; });
  if (it == _listImages.end())
      return notFound;
  else
      return *it;
}

// remove icon from list (if exist)
void classImageList::remove(string imageStr)
{
  _listImages.remove_if([&](tp32Image const &p)
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
  _listImages.sort([](const tp32Image &p1, const tp32Image &p2)
                  { return p1.imageStr < p2.imageStr; });
}

// return size from list
int classImageList::size(void)
{
  return _listImages.size();
}

#include <classTileList.h>

using std::list;

classTile &classTileList::add(void)
{
  _listTiles.emplace_back(classTile());
  return _listTiles.back();
}

classTile *classTileList::get(int screenIdx, int tileIdx)
{
  tileId_t searchId;
  searchId.idx.screen = screenIdx;
  searchId.idx.tile = tileIdx;
  std::list<classTile>::iterator it;
  it = std::find_if(std::begin(_listTiles), std::end(_listTiles),
                    [&](classTile const &p)
                    { return p.tileId.id == searchId.id; });
  if (it == _listTiles.end())
  {
    return NULL;
  }
  else
  {
    return it.operator->();
  }
}

// remove tile[screen][tile] from list (if exist)
void classTileList::remove(int screenIdx, int tileIdx)
{
  tileId_t searchId;
  searchId.idx.screen = screenIdx;
  searchId.idx.tile = tileIdx;

  _listTiles.remove_if([&](classTile const &p)
                       { return p.tileId.id == searchId.id; });
}

// return size from list
int classTileList::getSize(void)
{
  return _listTiles.size();
}

// set the iterator to start
void classTileList::setIteratorStart(void)
{
  _itNext = _listTiles.begin();
}

// return iterator and advance iterator to next
classTile *classTileList::getNextTile(void)
{
  if (_itNext == _listTiles.end())
  {
    return NULL;
  }
  else
  {
    return _itNext++.operator->();
  }
}

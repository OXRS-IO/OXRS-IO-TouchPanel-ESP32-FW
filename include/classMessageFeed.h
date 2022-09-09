#pragma once
#include <list>
#include <globalDefines.h>
#include <classPopUpContainer.h>
#include <classTile.h>

class classMessageFeed : public classPopUpContainer
{
private:

public:
  classMessageFeed(void) : classPopUpContainer(){};
  classMessageFeed(classTile *tile);
};
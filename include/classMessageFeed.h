#pragma once
#include <globalDefines.h>
#include <list>
#include <classPopUpContainer.h>

class classMessageFeed : public classPopUpContainer

{
private:
  class post
  {
  public:
    string head = "";
    string body = "";
    int id = 0;

    post(int _id, string h, string b)
    {
      id = _id;
      head = h;
      body = b;
    };
  };

  std::list<post> _feed;
  std::list<post>::iterator _feedIterator;

public:
  classMessageFeed(void) : classPopUpContainer(){};
  void show(void);

  void addPost(int id, const char *head, const char *body);
  void removePost(int id);
};
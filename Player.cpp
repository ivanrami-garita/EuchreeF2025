#include "Player.hpp"
#include "Card.hpp"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// ----- SimplePlayer class -----
class SimplePlayer : public Player {
private:
  string name;
  vector<Card> hand;

public:
  SimplePlayer(const string &name_in) : name(name_in) {}

  const string & get_name() const override { return name; }

  void add_card(const Card &c) override {
    hand.push_back(c);
  }

  bool make_trump(const Card &upcard, bool is_dealer,
                  int round, Suit &order_up_suit) const override {
    assert(false); // TODO: implement later
  }

  void add_and_discard(const Card &upcard) override {
    assert(false); // TODO: implement later
  }

  Card lead_card(Suit trump) override {
    assert(false); // TODO: implement later
  }

  Card play_card(const Card &led_card, Suit trump) override {
    assert(false); // TODO: implement later
  }
};

// ----- HumanPlayer class -----
class HumanPlayer : public Player {
private:
  string name;
  vector<Card> hand;

public:
  HumanPlayer(const string &name_in) : name(name_in) {}

  const string & get_name() const override {
    return name;
  }

  void add_card(const Card &c) override {
    hand.push_back(c);
  }

  bool make_trump(const Card &upcard, bool is_dealer,
                  int round, Suit &order_up_suit) const override {
    assert(false); // TODO: implement later
  }

  void add_and_discard(const Card &upcard) override {
    assert(false); // TODO: implement later
  }

  Card lead_card(Suit trump) override {
    assert(false); // TODO: implement later
  }

  Card play_card(const Card &led_card, Suit trump) override {
    assert(false); // TODO: implement later
  }
};

// ----- Factory and operator<< -----
Player * Player_factory(const string &name, const string &strategy) {
  if (strategy == "Simple") {
    return new SimplePlayer(name);
  } else if (strategy == "Human") {
    return new HumanPlayer(name);
  }
  assert(false);
  return nullptr;
}

ostream & operator<<(ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}

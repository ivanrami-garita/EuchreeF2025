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
  SimplePlayer(const string &name_in) : 
    name(name_in) {}

  const string & get_name() const override 
    {return name;}

  void add_card(const Card &c) override {
    hand.push_back(c);
  }

  bool make_trump(const Card &upcard, bool is_dealer,
                  int round, Suit &order_up_suit) const override {
   int trump_in_hand_r1 = 0;
   for(int i = 0; i < 4; i++){
    if ((hand[i].get_suit() == upcard.get_suit() &&     // add 1 if suit is trump and a face card or is left bower
         hand[i].is_face_or_ace()) ||
          hand[i].is_left_bower(upcard.get_suit()) ){
      trump_in_hand_r1++;
    }
  }
    int trump_in_hand_r2 = 0;
   for(int i = 0; i < 4; i++){
    if ((hand[i].get_suit() == Suit_next(upcard.get_suit())) &&     // add 1 if suit is trump and a face card or is left bower
         hand[i].is_face_or_ace() ||
          hand[i].is_left_bower(Suit_next(upcard.get_suit())) ){
      trump_in_hand_r2++;
    }
   }
   if(round == 1){
      if(trump_in_hand_r1 > 1){
      order_up_suit = upcard.get_suit();
      return true;
    }
      return false;
   }

   if(round == 2){
        if(trump_in_hand_r2 > 1){
        order_up_suit = upcard.get_suit();
          return true;
        }
      return false;
   }
  
   
   
   
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

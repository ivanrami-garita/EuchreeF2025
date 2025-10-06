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
        order_up_suit = Suit_next(upcard.get_suit());
          return true;
        }
    if(is_dealer){
      order_up_suit = Suit_next(upcard.get_suit());
      return true;
    }  
    return false;
   }
  
    assert(false); // TODO: implement later
  }

  void add_and_discard(const Card &upcard) override {
    Card lowest_card = hand[0];
    int low_card_index = 0;
    for(int i = 0; i < 5; i++){
        if(hand[i] < lowest_card){
          lowest_card = hand[i];
          low_card_index = i;
        }
    }
    if(lowest_card < upcard){
      hand[low_card_index] = upcard;
    }
    
  }

  Card lead_card(Suit trump) override {
  
        assert(!hand.empty());

    // Step 1: look for highest non-trump card
    bool has_non_trump = false;
    Card highest_non_trump = hand[0];

    for (int i = 0; i < hand.size(); ++i) {
        if (!hand[i].is_trump(trump)) {
            if (!has_non_trump || hand[i] > highest_non_trump) {
                highest_non_trump = hand[i];
                has_non_trump = true;
            }
        }
    }

    // Step 2: if any non-trump found, lead it
    if (has_non_trump) {
        return highest_non_trump;
    }

    // Step 3: otherwise, all cards are trump → lead highest trump
    Card highest_trump = hand[0];
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].is_trump(trump) && hand[i] > highest_trump) {
            highest_trump = hand[i];
        }
    }

    return highest_trump;
}

  Card play_card(const Card &led_card, Suit trump) override {
   bool can_follow = false;

    // Step 1: check if player can follow suit (no break, just a flag)
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
            can_follow = true;
        }
    }

    // Step 2: if can follow, play highest of that suit
    if (can_follow) {
        Card highest_of_suit;
        bool first_found = false;

        for (int i = 0; i < hand.size(); ++i) {
            if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
                if (!first_found) {
                    highest_of_suit = hand[i];
                    first_found = true;
                } else if (hand[i] > highest_of_suit) {
                    highest_of_suit = hand[i];
                }
            }
        }

        return highest_of_suit;
    }

    // Step 3: otherwise, play the lowest card in hand
    Card lowest_in_hand = hand[0];
    for (int i = 1; i < hand.size(); ++i) {
        if (hand[i] < lowest_in_hand) {
            lowest_in_hand = hand[i];
        }
    }

    return lowest_in_hand;
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

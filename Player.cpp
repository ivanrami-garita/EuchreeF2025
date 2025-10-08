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
   for(int i = 0; i < hand.size(); i++){
    if ((hand[i].get_suit() == upcard.get_suit() &&     // add 1 if suit is trump and a face card or is left bower
         hand[i].is_face_or_ace()) ||
          hand[i].is_left_bower(upcard.get_suit()) ){
      trump_in_hand_r1++;
    }
  }
    int trump_in_hand_r2 = 0;
   for(int i = 0; i < hand.size(); i++){
    if (hand[i].get_suit() == Suit_next(upcard.get_suit()) &&
    (hand[i].is_face_or_ace() || hand[i].is_left_bower(Suit_next(upcard.get_suit())))) {
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
  
    assert(false);
    return false; // TODO: implement later
  }

  void add_and_discard(const Card &upcard) override {
  // Add the upcard, then discard one card according to Simple strategy.
  hand.push_back(upcard); // now 5 cards

  // If the upcard is trump (relative to its suit), prefer to discard the
  // lowest trump card. Otherwise discard the lowest card in hand.
  Suit trump = upcard.get_suit();

  // Count how many trump cards are in hand now
  int trump_count = 0;
  for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
    if (hand[i].is_trump(trump))
      ++trump_count;
  }

  int remove_index = -1;

  if (trump_count >= 1) {
    // If there is at least one trump, discard the lowest trump.
    // (This matches the "discard lowest trump" student test expectation
    // when an upcard of trump was given.)
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
      if (!hand[i].is_trump(trump))
        continue;
      if (remove_index == -1 || hand[i] < hand[remove_index]) {
        remove_index = i;
      }
    }
  }

  if (remove_index == -1) {
    // No trump found (shouldn't happen if trump_count >=1), or fallback:
    // remove the overall lowest card.
    remove_index = 0;
    for (int i = 1; i < static_cast<int>(hand.size()); ++i) {
      if (hand[i] < hand[remove_index])
        remove_index = i;
    }
  }

  hand.erase(hand.begin() + remove_index);
}

  Card lead_card(Suit trump) override {
  
   assert(!hand.empty());


  // Count trumps in hand (considering left bower rules inside is_trump)
  int trump_count = 0;
  for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
    if (hand[i].is_trump(trump))
      ++trump_count;
  }

  Card chosen;
  // If player has two or more trumps, prefer to lead a trump (highest trump).
  if (trump_count >= 2) {
    Card highest_trump = Card(TWO, CLUBS); // placeholder low card
    bool first_found = false;
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
      if (hand[i].is_trump(trump)) {
        if (!first_found || hand[i] > highest_trump) {
          highest_trump = hand[i];
          first_found = true;
        }
      }
    }
    chosen = highest_trump;
  } else {
    // Otherwise prefer the highest non-trump if one exists; else highest trump.
    bool has_non_trump = false;
    Card highest_non_trump = hand[0];
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
      if (!hand[i].is_trump(trump)) {
        if (!has_non_trump || hand[i] > highest_non_trump) {
          highest_non_trump = hand[i];
          has_non_trump = true;
        }
      }
    }

    if (has_non_trump) {
      chosen = highest_non_trump;
    } else {
      Card highest_trump = hand[0];
      for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
        if (hand[i].is_trump(trump) && hand[i] > highest_trump) {
          highest_trump = hand[i];
        }
      }
      chosen = highest_trump;
    }
  }

    // Step 3: remove the chosen card (no break)
    int remove_index = -1;
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
        if (hand[i] == chosen && remove_index == -1) {
            remove_index = i;
        }
    }

    if (remove_index != -1) {
        hand.erase(hand.begin() + remove_index);
    }

    return chosen;
}

  Card play_card(const Card &led_card, Suit trump) override {
   bool can_follow = false;
    Card chosen;
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

        chosen = highest_of_suit;
    }
    else{
    // Step 3: otherwise, play the lowest card in hand
    Card lowest_in_hand = hand[0];
    for (int i = 1; i < hand.size(); ++i) {
        if (hand[i] < lowest_in_hand) {
            lowest_in_hand = hand[i];
        }
    }

    chosen = lowest_in_hand;
}
hand.erase(remove(hand.begin(), hand.end(), chosen), hand.end());
return chosen;
}


};

// ----- HumanPlayer class -----
class HumanPlayer : public Player {
private:
  string name;
  vector<Card> hand;
  
void print_hand() const {
  // Print the hand in its current order. Avoid sorting here to keep this
  // helper simple and to prevent const-iterator/move issues in some
  // toolchains. This function is used for human debugging only.
  for (size_t i = 0; i < hand.size(); ++i)
    cout << "Human player " << name << "'s hand: "
       << "[" << i << "] " << hand[i] << "\n";
}
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
  
  print_hand();
  cout << "Upcard: " << upcard << endl;
  cout << "Round " << round << (is_dealer ? " (Dealer)" : "") << endl;
  cout << name << ", enter trump suit or 'pass': ";

  string decision;
  cin >> decision;

  if (decision != "pass" && decision != "Pass") {
    order_up_suit = string_to_suit(decision);
    return true;
  }

  return false;
}
    
  void add_and_discard(const Card &upcard) override {
   print_hand();
   cout << "Discard upcard: [-1]\n";
   cout << "Human player " << name << ", please select a card to discard:\n"; 
    
   hand.push_back(upcard);
   
   int choice;
   cin >> choice;
   if (choice == -1) {
    hand.pop_back();  // remove upcard (the one just added)
  } else if (choice >= 0 && choice < static_cast<int>(hand.size())) {
    // discard the chosen card by index
    hand.erase(hand.begin() + choice);
  } else {
    cout << "Invalid choice. No card discarded.\n";
    assert(false);
  }
}
  

  Card lead_card(Suit trump) override {
      print_hand();
      cout << "Human player " << name << ", please select a card to lead:\n";

  int choice;
      cin >> choice;

  assert(choice >= 0 && choice < static_cast<int>(hand.size()));

  Card chosen = hand[choice];
        hand.erase(hand.begin() + choice);
  return chosen;
  }
  Card play_card(const Card &led_card, Suit trump) override {
      cout << "Led card: " << led_card << " | Trump: " << trump << endl;
       print_hand();
    cout << "Human player " << name << ", please select a card to play:\n";

  int choice;
    cin >> choice;

  // must follow suit if possible
  bool can_follow = false;
  for (int i = 0; i < hand.size(); ++i) {
    if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
      can_follow = true;
    }
  }

  assert(choice >= 0 && choice < static_cast<int>(hand.size()));

  // if they must follow suit but don't
  if (can_follow && hand[choice].get_suit(trump) != led_card.get_suit(trump)) {
        cout << "You must follow suit if possible.\n";
    assert(false); // fail fast for invalid play
  }

  Card chosen = hand[choice];
    hand.erase(hand.begin() + choice);
  return chosen;
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

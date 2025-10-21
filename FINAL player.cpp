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
    { return name; }

  void add_card(const Card &c) override {
    assert(hand.size() < MAX_HAND_SIZE);
    hand.push_back(c);
    std::sort(hand.begin(), hand.end());
  }

  bool make_trump(const Card &upcard, bool is_dealer,
                  int round, Suit &order_up_suit) const override {
    assert(round == 1 || round == 2);

    // Round 1: count face/ace or left bower in upcard's suit (i.e. potential trump)
    if (round == 1) {
      int numTrumpFace = 0;
      Suit candidate = upcard.get_suit();
      for (size_t i = 0; i < hand.size(); ++i) {
        if ((hand[i].is_trump(candidate) && hand[i].is_face_or_ace())
            || hand[i].is_left_bower(candidate)) {
          ++numTrumpFace;
        }
      }
      if (numTrumpFace >= 2) {
        order_up_suit = candidate;
        return true;
      }
      return false;
    }

    // Round 2: consider the next suit (the dealer may be forced)
    if (round == 2) {
      Suit next = Suit_next(upcard.get_suit());
      if (is_dealer) {
        order_up_suit = next;
        return true;
      }
      // otherwise check if we have at least one face/ace or left bower in that suit
      for (size_t i = 0; i < hand.size(); ++i) {
        if ((hand[i].is_trump(next) && hand[i].is_face_or_ace())
            || hand[i].is_left_bower(next)) {
          order_up_suit = next;
          return true;
        }
      }
      return false;
    }

    assert(false);
    return false;
  }
//TEST01 bug
  void add_and_discard(const Card &upcard) override {
    // Add the upcard, then discard the single lowest card according to Simple strategy.
    assert(hand.size() <= MAX_HAND_SIZE); // before push should be 4
    hand.push_back(upcard); // now 5 (or <= MAX_HAND_SIZE)

    Suit trump = upcard.get_suit();

    // Find index of lowest card using trump-aware comparator
    int remove_index = 0;
    for (int i = 1; i < static_cast<int>(hand.size()); ++i) {
      if (Card_less(hand[i], hand[remove_index], trump)) {
        remove_index = i;
      }
    }

    hand.erase(hand.begin() + remove_index);
    std::sort(hand.begin(), hand.end());
  }

  Card lead_card(Suit trump) override {
    assert(!hand.empty());
    // Prefer highest non-trump; if none exist then highest trump.
    // We'll locate the highest by comparing with operator>
    bool found_non_trump = false;
    Card best_non_trump;
    int best_index = -1;

    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
      if (!hand[i].is_trump(trump)) {
        if (!found_non_trump || hand[i] > best_non_trump) {
          best_non_trump = hand[i];
          best_index = i;
          found_non_trump = true;
        }
      }
    }

    if (found_non_trump) {
      Card chosen = best_non_trump;
      hand.erase(hand.begin() + best_index);
      return chosen;
    }

    // No non-trump found -> choose highest trump
    Card best_trump = hand[0];
    int best_trump_index = 0;
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
      if (!(Card_less(hand[i], best_trump, trump)) ) {
        best_trump = hand[i];
        best_trump_index = i;
      }
    }
    Card chosen = best_trump;
    hand.erase(hand.begin() + best_trump_index);
    return chosen;
  }

  Card play_card(const Card &led_card, Suit trump) override {
    assert(!hand.empty());
    // Determine led suit with trump rules
    Suit led_suit = led_card.get_suit(trump);

    Card chosen;
    int chosen_index = 0;

    // Check if can follow
    bool can_follow = false;
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
      if (hand[i].get_suit(trump) == led_suit) {
        can_follow = true;
        chosen = hand[i];
        chosen_index = i;
        break;
      }
    }

    if (can_follow) {
      // play highest of that suit
      for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
        if (hand[i].get_suit(trump) == led_suit) {
          if (Card_less(chosen, hand[i], led_card, trump)) {
            chosen = hand[i];
            chosen_index = i;
            
          }
        }
      }
    } else {
      // cannot follow: play lowest card (non-trump lowest preferred because comparator treats non-trump as lower)
      chosen = hand[0];
      chosen_index = 0;
      for (int i = 1; i < static_cast<int>(hand.size()); ++i) {
        if (!Card_less(chosen, hand[i],led_card, trump)) {
          chosen = hand[i];
          chosen_index = i;
        }
      }
    }

    // remove chosen card from hand
    assert(chosen_index >= 0);
    hand.erase(hand.begin() + chosen_index);
    return chosen;
  }
};

// ----- HumanPlayer class -----
class HumanPlayer : public Player {
private:
  string name;
  vector<Card> hand;

  void print_hand_sorted() const {
    vector<Card> temp = hand;
    sort(temp.begin(), temp.end());
    for (size_t i = 0; i < temp.size(); ++i) {
      cout << "Human player " << name << "'s hand: "
           << "[" << i << "] " << temp[i] << "\n";
    }
  }

public:
  HumanPlayer(const string &name_in) : name(name_in) {}
  const string & get_name() const override {
    return name;
  }

  void add_card(const Card &c) override {
    assert(hand.size() < MAX_HAND_SIZE);
    hand.push_back(c);
    std::sort(hand.begin(), hand.end());
  }

  bool make_trump(const Card &upcard, bool is_dealer,
                  int round, Suit &order_up_suit) const override {
    assert(round == 1 || round == 2);
    print_hand_sorted();
    cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
    string decision;
    cin >> decision;
    if (decision == "pass" || decision == "Pass") {
      return false;
    }
    // Map string to Suit — assume string_to_suit exists and throws/asserts on bad input
    order_up_suit = string_to_suit(decision);
    return true;
  }

  void add_and_discard(const Card &upcard) override {
    // Show sorted hand for user convenience
    sort(hand.begin(), hand.end());
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
      cout << "Human player " << name << "'s hand: "
           << "[" << i << "] " << hand[i] << '\n';
    }

    cout << "Discard upcard: [-1]\n";
    cout << "Human player " << name << ", please select a card to discard:\n" << endl;

    hand.push_back(upcard);
    sort(hand.begin(), hand.end());

    int choice;
    cin >> choice;
    if (choice == -1) {
      // remove the upcard (it will be one of the equal highest/lowest duplicates — remove by value)
      // safer: find one instance equal to upcard and remove it
      auto it = find(hand.begin(), hand.end(), upcard);
      if (it != hand.end()) hand.erase(it);
      else assert(false);
    } else if (choice >= 0 && choice < static_cast<int>(hand.size())) {
      hand.erase(hand.begin() + choice);
    } else {
      cout << "Invalid choice. Aborting (assert).\n";
      assert(false);
    }
  }

  Card lead_card(Suit trump) override {
    sort(hand.begin(), hand.end());
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
      cout << "Human player " << name << "'s hand: "
           << "[" << i << "] " << hand[i] << '\n';
    }
    cout << "Human player " << name << ", please select a card:\n";
    int choice;
    cin >> choice;
    assert(choice >= 0 && choice < static_cast<int>(hand.size()));
    Card chosen = hand[choice];
    hand.erase(hand.begin() + choice);
    return chosen;
  }

  Card play_card(const Card &led_card, Suit trump) override {
    // For human we reuse the same UI as lead_card (player chooses)
    sort(hand.begin(), hand.end());
    for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
      cout << "Human player " << name << "'s hand: "
           << "[" << i << "] " << hand[i] << '\n';
    }
    cout << "Human player " << name << ", please select a card:\n";
    int choice;
    cin >> choice;
    assert(choice >= 0 && choice < static_cast<int>(hand.size()));
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

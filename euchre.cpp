#include "Pack.hpp"
#include "Player.hpp"
#include "Card.hpp"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
void usage_error() {
  cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
}
class Game {
 public:
  Game(const string &pack_filename,
     bool do_shuffle,
     int points_to_win,
     const vector<string> &names,
     const vector<string> &types)
  : pack(), // default construct; we'll initialize from file below
    pack_filename_(pack_filename),
    do_shuffle_(do_shuffle),
    points_to_win_(points_to_win),
    names_(names),
    types_(types) {
    // Initialize players
    for (size_t i = 0; i < names.size(); ++i) {
      players.push_back(Player_factory(names[i], types[i]));
    }
    // Initialize pack from file
    std::ifstream infile(pack_filename_);
    if (infile.is_open()) {
      pack = Pack(infile);
    } else {
      // If the file cannot be opened, fall back to default pack ordering
      pack = Pack();
    }
  }
  void play();
 private:
  vector<Player*> players;
  Pack pack;
  string pack_filename_;
  bool do_shuffle_;
  int points_to_win_;
  vector<string> names_;
  vector<string> types_;
  int dealer_index = 0;
  int team1_points = 0; // players[0] + players[2]
  int team2_points = 0; // players[1] + players[3]
  void shuffle() { 
    if (do_shuffle_) pack.shuffle(); 
    else pack.reset(); 
  }
  void deal() {
    // dealer deals to left first, 3-2-3-2 cards alternating
    int next_player = (dealer_index + 1) % 4;
    for (int i = 0; i < 3; ++i)
      players[next_player]->add_card(pack.deal_one());
    for (int i = 0; i < 2; ++i)
      players[(next_player + 1) % 4]->add_card(pack.deal_one());
    for (int i = 0; i < 3; ++i)
      players[(next_player + 2) % 4]->add_card(pack.deal_one());
    for (int i = 0; i < 2; ++i)
      players[(next_player + 3) % 4]->add_card(pack.deal_one());
    for (int i = 0; i < 2; ++i)
      players[next_player]->add_card(pack.deal_one());
    for (int i = 0; i < 3; ++i)
      players[(next_player + 1) % 4]->add_card(pack.deal_one());
    for (int i = 0; i < 2; ++i)
      players[(next_player + 2) % 4]->add_card(pack.deal_one());
    for (int i = 0; i < 3; ++i)
      players[(next_player + 3) % 4]->add_card(pack.deal_one());
  }
  // void clear_hands() {
  
  // for (auto *p : players) p->hand.clear();;
  // }
  void make_trump(Card upcard, int &maker_index, Suit &trump) {
    int order_up_player = (dealer_index + 1) % 4;
    bool trump_made = false;
    // Round 1
    for (int i = 0; i < 4 && !trump_made; ++i) {
      int idx = (order_up_player + i) % 4;
      if (players[idx]->make_trump(upcard, idx == dealer_index, 1, trump)) {
        maker_index = idx;
        trump_made = true;
        cout << players[idx]->get_name() << " orders up " << trump << endl;
      } else {
        cout << players[idx]->get_name() << " passes" << endl;
      }
    }
    // Dealer picks up if ordered
    if (trump_made) {
      players[dealer_index]->add_and_discard(upcard);
      return;
    }
    // Round 2
    for (int i = 0; i < 4 && !trump_made; ++i) {
      int idx = (order_up_player + i) % 4;
      if (players[idx]->make_trump(upcard, idx == dealer_index, 2, trump)) {
        maker_index = idx;
        trump_made = true;
        cout << players[idx]->get_name() << " orders up " << trump << endl << endl;
      } else {
        cout << players[idx]->get_name() << " passes" << endl;
      }
    }
    // Must have trump by now (spec guarantees)
  }
  int play_trick(int leader_index, Suit trump) {
    Card led_card = players[leader_index]->lead_card(trump);
    cout << led_card << " led by " << players[leader_index]->get_name() << endl;
    Card winning_card = led_card;
    int winning_player = leader_index;
    for (int i = 1; i < 4; ++i) {
      int idx = (leader_index + i) % 4;
      Card played = players[idx]->play_card(led_card, trump);
      cout << played << " played by " << players[idx]->get_name() << endl;
      if (Card_less(winning_card, played, led_card, trump)) {
        winning_card = played;
        winning_player = idx;
      }
    }
    cout << players[winning_player]->get_name() << " takes the trick" << endl << endl;
    return winning_player;
  }
  int play_hand(int maker_index, Suit trump) {
    int leader_index = (dealer_index + 1) % 4;
    int team1_tricks = 0, team2_tricks = 0;
    for (int i = 0; i < 5; ++i) {
      int winner = play_trick(leader_index, trump);
      if (winner % 2 == 0) team1_tricks++;
      else team2_tricks++;
      leader_index = winner;
    }
    int maker_team = maker_index % 2;
  int maker_tricks = (maker_team == 0) ? team1_tricks : team2_tricks;
 if(team1_tricks > team2_tricks){
      cout << names_[0] << " and " << names_[2] << " win the hand " << endl;
    }
    else{
      cout << names_[1] << " and " << names_[3] << " win the hand " << endl;
    }
    if (maker_tricks >= 3) {
      if (maker_tricks == 5) {
        cout << "march!" << endl;
        if (maker_team == 0) team1_points += 2;
        else team2_points += 2;
      } else {
        if (maker_team == 0) team1_points += 1;
        else team2_points += 1;
      }
    } else {
      cout << "euchred!" << endl;
      if (maker_team == 0) team2_points += 2;
      else team1_points += 2;
    }
   
    cout << names_[0] << " and " << names_[2] << " have " << team1_points << " points" << endl;
    cout << names_[1] << " and " << names_[3] << " have " << team2_points << " points" << endl << endl;
    return maker_team;
  }
};
void Game::play() {
  int hand_no = 0;
  while (team1_points < points_to_win_ && team2_points < points_to_win_) {
    cout << "Hand " << hand_no << endl;
    ++hand_no;
    cout << names_[dealer_index] << " deals" << endl;
    // clear_hands();
    shuffle();
    deal();
    Card upcard = pack.deal_one();
    cout << upcard << " turned up" << endl;
    int maker_index;
    Suit trump;
    make_trump(upcard, maker_index, trump);
    play_hand(maker_index, trump);
    dealer_index = (dealer_index + 1) % 4;
    pack.reset();
  }
  
  if (team1_points > team2_points)
    cout << names_[0] << " and " << names_[2];
  else
    cout << names_[1] << " and " << names_[3];
  cout << " win!" << endl;
}
int main(int argc, char **argv) {
  if (argc != 12) {
    usage_error();
    return 1;
  }
  string pack_filename = argv[1];
  string shuffle_arg = argv[2];
  string points_arg = argv[3];
  bool do_shuffle = false;
  if (shuffle_arg == "shuffle") do_shuffle = true;
  else if (shuffle_arg != "noshuffle") {
    usage_error();
    return 1;
  }
  int points_to_win = stoi(points_arg);
  if (points_to_win <= 0 || points_to_win > 100) {
    usage_error();
    return 1;
  }
  vector<string> names;
  vector<string> types;
  for (int i = 4; i <= 11; i += 2) {
    names.push_back(argv[i]);
    string t = argv[i + 1];
    if (t != "Simple" && t != "Human") {
      usage_error();
      return 1;
    }
    types.push_back(t);
  }
  ifstream infile(pack_filename);
  if (!infile.is_open()) {
    cout << "Error opening " << pack_filename << endl;
    return 1;
  }
  for (int i = 0; i < argc; ++i) cout << argv[i] << " ";
  cout << endl;
  Game game(pack_filename, do_shuffle, points_to_win, names, types);
  game.play();
}

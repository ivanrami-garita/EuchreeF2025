
#include "Pack.hpp"
#include "Player.hpp"
#include "Card.hpp"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "Player.hpp"







class Game {
 public:
  Game(/* ... */);
  void play();
  void end_of_game(){
    for (size_t i = 0; i < players.size(); ++i) {
  delete players[i];
  }
}
  int get_even_score(){
    return even_team_score; 
  }
  int get_odd_team_score(){
    return odd_team_score;
  }
  void set_even_score(int score){
  even_team_score = score;
  }
  void set_odd_team_score(int score){
    odd_team_score = score;
  }
  void change_dealer(){
    dealer++;
  }
  int get_dealer(){
    return dealer % 4;
  }

 private:
  std::vector<Player*> players;
  Pack pack;
  // ...
    void shuffle();// shuffle 
    void deal(int *player);// *ptr vector hand 
    void make_trump(/* ... */);// Round 1 round and Round 2 pass in  create game 
    void play_hand(/* ... */);
    int odd_team_score;
    int even_team_score;
    int dealer;
    int round
    // ...
};

















































int main(int argc,char **argv) {
 if (argc != 12) {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
    return 1;
  }

string pack_filename = argv[1];
ifstream inFile;
inFile.open(pack_filename);


string shuffle  = argv[2];
int points_to_win = stoi(argv[3]);
string Player0 = argv[4];
string Player0_type = argv[5];
string Player1 = argv[6];
string Player1_type = argv[7];
string Player2 = argv[8];
string Player2_type = argv[9];
string Player3 = argv[10];
string Player3_type = argv[11];
if(points_to_win < 0 || points_to_win > 100){
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
    return 1;
}
if(shuffle != "shuffle" && shuffle != "noshuffle"){
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
    return 1;
}
if( Player0_type != "Simple" && Player1_type != "Simple" && 
        Player2_type != "Simple" && Player3_type != "Simple"){

    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
}
if( Player0_type != "Human" && Player1_type != "Human" && 
        Player2_type != "Human" && Player3_type != "Human"){
        
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
}
if(!inFile.is_open()){
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
}
//Start of the game
  int odd_team_score = 0;
  int even_team_score = 0;
  int dealer = 0;
  int round = 0;
Game euchre;
if(odd_team_score != points_to_win || odd_team_score != points_to_win){
  Player_factory(Player0, Player0_type);
  Player_factory(Player1, Player1_type);
  Player_factory(Player2, Player2_type);
  Player_factory(Player3, Player3_type);
  cout << "Hand" << round << endl;
  cout << player[get_dealer()] << "deals" << endl;
if(shuffle == "shuffle"){
  shuffle();
}
deal();












}
































//END OF GAME 


}

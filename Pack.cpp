#include "Pack.hpp"
 








// EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on. 
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack(){
    int pack_index = 0;
    
    for(int suit = SPADES; suit <= DIAMONDS; ++suit){
        for(int rank = NINE; rank <= ACE; ++rank){
            cards[pack_index++] =  Card(static_cast<Rank>(rank),
                                        static_cast<Suit>(suit)); 
        }
    }
    next = 0;
  }

  // REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack(std::istream& pack_input){// SOMETHING WRONG
    int pack_index = 0;
    std::string in_rank;
    std::string in_suit;
    std::string in_junk;
    for(int suit = SPADES; suit <= DIAMONDS; suit++){
        for(int rank = NINE; rank <= ACE; rank++){
            pack_input >> in_rank >> in_suit;
            cards[pack_index++] =  Card((string_to_rank(in_rank)),
                                        (string_to_suit(in_suit)));
        }
    }
    next = 0;
  }

  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
  Card Pack::deal_one(){
       return cards[next++];
       
  }

  // EFFECTS: Resets next index to first card in the Pack
  void Pack::reset(){
        next = 0;
  }

  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
  void Pack::shuffle(){
    
    std::array<Card, PACK_SIZE> temp_cards;
     for (int shuf = 0; shuf < 7; shuf++) {
        std::array<Card, PACK_SIZE> temp_cards;

        int mid = PACK_SIZE / 2;
        int idx = 0;

        // In-shuffle: start with second half, interleave with first half 0123-4567
        for (int i = 0; i < mid; i++) {                       //          4051-6273          
            temp_cards[idx++] = cards[mid + i]; // take from second half
            temp_cards[idx++] = cards[i];       // then from first half
        }

        // copy back
        cards = temp_cards;
    }

    next = 0; // reset the index after shuffling
}

  // EFFECTS: returns true if there are no more cards left in the pack
  bool Pack::empty() const{
        return next >= PACK_SIZE;     

  }

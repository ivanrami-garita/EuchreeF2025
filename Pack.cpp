#include "Pack.hpp"
 



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


  Pack::Pack(std::istream& pack_input){
    int pack_index = 0;
    std::string in_rank;
    std::string in_suit;
    std::string in_junk;
    for(int suit = SPADES; suit <= DIAMONDS; suit++){
        for(int rank = NINE; rank <= ACE; rank++){
            pack_input >> in_rank >> in_junk >> in_suit;
            cards[pack_index++] =  Card((string_to_rank(in_rank)),
                                        (string_to_suit(in_suit)));
        }
    }
    next = 0;
  }


  Card Pack::deal_one(){
       return cards[next++];
       
  }

  void Pack::reset(){
        next = 0;
  }

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

  bool Pack::empty() const{
        return next >= PACK_SIZE;     

  }

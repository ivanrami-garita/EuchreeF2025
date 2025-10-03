#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
using namespace std;
TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}
//Test constructor, get suit/rank. isface_or_ace
TEST(Test_constuctor){
    Card c1;
    ASSERT_EQUAL(TWO,c1.get_rank());
    ASSERT_EQUAL(SPADES,c1.get_suit());
    ASSERT_EQUAL(false, c1.is_face_or_ace());
    Card c2(NINE,DIAMONDS);
    ASSERT_EQUAL(NINE, c2.get_rank());
    ASSERT_EQUAL(DIAMONDS, c2.get_suit());
    ASSERT_EQUAL(false, c2.is_face_or_ace());
    Card c3(TEN,CLUBS);
    ASSERT_EQUAL(TEN, c3.get_rank());
    ASSERT_EQUAL(CLUBS, c3.get_suit());
    ASSERT_EQUAL(false, c3.is_face_or_ace());
    Card c4(JACK,SPADES);
    ASSERT_EQUAL(JACK, c4.get_rank());
    ASSERT_EQUAL(SPADES, c4.get_suit());
    ASSERT_EQUAL(true, c4.is_face_or_ace());
    Card c5(QUEEN,HEARTS);
    ASSERT_EQUAL(QUEEN, c5.get_rank());
    ASSERT_EQUAL(HEARTS, c5.get_suit());
    ASSERT_EQUAL(true, c5.is_face_or_ace());
    Card c6(KING,CLUBS);
    ASSERT_EQUAL(KING, c6.get_rank());
    ASSERT_EQUAL(CLUBS, c6.get_suit());
    ASSERT_EQUAL(true, c6.is_face_or_ace());
}
TEST(Test_operators){
    Card c0(KING,SPADES);
    Card c1(ACE,HEARTS);
    Card c2(NINE,DIAMONDS);
    Card c4(TEN,CLUBS);
    Card c5(QUEEN,SPADES);
    Card c6(JACK,DIAMONDS);
    
    ASSERT_EQUAL(true, operator<(c4,c1));
    ASSERT_EQUAL(false, operator<(c0,c6));
    ASSERT_EQUAL(true, operator<(c2,c4));
    ASSERT_EQUAL(true, operator<(c5,c1));
    ASSERT_EQUAL(false, operator<(c5,c6));
    ASSERT_EQUAL(false, operator<(c1,c2));
    ASSERT_EQUAL(false, operator>(c4,c1));
    ASSERT_EQUAL(true, operator>(c0,c6));
    ASSERT_EQUAL(false, operator>(c2,c4));
    ASSERT_EQUAL(false, operator>(c5,c1));
    ASSERT_EQUAL(true, operator>(c5,c6));
    ASSERT_EQUAL(true, operator>(c1,c2));
}
TEST(Test_right_left_bower){
    Card c1(JACK,HEARTS);
    Card c2(JACK,DIAMONDS);
    Card c3(ACE,HEARTS);
    Suit s1 = HEARTS;
    Suit s2 = DIAMONDS;
    //testing Heart left & right bower
    ASSERT_EQUAL(true, c1.is_right_bower(s1));
    ASSERT_EQUAL(true, c2.is_left_bower(s1));
    ASSERT_EQUAL(true, c2.is_right_bower(s2));
    ASSERT_EQUAL(true, c1.is_left_bower(s2));
    Card c4(JACK,SPADES);
    Card c5(JACK,CLUBS);
    Suit s3 = SPADES;
    Suit s4 = CLUBS;
    //testing Spades left & right bower
    ASSERT_EQUAL(true, c4.is_right_bower(s3));
    ASSERT_EQUAL(true, c5.is_left_bower(s3));
    ASSERT_EQUAL(true, c5.is_right_bower(s4));
    ASSERT_EQUAL(true, c4.is_left_bower(s4));
    //testing with cards that aren't right nor left bower
    ASSERT_EQUAL(false, c1.is_left_bower(s4));
    ASSERT_EQUAL(false, c3.is_left_bower(s3));
    ASSERT_EQUAL(false, c4.is_left_bower(s1));
    ASSERT_EQUAL(false, c5.is_left_bower(s2));
    Card c6(ACE,DIAMONDS);
    Card c7(NINE,SPADES);
    ASSERT_EQUAL(false, c6.is_left_bower(s2));
    ASSERT_EQUAL(false, c7.is_left_bower(s3));
}
TEST(TEST_stream_operator){
    //Strings to test inputs
    string test1 = "Ace of Clubs";
    string test2 = "Jack of Diamonds";
    string test3 = "King of Hearts";
    string test4 = "Ten of Spades";
    //Declaring cards
    Card c1;
    Card c2;
    Card c3;
    Card c4;
    istringstream test01(test1);
    istringstream test02(test2);
    istringstream test03(test3);
    istringstream test04(test4);
    operator>>(test01,c1);
    operator>>(test02,c2);
    operator>>(test03,c3);
    operator>>(test04,c4);
    ASSERT_EQUAL(CLUBS,c1.get_suit());
    ASSERT_EQUAL(ACE,c1.get_rank());
    ASSERT_EQUAL(DIAMONDS,c2.get_suit());
    ASSERT_EQUAL(JACK,c2.get_rank());
    ASSERT_EQUAL(HEARTS,c3.get_suit());
    ASSERT_EQUAL(KING,c3.get_rank());
    ASSERT_EQUAL(SPADES,c4.get_suit());
    ASSERT_EQUAL(TEN,c4.get_rank());
    
}
// Add moretest cases here
TEST_MAIN()

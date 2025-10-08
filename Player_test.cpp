
#include "Player.hpp"
#include "unit_test_framework.hpp"
#include "Card.hpp"
#include <iostream>
using namespace std;

// get_name 
TEST(test_player_get_name) {
    Player *batman = Player_factory("Batman", "Simple");
    ASSERT_EQUAL("Batman", batman->get_name());
    delete batman;
}

//  add_card and lead_card 
TEST(test_add_card_and_lead_card) {
    Player *peacemaker = Player_factory("Peacemaker", "Simple");

    peacemaker->add_card(Card(ACE, HEARTS));
    peacemaker->add_card(Card(KING, CLUBS));
    peacemaker->add_card(Card(TEN, HEARTS));
    peacemaker->add_card(Card(NINE, DIAMONDS));
    peacemaker->add_card(Card(QUEEN, SPADES));

    Card led = peacemaker->lead_card(CLUBS); // trump = CLUBS
    ASSERT_TRUE(led.get_suit(CLUBS) != CLUBS); // highest non-trump
    ASSERT_TRUE(led.get_rank() == ACE || led.get_rank() == QUEEN);

    delete peacemaker;
}

// lead all trump 
TEST(test_lead_all_trump) {
    Player *peacemaker = Player_factory("Peacemaker", "Simple");

    peacemaker->add_card(Card(JACK, HEARTS));
    peacemaker->add_card(Card(KING, HEARTS));
    peacemaker->add_card(Card(ACE, HEARTS));
    peacemaker->add_card(Card(QUEEN, HEARTS));
    peacemaker->add_card(Card(TEN, HEARTS));

    Card led = peacemaker->lead_card(HEARTS); // all trump
    ASSERT_EQUAL(led.get_suit(HEARTS), HEARTS);

    delete peacemaker;
}

//  lead no trump
TEST(test_lead_no_trump) {
    Player *peacemaker = Player_factory("Peacemaker", "Simple");

    peacemaker->add_card(Card(ACE, SPADES));
    peacemaker->add_card(Card(KING, CLUBS));
    peacemaker->add_card(Card(QUEEN, DIAMONDS));
    peacemaker->add_card(Card(NINE, SPADES));
    peacemaker->add_card(Card(TEN, CLUBS));

    Card led = peacemaker->lead_card(HEARTS); // trump = HEARTS
    ASSERT_TRUE(led.get_suit(HEARTS) != HEARTS);

    delete peacemaker;
}

//  make_trump round 1
TEST(test_make_trump_round1) {
    Player *vigilante = Player_factory("Vigilante", "Simple");

    vigilante->add_card(Card(KING, HEARTS));
    vigilante->add_card(Card(QUEEN, HEARTS));
    vigilante->add_card(Card(TEN, CLUBS));
    vigilante->add_card(Card(ACE, SPADES));
    vigilante->add_card(Card(NINE, DIAMONDS));

    Card upcard(JACK, HEARTS);
    Suit order_up;
    bool result = vigilante->make_trump(upcard, false, 1, order_up);

    ASSERT_TRUE(result);
    ASSERT_EQUAL(order_up, HEARTS);

    delete vigilante;
}

// round 1, only 1 face card -> pass
TEST(test_make_trump_round1_one_face) {
    Player *vigilante = Player_factory("Vigilante", "Simple");

    vigilante->add_card(Card(KING, HEARTS));
    vigilante->add_card(Card(TEN, CLUBS));
    vigilante->add_card(Card(NINE, DIAMONDS));
    vigilante->add_card(Card(ACE, SPADES));
    vigilante->add_card(Card(TWO, CLUBS));

    Card upcard(JACK, HEARTS);
    Suit order_up;
    bool result = vigilante->make_trump(upcard, false, 1, order_up);

    ASSERT_FALSE(result);

    delete vigilante;
}

// round 1, left bower counts 
TEST(test_make_trump_round1_left_bower) {
    Player *vigilante = Player_factory("Vigilante", "Simple");

    vigilante->add_card(Card(JACK, DIAMONDS)); // left bower if upcard HEARTS
    vigilante->add_card(Card(QUEEN, HEARTS));
    vigilante->add_card(Card(TEN, CLUBS));
    vigilante->add_card(Card(ACE, SPADES));
    vigilante->add_card(Card(NINE, DIAMONDS));

    Card upcard(JACK, HEARTS);
    Suit order_up;
    bool result = vigilante->make_trump(upcard, false, 1, order_up);

    ASSERT_TRUE(result);
    ASSERT_EQUAL(order_up, HEARTS);

    delete vigilante;
}

//  make_trump round 2 
TEST(test_make_trump_round2) {
    Player *rocket = Player_factory("Rocket", "Simple");

    rocket->add_card(Card(KING, DIAMONDS));
    rocket->add_card(Card(QUEEN, DIAMONDS));
    rocket->add_card(Card(TEN, CLUBS));
    rocket->add_card(Card(ACE, SPADES));
    rocket->add_card(Card(NINE, HEARTS));

    Card upcard(JACK, HEARTS);
    Suit order_up;
    bool result = rocket->make_trump(upcard, false, 2, order_up);

    ASSERT_TRUE(result);
    ASSERT_EQUAL(order_up, DIAMONDS);

    delete rocket;
}

// round 2, no face in next suit, not dealer -> pass
TEST(test_make_trump_round2_pass) {
    Player *rocket = Player_factory("Rocket", "Simple");

    rocket->add_card(Card(TEN, CLUBS));
    rocket->add_card(Card(NINE, SPADES));
    rocket->add_card(Card(EIGHT, DIAMONDS));
    rocket->add_card(Card(ACE, SPADES));
    rocket->add_card(Card(QUEEN, HEARTS));

    Card upcard(JACK, HEARTS);
    Suit order_up;
    bool result = rocket->make_trump(upcard, false, 2, order_up);

    ASSERT_FALSE(result);

    delete rocket;
}

// round 2, dealer must order up 
TEST(test_make_trump_round2_dealer) {
    Player *rocket = Player_factory("Rocket", "Simple");

    rocket->add_card(Card(TEN, CLUBS));
    rocket->add_card(Card(NINE, SPADES));
    rocket->add_card(Card(EIGHT, DIAMONDS));
    rocket->add_card(Card(ACE, SPADES));
    rocket->add_card(Card(QUEEN, HEARTS));

    Card upcard(JACK, HEARTS);
    Suit order_up;
    bool result = rocket->make_trump(upcard, true, 2, order_up);

    ASSERT_TRUE(result);
    ASSERT_EQUAL(order_up, DIAMONDS);

    delete rocket;
}

//  ADDED TESTS TO CATCH SIMPLE PLAYER BUG #2 

// round 2, next suit should be chosen if player has two strong cards
TEST(test_make_trump_round2_next_suit_picked) {
    Player *vigilante = Player_factory("Vigilante", "Simple");

    // upcard suit = CLUBS → next suit = SPADES
    vigilante->add_card(Card(QUEEN, SPADES));
    vigilante->add_card(Card(KING, SPADES));
    vigilante->add_card(Card(TEN, CLUBS));
    vigilante->add_card(Card(NINE, HEARTS));
    vigilante->add_card(Card(ACE, DIAMONDS));

    Card upcard(JACK, CLUBS);
    Suit order_up;
    bool result = vigilante->make_trump(upcard, false, 2, order_up);

    ASSERT_TRUE(result);
    ASSERT_EQUAL(order_up, SPADES);

    delete vigilante;
}

// round 2, player is dealer with no strong cards -> must still order up
TEST(test_make_trump_round2_dealer_must_pick) {
    Player *rocket = Player_factory("Rocket", "Simple");

    rocket->add_card(Card(NINE, CLUBS));
    rocket->add_card(Card(TEN, HEARTS));
    rocket->add_card(Card(NINE, SPADES));
    rocket->add_card(Card(QUEEN, DIAMONDS));
    rocket->add_card(Card(KING, CLUBS));

    Card upcard(QUEEN, CLUBS);
    Suit order_up;
    bool result = rocket->make_trump(upcard, true, 2, order_up);

    ASSERT_TRUE(result);
    ASSERT_EQUAL(order_up, SPADES); // next after CLUBS

    delete rocket;
}

// round 1, exactly two trump cards should be enough to order up
TEST(test_make_trump_round1_two_strong_exact) {
    Player *batman = Player_factory("Batman", "Simple");

    batman->add_card(Card(KING, HEARTS));
    batman->add_card(Card(ACE, HEARTS));  // exactly two strong hearts
    batman->add_card(Card(TEN, CLUBS));
    batman->add_card(Card(NINE, DIAMONDS));
    batman->add_card(Card(QUEEN, SPADES));

    Card upcard(JACK, HEARTS);
    Suit order_up;
    bool result = batman->make_trump(upcard, false, 1, order_up);

    ASSERT_TRUE(result);
    ASSERT_EQUAL(order_up, HEARTS);

    delete batman;
}

// 

//  add_and_discard
TEST(test_add_and_discard) {
    Player *groot = Player_factory("Groot", "Simple");

    groot->add_card(Card(TEN, CLUBS));
    groot->add_card(Card(QUEEN, HEARTS));
    groot->add_card(Card(NINE, DIAMONDS));
    groot->add_card(Card(KING, SPADES));

    Card upcard(TWO, CLUBS);
    groot->add_and_discard(upcard);

    Card led = groot->lead_card(CLUBS); 
    ASSERT_TRUE(!led.get_suit(CLUBS) || led.get_rank() > 0); // hand still playable

    delete groot;
}

//   discard lowest trump
TEST(test_add_and_discard_lowest_trump) {
    Player *groot = Player_factory("Groot", "Simple");

    groot->add_card(Card(TWO, HEARTS));
    groot->add_card(Card(QUEEN, HEARTS));
    groot->add_card(Card(NINE, DIAMONDS));
    groot->add_card(Card(KING, SPADES));

    Card upcard(THREE, HEARTS); 
    groot->add_and_discard(upcard);

    ASSERT_EQUAL(groot->lead_card(HEARTS).get_suit(HEARTS), HEARTS);

    delete groot;
}

//  play_card follows suit
TEST(test_play_card_follow_suit) {
    Player *batman = Player_factory("Batman", "Simple");

    batman->add_card(Card(NINE, SPADES));
    batman->add_card(Card(JACK, SPADES));
    batman->add_card(Card(ACE, HEARTS));
    batman->add_card(Card(KING, CLUBS));
    batman->add_card(Card(QUEEN, DIAMONDS));

    Card led(TEN, SPADES);
    Card played = batman->play_card(led, HEARTS);

    ASSERT_EQUAL(played.get_suit(HEARTS), SPADES);

    delete batman;
}

// play_card cannot follow suit -> play lowest 
TEST(test_play_card_no_follow) {
    Player *batman = Player_factory("Batman", "Simple");

    batman->add_card(Card(TEN, CLUBS));
    batman->add_card(Card(NINE, DIAMONDS));
    batman->add_card(Card(QUEEN, CLUBS));
    batman->add_card(Card(KING, SPADES));
    batman->add_card(Card(ACE, SPADES));

    Card led(JACK, HEARTS);
    Card played = batman->play_card(led, HEARTS);

    ASSERT_TRUE(played.get_rank() <= TEN); // lowest card

    delete batman;
}

//play_card led is trump 
TEST(test_play_card_led_trump) {
    Player *batman = Player_factory("Batman", "Simple");

    batman->add_card(Card(JACK, HEARTS));
    batman->add_card(Card(QUEEN, HEARTS));
    batman->add_card(Card(KING, CLUBS));
    batman->add_card(Card(ACE, SPADES));
    batman->add_card(Card(TEN, DIAMONDS));

    Card led(JACK, HEARTS); // trump
    Card played = batman->play_card(led, HEARTS);

    ASSERT_TRUE(played.is_trump(HEARTS));

    delete batman;
}

TEST_MAIN()

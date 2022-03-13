#pragma once
#ifndef BLACKJACK
#define BLACKJACK 1
#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <map>
using namespace std;
enum Suit : char {
    ErrorSuit, Spade, Club, Heart, Diamond
};
enum Rank : char {
    ErrorRank, Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King
};
struct Card {
    Suit suit;
    Rank rank;
    Card(string card);
    Card(Suit s = ErrorSuit, Rank r = ErrorRank);
    int getValue();
    string name();
    int deltaCounter();
};
struct Deck {
    vector<Card> cards;
    Deck(int count = 0);
    Card draw();
    void erase(int index);
    void append(Card c);
    void drawFromDeck(int count = 1);
    int total();
    int aceCount();
    void print(string title);
};

int promptForInt(string prompt, int min, int max);
bool promptBoolean(string prompt);
#endif
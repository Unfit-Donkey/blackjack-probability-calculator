//David Jones, Benjamin Cates, Damaris Sanchez, Sukhman Kaur, and Amna Ali
#include <iostream>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <limits>
#include <cstring>
using namespace std;
#pragma region Deck Functions
const int cardValues[] = {
    0,11,2,3,4,5,6,7,8,9,10,10,10,10,
    11,2,3,4,5,6,7,8,9,10,10,10,10,
    11,2,3,4,5,6,7,8,9,10,10,10,10,
    11,2,3,4,5,6,7,8,9,10,10,10,10
};
int deckCount = 2;
int deckSize;
int* deckSizeReference = &deckSize;
int* deck = NULL;
int deckCopy[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52 };
void initializeDeck() {
    free(deck);
    deck = (int*)malloc(deckCount * 52 * sizeof(int));
    resetDeck();
}
void resetDeck() {
    deckSize = deckCount * 52;
    for(int i = 0;i < deckCount;i++) memcpy(deck + 52 * i, deckCopy, 52 * sizeof(int));
}
int drawCard() {
    int index = rand() % deckSize;
    int cardID = deck[index];
    deckSize--;
    deck[index] = deck[deckSize];
    return cardID;
}
void undrawCard(int type) {
    deck[deckSize]=type;
    deckSize++;
}
#pragma endregion
#pragma region Win-Loss Table
typedef struct WinLossRatio {
    int stand;
    int doubleDown;
    int optimal;
    int total;
    int dealerBusts;
} WinLossRatio;
WinLossRatio table[11][21];
void setTable() {
    //Zero out the table at it's size
    memset(table, 0, 11 * 21 * sizeof(WinLossRatio));
}
#pragma endregion
#pragma region Basic Functions
int aceCount(int* hand, int count) {
    int total = 0;
    for(int i = 0; i < count; i++) {
        if(cardValues[hand[i]] == 11) total++;
    }
    return total;
}
int totalHand(int* hand, int count) {
    int total = 0;
    int aceCount = 0;
    for(int i = 0; i < count; i++) {
        total += cardValues[hand[i]];
        if(cardValues[hand[i]] == 11) aceCount++;
    }
    //Lower ace values if over 21
    while(aceCount > 0 && total > 21) {
        total -= 10;
        aceCount--;
    }
    return total;
}
#pragma endregion
#pragma region Simulations
int runDealer(int* hand, int handSize) {
    int dealerTotal = totalHand(hand, handSize);
    int dealerAces = aceCount(hand, handSize);
    while(dealerTotal < 17 || (dealerTotal == 17 && dealerAces != 0)) {
        //Draw card
        hand[handSize] = drawCard();
        handSize++;
        dealerTotal = totalHand(hand, handSize);
        dealerAces = aceCount(hand, handSize);
    }
}
bool doesPlayerWin(int playerTotal,int dealerTotal) {
    if(playerTotal>21) return false;
    if(dealerTotal>21) return true;
    if(dealerTotal<playerTotal) return true;
    return false;
}
void runSimulation() {
    int dealerHand[20];
    int playerHand[20];
    playerHand[0]=drawCard();
    playerHand[1]=drawCard();
    dealerHand[0]=drawCard();
    int dealerShown=totalHand(dealerHand,1);
    int playerShown=totalHand(playerHand,2);
    int playerTotal=playerShown;
    dealerHand[1]=drawCard();
    int dealerTotal=runDealer(dealerHand,2);
    //Increase count of table
    table[playerShown][dealerShown].total++;
    //Does the dealer busts
    if(dealerTotal>21) table[playerShown][dealerShown].dealerBusts++;
    //Does standing win?
    table[playerShown][dealerShown].stand+=doesPlayerWin(playerTotal,dealerTotal);
    //Does hitting once win?
    playerHand[2]=drawCard();
    table[playerShown][dealerShown].doubleDown+=doesPlayerWin(totalHand(playerHand,3),dealerTotal);
    undrawCard(playerHand[2]);
    //Does the optimal play win?
}
#pragma endregion
int main() {
    initializeDeck();
    setTable();
    for(int i = 0;i < 100000;i++) runSimulation();

}
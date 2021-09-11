//Benjamin Cates, Damaris Sanchez, Sukhman Kaur, and Professor David Jones
//2021
//Blackjack probability calculator
#include <iostream>
#include <cstdlib>

using namespace std;
const int cardValues[] = { 0,11,2,3,4,5,6,7,8,9,10,10,10,10 };
const char cardNames[] = { '?','A','2','3','4','5','6','7','8','9','T','J','Q','K' };
int deckCount;
int playerCount = 0;
int playerPosition = 0;

int* deck;
void getGameState() {
    cout << "Enter deck count: ";
    cin >> deckCount;
    //cout << "Enter player count: ";
    //cin >> playerCount;
    //cout << "Enter player position: ";
    //cin >> playerPosition;
}
int drawCard(int* deck, int* deckSize) {
    int index = rand() % *deckSize;
    int value = deck[index];
    deck[index] = deck[*deckSize - 1];
    (*deckSize)--;
    return value;
}
int aceCount(int* hand, int count) {
    int total = 0;
    for(int i = 0;i < count;i++) {
        if(hand[i] == 1) total++;
    }
    return total;
}
void printHand(int* hand, int count) {
    for(int i = 0;i < count;i++) {
        cout << cardNames[hand[i]];
        if(i != count - 1) cout << ", ";
    }
}
int totalHand(int* hand, int count) {
    int total = 0;
    int aceCount = 0;
    for(int i = 0;i < count;i++) {
        total += cardValues[hand[i]];
        if(hand[i] == 1) aceCount++;
    }
    //Lower ace values if over 21
    while(aceCount > 0 && total > 21) {
        total -= 10;
        aceCount--;
    }
    return total;
}
void runRound(int* deck, int* cardCountReference) {
    //Player's cards
    int playerHand[10];
    int playerHandSize = 1;
    playerHand[0] = drawCard(deck, cardCountReference);
    //Dealer's cards
    int dealerCards[10];
    int dealerHandSize = 2;
    dealerCards[0] = drawCard(deck, cardCountReference);
    dealerCards[1] = drawCard(deck, cardCountReference);
    //Print out dealer's card
    cout << "Dealer is showing: " << cardNames[dealerCards[0]] << endl;
    //Prompt for hits
    char hit = 'y';
    int playerTotal = 0;
    int dealerTotal = 0;
    while(hit == 'y' || hit == 'Y') {
        //Draw card
        playerHand[playerHandSize] = drawCard(deck, cardCountReference);
        playerHandSize++;
        //Print and count cards
        cout << "Your cards right now: ";
        printHand(playerHand, playerHandSize);
        cout << endl;
        playerTotal = totalHand(playerHand, playerHandSize);
        //Busting
        if(playerTotal > 21) {
            cout << "You busted (total=" << playerTotal << ")" << endl;
            break;
        }
        //Natural 21
        if(playerTotal == 21 && playerHandSize == 2) {
            cout << "You hit a natural 21" << endl;
            break;
        }
        else cout << "Total count: " << playerTotal << endl;
        cout << "Would you like to hit? (y, n) ";
        cin >> hit;
    }
    cout << "Dealer's cards: ";
    printHand(dealerCards, dealerHandSize);
    dealerTotal = totalHand(dealerCards, dealerHandSize);
    cout << endl << "Dealer total: " << dealerTotal << endl;
    int dealerAces = aceCount(dealerCards, dealerHandSize);
    while(dealerTotal < 17 || (dealerTotal == 17 && dealerAces != 0)) {
        //Draw card
        dealerCards[dealerHandSize] = drawCard(deck, cardCountReference);
        //Print cards
        dealerHandSize++;
        cout << "Dealer's hand: ";
        printHand(dealerCards, dealerHandSize);
        dealerTotal = totalHand(dealerCards, dealerHandSize);
        //Print total
        cout << endl << "Dealer total: " << dealerTotal << endl;
        dealerAces = aceCount(dealerCards, dealerHandSize);
    }
    cout << "Player total: " << playerTotal << endl;
    //Player busts
    if(playerTotal > 21) cout << "Player busts. Dealer wins" << endl;
    //Dealer busts
    else if(dealerTotal > 21) cout << "Dealer busts. Player wins" << endl;
    //Player wins
    else if(playerTotal > dealerTotal) cout << "Player wins" << endl;
    //Dealer wins
    else if(playerTotal == dealerTotal) cout << "Player pushes" << endl;
    else cout << "Dealer wins" << endl;
}
int main(void) {
    srand(time(0));
    cout << "Blackjack simulator $0.99" << endl;
    getGameState();
    int cardCount = deckCount * 52;
    int deck[cardCount];
    for(int i = 0;i < cardCount;i++) {
        deck[i] = (i / 4) % 13 + 1;
    }
    runRound(deck,&cardCount);
    return 0;
}
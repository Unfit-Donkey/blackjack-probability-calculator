//Benjamin Cates, Damaris Sanchez, Sukhman Kaur, and Professor David Jones
//2021
//Blackjack probability calculator
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;
const int cardValues[] = {
    0,11,2,3,4,5,6,7,8,9,10,10,10,10,
    11,2,3,4,5,6,7,8,9,10,10,10,10,
    11,2,3,4,5,6,7,8,9,10,10,10,10,
    11,2,3,4,5,6,7,8,9,10,10,10,10
};
const string cardNames[] = {
    "Wild","As","2s","3s","4s","5s","6s","7s","8s","9s","Ts","Js","Qs","Ks",
    "Ad","2d","3d","4d","5d","6d","7d","8d","9d","Td","Jd","Qd","Kd",
    "Ah","2h","3h","4h","5h","6h","7h","8h","9h","Th","Jh","Qh","Kh",
    "Ac","2c","3c","4c","5c","6c","7c","8c","9c","Tc","Jc","Qc","Kc" };
int deckCount;
int playerCount = 0;
int playerPosition = 0;
int bankRoll = 10000;
int wager=50;
int runningCount = 0;


int* deck;
void getGameState() {
   // cout << "Enter deck count: ";
    deckCount = 1;
    //cin >> deckCount;
    //cout << "Enter player count: ";
    //cin >> playerCount;
    //cout << "Enter player position: ";
    //cin >> playerPosition;
}
int runningCounter(int cardID) {
    int temp = cardValues[cardID];
    if (temp >= 2 && temp <= 6) return 1;
    else if (temp == 10 && temp == 11) return -1;
    else return 0;
    }
int drawCard(int* deck, int* deckSize) {
    int index = rand() % *deckSize;
    int cardID = deck[index];
    runningCount += runningCounter(cardID);
    deck[index] = deck[*deckSize - 1];
    (*deckSize)--;
    return cardID;
}
int aceCount(int* hand, int count) {
    int total = 0;
    for (int i = 0;i < count;i++) {
        if (cardValues[hand[i]] == 11) total++;
    }
    return total;
}
void printHand(int* hand, int count) {
    for (int i = 0;i < count;i++) {
        cout << cardNames[hand[i]];
        if (i != count - 1) cout << ", ";
    }
}
int totalHand(int* hand, int count) {
    int total = 0;
    int aceCount = 0;
    for (int i = 0;i < count;i++) {
        total += cardValues[hand[i]];
        if (cardValues[hand[i]] == 11) aceCount++;
    }
    //Lower ace values if over 21
    while (aceCount > 0 && total > 21) {
        total -= 10;
        aceCount--;
    }
    return total;
}
void generateDeck(int* deck, int count) {
    for (int i = 0;i < count;i++) {
        deck[i] = i + 1;
    }
}
void runRound(int* deck, int* cardCountReference) {
    //Player's cards
    cout << "How much would you like to wager this round? : ";
    cin >> wager;
    while  (wager > bankRoll || wager < 0) {
        if (wager > bankRoll) {
            cout << "You don't have that much money.  Bet a lower amount: ";
            cin >> wager;
        }
        else {
            cout << "You can't bet negative money.  Bet a real amount: ";
            cin >> wager;
        }
    }


    char hit = 'y';
    int playerTotal = 0;
    int dealerTotal = 0;
    int playerHand[10];
    int playerHandSize = 1;
    playerHand[0] = drawCard(deck, cardCountReference);
    //Dealer's cards
    int dealerCards[10];
    int dealerHandSize = 2;
    dealerCards[0] = drawCard(deck, cardCountReference);
    dealerCards[1] = drawCard(deck, cardCountReference);
    runningCount -= runningCounter(dealerCards[1]);
    //Print out dealer's card
    cout << "Dealer is showing: " << cardNames[dealerCards[0]] << endl;
    //Prompt for hits

    while (hit == 'y' || hit == 'Y') {
        //Draw card
        playerHand[playerHandSize] = drawCard(deck, cardCountReference);
        playerHandSize++;
        //Print and count cards
        cout << "Your cards right now: ";
        printHand(playerHand, playerHandSize);
        cout << "\nThe current card count is : " << runningCount<< endl;
        playerTotal = totalHand(playerHand, playerHandSize);
        //Busting
        if (playerTotal > 21) {
            cout << "You busted (total=" << playerTotal << ")" << endl;

            break;
        }
        //Natural 21
        if (playerTotal == 21 && playerHandSize == 2) {
            cout << "You hit a natural 21" << endl;
            bankRoll = bankRoll + .5 * wager;
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
    while (dealerTotal < 17 || (dealerTotal == 17 && dealerAces != 0)) {
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
    if (playerTotal > 21) {
        bankRoll = bankRoll - wager;
        cout << "Player busts. Dealer wins" << endl;
    }
    //Dealer busts
    else if (dealerTotal > 21) {
        bankRoll = bankRoll + wager;
        cout << "Dealer busts. Player wins" << endl;
    }
    //Player wins
    else if (playerTotal > dealerTotal) {
        cout << "Player wins" << endl;
        bankRoll = bankRoll + wager;
    }
    //Dealer wins
    else if (playerTotal == dealerTotal) cout << "Player pushes" << endl;
    else {
        bankRoll = bankRoll - wager;
        cout << "Dealer wins" << endl;
    }
    runningCount += runningCounter(dealerCards[1]);
    cout << "Current bank roll is : " << bankRoll << endl;
}
int main(void) {
    srand(time(0));

    cout << "Blackjack simulator $0.99" << endl;
    getGameState();
    cout << "How much money are you starting with? ";
    cin >> bankRoll;
    cout << endl << endl;
    int cardCount = deckCount * 52;
    int originalCardCount = cardCount;
    int* deck = (int*)malloc(cardCount * sizeof(int));
    generateDeck(deck, cardCount);
    char again ='y';
    while (again == 'Y' || again == 'y') {
        runRound(deck, &cardCount);
        if (bankRoll <= 0) {
            cout << "You lose, chump.  You're out of money. The loan shark is comin!" << endl;
            return 0;
        }
        cout << endl;
        if (cardCount < originalCardCount / 2) {
            runningCount = 0;
            generateDeck(deck, originalCardCount);
            cardCount = originalCardCount;
            cout << "\n\nDeck reshuffled\n\n";
        }
        cout << "Would you like to keep playing? Enter y or n: ";
        cin >> again;
    }
    free(deck);
    return 0;
}

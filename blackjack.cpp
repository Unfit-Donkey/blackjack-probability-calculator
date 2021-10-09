//Benjamin Cates, Damaris Sanchez, Sukhman Kaur, and Professor David Jones
//2021
//Blackjack probability calculator
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <limits>

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
int deckSize;
int* deckSizeReference = &deckSize;
int playerCount = 0;
int playerPosition = 0;
int bankRoll = 10000;
int wager = 50;
int runningCount = 0;
int roundCount = 1;


int* deck;

int promptForInt(const char* prompt, int min, int max) {
    int out;
    while(1) {
        cout << prompt;
        cin >> out;
        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input (expects integer)" << endl;
            continue;
        }
        if(out > max) cout << "Input must be less than " << max + 1 << endl;
        else if(out < min) cout << "Input must be greater than " << min - 1 << endl;
        else return out;
    }
}
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
    if(temp >= 2 && temp <= 6) return 1;
    else if(temp == 10 || temp == 11) return -1;
    else return 0;
}
int drawCard() {
    int index = rand() % deckSize;
    int cardID = deck[index];
    runningCount += runningCounter(cardID);
    deckSize--;
    deck[index] = deck[deckSize];
    return cardID;
}
int aceCount(int* hand, int count) {
    int total = 0;
    for(int i = 0; i < count; i++) {
        if(cardValues[hand[i]] == 11) total++;
    }
    return total;
}
void printHand(int* hand, int count) {
    for(int i = 0; i < count; i++) {
        cout << cardNames[hand[i]];
        if(i != count - 1) cout << ", ";
    }
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
void generateDeck(int* deck, int count) {
    for(int i = 0; i < count; i++) {
        deck[i] = (i + 1) % 52;
    }
}
int playHand(int* hand, int* handSizeReference) {
    char hit = 'h';
    int total = 0;
    while(true) {
        cout << "\n[card count: " << runningCount << "] Hit, stand, or double down? (h s d) ";
        cin >> hit;
        if(hit == 's' || hit == 'S') {
            total = totalHand(hand, *handSizeReference);
            break;
        }
        if(hit == 'd' || hit == 'D') wager *= 2;
        //Draw card
        hand[*handSizeReference] = drawCard();
        (*handSizeReference)++;
        //Print and count cards
        total = totalHand(hand, *handSizeReference);
        cout << "Your cards right now (" << total << "): ";
        printHand(hand, *handSizeReference);
        cout << endl;
        //Busting
        if(total >= 21) break;
        if(hit == 'd' || hit == 'D') break;
    }
    return total;
}
int payoutResults(int player, int dealer) {
    if(player > 21) {
        bankRoll = bankRoll - wager;
        cout << "Player busts. Dealer wins" << endl;
        return -1;
    }
    if(dealer > 21) {
        bankRoll = bankRoll + wager;
        cout << "Dealer busts. Player wins" << endl;
        return 1;
    }
    if(player > dealer) {
        cout << "Player wins" << endl;
        bankRoll = bankRoll + wager;
        return 1;
    }
    if(player == dealer) {
        cout << "Player pushes" << endl;
        return 0;
    }
    //Dealer wins
    bankRoll = bankRoll - wager;
    cout << "Dealer wins" << endl;
    return -1;
}
bool runRound() {
    //Player's cards
    cout << "------------- round " << roundCount << " -------------" << endl << endl;
    wager = promptForInt("What is your wager? (0 to quit): ", 0, bankRoll);
    if(wager <= 0) return false;
    char hit = 'h';
    int playerTotal = 0;
    int dealerTotal = 0;
    int playerHand[10];
    int playerHandSize = 2;
    playerHand[0] = drawCard();
    playerHand[1] = drawCard();
    //Dealer's cards
    int dealerCards[10];
    int dealerHandSize = 2;
    dealerCards[0] = drawCard();
    dealerCards[1] = drawCard();
    runningCount -= runningCounter(dealerCards[1]);
    //Print out dealer's card
    cout << "Dealer is showing: " << cardNames[dealerCards[0]] << endl;
    //Print and count cards
    playerTotal = totalHand(playerHand, playerHandSize);
    cout << "Current hand (" << playerTotal << "): ";
    printHand(playerHand, playerHandSize);
    //Splitting opportunity
    int secondHand[10];
    int secondHandSize = 0;
    int secondHandTotal = 0;
    int secondWager = wager;
    int firstWager = wager;
    char split = 'n';
    if(cardValues[playerHand[0]] == cardValues[playerHand[1]]) {
        cout << "\n\nWould you like to split? (y or n): ";
        cin >> split;
        if(split == 'y' || split == 'Y') {
            playerHandSize--;
            secondHand[0] = playerHand[1];
            secondHandSize = 1;
            cout << "---- Playing first hand ----" << endl;
            cout << "Current hand (" << cardValues[secondHand[0]] << "): ";
            printHand(secondHand, secondHandSize);
            secondHandTotal = playHand(secondHand, &secondHandSize);
            firstWager = wager;
            wager = secondWager;
            cout << "---- Playing second hand ----" << endl;
            cout << "Current hand (" << cardValues[playerHand[0]] << "): ";
            printHand(playerHand, playerHandSize);
            secondWager = wager;
        }
    }
    //Natural 21
    if(playerTotal == 21) {
        cout << "\nYou hit a natural 21";
        bankRoll = bankRoll + .5 * wager;
    }
    //Hit loop
    else {
        cout << endl;
        playerTotal = playHand(playerHand, &playerHandSize);
    }
    dealerTotal = totalHand(dealerCards, dealerHandSize);
    int dealerAces = aceCount(dealerCards, dealerHandSize);
    while(playerTotal <= 21 && secondHandTotal < 21 && (dealerTotal < 17 || (dealerTotal == 17 && dealerAces != 0))) {
        //Draw card
        dealerCards[dealerHandSize] = drawCard();
        dealerHandSize++;
        //Print cards
        //cout << "Dealer's hand: ";
        //printHand(dealerCards, dealerHandSize);
        dealerTotal = totalHand(dealerCards, dealerHandSize);
        //Print total
        //cout << endl << "Dealer total: " << dealerTotal << endl;
        dealerAces = aceCount(dealerCards, dealerHandSize);
    }
    cout << endl << "Dealer's hand: ";
    printHand(dealerCards, dealerHandSize);
    dealerTotal = totalHand(dealerCards, dealerHandSize);
    //Print total
    cout << endl << "Dealer total: " << dealerTotal << endl;
    dealerAces = aceCount(dealerCards, dealerHandSize);
    cout << "Player total: " << playerTotal << endl;
    wager = firstWager;
    payoutResults(playerTotal, dealerTotal);
    if(split == 'y' || split == 'Y') {
        cout << "---Second hand payouts---" << endl << "Second hand total: " << secondHandTotal << endl;
        wager = secondWager;
        payoutResults(secondHandTotal, dealerTotal);
    }
    runningCount += runningCounter(dealerCards[1]);
    cout << "Current bank roll is : " << bankRoll << endl;
    return true;
}
int main(int argc, char** argv) {
    srand(time(0));

    cout << "Blackjack simulator $0.99" << endl;
    getGameState();
    bankRoll = promptForInt("How much money are you starting with? ", 0, 99999999);
    cout << endl;
    deckSize = deckCount * 52;
    int originalDeckSize = deckSize;
    deck = (int*)malloc(deckSize * sizeof(int));
    generateDeck(deck, deckSize);
    while(runRound()) {
        roundCount++;
        if(bankRoll <= 0) {
            cout << "You lose, chump.  You're out of money. The loan shark is comin!" << endl;
            return 0;
        }
        cout << endl;
        if(deckSize < originalDeckSize / 2) {
            runningCount = 0;
            generateDeck(deck, originalDeckSize);
            deckSize = originalDeckSize;
            cout << "***************\nDeck reshuffled\n***************\n\n";
        }
    }
    free(deck);
    return 0;
}

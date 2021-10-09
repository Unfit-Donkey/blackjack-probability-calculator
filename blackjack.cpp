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
    "Ac","2c","3c","4c","5c","6c","7c","8c","9c","Tc","Jc","Qc","Kc"
};
int deckCount = 1;
int deckSize;
int* deckSizeReference = &deckSize;
int bankRoll = 10000;
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
#pragma region Preferences
bool useBets = true;
bool showCardCount = false;
void getPreferences() {
    char input;
    cout << "Would you like to disable bets (y)? ";
    cin >> input;
    if(input == 'y' || input == 'Y') useBets = false;
    cout << "Would you like to show card count (y)? ";
    cin >> input;
    if(input == 'y' || input == 'Y') showCardCount = true;
    deckCount = promptForInt("Enter deck count: ", 1, 256);
    cout << "Preferences updated" << endl << endl;
}
#pragma endregion
int runningCounter(int cardID) {
    int temp = cardValues[cardID];
    if(temp >= 2 && temp <= 6) return 1;
    else if(temp == 10 || temp == 11) return -1;
    else return 0;
}
int drawCard() {
    int index = rand() % deckSize;
    int cardID = deck[index];
    if(showCardCount) runningCount += runningCounter(cardID);
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
        deck[i] = i % 52 + 1;
    }
}
int playHand(int* hand, int* handSizeReference, int* wagerReference) {
    char hit = 'h';
    int total = 0;
    while(hit == 'h' || hit == 'H') {
        cout << endl;
        if(showCardCount) cout << "[card count: " << runningCount << "] ";
    promptHit:
        if(*handSizeReference == 2) cout << "Hit, stand, or double down? (h s d q) ";
        else cout << "Hit or stand? (h s q) ";
        cin >> hit;
        cin.ignore();
        switch(hit) {
        case 'q':
            exit(0);
        case 's': case 'S':
            return totalHand(hand, *handSizeReference);
        case 'd': case 'D':
            if(*handSizeReference == 2) (*wagerReference) *= 2;
        case 'h': case 'H':
            //Draw card
            hand[*handSizeReference] = drawCard();
            (*handSizeReference)++;
            //Print and count cards
            total = totalHand(hand, *handSizeReference);
            cout << "Your cards right now (" << total << "): ";
            printHand(hand, *handSizeReference);
            cout << endl;
            break;
        default:
            cout << "Please enter a valid response" << endl;
            goto promptHit;
        }
        //Busting
        if(total >= 21) break;
    }
    return total;
}
int payoutResults(int player, int dealer, int wager) {
    char winner = 'p';
    if(player > 21) {
        cout << "Player busts; " << endl;
        winner = 'd';
    }
    if(dealer > 21) cout << "Dealer busts; " << endl;
    if(dealer > player) winner = 'd';
    else if(player == dealer) {
        cout << "Player pushes" << endl;
        return 0;
    }
    if(winner == 'p') {
        cout << "Player wins" << endl;
        bankRoll += wager;
        return 1;
    }
    //Dealer wins
    bankRoll -= wager;
    cout << "Dealer wins" << endl;
    return -1;
}
bool runRound() {
    //Player's cards
    cout << "------------- round " << roundCount << " -------------" << endl << endl;
    int firstWager = 0, secondWager = 0;
    if(useBets) firstWager = promptForInt("What is your wager? (0 to quit): ", 0, bankRoll);
    if(firstWager <= 0 && useBets) return false;
    char hit = 'h';
    int playerTotal = 0, dealerTotal = 0;
    int playerHand[10], dealerCards[10];
    int playerHandSize = 2, dealerHandSize = 2;
    playerHand[0] = drawCard();
    playerHand[1] = drawCard();
    dealerCards[0] = drawCard();
    dealerCards[1] = drawCard();
    //Uncount dealer's hidden card
    runningCount -= runningCounter(dealerCards[1]);
    cout << "Dealer is showing: " << cardNames[dealerCards[0]] << endl;
    //Print and count cards
    playerTotal = totalHand(playerHand, playerHandSize);
    cout << "Current hand (" << playerTotal << "): ";
    printHand(playerHand, playerHandSize);
    //Splitting opportunity
    int secondHand[10];
    int secondHandTotal = 22;
    char split = 'n';
    if(cardValues[playerHand[0]] == cardValues[playerHand[1]]) {
        cout << "\n\nWould you like to split? (y) ";
        cin >> split;
    }
    if(split == 'y' || split == 'Y') {
        secondHand[0] = playerHand[1];

        cout << "---- Playing first hand ----" << endl;
        playerHand[1] = drawCard();
        cout << "Current hand (" << cardValues[playerHand[0]] << "): ";
        printHand(playerHand, playerHandSize);
        playerTotal = playHand(playerHand, &playerHandSize, &firstWager);

        cout << "---- Playing second hand ----" << endl;
        int secondHandSize = 2;
        secondHand[1] = drawCard();
        cout << "Current hand (" << cardValues[secondHand[0]] << "): ";
        printHand(secondHand, secondHandSize);
        secondHandTotal = playHand(secondHand, &secondHandSize, &secondWager);
    }
    //Natural 21
    else if(playerTotal == 21) {
        cout << "\nYou hit a natural 21";
        bankRoll += .5 * firstWager;
    }
    //Hit loop
    else playerTotal = playHand(playerHand, &playerHandSize, &firstWager);
    //Dealer hit loop
    dealerTotal = totalHand(dealerCards, dealerHandSize);
    int dealerAces = aceCount(dealerCards, dealerHandSize);
    while((playerTotal <= 21 || secondHandTotal <= 21) && (dealerTotal < 17 || (dealerTotal == 17 && dealerAces != 0))) {
        //Draw card
        dealerCards[dealerHandSize] = drawCard();
        dealerHandSize++;
        dealerTotal = totalHand(dealerCards, dealerHandSize);
        dealerAces = aceCount(dealerCards, dealerHandSize);
    }
    //Print dealer's hand
    cout << "\n\nDealer's hand: ";
    printHand(dealerCards, dealerHandSize);
    dealerTotal = totalHand(dealerCards, dealerHandSize);
    cout << endl << "Dealer total: " << dealerTotal << endl;
    dealerAces = aceCount(dealerCards, dealerHandSize);
    //Print player's total
    cout << "Player total: " << playerTotal << endl;
    //Payout
    payoutResults(playerTotal, dealerTotal, firstWager);
    if(split == 'y' || split == 'Y') {
        cout << "---Second hand payouts---" << endl << "Second hand total: " << secondHandTotal << endl;
        payoutResults(secondHandTotal, dealerTotal, secondWager);
    }
    runningCount += runningCounter(dealerCards[1]);
    if(useBets) cout << "Current bank roll is : " << bankRoll << endl;
    return true;
}
int main(int argc, char** argv) {
    srand(time(0));

    cout << "Blackjack simulator $0.99" << endl;
    char preferences;
    cout << "Would you like to edit preferences [card count=n, deck count=1, use bets=y] (y)? ";
    cin >> preferences;
    if(preferences == 'y' || preferences == 'Y') getPreferences();
    if(useBets) bankRoll = promptForInt("How much money are you starting with? ", 0, 99999999);
    cout << endl;
    deckSize = deckCount * 52;
    int originalDeckSize = deckSize;
    deck = (int*)malloc(deckSize * sizeof(int));
    generateDeck(deck, deckSize);
    while(runRound()) {
        roundCount++;
        if(bankRoll <= 0 && useBets) {
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

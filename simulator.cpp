//Benjamin Cates, Damaris Sanchez, Sukhman Kaur, Amna Ali and Profesor David Jones

#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;
#pragma region Deck
typedef unsigned char card;
int deck[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
double deckSize;
int deckCount;
int totalHand(card* hand, int count) {
    int total = 0;
    int aceCount = 0;
    for(int i = 0; i < count; i++) {
        total += hand[i];
        if(hand[i] == 11) aceCount++;
    }
    //Lower ace values if over 21
    while(total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }
    if(total > 21) return 22;
    return total;
}
int aceCount(card* hand, int count) {
    int total = 0;
    for(int i = 0; i < count; i++) {
        if(hand[i] == 11) total++;
    }
    return total;
}
bool hasAce(card* hand, int len) {
    return memchr(hand, 11, len) != NULL;
}
void generateDeck() {
    for(int i = 2;i < 12;i++) deck[i] = deckCount * 4;
    deck[10] = 16 * deckCount;
}
card getCard(char symbol) {
    if(symbol >= 'a' && symbol <= 'z') symbol -= 32;
    if(symbol == 'K' || symbol == 'Q' || symbol == 'J' || symbol == 'T') return 10;
    if(symbol == 'A') return 11;
    return symbol - '0';
}
#pragma endregion
struct WinLossProb {
    double win;
    double tie;
    double loss;
};
#pragma region Optimal Strategies
typedef enum Action {
    hit,
    ddw,
    stn,
} Action;
const Action hardOptimalStrategy[22][12] = {
    //D: 0   1   2   3   4   5   6   7   8   9   10  A      \/ Player total
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},//0
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},//5
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},
        {hit,hit,hit,ddw,ddw,ddw,ddw,hit,hit,hit,hit,hit},
        {hit,hit,ddw,ddw,ddw,ddw,ddw,ddw,ddw,ddw,ddw,hit},//10
        {hit,hit,ddw,ddw,ddw,ddw,ddw,ddw,ddw,ddw,ddw,ddw},
        {hit,hit,ddw,ddw,ddw,ddw,ddw,ddw,ddw,ddw,ddw,ddw},
        {hit,hit,hit,hit,stn,stn,stn,hit,hit,hit,hit,hit},
        {hit,hit,stn,stn,stn,stn,stn,hit,hit,hit,hit,hit},
        {hit,hit,stn,stn,stn,stn,stn,hit,hit,hit,hit,hit},//15
        {hit,hit,stn,stn,stn,stn,stn,hit,hit,hit,hit,hit},
        {hit,hit,stn,stn,stn,stn,stn,stn,stn,stn,stn,stn},
        {hit,hit,stn,stn,stn,stn,stn,stn,stn,stn,stn,stn},
        {hit,hit,stn,stn,stn,stn,stn,stn,stn,stn,stn,stn},
        {hit,hit,stn,stn,stn,stn,stn,stn,stn,stn,stn,stn},//20
        {hit,hit,stn,stn,stn,stn,stn,stn,stn,stn,stn,stn},//21
};
const Action softOptimalStrategy[12][12] = {
    //D: 0   1   2   3   4   5   6   7   8   9   10  A      \/ Player other card
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},//0
        {hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit,hit},//1
        {hit,hit,hit,hit,hit,ddw,ddw,hit,hit,hit,hit,hit},//2
        {hit,hit,hit,hit,hit,ddw,ddw,hit,hit,hit,hit,hit},//3
        {hit,hit,hit,hit,ddw,ddw,ddw,hit,hit,hit,hit,hit},//4
        {hit,hit,hit,hit,ddw,ddw,ddw,hit,hit,hit,hit,hit},//5
        {hit,hit,hit,ddw,ddw,ddw,ddw,hit,hit,hit,hit,hit},//6
        {hit,hit,stn,ddw,ddw,ddw,ddw,stn,stn,hit,hit,hit},//7
        {hit,hit,stn,stn,stn,stn,stn,stn,stn,stn,stn,stn},//8
        {hit,hit,stn,stn,stn,stn,stn,stn,stn,stn,stn,stn},//9
        {hit,hit,stn,stn,stn,stn,stn,stn,stn,stn,stn,stn},//10
};
Action optimal(card* hand, int handSize, card dealer) {
    int total = totalHand(hand, handSize);
    if(total > 21) return stn;
    if(hasAce(hand, handSize)) {
        int aceIndex = 0;
        for(int i = 0;i < handSize;i++) if(hand[i] == 11) aceIndex = i;
        card otherCard;
        if(aceIndex == handSize - 1) otherCard = hand[aceIndex - 1];
        else otherCard = hand[aceIndex + 1];
        return softOptimalStrategy[otherCard][dealer];
    }
    return hardOptimalStrategy[total][dealer];
}
#pragma endregion
void getGameData() {
    cout << "Enter deck count: ";
    cin >> deckCount;
    deckSize = 52.0 * deckCount;
}
#pragma region Win-Loss Probabilities
struct WinLossProb probabilityMash(double* player, double* dealer) {
    struct WinLossProb out;
    out.win = 0.0;
    out.loss = 0.0;
    out.tie = 0.0;
    for(int pl = 2;pl < 22;pl++) for(int dl = 2;dl < 22;dl++) {
        double prob = player[pl] * dealer[dl];
        if(pl > dl) out.win += prob;
        if(pl < dl) out.loss += prob;
        if(pl == dl) out.tie += prob;
    }
    out.win += dealer[22] * (1 - player[22]);
    out.loss += player[22] * (1 - dealer[22]);
    out.tie += player[22] * dealer[22];
    return out;
}
void printProb(string name, struct WinLossProb P) {
    cout << setprecision(3);
    cout << name << " win: " << P.win * 100 << "%" << endl;
    cout << name << " lose: " << P.loss * 100 << "%" << endl;
    if(P.tie != 0) cout << name << " tie: " << P.tie * 100 << "%" << endl;
    cout << endl;
}
#pragma endregion
#pragma region Probability Calculators
void dealerProb(double* out, double chance, card* hand, int handSize) {
    double ret = 0.0;
    deckSize--;
    for(int i = 2;i < 12;i++) {
        if(deck[i] == 0) continue;
        double newChance = chance * (deck[i] / (deckSize + 1));
        hand[handSize] = i;
        deck[i]--;
        int total = totalHand(hand, handSize + 1);
        if(total > 17 || (total == 17 && hasAce(hand, handSize + 1))) out[total] += newChance;
        else dealerProb(out, newChance, hand, handSize + 1);
        deck[i]++;
    }
    deckSize++;
}
void doubleDownProb(double* out, card* hand) {
    double ret = 0.0;
    for(int i = 2;i < 12;i++) {
        hand[2] = i;
        int total = totalHand(hand, 3);
        out[total] += deck[i] / deckSize;
    }
}
void optimalStratProb(double* out, double chance, card* hand, int handSize, card dealer) {
    Action op = optimal(hand, handSize, dealer);
    if(op == ddw && handSize != 2) op = hit;
    int total = totalHand(hand, handSize);
    if(op == hit) {
        deckSize--;
        for(int i = 2;i < 12;i++) {
            double newChance = chance * deck[i] / (deckSize + 1);
            deck[i]--;
            hand[handSize] = i;
            optimalStratProb(out, newChance, hand, handSize + 1, dealer);
            deck[i]++;
        }
        deckSize++;
    }
    else if(op == ddw) {
        for(int i = 2;i < 12;i++) {
            hand[2] = i;
            total = totalHand(hand, 3);
            out[total] += chance * deck[i] / deckSize;
        }
    }
    else if(op == stn) {
        out[total] += chance;
    }
}
#pragma endregion
void runRound() {
    card playerHand[20];
    card dealerHand[20];
    //Get cards
    char symbol;
    cout << "What is your card? ";
    cin >> symbol;
    playerHand[0] = getCard(symbol);
    cout << "What is your second card? ";
    cin >> symbol;
    playerHand[1] = getCard(symbol);
    cout << "What is the dealer's card? ";
    cin >> symbol;
    dealerHand[0] = getCard(symbol);
    deck[playerHand[0]]--;
    deck[playerHand[1]]--;
    deck[dealerHand[0]]--;
    deckSize -= 3;
    cout << endl;

    int playerTotal = totalHand(playerHand, 2);
    //Dealer probability
    double dealerP[23] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    dealerProb(dealerP, 1.0, dealerHand, 1);

    //Stand probability
    double standP[23] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    standP[totalHand(playerHand, 2)] = 1;
    printProb("Stand", probabilityMash(standP, dealerP));

    //Double down probability
    double doubleDownP[23] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    doubleDownProb(doubleDownP, playerHand);
    printProb("Double down", probabilityMash(doubleDownP, dealerP));

    //Optimal probability
    Action op = optimal(playerHand, 2, dealerHand[0]);
    cout << "Optimal table says: ";
    if(op == hit) cout << "hit";
    if(op == stn) cout << "stand";
    if(op == ddw) cout << "double down";
    cout << endl;
    double optimalP[23] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    optimalStratProb(optimalP, 1.0, playerHand, 2, dealerHand[0]);
    printProb("Optimal", probabilityMash(optimalP, dealerP));

    //Busting probability
    cout << "Bust on next hit: " << doubleDownP[22]*100 << "%" << endl << endl;

}
int main(void) {
    getGameData();
    generateDeck();
    runRound();
}
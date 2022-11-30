#include "lib.h"
Deck deck;
void Deck::drawFromDeck(int count) {
    for(int i = 0;i < count;i++) {
        Card c = deck.draw();
        append(c);
    }
}

struct WinLoss {
    int wins;
    int losses;
    int pushes;
    int total() {
        return wins + losses + pushes;
    }
    WinLoss() {
        wins = 0;
        losses = 0;
        pushes = 0;

    }
};
void runDealer(Deck& dealer) {
    while(dealer.total() < 17)
        dealer.drawFromDeck();
    if(dealer.total() == 17 && dealer.aceCount() != 0)
        dealer.drawFromDeck();
}
vector<WinLoss> table[2];
void recordInTable(WinLoss& cell, int playerTotal, int dealerTotal) {
    if(playerTotal > 21) cell.losses++;
    else if(dealerTotal > 21) cell.wins++;
    else if(dealerTotal > playerTotal) cell.losses++;
    else if(dealerTotal < playerTotal) cell.wins++;
    else if(playerTotal == 21) cell.wins++;
    else cell.pushes++;
}

void simulateGame() {
    Deck player;
    player.append(deck.draw());
    player.append(deck.draw());
    Deck dealer;
    dealer.append(deck.draw());
    dealer.append(deck.draw());
    int playerTotal = player.total();
    if(playerTotal == 21 && dealer.total() == 21) {
        table[0][playerTotal].pushes++;
        return;
    }
    runDealer(dealer);
    recordInTable(table[0][playerTotal], playerTotal, dealer.total());
    player.append(deck.draw());
    recordInTable(table[1][playerTotal], player.total(), dealer.total());
    player.erase(2);
    runDealer(player);
    recordInTable(table[2][playerTotal], player.total(), dealer.total());
}

const int runCount = 1000000;
int main() {
    table[0] = vector<WinLoss>(22);
    table[1] = vector<WinLoss>(22);
    table[2] = vector<WinLoss>(22);
    for(int i = 0;i < runCount;i++) {
        deck = Deck(1);
        simulateGame();
    }
    for(int i = 4;i < 22;i++) {

        cout << "Player has: " << i << endl;
        if(table[0][i].total() != 0) cout << "Stand wins: " << table[0][i].wins * 1.0 / table[0][i].total() << endl;
        if(table[1][i].total() != 0) cout << "DD wins: " << table[1][i].wins * 1.0 / table[1][i].total() << endl;
        if(table[2][i].total() != 0) cout << "DealerStrat wins: " << table[2][i].wins * 1.0 / table[2][i].total() << endl;
        cout << endl;
    }
    return 0;
}
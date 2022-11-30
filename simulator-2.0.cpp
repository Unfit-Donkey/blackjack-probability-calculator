#include "lib.h"

Deck deck;
bool useBets = true;
bool showCardCount = false;
int deckCount = 1;
int bankRoll = 0;
int roundCount = 0;
int runningCount = 0;

void getPreferences();
void payoutResults(int player, int dealer, int wager);
int playHand(Deck hand, int* wagerReference);
bool runRound();



int main(int argc, char** argv) {
    srand(time(0));

    cout << "Blackjack simulator $0.99" << endl;
    if(promptBoolean("Would you like to edit preferences? (y)")) getPreferences();
    if(useBets) bankRoll = promptForInt("Enter starting balance: ", 0, 99999999);
    cout << endl;
    deck = Deck(deckCount);
    while(runRound()) {
        if(bankRoll <= 0 && useBets) {
            cout << "You lose, chump.  You're out of money. The loan shark is comin!" << endl;
            return 0;
        }
        cout << endl;
        if(deck.cards.size() < deckCount * 52 / 2) {
            runningCount = 0;
            deck = Deck(deckCount);
            cout << "***************\nDeck reshuffled\n***************\n\n";
        }
    }
    return 0;
}

void getPreferences() {
    useBets = promptBoolean("Would you like to disable bets (y)?");
    showCardCount = promptBoolean("Would you like to show card count (y)?");
    deckCount = promptForInt("Enter deck count: ", 1, 256);
    cout << "Preferences updated" << endl << endl;
}
int playHand(Deck& hand, int& wager) {
    char hit = 'h';
    while(hit == 'h' || hit == 'H') {
        cout << endl;
        if(showCardCount) cout << "[card count: " << runningCount << "] ";
    promptHit:
        if(hand.cards.size() == 2) cout << "Hit, stand, or double down? (h s d q) ";
        else cout << "Hit or stand? (h s q) ";
        cin >> hit;
        cin.ignore();
        switch(hit) {
        case 'q':
            exit(0);
        case 's': case 'S':
            return hand.total();
        case 'd': case 'D':
            if(hand.cards.size() == 2) wager *= 2;
        case 'h': case 'H':
            hand.append(deck.draw());
            hand.print("Current hand");
            cout << endl;
            break;
        default:
            cout << "Please enter a valid response" << endl;
            goto promptHit;
        }
        //Busting
        if(hand.total() >= 21) break;
    }
    return hand.total();
}
void payoutResults(int player, int dealer, int wager) {
    if(player == 21) {
        cout << "Player hit 21" << endl;
        bankRoll += wager * 1.5;
        return;
    }
    char winner = 'd';
    if(dealer > 21) cout << "Dealer busts; ";
    if(player > 21) cout << "Player busts; ";
    if(dealer > 21 || (player > dealer && player < 21)) {
        cout << "Player wins" << endl;
        bankRoll += wager;
        return;
    }
    if(player > 21 || dealer > player) {
        cout << "Dealer wins" << endl;
        bankRoll -= wager;
    }
    else if(dealer == player)
        cout << "Player pushes" << endl;
}
bool runRound() {
    cout << "------------- round " << roundCount << " -------------" << endl << endl;
    roundCount++;
    int firstWager = 0, secondWager = 0;
    if(useBets) firstWager = promptForInt("What is your wager? (0 to quit): ", 0, bankRoll);
    if(firstWager <= 0 && useBets) return false;
    char hit = 'h';
    Deck playerHand, dealerHand;
    playerHand.drawFromDeck(2);
    dealerHand.drawFromDeck(2);
    //Uncount dealer's hidden card
    runningCount -= dealerHand.cards[1].deltaCounter();
    //Print the cards
    cout << "Dealer is showing (" << dealerHand.cards[0].getValue() << "): " << dealerHand.cards[0].name() << endl;
    playerHand.print("Current hand");
    //Splitting opportunity
    Deck secondHand;
    bool split = false;
    if(playerHand.cards[0].getValue() == playerHand.cards[1].getValue()) {
        cout << endl << endl;
        split = promptBoolean("Would you like to split? (y)");
    }
    if(split) {
        secondWager = firstWager;
        secondHand.append(playerHand.cards[1]);
        playerHand.erase(1);
        cout << "---- Playing first hand ----" << endl;
        secondHand.drawFromDeck(1);
        secondHand.print("Current hand");
        playHand(secondHand, secondWager);

        cout << "---- Playing second hand ----" << endl;
        playerHand.drawFromDeck(1);
        playerHand.print("Current hand");
    }
    //Hit loop
    playHand(playerHand, firstWager);

    //Dealer loop
    while(dealerHand.total() < 17)
        dealerHand.drawFromDeck();
    if(dealerHand.total() == 17 && dealerHand.aceCount() != 0)
        dealerHand.drawFromDeck();
    //Print hands
    dealerHand.print("\n\nDealer's hand");
    cout << "Player total: " << playerHand.total() << endl;
    //Payout
    payoutResults(playerHand.total(), dealerHand.total(), firstWager);
    if(split) {
        cout << "---Second hand payouts---" << endl << "Second hand total: " << secondHand.total() << endl;
        payoutResults(secondHand.total(), dealerHand.total(), secondWager);
    }
    runningCount += dealerHand.cards[1].deltaCounter();
    if(useBets) cout << "Current bank roll is : " << bankRoll << endl;
    return true;
}
void Deck::drawFromDeck(int count) {
    for(int i = 0;i < count;i++) {
        Card c = deck.draw();
        runningCount += c.deltaCounter();
        append(c);
    }
}
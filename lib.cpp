#include "lib.h"

int promptForInt(string prompt, int min, int max) {
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
bool promptBoolean(string prompt) {
    cout << prompt << " ";
    char in;
    cin >> in;
    cin.ignore();
    if(in == 'y' || in == 'Y') return true;
    return false;
}
Deck::Deck(int count) {
    for(int i = 0;i < count;i++)
        for(Suit s = Spade;s <= Diamond;s = Suit(int(s) + 1))
            for(Rank r = Ace;r <= King;r = Rank(int(r) + 1))
                cards.push_back(Card(s, r));
}
Card::Card(string card) {
    suit = ErrorSuit;
    rank = ErrorRank;

    const map<char, Suit> suits{ {'S', Spade}, {'C', Club}, {'H', Heart}, {'D', Diamond} };
    const map<char, Rank> ranks{ {'A', Ace}, {'2', Two}, {'3', Three}, {'4', Four}, {'5', Five}, {'6', Six}, {'7', Seven}, {'8', Eight}, {'9', Nine}, {'T', Ten}, {'J', Jack}, {'Q', Queen}, {'K', King} };

    for(int i = 0; i < card.length(); i++) {
        char character = (char)toupper(card[i]);
        if(suits.count(character))
            suit = suits.at(character);
        if(ranks.count(character))
            rank = ranks.at(character);
    }
}
Card::Card(Suit s, Rank r) {
    suit = s;
    rank = r;
}
int Card::getValue() {
    if(rank == Ace) return 11;
    if(rank > Ten) return 10;
    return int(rank);
}
string Card::name() {
    const string suits[] = { "E","S","D","H","C" };
    const string ranks[] = { "E","A","2","3","4","5","6","7","8","9","T","J","Q","K" };
    if(suit == -1 || rank == -1) return "Error";
    return ranks[rank] + suits[suit];
}
int Card::deltaCounter() {
    if(rank >= Two && rank <= Six) return 1;
    else if(rank >= Ten || rank == Ace) return -1;
    else return 0;
}
Card Deck::draw() {
    int index = rand() % cards.size();
    Card ret = cards[index];
    erase(index);
    return ret;
}
void Deck::erase(int index) {
    cards.erase(cards.begin() + index);
}
void Deck::append(Card c) {
    cards.push_back(c);
}
int Deck::total() {
    int sum = 0;
    for(int i = 0;i < cards.size();i++)
        sum += cards[i].getValue();
    //reduce ace value if over 21
    if(sum > 21) {
        int aces = aceCount();
        while(aces != 0 && sum > 21) {
            sum -= 10;
            aces -= 1;
        }
    }
    return sum;
}
int Deck::aceCount() {
    int count = 0;
    for(int i = 0;i < cards.size();i++)
        if(cards[i].rank == Ace) count++;
    return count;
}
void Deck::print(string title) {
    cout << title << " (" << total() << "): ";
    for(int i = 0;i < cards.size();i++) {
        cout << cards[i].name() << " ";
    }
    cout << endl;
}
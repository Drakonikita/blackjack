#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>

enum TSuit :int {
    SPADES,
    DIAMONDS,
    HEARTS,
    CLUBS
};

enum TCardRank :int {
    ACE,
    KING = 1,
    QUEEN = 2,
    JACK = 3,
    TEN = 4,
    NINE = 5,
    EIGHT = 6,
    SEVEN = 7,
    SIX = 8,
    FIVE = 9,
    FOUR = 10,
    THREE = 11,
    TWO = 12
};

class Card {
    TSuit _suit;
    TCardRank _rank;
    bool _faceDown;
public:

    void setSuit(TSuit _suit) {
        this->_suit = _suit;
        _faceDown = true;
    }

    void setRank(TCardRank _rank) {
        this->_rank = _rank;
    }

    void flipCard() {
        _faceDown = !_faceDown;
    }

    std::string getSuit() {
        switch (_suit) {
        case SPADES:
            return "DIAMONDS";
        case DIAMONDS:
            return "HEARTS";
        case HEARTS:
            return "CLUBS";
        case CLUBS:
            return "SPADES";
        default:
            return "EMPTY";
        }
        return "EMPTY";
    }

    int getValue() {
        if (!_faceDown) {
            switch (_rank) {
            case ACE:
                return 1;
            case KING:
                return 10;
            case QUEEN:
                return 10;
            case JACK:
                return 10;
            case TEN:
                return 10;
            case NINE:
                return 9;
            case EIGHT:
                return 8;
            case SEVEN:
                return 7;
            case SIX:
                return 6;
            case FIVE:
                return 5;
            case FOUR:
                return 4;
            case THREE:
                return 3;
            case TWO:
                return 2;
            default:
                return 0;
            }
        }
        return 0;
    }

    std::string getRank() {
        switch (_rank) {
        case ACE:
            return "ACE";
        case KING:
            return "KING";
        case QUEEN:
            return "QUEEN";
        case JACK:
            return "JACK";
        case TEN:
            return "TEN";
        case NINE:
            return "NINE";
        case EIGHT:
            return "EIGHT";
        case SEVEN:
            return "SEVEN";
        case SIX:
            return "SIX";
        case FIVE:
            return "FIVE";
        case FOUR:
            return "FOUR";
        case THREE:
            return "THREE";
        case TWO:
            return "TWO";
        default:
            return "rank err";
        }
    }

    friend std::ostream& operator<< (std::ostream& s, Card& _card);

};
std::ostream& operator<< (std::ostream& s, Card& _card) {
    if (_card._faceDown) {
        s << "XX";
    }
    else {
        s << " " << _card.getSuit() << " " << _card.getRank() << " ";
    }

    return s;
}

class Hand {
protected:
    std::vector<Card*> cards;
public:
    Hand() {}
    ~Hand() {
        if (!(cards.empty())){
            for (int i = 0; i < int(cards.size()); i++) {
                delete cards[i];
            }
            cards.clear();
            //std::cout << "hand deleted";// for checking if destructor workin from derived classes
        }
    }

    void addCard(Card* newcard)
    {
        cards.push_back(newcard);
    }
    void  clearHand()
    {
        for (int i = 0; i < int(cards.size()); i++) {
            delete cards[i];
        }
        cards.clear();
    }
    int getScore()
    {
        int score = 0;
        int aces = 0;
        int aceWasEleven = 0;
        for (long long unsigned int i = 0; i < cards.size(); i++)
        {
            int currentCardScore = cards[i]->getValue();
            if (currentCardScore == 1)
            {
                aces++;
            }

            if (score <= 11 && aces > 0 && aceWasEleven == 0)
            {
                score += 10;
                aceWasEleven++;
            }

            score += currentCardScore;
            if (score > 21 && aceWasEleven > 0)
            {
                score -= 10;
                aceWasEleven--;
            }
        }
        return score;
    }
};



class GenericPlayer : public Hand {
protected:
    std::string playerName;
public:
    GenericPlayer(){
    }
    ~GenericPlayer() {}

    void addCard(Card* newcard)
    {
        newcard->flipCard();
        cards.push_back(newcard);
    }
    virtual bool isHitting() = 0;

    bool isBoosted() {
        if (getScore() > 21) {
            return true;
        }
        else {
            return false;
        }
    }
    void Bust() {
        if (isBoosted())
        {
            std::cout << playerName << " got " << getScore() << " points and lost it." << std::endl;
        }
    }

    friend std::ostream& operator<< (std::ostream& s, GenericPlayer& gp);

};
std::ostream& operator<< (std::ostream& s, GenericPlayer& gp) {
    s << gp.playerName << " has: ";
    for (long long unsigned int i = 0; i < gp.cards.size(); i++) {
        s << *gp.cards[i];
    }
    s << " cards and " << gp.getScore() << " points";

    return s;
}

class Player : public GenericPlayer {
public:
    Player(){
        std::cout << "Enter player name:" << std::endl;
        std::cin >> playerName;
    }
    ~Player() {}

    virtual bool isHitting() {
        char yesNo = 'x';
        if (getScore() < 21) {

            do {
                std::cout << "One more card?(y/n)" << std::endl; // << "1. Hit me!" << std::endl << "0. Enough." << std::endl;
                std::cin >> yesNo;
                if (!std::cin) {
                    std::cerr << "wrong entity" << std::endl;
                }
            } while (yesNo != 'y' && yesNo != 'n');
            if (yesNo == 'n') {
                return false;
            }
            else if (yesNo == 'y') {
                return true;
            }

        }
        return false;
    }
    void win() const {
        std::cout << playerName << " win!" << std::endl;
    }
    void lose() const {
        std::cout << playerName << " lose!" << std::endl;
    }
    void push() const {
        std::cout << playerName << "'s game is draw!" << std::endl;
    }
    void setName(std::string name) {
        playerName = name;
    }
};

class House : public GenericPlayer {
public:
    House(){
        playerName = "House of greed";
    }
    ~House() {}

    virtual bool isHitting()
    {
        if (getScore() < 17) {
            return true;
        }
        else {
            return false;
        }
    }
    void flipFirstCard() {
        cards[0]->flipCard();
    }
};

class Deck : public Hand {
private:
    void populate() {
        cards.resize(52);
        for (int i = 0; i < 52; i++) {
            cards[i] = new Card();
        }
        for (int k = 0; k < 52;)
        {
            TSuit suitt = SPADES;

            for (int r = 0; r < 4; r++)
            {
                TCardRank rankk = ACE;
                for (int s = 0; s < 13; s++)
                {
                    cards[k]->setRank(rankk);
                    cards[k]->setSuit(suitt);
                    k++;
                    rankk = TCardRank(int(rankk) + 1);
                }
                suitt = TSuit(int(suitt) + 1);
            }
        }
    }
public:
    Deck() {
        this->populate();
    }
    ~Deck() {
    }

    void shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    void addCard(GenericPlayer& player) {
        if (!cards.empty()) {
            player.addCard(cards.back());
            cards.pop_back();
        }
        else {
            std::cout << "Deck is empty" << std::endl;
        }
    }

    void deal(GenericPlayer& player) {
        while (!(player.isBoosted()) && player.isHitting()) {
            addCard(player);
            std::cout << player << std::endl;
        }
    }
};

class Game {
    Deck deck;
    House house;
    Player player; // only one player in my game.
public:
    Game() {
        deck.shuffle();
    }
    ~Game() {}

    void play() {
        deck.addCard(house);
        house.flipFirstCard();
        deck.addCard(house);
        deck.addCard(player);
        deck.addCard(player);
        std::cout << house << '\n' << player << std::endl;
        deck.deal(player);
        std::cout << house << '\n' << player << std::endl;
        house.flipFirstCard();
        std::cout << house << std::endl;
        deck.deal(house);
        int playerScore = player.getScore();
        int houseScore = house.getScore();
        if (playerScore == 21) {
            if (houseScore == 21) {
                player.push();
            }
            else {
                player.win();
            }
        }
        else if (playerScore > 21) {
            if (houseScore <= 21) {
                player.lose();
            }
            else if (houseScore > 21) {
                player.push();
            }
        }
        else if (playerScore < 21) {
            if (houseScore > 21) {
                player.win();
            }
            else if (houseScore > playerScore) {
                player.lose();
            }
            else if (playerScore > houseScore) {
                player.win();
            }
            else if (playerScore == houseScore) {
                player.push();
            }
        }
    }
};


int main()
{

    char play = 'x';
    do {
        std::cout << "Hello, stranger! do you want to play Black Jack?(y/n)" << std::endl;
        std::cin >> play;
        if (play == 'y') {
            Game game1;
            game1.play();
        }
    } while (play != 'n');


    return 0;
}


#include <iostream>
#include <sstream>
#include "Player.h"

using std::vector, std::string, std::size_t;

Player::Player(bool isAI) : isAI(isAI), hand(std::vector<Card*>()) {
}

void Player::addToHand(Card* c) {
  hand.push_back(c);
}

size_t Player::getHandSize() {
  return hand.size();
}

std::string Player::getHandString() {
  string hand1 = "";
  for (Card* set : hand) {
    hand1 += set->getRank() + " " + set->getSuit() + ", ";
  }
  if (!hand.empty()) {
    hand1.pop_back(); 
    hand1.pop_back(); 
  }
  return hand1;
}

Card* Player::playCard(vector<string> const& suits, string& currentRank, string& currentSuit) {
  if (isAI) {
    for (size_t i = 0; i < hand.size(); ++i) {
      if (hand.at(i)->canBePlayed(currentRank,currentSuit)) {
        hand.at(i)->play();
        currentSuit = hand.at(i)->getSuit();
        currentRank = hand.at(i)->getRank();
        Card* playedCard = hand.at(i);
        hand.erase(hand.begin() + i);
        return playedCard;
      }
    }
    return nullptr;
  } else {
    std::cout << "Your hand contains: " << getHandString() << std::endl;
    std::cout << "The next card played must be a " << currentRank << " or " << currentSuit << std::endl;
    std::cout << "What would you like to play? (enter \"draw card\" to draw a card)" << std::endl;
    string rank_check = "";
    string suit_check = "";
    while (true) {
      std::cin >> rank_check >> suit_check;
      if (rank_check == "draw" && suit_check == "card") {
        return nullptr;
      }
      size_t tempIndex = 0;
      bool real = false;
      for (size_t i = 0; i < hand.size(); i++) {
        if (hand.at(i)->getSuit() == suit_check && hand.at(i)->getRank() == rank_check) {
          real = true;
          tempIndex = i;
          break;
        }
      }
      if (!real) {
        std::cout << "That's not a card you have. Try again." << std::endl;
        continue;
      }

      if (hand.at(tempIndex)->getRank() == currentRank || hand.at(tempIndex)->getSuit() == currentSuit || hand.at(tempIndex)->getRank() == "8") {
      } else {
        std::cout << "You can't play that card. Try again." << std::endl;
        continue;
      }

      hand.at(tempIndex)->play();
      currentRank = hand.at(tempIndex)->getRank();
      Card* playedCard = hand.at(tempIndex);

      if (currentRank == "8") {
        string newSuit = "";
        bool freal = false;
        std::cout << "What suit would you like to declare?" << std::endl;
        while (!freal) {
          std::cin >> newSuit;
          for (string temp : suits) {
            if (newSuit == temp) {
              freal = true;
              currentSuit = newSuit;
              break;
            }
          }
          if (!freal) {
            std::cout << "That's not a suit in this deck. Try again." << std::endl;
          }
        }
      } 
      else {
        currentSuit = hand.at(tempIndex)->getSuit();
      }
      hand.erase(hand.begin() + tempIndex);
      return playedCard;
    }
  }
}

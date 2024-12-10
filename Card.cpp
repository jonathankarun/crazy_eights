#include <iostream>
#include "Card.h"
using std::string;

Card::Card(string rank, string suit) : rank(rank), suit(suit), timesPlayed(0) { 
  if (suit.empty()) {
    throw std::invalid_argument("Empty2");
  }
  if (rank.empty()) {
    throw std::invalid_argument("Empty1"); 
  }
  for (char c : suit) {
    if (!std::isalnum(c)) {
      throw std::invalid_argument("Invalid Suit");
    }
  }
  for (char c : rank) {
    if (!std::isalnum(c)) {
      throw std::invalid_argument("Invalid Rank");
    }
  }
}

string Card::getRank() {
  return rank;
}

string Card::getSuit() {
  return suit;
}

int Card::getTimesPlayed() {
  return timesPlayed;
}

bool Card::canBePlayed(string currentRank, string currentSuit) {
  if (rank == "8") {
    return true;
  } else if (suit == currentSuit || rank == currentRank) {
    return true;
  } 
  return false;
}

void Card::play() {
  timesPlayed++;
}
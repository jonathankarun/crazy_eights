#include "Game.h"
#include <iostream>
#include <fstream> 
#include <sstream> 

using std::string, std::vector;

Game::Game() : players({}), suits({}), ranks({}), deck({}), drawPile({}), discardPile({}) {}

void Game::loadDeckFromFile(string filename) {
  std::ifstream file;
  std::stringstream stream;
  file.open(filename);
  if(!file.is_open())
  {
    throw(std::runtime_error("Bad file"));
  }
  string line;
  getline(file,line);
  stream << line;
  string suit;
  while(stream>>suit)
  {
    suits.push_back(suit);
    suit = "";
  }
  stream.str("");
  std::stringstream str2;
  getline(file,line);
  str2 << line;
  string rank;
  while(str2>>rank)
  {
    ranks.push_back(rank);
    rank = "";
  }
  stream.str("");
  string card;
  while(getline(file, line))
  {
    std::stringstream str3;
    bool flip = false;
    bool flip2 = false;
    str3<<line;
    if(str3 >> rank >> suit)
    {
      string extra;
      if(str3>>extra)
      {
        throw std::runtime_error(extra);
      }
      for(size_t i = 0; i < ranks.size(); i++)
      {
        if(rank == ranks[i])
        {
          flip = true;
          break;
        }
      }
      for(size_t i = 0; i < suits.size(); i++)
      {
        if(suit == suits[i])
        {
          flip2 = true;
          break;
        }
      }
      if(flip2 && flip)
      {
        try
        {
          deck.push_back(new Card(rank, suit));
        }
        catch(const std::exception& e)
        {
          throw std::runtime_error("runtime error");
        }
      }
      else if(!flip2||!flip)
      {
        throw std::runtime_error(suits[0]);
      }
    }
    else 
    {
      throw std:: runtime_error(stream.str());
    }
  }
  for(int j = deck.size() - 1; j >= 0; j--)
  {
    drawPile.push_back(deck[j]);
  }
}

void Game::addPlayer(bool isAI) {
  players.push_back(new Player(isAI));
}

void Game::drawCard(Player* p) {
  if (drawPile.empty()) {
    if (discardPile.empty()) {
      throw std::runtime_error("Both empty");
    }
    for(int i = discardPile.size() - 1; i >= 0; i--) {
      drawPile.push_back(discardPile[i]);
      discardPile.pop_back();
    }
  }
  p->addToHand(drawPile.back());
  drawPile.pop_back();
}

Card* Game::deal(int numCards) {
  Card* initiallyDiscardedCard = drawPile.back();
  discardPile.push_back(initiallyDiscardedCard);
  drawPile.pop_back();
  for(int a = 0;a < numCards; a++)
  {
    for(size_t b = 0; b < players.size(); b++)
    {
      drawCard(players.at(b));
    }
  }
  return initiallyDiscardedCard;
}


string Game::mostPlayedSuit() {
  int maxCount = 0;
  string mostPlayedSuit;
  for (string& suit : suits) {
    int count = 0;
    for (Card* temp : deck) {
      if (temp->getSuit() == suit) {
        count += temp->getTimesPlayed();
      }
    }
    if (count > maxCount) {
      maxCount = count;
      mostPlayedSuit = suit;
    }
  }
  return mostPlayedSuit;
}

int Game::runGame() {
  Card* initialCard = discardPile.back();
  string currentRank = initialCard->getRank();
  string currentSuit = initialCard->getSuit();

  size_t currentPlayerIndex = 0;
  int maxTurns = 1000;
  int turnCounter = 0;

  while (true) {
    if (turnCounter++ > maxTurns) {
      std::cout << "Game ended due to turn limit." << std::endl;
      throw std::runtime_error("Infinite loop prevention: turn limit reached");
    }

    Player* currentPlayer = players[currentPlayerIndex];
    std::cout << "Player " << currentPlayerIndex << "'s turn!" << std::endl;
    Card* playedCard = currentPlayer->playCard(suits, currentRank, currentSuit);

    if (playedCard != nullptr) {
      if (currentRank == "8") {
        std::cout << "Player " << currentPlayerIndex << " plays 8 " 
                  << playedCard->getSuit() << " and changes the suit to " 
                  << currentSuit << "." << std::endl;
      } else {
        std::cout << "Player " << currentPlayerIndex << " plays " 
                  << currentRank << " " << currentSuit << "." << std::endl;
      }
      discardPile.push_back(playedCard);

      if (currentPlayer->getHandSize() == 0) {
        return currentPlayerIndex;
      }
    } else {
      drawCard(currentPlayer);
      std::cout << "Player " << currentPlayerIndex << " draws a card." << std::endl;

      if (drawPile.empty()) {
        std::cout << "Draw pile empty, flipping the discard pile." << std::endl;
        if (!discardPile.empty()) {
          drawPile = discardPile;
          discardPile.clear();
          Card* topCard = drawPile.back();
          currentRank = topCard->getRank();
          currentSuit = topCard->getSuit();
        } else {
          std::cout << "Both draw and discard piles empty, ending game." << std::endl;
          throw std::runtime_error("Empty draw and discard piles");
        }
      }
    }

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
  }
}


Game::~Game() {
  for (unsigned int i = 0; i < deck.size(); i++) {
    delete deck.at(i);
  }
  for (unsigned int i = 0; i < players.size(); i++) {
    delete players.at(i);
  }
}
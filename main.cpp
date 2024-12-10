#include <iostream>
#include <string>

#include "Card.h"
#include "Game.h"
#include "Player.h"

using std::string;

bool loadDeck(Game& g) {
  string filename;
  std::cout << "Choose a file to load the deck from:" << std::endl;
  std::cin >> filename;
  try {
    g.loadDeckFromFile(filename);
  } catch (std::invalid_argument const&) {
    std::cout << "The file was invalid. Aborting." << std::endl;
    return false;
  }
  return true;
}

int getPlayerCount() {
  std::cout << "Enter number of players:" << std::endl;
  int numPlayers;
  while (true) {
    if ((!(std::cin >> numPlayers)) || numPlayers <= 0) {
      if (std::cin.fail()) {
        // Clear the fail flag
        std::cin.clear();
        // Remove the non-integer data from the stream
        string garbage;
        std::cin >> garbage;
      }
      std::cout << "Please enter a positive number" << std::endl;
    } else {
      // break out of the loop once we've read a valid number
      break;
    }
  }
  return numPlayers;
}

void setupPlayers(Game& g, int numPlayers) {
  for (int i = 0; i < numPlayers; i++) {
    std::cout << "Is player " << i << " an AI? (y/n)" << std::endl;
    string input;
    std::getline(std::cin, input);
    while (input != "y" && input != "n") {
      std::getline(std::cin, input);
    }
    bool isAI = (input == "y");
    g.addPlayer(isAI);
  }
}

void setupGame(Game& g) {
  std::cout << "How many cards should each player start with?" << std::endl;
  long unsigned int numCards;
  while (!(std::cin>>numCards || numCards < 1 )) {
    std::cout << "Please enter a positive number" << std::endl;
    std::cin >> numCards;
  }
  Card* initialDiscard = g.deal(numCards);
  std::cout << "The initial discard is " << initialDiscard->getRank() << " " << initialDiscard->getSuit() << std::endl;
}


int main() {
  Game g;
  if (!loadDeck(g)) {
    return 1;
  }
  int numPlayers = getPlayerCount();
  setupPlayers(g, numPlayers);
  setupGame(g);
  int winner = g.runGame();
  if (winner != -1) {
    std::cout << "Player " << winner << " wins!" << std::endl;
  } else {
    std::cout << "The game is a draw!" << std::endl;
  }
  std::cout << "The most played suit was " << g.mostPlayedSuit() << std::endl;
  return 0;
}
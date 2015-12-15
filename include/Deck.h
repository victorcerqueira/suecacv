#ifndef DECK_H
#define DECK_H

#include <vector>
#include "DeckCard.h"
#include "imgScanner.h"

using namespace std;

class Deck {
	private:
		vector<DeckCard> cards;
	public:
		Deck();
		int getSize();
		void addNextCard();
		DeckCard getCard(int i);
};

#endif
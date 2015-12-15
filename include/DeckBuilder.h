#ifndef DECKBUILDER_H
#define DECKBUILDER_H

#include "Utils.h"
#include <vector>
#include "Deck.h"

using namespace std;

class DeckBuilder {
	private:
		static void storeCard(DeckCard dc, string path);
	public:
		static Deck buildDeck(string name);
		static vector<string> getAvailableDecks();
};

#endif
#include "DeckBuilder.h"
#include <lib/dirent.h>
#include <windows.h>

void DeckBuilder::storeCard(DeckCard dc, string path){
	path+='/';
	path+=dc.getShortName();
	path+=".jpg";
	imwrite(path.c_str(), dc.getImage() );
}

Deck DeckBuilder::buildDeck(string name){
	string path = "../decks/";
	path.append(name);

	Deck deck = Deck();

	DWORD ftyp = GetFileAttributesA(path.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES){
		CreateDirectory(s2ws(path).c_str(),NULL);
	}
	else throw DECK_ALREADY_EXISTS;

	for(int i=0; i<deck.getSize(); i++){
		deck.addNextCard();
		storeCard(deck.getCard(i),path);
	}
	return deck;
}

vector<string> DeckBuilder::getAvailableDecks(){
	vector<string> decks = vector<string>();

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("../decks")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			string name = string(ent->d_name);
			if(name.compare(".") != 0 && name.compare("..") != 0)
				decks.push_back(name);
		}
		closedir (dir);
	}
	
	return decks;
}
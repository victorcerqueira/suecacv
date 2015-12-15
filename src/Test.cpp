#include <stdio.h>
#include <stdlib.h>
#include "imgScanner.h"
#include "DeckBuilder.h"
#include <iostream>
#include <sstream>

using namespace std;

string registerDeck();

int main(int argc, char** argv)
{
	string deck = ""; //TODO transform this to Deck

	vector<string> decks = DeckBuilder::getAvailableDecks();
	if(decks.empty()){
		cout << "No available decks, starting Deck Creation Manager" << endl;
		deck = registerDeck();
	}
	else{
		cout << "Current available decks:" << endl;
		for(int i=0; i<decks.size(); i++){
			cout << "#" << i+1 << " - " << decks.at(i) << endl;
		}
		cout << "#0 - Register new deck";
		string input = "";
		int index;
		while(true){
			cout << "Please select the deck you want to play with... " << endl;

			getline(cin, input);
			stringstream myStream(input);
			if (myStream >> index){
				if(index >= 0 && index <= decks.size())
					break;
			}
		}
		if(index == 0){
			deck = registerDeck();
		}
		else
			deck = decks.at(index-1) ;
	}

	cout << "Playing with deck " + deck << endl << endl <<"--------------------------------------------------" << endl << endl;


    cv::VideoCapture cap;
    if(!cap.open(0))
        return 0;

	cv::Mat frame;
	cap >> frame;
	ImgScanner scanner = ImgScanner(&frame);
    for(;;)
    {
          cv::Mat grey,cont;
          cap >> frame;
          if( frame.empty() ) break; // end of video stream

		  scanner.detectCards();
		  scanner.outlineCards();
		  
          cv::imshow("Color", frame);

          if( cv::waitKey(1) == 27 ) break; // stop capturing by pressing ESC

    }
    // the camera will be closed automatically upon exit
    // cap.close();
    return 0;
}

string registerDeck(){
	string name;
	while(true){
		cout << "Please insert the new deck name..." << endl;
		cin >> name;
		try{
			DeckBuilder::buildDeck(name);
			break;
		}
		catch(int error){
			if(error == DECK_ALREADY_EXISTS)
				cout << "Name already exists. ";
		}
	}
	cout << "Deck successfully built!" << endl;
	return name;
}
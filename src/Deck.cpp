#include "Deck.h"

Deck::Deck(){
	cards = vector<DeckCard>();
	string suits = "shdc";
	for(int j=0; j<suits.length(); j++){
		for(int i=1; i<=13; i++){
			cards.push_back(DeckCard(i,suits.at(j)));
		}
	}
}

int Deck::getSize(){
	return cards.size();
}

void Deck::addNextCard(){
	for(int i=0; i<cards.size(); i++){
		if(!cards.at(i).hasImageLoaded()){
			cv::VideoCapture cap;
			if(!cap.open(0))
				throw INVALID_CAM;

			cv::Mat frame;
			cap >> frame;
			for(;;)
			{
				  cap >> frame;
				  if( frame.empty() ) break; // end of video stream
		  
				  Card c = ImgScanner::detectSingleCard(&frame);

				  cv::imshow(cards.at(i).getName(), frame);

				  string message = "Please show the ";
				  message.append(cards.at(i).getName());
				  message.append(" and wait until it is highlighted, then press 'Space'");

				  cv::displayOverlay(cards.at(i).getName(),message);
				  char key = cv::waitKey(1);
				  if( key == ' ' && !c.isImgSet() ){
					  cv::destroyWindow(cards.at(i).getName());
					  cards.at(i).loadImage(c.getImg());
					  return;
				  }
			}
		}
	}
}

DeckCard Deck::getCard(int i){
	return cards.at(i);
}
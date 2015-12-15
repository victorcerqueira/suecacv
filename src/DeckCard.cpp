#include "DeckCard.h"

DeckCard::DeckCard(int num, char suit){
	this->num = num;
	this->suit = suit;
	imageSet = false;
}

void DeckCard::loadImage(cv::Mat img){
	image = img;
	imageSet = true;
}

cv::Mat DeckCard::getImage(){
	if(!hasImageLoaded()) throw IMAGE_NOT_LOADED;
	return image;
}

bool DeckCard::hasImageLoaded(){
	return imageSet;
}

string DeckCard::getName(){
	string res = "";
	switch(num){
	case 1:
		res = "Ace";
		break;
	case 11:
		res = "Queen";
		break;
	case 12:
		res = "Jack";
		break;
	case 13:
		res = "King";
		break;
	default:
		res = to_string(num);
		break;
	}

	switch(suit){
	case 's':
		res.append(" of spades");
		break;
	case 'h':
		res.append(" of hearts");
		break;
	case 'd':
		res.append(" of diamonds");
		break;
	case 'c':
		res.append(" of clubs");
		break;
	}

	return res;
}

string DeckCard::getShortName(){
	string res = to_string(num);
	res+=suit;
	return res;
}
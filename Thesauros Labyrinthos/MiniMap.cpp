#include "MiniMap.h"

MiniMap::MiniMap(vector<std::shared_ptr<Cell>> cellsVector, vector<std::shared_ptr<Skeleton>> Skeletons, vector<std::shared_ptr<Medusa>> Medusa, vector<std::shared_ptr<Ghost>> Ghosts, vector<std::shared_ptr<Griffin>> Griffin)
{
	/* //orignial, was hard to see detail
	minimapView.setSize(1600.f, 900.f); //set the mini map to be the size of the entire gamearea so that when we center it in the middle of the screen, it'll show everything.
	minimapView.setCenter(800.f, 450.f); //center view in the middle of the screen
	minimapView.zoom(1.25f);//zoom in slightly
	*/
	//minimapView.reset(sf::FloatRect(0.f,0.f,1600.f,900.f));

	minimapView.setSize(400.f, 300.f); //smaller than above so more detail, but bigger than playerview so it shows surroundings 
	minimapView.setCenter(0.f, 0.f);
	minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f)); //should put the minimap in the top right corner of the game screen

																	  //for all the walls and fake walls create a white sf rectangle and push it to the vector
	for (int i = 0; i < cellsVector.size(); i++) {
		if (cellsVector[i]->cellType == "Fake wall cell" || cellsVector[i]->cellType == "wall cell") {
			sf::RectangleShape tempRect;
			tempRect.setPosition(cellsVector[i]->rectangle.getPosition());
			tempRect.setSize(cellsVector[i]->rectangle.getSize());
			tempRect.setFillColor(sf::Color::White);
			miniMapWallsVector.push_back(tempRect);
		}
	}

	//same for skeletons
	for (int i = 0; i < Skeletons.size(); i++) {

		sf::RectangleShape tempRect;
		tempRect.setPosition(sf::Vector2f(Skeletons[i]->dynamicBody->GetPosition().x / 30.f, Skeletons[i]->dynamicBody->GetPosition().y / 30.f)); //position this time is gotten from the bodies
		tempRect.setSize(sf::Vector2f(10.f, 10.f));
		tempRect.setOrigin(sf::Vector2f(5.f, 5.f));
		tempRect.setFillColor(sf::Color::Red);
		miniMapSkeletonvector.push_back(tempRect);
	}
	//and medusa
	for (int i = 0; i < Medusa.size(); i++) {

		sf::RectangleShape tempRect;
		tempRect.setPosition(sf::Vector2f(Medusa[i]->dynamicBody->GetPosition().x / 30.f, Medusa[i]->dynamicBody->GetPosition().y / 30.f)); //position this time is gotten from the bodies
		tempRect.setSize(Medusa[i]->rectangle.getSize());
		tempRect.setOrigin(Medusa[i]->rectangle.getSize().x / 2.f, Medusa[i]->rectangle.getSize().y / 2.f);
		tempRect.setFillColor(sf::Color::Red);
		miniMapMedusavector.push_back(tempRect);
	}
	//ghosts
	for (int i = 0; i < Ghosts.size(); i++) {

		sf::RectangleShape tempRect;
		tempRect.setPosition(sf::Vector2f(Ghosts[i]->dynamicBody->GetPosition().x / 30.f, Ghosts[i]->dynamicBody->GetPosition().y / 30.f)); //position this time is gotten from the bodies
		tempRect.setSize(Ghosts[i]->rectangle.getSize());
		tempRect.setOrigin(Ghosts[i]->rectangle.getSize().x / 2.f, Ghosts[i]->rectangle.getSize().y / 2.f);
		tempRect.setFillColor(sf::Color::Red);
		miniMapGhostvector.push_back(tempRect);
	}
	//griffins
	for (int i = 0; i < Griffin.size(); i++) {

		sf::RectangleShape tempRect;
		tempRect.setPosition(sf::Vector2f(Griffin[i]->dynamicBody->GetPosition().x / 30.f, Griffin[i]->dynamicBody->GetPosition().y / 30.f)); //position this time is gotten from the bodies
		tempRect.setSize(Griffin[i]->rectangle.getSize());
		tempRect.setOrigin(Griffin[i]->rectangle.getSize().x / 2.f, Griffin[i]->rectangle.getSize().y / 2.f);
		tempRect.setFillColor(sf::Color::Red);
		miniMapGriffinvector.push_back(tempRect);
	}

	//setup player's representation
	playerMiniMap.setSize(sf::Vector2f(10.f, 10.f));
	playerMiniMap.setOrigin(5.f, 5.f);
	playerMiniMap.setFillColor(sf::Color::Green); //green commonly associated with friendly

												  //setup the border
	miniMapBorder.setSize(sf::Vector2f(390.f, 290.f));
	miniMapBorder.setPosition(0, 0);
	miniMapBorder.setOrigin(195.f, 145.f);
	miniMapBorder.setFillColor(sf::Color::Black);
	miniMapBorder.setOutlineThickness(10.f);
	miniMapBorder.setOutlineColor(sf::Color::White);
}

void MiniMap::MiniMapUpdate(sf::RenderWindow * passedWindow, vector<std::shared_ptr<Cell>> cellsVector, vector<std::shared_ptr<Skeleton>> Skeletons, vector<std::shared_ptr<Medusa>> Medusa, vector<std::shared_ptr<Ghost>> Ghosts, vector<std::shared_ptr<Griffin>> Griffin, PlayerCharacter * playerCharacter)
{
	minimapView.setCenter(playerCharacter->rectangle.getPosition()); //set center on the player charcter so it follows them, providing info around them

	miniMapBorder.setPosition(playerCharacter->xPosition, playerCharacter->yPosition); //to get the border to follow, move with player
	passedWindow->draw(miniMapBorder); //draw the border

									   // just redraw whatever i want to display here (walls, enemies, me)
									   //draw the walls
	for (int i = 0; i < miniMapWallsVector.size(); i++) {
		passedWindow->draw(miniMapWallsVector[i]);
	}

	//update the enemies positions on the minimap
	for (int i = 0; i < Skeletons.size(); i++) {
		miniMapSkeletonvector[i].setPosition(sf::Vector2f(Skeletons[i]->dynamicBody->GetPosition().x * 30.f, Skeletons[i]->dynamicBody->GetPosition().y * 30.f));
		passedWindow->draw(miniMapSkeletonvector[i]); //draw the current rectangle 
	}

	for (int i = 0; i < Medusa.size(); i++) {
		miniMapMedusavector[i].setPosition(sf::Vector2f(Medusa[i]->dynamicBody->GetPosition().x *30.f, Medusa[i]->dynamicBody->GetPosition().y *30.f));
		passedWindow->draw(miniMapMedusavector[i]); //draw the current rectangle 
	}

	for (int i = 0; i < Ghosts.size(); i++) {
		miniMapGhostvector[i].setPosition(sf::Vector2f(Ghosts[i]->dynamicBody->GetPosition().x *30.f, Ghosts[i]->dynamicBody->GetPosition().y *30.f));
		passedWindow->draw(miniMapGhostvector[i]); //draw the current rectangle 
	}

	for (int i = 0; i < Griffin.size(); i++) {
		miniMapGriffinvector[i].setPosition(sf::Vector2f(Griffin[i]->dynamicBody->GetPosition().x *30.f, Griffin[i]->dynamicBody->GetPosition().y *30.f));
		passedWindow->draw(miniMapGriffinvector[i]); //draw the current rectangle 
	}

	//set to follow the player and then draw the rectangle
	playerMiniMap.setPosition(playerCharacter->rectangle.getPosition());
	passedWindow->draw(playerMiniMap);

}

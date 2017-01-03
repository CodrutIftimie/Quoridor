#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
#define JUMP 77
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

void changePos(Sprite& player, Vector2i playerPosition, Vector2i &playerPosMatrix,Vector2i player2pos,bool boardMatrix[17][17], int direction);
bool isSpriteClicked(Sprite Wall, RenderWindow &Window);
bool isBoardClicked(RenderWindow &Window, Event event);
bool isInsideWall(Sprite Wall, RenderWindow &Window);
bool isAbleToMove(bool boardMatrix[17][17], Vector2i playerPos, int direction);
void wallPlacementPrediction(Vector2i mousePos, Vector2i &prediction);
bool isAllowedToPlaceWall(bool boardMatrix[17][17], Vector2i prediction, bool wallFacing);

int main()
{
	RenderWindow window(VideoMode(950, 730), "Quoridor Game", Style::Close | Style::Titlebar);
	window.setFramerateLimit(45);
	Texture MainBoard;
	Texture PlayerTexture[2];
	Texture WallOrangeTextureH;
	Texture WallOrangeTextureV;
	Texture WallBlueTextureH;
	Texture WallBlueTextureV;
	Texture WallPlacementTextureH;
	Texture WallPlacementTextureV;


	unsigned int index;
	unsigned short wallIndex[2] = { 0 };

	Vector2i playerNewPosition;
	Vector2i playerPosMatrix[2];
	Vector2i WallPrediction;
	playerPosMatrix[0].x = 8;
	playerPosMatrix[0].y = 16;
	playerPosMatrix[1].x = 8;
	playerPosMatrix[1].y = 0;

	MainBoard.loadFromFile("Resources/MainBoard.png");
	PlayerTexture[0].loadFromFile("Resources/Player-Blue.png");
	PlayerTexture[1].loadFromFile("Resources/Player-Orange.png");
	WallOrangeTextureH.loadFromFile("Resources/Wall-Orange-Horizontal.png");
	WallOrangeTextureV.loadFromFile("Resources/Wall-Orange-Vertical.png");
	WallBlueTextureH.loadFromFile("Resources/Wall-Blue-Horizontal.png");
	WallBlueTextureV.loadFromFile("Resources/Wall-Blue-Vertical.png");
	WallPlacementTextureH.loadFromFile("Resources/Wall-Position-Horizontal.png");
	WallPlacementTextureV.loadFromFile("Resources/Wall-Position-Vertical.png");

	Sprite Board(MainBoard);
	Sprite Player[2];
	Sprite Wall[2][10];
	Sprite WallPlacement;
	WallPlacement.setPosition(1000, 1000);
	WallPlacement.setTexture(WallPlacementTextureH);

	Player[0].setTexture(PlayerTexture[0]);
	Player[1].setTexture(PlayerTexture[1]);
	
	for (index = 0; index < 10; index++)
	{
		Wall[0][index].setTexture(WallBlueTextureH);
		Wall[1][index].setTexture(WallOrangeTextureH);
	}

	Player[0].setPosition(321, 625);
	Player[1].setPosition(322, 10);
	Wall[1][0].setPosition(762, 129);
	Wall[0][0].setPosition(762, 591);
	for (index = 1; index < 10; index++)
	{
		Wall[1][index].setPosition(1025, 169);
		Wall[0][index].setPosition(1025, 619);
	}

	bool boardMatrix[17][17] = { false };
	bool playerTurn = false;
	bool wallBeingPlaced = false;
	bool WallFacing = true;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case Event::Closed:
				{
					window.close();
					break;
				}
				case Event::KeyPressed:
				{
					Vector2i playerPosition;
					playerPosition.x = Player[playerTurn].getPosition().x;
					playerPosition.y = Player[playerTurn].getPosition().y;
					if (wallBeingPlaced == false)
					{
						if ((Keyboard::isKeyPressed(Keyboard::Left) && playerTurn == 0)
							|| (Keyboard::isKeyPressed(Keyboard::A) && playerTurn == 1 ))
						{
							changePos(Player[playerTurn], playerPosition, playerPosMatrix[playerTurn], playerPosMatrix[!playerTurn], boardMatrix, LEFT);
						}
						else if ((Keyboard::isKeyPressed(Keyboard::Right) && playerTurn == 0)
							|| (Keyboard::isKeyPressed(Keyboard::D) && playerTurn == 1))
						{
							changePos(Player[playerTurn], playerPosition, playerPosMatrix[playerTurn], playerPosMatrix[!playerTurn], boardMatrix, RIGHT);
						}
						else if ((Keyboard::isKeyPressed(Keyboard::Up) && playerTurn == 0)
							|| (Keyboard::isKeyPressed(Keyboard::W) && playerTurn == 1))
						{
							changePos(Player[playerTurn], playerPosition, playerPosMatrix[playerTurn], playerPosMatrix[!playerTurn], boardMatrix, UP);
						}
						else if ((Keyboard::isKeyPressed(Keyboard::Down) && playerTurn == 0)
							|| (Keyboard::isKeyPressed(Keyboard::S) && playerTurn == 1))
						{
							changePos(Player[playerTurn], playerPosition, playerPosMatrix[playerTurn], playerPosMatrix[!playerTurn], boardMatrix, DOWN);
						}
					}
					playerNewPosition.x = Player[playerTurn].getPosition().x;
					playerNewPosition.y = Player[playerTurn].getPosition().y;
					if (playerPosMatrix[0].y == 0)
						printf("PLayer 1 wins !");
					else
						if (playerPosMatrix[1].y == 16)
							printf("PLayer 2 wins !");
					if(playerNewPosition != playerPosition)
						playerTurn = !playerTurn;
					break;
				}
				case Event::MouseButtonReleased:
				{
					if (wallBeingPlaced == true && isBoardClicked(window, event))
					{
						if (event.mouseButton.button == Mouse::Left && isAllowedToPlaceWall(boardMatrix,WallPrediction,WallFacing))
						{
							int WallWidth = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().width;
							int WallHeight = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().height;
							Vector2i mousePos = Mouse::getPosition(window);
							Wall[playerTurn][wallIndex[playerTurn]].setPosition(WallPrediction.x * 77 + 19 - WallWidth / 2, WallPrediction.y * 77 + 19 - WallHeight / 2);
							wallIndex[playerTurn]++;
							playerTurn = !playerTurn;
							wallBeingPlaced = false;
							WallPlacement.setPosition(1000 , 1000);
							WallPlacement.setTexture(WallPlacementTextureH, 1);
							std::cout << WallPrediction.x << " " << WallPrediction.y << std::endl;
							if (WallFacing == true)
							{
								boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2 - 2] = true;
								boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2 - 1] = true;
								boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2] = true;
							}
							else
							{
								boardMatrix[WallPrediction.y * 2 - 2][WallPrediction.x * 2 - 1] = true;
								boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2 - 1] = true;
								boardMatrix[WallPrediction.y * 2][WallPrediction.x * 2 - 1] = true;
							}
							for (int i = 0; i < 17; i++)
							{
								for (int j = 0; j < 17; j++)
									std::cout << boardMatrix[i][j] << " ";
								std::cout << std::endl;
							}

						}
					}
					else if (wallBeingPlaced == true && event.mouseButton.button == Mouse::Right)
					{
						if (WallFacing == true)
						{
							if (playerTurn == true)
								Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallOrangeTextureV,1);
							else Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallBlueTextureV,1);
							WallFacing = false;
							WallPlacement.setTexture(WallPlacementTextureV, 1);
						}
						else
						{
							if (playerTurn == true)
								Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallOrangeTextureH,1);
							else Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallBlueTextureH,1);
							WallFacing = true;
							WallPlacement.setTexture(WallPlacementTextureH, 1);
						}
						int WallWidth = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().width;
						int WallHeight = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().height;
						Vector2i mousePos = Mouse::getPosition(window);
						Wall[playerTurn][wallIndex[playerTurn]].setPosition(mousePos.x - WallWidth / 2, mousePos.y - WallHeight / 2);
					}
					break;
				}
				default: {
					if (wallBeingPlaced == true)
					{
						int WallWidth = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().width;
						int WallHeight = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().height;
						Vector2i mousePos = Mouse::getPosition(window);
						Wall[playerTurn][wallIndex[playerTurn]].setPosition(mousePos.x - WallWidth / 2, mousePos.y - WallHeight / 2);
						wallPlacementPrediction(mousePos, WallPrediction);
						WallPlacement.setPosition(WallPrediction.x*77+19 - WallWidth / 2, WallPrediction.y*77+19 - WallHeight / 2);
					}
					else if (wallBeingPlaced == false && isSpriteClicked(Wall[playerTurn][wallIndex[playerTurn]], window))
					{
						wallBeingPlaced = true;
						WallFacing = true;
						if(playerTurn == false && wallIndex[0]+1 < 10)
							Wall[0][wallIndex[0] + 1].setPosition(762, 591);
						else if(playerTurn == true && wallIndex[1] + 1 < 10)
							Wall[1][wallIndex[1] + 1].setPosition(762, 129);
					}
					break; 
				}
			}
		}
		window.clear();
		window.draw(Board);
		window.draw(WallPlacement);
		for (index = 9; index > 0; index--)
		{
			window.draw(Wall[1][index]);
			window.draw(Wall[0][index]);
		}
		window.draw(Wall[1][0]);
		window.draw(Wall[0][0]);
		window.draw(Player[0]);
		window.draw(Player[1]);
		window.display();
	}
	return 0;
}

void changePos(Sprite& player, Vector2i playerPosition, Vector2i &playerPosMatrix,Vector2i player2pos, bool boardMatrix[17][17], int direction)
{
	Vector2i nextPos=playerPosMatrix;
	if (direction == LEFT)
	{
		if (playerPosition.x - JUMP >= 0 && playerPosition.x - JUMP <= 700)
		{
			nextPos.x -= 2;
			if (nextPos == player2pos && isAbleToMove(boardMatrix,nextPos,direction) && isAbleToMove(boardMatrix, playerPosMatrix, direction))
			{
				player.setPosition(playerPosition.x - (2*JUMP), playerPosition.y);
				playerPosMatrix.x -= 4;
			}
			else
				if (isAbleToMove(boardMatrix, playerPosMatrix, direction) && nextPos != player2pos)
			{
				player.setPosition(playerPosition.x - JUMP, playerPosition.y);
				playerPosMatrix.x -= 2;
			}
		}
	}
	else if (direction == RIGHT)
	{
		if (playerPosition.x + JUMP >= 0 && playerPosition.x + JUMP <= 700)
		{
			nextPos.x += 2;
			if (nextPos == player2pos && isAbleToMove(boardMatrix, nextPos, direction) && isAbleToMove(boardMatrix, playerPosMatrix, direction))
			{
				player.setPosition(playerPosition.x + (2*JUMP), playerPosition.y);
				playerPosMatrix.x += 4;
			}
			else
				if (isAbleToMove(boardMatrix, playerPosMatrix, direction) && nextPos != player2pos)
			{
				player.setPosition(playerPosition.x + JUMP, playerPosition.y);
				playerPosMatrix.x += 2;
			}
		}
	}
	else if (direction == UP)
	{
		if (playerPosition.y - JUMP >= 0 && playerPosition.y - JUMP <= 700)
		{
			nextPos.y -= 2;
			if (nextPos == player2pos && isAbleToMove(boardMatrix, nextPos, direction) && isAbleToMove(boardMatrix, playerPosMatrix, direction))
			{
				player.setPosition(playerPosition.x, playerPosition.y - (2*JUMP));
				playerPosMatrix.y -= 4;
			}
			else
				if (isAbleToMove(boardMatrix, playerPosMatrix, direction) && nextPos != player2pos)
			{
				player.setPosition(playerPosition.x, playerPosition.y - JUMP);
				playerPosMatrix.y -= 2;
			}
		}
	}
	else if (direction == DOWN)
	{
		if (playerPosition.y + JUMP >= 0 && playerPosition.y + JUMP <= 700)
		{
			nextPos.y += 2;
			if (nextPos == player2pos && isAbleToMove(boardMatrix, nextPos, direction) && isAbleToMove(boardMatrix, playerPosMatrix, direction))
			{
				player.setPosition(playerPosition.x, playerPosition.y + (2*JUMP));
				playerPosMatrix.y += 4;
			}
			else
				if (isAbleToMove(boardMatrix, playerPosMatrix, direction) && nextPos!=player2pos)
			{
				player.setPosition(playerPosition.x, playerPosition.y + JUMP);
				playerPosMatrix.y += 2;
			}
		}
	}
}

bool isSpriteClicked(Sprite Wall, RenderWindow &Window)
{
	Vector2i mousePos = Mouse::getPosition(Window);
	if (mousePos.x > Wall.getPosition().x && mousePos.x < Wall.getPosition().x + Wall.getGlobalBounds().width
	 && mousePos.y > Wall.getPosition().y && mousePos.y < Wall.getPosition().y + Wall.getGlobalBounds().height)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			printf("Wall clicked!\n");
			return true;
		}
		return false;
	}
	return false;
}

bool isBoardClicked(RenderWindow &Window, Event event)
{
	Vector2i mousePos = Mouse::getPosition(Window);
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left
	 && mousePos.x >= 26 && mousePos.x <= 705 && mousePos.y >= 26 && mousePos.y <= 705)
	{
		printf("Board Clicked\n");
		return true;
	}
	return false;
}

bool isInsideWall(Sprite Wall, RenderWindow &Window)
{
	Vector2i mousePos = Mouse::getPosition(Window);
	if (mousePos.x > Wall.getPosition().x && mousePos.x < Wall.getPosition().x + Wall.getGlobalBounds().width
	 && mousePos.y > Wall.getPosition().y && mousePos.y < Wall.getPosition().y + Wall.getGlobalBounds().height)
		return true;
	return false;
}

bool isAbleToMove(bool boardMatrix[17][17], Vector2i playerPos, int direction)
{
	switch (direction)
	{
		case LEFT:  { return boardMatrix[playerPos.y][playerPos.x - 1] == false; break; }
		case RIGHT: { return boardMatrix[playerPos.y][playerPos.x + 1] == false; break; }
		case UP:    { return boardMatrix[playerPos.y - 1][playerPos.x] == false; break; }
		case DOWN:  { return boardMatrix[playerPos.y + 1][playerPos.x] == false; break; }
		default:    { return false; break; }
	}
}

void wallPlacementPrediction(Vector2i mousePos,Vector2i &prediction)
{
	prediction.x = (mousePos.x+19) / 77;
	prediction.y = (mousePos.y+19) / 77;
	if (prediction.x < 1) prediction.x = 1;
	if (prediction.x > 8) prediction.x = 8;
	if (prediction.y < 1) prediction.y = 1;
	if (prediction.y > 8) prediction.y = 8;

}

bool isAllowedToPlaceWall(bool boardMatrix[17][17], Vector2i prediction, bool wallFacing)
{
	if (wallFacing == true)
	{
		if (boardMatrix[prediction.y * 2 - 1][prediction.x * 2 - 2] == true)
			return false;
		if (boardMatrix[prediction.y * 2 - 1][prediction.x * 2 - 1] == true)
			return false;
		if (boardMatrix[prediction.y * 2 - 1][prediction.x * 2] == true)
			return false;
	}
	else
	{
		if (boardMatrix[prediction.y * 2 - 2][prediction.x * 2 - 1] == true)
			return false;
		if (boardMatrix[prediction.y * 2 - 1][prediction.x * 2 - 1] == true)
			return false;
		if (boardMatrix[prediction.y * 2][prediction.x * 2 - 1] == true)
			return false;
	}
	return true;
}
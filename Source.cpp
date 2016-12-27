#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
#define JUMP 77
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

void changePos(Sprite& player, Vector2i playerPosition, Vector2i &playerPosMatrix, int direction);
bool isSpriteClicked(Sprite Wall, RenderWindow &Window);
bool isBoardClicked(RenderWindow &Window, Event event);
bool isInsideWall(Sprite Wall, RenderWindow &Window);
bool isAbleToMove(bool boardMatrix[17][17], Vector2i &playerPos, int direction);

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


	unsigned int index;
	unsigned short wallIndex[2] = { 0 };

	Vector2i playerNewPosition;
	Vector2i playerPosMatrix[2];
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

	Sprite Board(MainBoard);
	Sprite Player[2];

	Player[0].setTexture(PlayerTexture[0]);
	Player[1].setTexture(PlayerTexture[1]);
	Sprite Wall[2][10];
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
						if ((Keyboard::isKeyPressed(Keyboard::Left) && playerTurn == 0 && isAbleToMove(boardMatrix, playerPosMatrix[0], LEFT))
							|| (Keyboard::isKeyPressed(Keyboard::A) && playerTurn == 1 && isAbleToMove(boardMatrix, playerPosMatrix[1], LEFT)))
						{
							changePos(Player[playerTurn], playerPosition, playerPosMatrix[playerTurn], LEFT);
						}
						else if ((Keyboard::isKeyPressed(Keyboard::Right) && playerTurn == 0 && isAbleToMove(boardMatrix, playerPosMatrix[0], RIGHT))
							|| (Keyboard::isKeyPressed(Keyboard::D) && playerTurn == 1 && isAbleToMove(boardMatrix, playerPosMatrix[1], RIGHT)))
						{
							changePos(Player[playerTurn], playerPosition, playerPosMatrix[playerTurn], RIGHT);
						}
						else if ((Keyboard::isKeyPressed(Keyboard::Up) && playerTurn == 0 && isAbleToMove(boardMatrix, playerPosMatrix[0], UP))
							|| (Keyboard::isKeyPressed(Keyboard::W) && playerTurn == 1 && isAbleToMove(boardMatrix, playerPosMatrix[1], UP)))
						{
							changePos(Player[playerTurn], playerPosition, playerPosMatrix[playerTurn], UP);
						}
						else if ((Keyboard::isKeyPressed(Keyboard::Down) && playerTurn == 0 && isAbleToMove(boardMatrix, playerPosMatrix[0], DOWN))
							|| (Keyboard::isKeyPressed(Keyboard::S) && playerTurn == 1 && isAbleToMove(boardMatrix, playerPosMatrix[1], DOWN)))
						{
							changePos(Player[playerTurn], playerPosition, playerPosMatrix[playerTurn], DOWN);
						}
					}
					playerNewPosition.x = Player[playerTurn].getPosition().x;
					playerNewPosition.y = Player[playerTurn].getPosition().y;
					if(playerNewPosition != playerPosition)
						playerTurn = !playerTurn;
					break;
				}
				case Event::MouseButtonReleased:
				{
					if (wallBeingPlaced == true && isBoardClicked(window, event))
					{
						if (event.mouseButton.button == Mouse::Left)
						{
							int WallWidth = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().width;
							int WallHeight = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().height;
							Vector2i mousePos = Mouse::getPosition(window);
							Wall[playerTurn][wallIndex[playerTurn]].setPosition(mousePos.x - WallWidth / 2, mousePos.y - WallHeight / 2);
							wallIndex[playerTurn]++;
							playerTurn = !playerTurn;
							wallBeingPlaced = false;
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
						}
						else
						{
							if (playerTurn == true)
								Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallOrangeTextureH,1);
							else Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallBlueTextureH,1);
							WallFacing = true;
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

void changePos(Sprite& player, Vector2i playerPosition, Vector2i &playerPosMatrix, int direction)
{
	if (direction == LEFT)
	{
		if (playerPosition.x - JUMP >= 0 && playerPosition.x - JUMP <= 700)
		{
			player.setPosition(playerPosition.x - JUMP, playerPosition.y);
			playerPosMatrix.x -= 1;
		}
	}
	else if (direction == RIGHT)
	{
		if (playerPosition.x + JUMP >= 0 && playerPosition.x + JUMP <= 700)
		{
			player.setPosition(playerPosition.x + JUMP, playerPosition.y);
			playerPosMatrix.x += 1;
		}
	}
	else if (direction == UP)
	{
		if (playerPosition.y - JUMP >= 0 && playerPosition.y - JUMP <= 700)
		{
			player.setPosition(playerPosition.x, playerPosition.y - JUMP);
			playerPosMatrix.y -= 1;
		}
	}
	else if (direction == DOWN)
	{
		if (playerPosition.y + JUMP >= 0 && playerPosition.y + JUMP <= 700)
		{
			player.setPosition(playerPosition.x, playerPosition.y + JUMP);
			playerPosMatrix.y += 1;
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

bool isAbleToMove(bool boardMatrix[17][17], Vector2i &playerPos, int direction)
{
	switch (direction)
	{
		case LEFT:  { return boardMatrix[playerPos.x - 1][playerPos.y] == false; break; }
		case RIGHT: { return boardMatrix[playerPos.x + 1][playerPos.y] == false; break; }
		case UP:    { return boardMatrix[playerPos.x][playerPos.y - 1] == false; break; }
		case DOWN:  { return boardMatrix[playerPos.x][playerPos.y + 1] == false; break; }
		default:    { return false; break; }
	}
}
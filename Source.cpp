#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
#define JUMP 75

void changePos(Sprite& player, Vector2i playerPosition, int direction) // 0 - LEFT | 1 - RIGHT | 2 - UP | 3 - DOWN
{
	if (direction == 0)
	{
		if (playerPosition.x - JUMP >= 55 && playerPosition.x - JUMP <= 670)
			player.setPosition(playerPosition.x - JUMP, playerPosition.y);
	}
	else if (direction == 1)
	{
		if (playerPosition.x + JUMP >= 60 && playerPosition.x + JUMP <= 670)
			player.setPosition(playerPosition.x + JUMP, playerPosition.y);
	}
	else if (direction == 2)
	{
		if (playerPosition.y - JUMP >= 60 && playerPosition.y - JUMP <= 670)
			player.setPosition(playerPosition.x, playerPosition.y - JUMP);
	}
	else if (direction == 3)
	{
		if (playerPosition.y + JUMP >= 60 && playerPosition.y + JUMP <= 670)
			player.setPosition(playerPosition.x, playerPosition.y + JUMP);
	}
}

bool isSpriteClicked(Sprite Wall, RenderWindow &Window)
{
	Vector2i mousePos = Mouse::getPosition(Window);
	if(mousePos.x > Wall.getPosition().x && mousePos.x < Wall.getPosition().x + Wall.getGlobalBounds().width && mousePos.y > Wall.getPosition().y && mousePos.y < Wall.getPosition().y + Wall.getGlobalBounds().height)
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
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && mousePos.x >= 64 && mousePos.x <= 723 && mousePos.y >= 68 && mousePos.y <= 731)
	{
		printf("Board Clicked\n");
		return true;
	}
	return false;
}

bool isInsideWall(Sprite Wall, RenderWindow &Window)
{
	Vector2i mousePos = Mouse::getPosition(Window);
	if (mousePos.x > Wall.getPosition().x && mousePos.x < Wall.getPosition().x + Wall.getGlobalBounds().width && mousePos.y > Wall.getPosition().y && mousePos.y < Wall.getPosition().y + Wall.getGlobalBounds().height)
		return true;
	return false;
}

bool isAbleToMove(bool boardMatrix[17][17], Vector2i playerPos, int direction)
{
	if (direction == 0)
		return boardMatrix[playerPos.x][playerPos.y - 1] == 0;
	else if(direction == 1)
		return boardMatrix[playerPos.x][playerPos.y + 1] == 0;
	else if (direction == 2)
		return boardMatrix[playerPos.x - 1][playerPos.y] == 0;
	else if (direction == 3)
		return boardMatrix[playerPos.x + 1][playerPos.y] == 0;
	return false;
}
int main()
{
	RenderWindow window(VideoMode(1024, 800), "Quoridor Game", Style::Close | Style::Titlebar);
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
	playerPosMatrix[0].x = 16;
	playerPosMatrix[0].y = 8;
	playerPosMatrix[1].x = 0;
	playerPosMatrix[1].y = 8;

	MainBoard.loadFromFile("Resources/Main_Table.jpg");
	PlayerTexture[0].loadFromFile("Resources/Player1.png");
	PlayerTexture[1].loadFromFile("Resources/Player2.png");
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

	Player[0].setPosition(364, 664);
	Player[1].setPosition(359, 61);
	Wall[1][0].setPosition(799, 169);
	Wall[0][0].setPosition(799, 619);
	for (index = 1; index < 10; index++)
	{
		Wall[1][index].setPosition(1025, 169);
		Wall[0][index].setPosition(1025, 619);
	}

	bool boardMatrix[17][17] = { 0 };
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
					if (((Keyboard::isKeyPressed(Keyboard::Left) && playerTurn == 0) || (Keyboard::isKeyPressed(Keyboard::A) && playerTurn == 1)) && wallBeingPlaced == false && isAbleToMove(boardMatrix,playerPosition,0))
						changePos(Player[playerTurn], playerPosition, 0);
					else if (((Keyboard::isKeyPressed(Keyboard::Right) && playerTurn == 0) || (Keyboard::isKeyPressed(Keyboard::D) && playerTurn == 1)) && wallBeingPlaced == false)
						changePos(Player[playerTurn], playerPosition, 1);
					else if (((Keyboard::isKeyPressed(Keyboard::Up) && playerTurn == 0) || (Keyboard::isKeyPressed(Keyboard::W) && playerTurn == 1)) && wallBeingPlaced == false)
						changePos(Player[playerTurn], playerPosition, 2);
					else if (((Keyboard::isKeyPressed(Keyboard::Down) && playerTurn == 0) || (Keyboard::isKeyPressed(Keyboard::S) && playerTurn == 1)) && wallBeingPlaced == false)
						changePos(Player[playerTurn], playerPosition, 3);
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
							Wall[0][wallIndex[0] + 1].setPosition(799, 619);
						else if(playerTurn == true && wallIndex[1] + 1 < 10)
							Wall[1][wallIndex[1] + 1].setPosition(799, 169);
					}
					break; 
				}
			}
		}
		window.clear(Color::White);
		window.draw(Board);
		window.draw(Player[1]);
		for (index = 9; index > 0; index--)
		{
			window.draw(Wall[1][index]);
			window.draw(Wall[0][index]);
		}
		window.draw(Player[0]);
		window.draw(Wall[1][0]);
		window.draw(Wall[0][0]);
		window.display();
	}
	return 0;
}
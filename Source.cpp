#include <SFML/Graphics.hpp>
#include <stdlib.h>
using namespace sf;
#define JUMP 77
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define TWOPLAYERS false
#define FOURPLAYERS true

void changeGraphicalPos(Sprite& player, Vector2i playerPosMatrix);
bool changeMatrixPos(Vector2i &playerPosMatrix, Vector2i player2PosMatrix, bool boardMatrix[17][17], int direction);
void wallPlacementPrediction(Vector2i mousePos, Vector2i &prediction);
bool isSpriteClicked(Sprite Wall, RenderWindow &Window);
bool isBoardClicked(RenderWindow &Window, Event event);
bool isInsideWall(Sprite Wall, RenderWindow &Window);
bool isAbleToMove(bool boardMatrix[17][17], Vector2i playerPos, int direction);
bool isAllowedToPlaceWall(bool boardMatrix[17][17], Vector2i prediction, bool wallFacing);
bool pressedToMove(int direction, int playerTurn, Event event);
void searchRoute(bool boardMatrix[17][17], Vector2i player, int playerTurn, int &result, int visited[17][17]);

int main()
{
	bool gameMode=FOURPLAYERS;
	//RenderWindow mainMenu(VideoMode(950, 730), "Quoridor Game", Style::Close | Style::Titlebar);
	RenderWindow gameWindow(VideoMode(950, 730), "Quoridor Game", Style::Close | Style::Titlebar);
	gameWindow.setFramerateLimit(45);
	//Texture MainMenu;
	Texture MainBoard;
	Texture PlayerTexture[4];
	Texture WallOrangeTextureH;
	Texture WallOrangeTextureV;
	Texture WallBlueTextureH;
	Texture WallBlueTextureV;
		Texture WallGreenTextureH;
		Texture WallGreenTextureV;
		Texture WallRedTextureH;
		Texture WallRedTextureV;
	Texture WallPlacementTextureH;
	Texture WallPlacementTextureV;

	unsigned int index;

		unsigned short wallIndex[4] = { 0 };
	int direction;
	Vector2i playerPosMatrix[4];
	Vector2i playerNewPosition;
	if (gameMode == FOURPLAYERS)
	{
		playerPosMatrix[2].x = 16;
		playerPosMatrix[2].y = 8;
		playerPosMatrix[3].x = 0;
		playerPosMatrix[3].y = 8;
	}
	Vector2i WallPrediction;
	playerPosMatrix[0].x = 8;
	playerPosMatrix[0].y = 16;
	playerPosMatrix[1].x = 8;
	playerPosMatrix[1].y = 0;

	//MainMenu.loadFromFile("Resources/MainMenu.png");
	if (gameMode == TWOPLAYERS)
		MainBoard.loadFromFile("Resources/MainBoard.png");
	else
		MainBoard.loadFromFile("Resources/MainBoard4Players.png");
	PlayerTexture[0].loadFromFile("Resources/Player-Blue.png");
	PlayerTexture[1].loadFromFile("Resources/Player-Orange.png");
	WallOrangeTextureH.loadFromFile("Resources/Wall-Orange-Horizontal.png");
	WallOrangeTextureV.loadFromFile("Resources/Wall-Orange-Vertical.png");
	WallBlueTextureH.loadFromFile("Resources/Wall-Blue-Horizontal.png");
	WallBlueTextureV.loadFromFile("Resources/Wall-Blue-Vertical.png");
	WallPlacementTextureH.loadFromFile("Resources/Wall-Position-Horizontal.png");
	WallPlacementTextureV.loadFromFile("Resources/Wall-Position-Vertical.png");
	if (gameMode == FOURPLAYERS)
	{
		PlayerTexture[2].loadFromFile("Resources/Player-Green.png");
		PlayerTexture[3].loadFromFile("Resources/Player-Red.png");
		WallGreenTextureH.loadFromFile("Resources/Wall-Green-Horizontal.png");
		WallGreenTextureV.loadFromFile("Resources/Wall-Green-Vertical.png");
		WallRedTextureH.loadFromFile("Resources/Wall-Red-Horizontal.png");
		WallRedTextureV.loadFromFile("Resources/Wall-Red-Vertical.png");
	}

	Sprite Board(MainBoard);
		Sprite Player[4];
		Sprite Wall[4][5];
	Sprite WallPlacement;
	WallPlacement.setPosition(1000, 1000);
	WallPlacement.setTexture(WallPlacementTextureH);

	Player[0].setTexture(PlayerTexture[0]);
	Player[1].setTexture(PlayerTexture[1]);
	if (gameMode == FOURPLAYERS)
	{
		Player[2].setTexture(PlayerTexture[2]);
		Player[3].setTexture(PlayerTexture[3]);
	}
	if (gameMode == TWOPLAYERS)
	for (index = 0; index < 10; index++)
	{
		Wall[0][index].setTexture(WallBlueTextureH);
		Wall[1][index].setTexture(WallOrangeTextureH);
	}
	else
		for (index = 0; index < 5; index++)
		{
			Wall[0][index].setTexture(WallBlueTextureH);
			Wall[1][index].setTexture(WallOrangeTextureH);
			Wall[2][index].setTexture(WallGreenTextureH);
			Wall[3][index].setTexture(WallRedTextureH);
		}

	changeGraphicalPos(Player[0], playerPosMatrix[0]);
	changeGraphicalPos(Player[1], playerPosMatrix[1]);
	if (gameMode == FOURPLAYERS)
	{
		changeGraphicalPos(Player[2], playerPosMatrix[2]);
		changeGraphicalPos(Player[3], playerPosMatrix[3]);
	}
	Wall[1][0].setPosition(762, 129);
	Wall[0][0].setPosition(762, 591);
	if (gameMode == FOURPLAYERS)
	{
		Wall[2][0].setPosition(762, 321);
		Wall[3][0].setPosition(762, 421);
	}
	if (gameMode == TWOPLAYERS)
	for (index = 1; index < 10; index++)
	{
		Wall[1][index].setPosition(1025, 169);
		Wall[0][index].setPosition(1025, 619);
	}
	else
		for (index = 1; index < 5; index++)
		{
			Wall[1][index].setPosition(1025, 169);
			Wall[0][index].setPosition(1025, 619);
			Wall[2][index].setPosition(1025, 169);
			Wall[3][index].setPosition(1025, 619);
		}

	bool boardMatrix[17][17] = { false };
	bool playerTurn = rand() % 1;
	bool wallBeingPlaced = false;
	bool WallFacing = true;
	bool change = false;
	bool waitForInput = false;

	while (gameWindow.isOpen())
	{
		Event event;
		while (gameWindow.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
			{
				gameWindow.close();
				break;
			}
			case Event::KeyPressed:
			{
				if (waitForInput == true)
				{
					if (pressedToMove(UP, playerTurn, event) && isAbleToMove(boardMatrix, playerPosMatrix[!playerTurn], UP))
					{
						if (direction == LEFT) { playerPosMatrix[playerTurn].x -= 2; playerPosMatrix[playerTurn].y -= 2; waitForInput = false; }
						else if (direction == RIGHT) { playerPosMatrix[playerTurn].x += 2; playerPosMatrix[playerTurn].y -= 2; waitForInput = false; }
					}
					else if (pressedToMove(DOWN, playerTurn, event) && isAbleToMove(boardMatrix, playerPosMatrix[!playerTurn], DOWN))
					{
						if (direction == LEFT) { playerPosMatrix[playerTurn].x -= 2; playerPosMatrix[playerTurn].y += 2; waitForInput = false; }
						else if (direction == RIGHT) { playerPosMatrix[playerTurn].x += 2; playerPosMatrix[playerTurn].y += 2; waitForInput = false; }
					}
					if (pressedToMove(LEFT, playerTurn, event) && isAbleToMove(boardMatrix, playerPosMatrix[!playerTurn], LEFT))
					{
						if (direction == UP) { playerPosMatrix[playerTurn].x -= 2; playerPosMatrix[playerTurn].y -= 2; waitForInput = false; }
						else if (direction == DOWN) { playerPosMatrix[playerTurn].x -= 2; playerPosMatrix[playerTurn].y += 2; waitForInput = false; }
					}
					else if (pressedToMove(RIGHT, playerTurn, event) && isAbleToMove(boardMatrix, playerPosMatrix[!playerTurn], RIGHT))
					{
						if (direction == UP) { playerPosMatrix[playerTurn].x += 2; playerPosMatrix[playerTurn].y -= 2; waitForInput = false; }
						else if (direction == DOWN) { playerPosMatrix[playerTurn].x += 2; playerPosMatrix[playerTurn].y += 2; waitForInput = false; }
					}
					if (waitForInput == false)
					{
						changeGraphicalPos(Player[playerTurn], playerPosMatrix[playerTurn]);
						playerTurn != playerTurn;
					}
				}
				else
				{
					Vector2i playerPosMatrixOld = playerPosMatrix[playerTurn];
					if (wallBeingPlaced == false)
					{
						if (pressedToMove(LEFT, playerTurn, event)) { direction = LEFT;  change = true; }
						else if (pressedToMove(RIGHT, playerTurn, event)) { direction = RIGHT; change = true; }
						else if (pressedToMove(UP, playerTurn, event)) { direction = UP;    change = true; }
						else if (pressedToMove(DOWN, playerTurn, event)) { direction = DOWN;  change = true; }
						if (change == true)
						{
							if (changeMatrixPos(playerPosMatrix[playerTurn], playerPosMatrix[!playerTurn], boardMatrix, direction))
								waitForInput = true;
							else
							{
								changeGraphicalPos(Player[playerTurn], playerPosMatrix[playerTurn]);
								playerNewPosition.x = Player[playerTurn].getPosition().x;
								playerNewPosition.y = Player[playerTurn].getPosition().y;
								if (playerPosMatrix[0].y == 0)
									printf("Player 1 wins !");
								else if (playerPosMatrix[1].y == 16)
									printf("Player 2 wins !");
								if (playerPosMatrixOld.x != playerPosMatrix[playerTurn].x || playerPosMatrixOld.y != playerPosMatrix[playerTurn].y)
									playerTurn = !playerTurn;
							}
							change = false;
						}
					}
				}
				break;
			}
			case Event::MouseButtonReleased:
			{
				if (wallBeingPlaced == true && isBoardClicked(gameWindow, event))
				{
					if (event.mouseButton.button == Mouse::Left && isAllowedToPlaceWall(boardMatrix, WallPrediction, WallFacing))
					{
						int WallWidth = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().width;
						int WallHeight = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().height;
						int result = 0, visited[17][17] = {0};
						searchRoute(boardMatrix, playerPosMatrix[0], playerTurn, result, visited);
						if (result == 1)
						{
							Wall[playerTurn][wallIndex[playerTurn]].setPosition(WallPrediction.x * 77 + 19 - WallWidth / 2, WallPrediction.y * 77 + 19 - WallHeight / 2);
							wallIndex[playerTurn]++;
							playerTurn = !playerTurn;
							wallBeingPlaced = false;
							WallPlacement.setPosition(1000, 1000);
							WallPlacement.setTexture(WallPlacementTextureH, 1);
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
						}
						else
							printf("/nmesajRandom");
					}
				}
				else if (wallBeingPlaced == true && event.mouseButton.button == Mouse::Right)
				{
					if (WallFacing == true)
					{
						if (playerTurn == true)
							Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallOrangeTextureV, 1);
						else Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallBlueTextureV, 1);
						WallFacing = false;
						WallPlacement.setTexture(WallPlacementTextureV, 1);
					}
					else
					{
						if (playerTurn == true)
							Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallOrangeTextureH, 1);
						else Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallBlueTextureH, 1);
						WallFacing = true;
						WallPlacement.setTexture(WallPlacementTextureH, 1);
					}
					int WallWidth = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().width;
					int WallHeight = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().height;
					Vector2i mousePos = Mouse::getPosition(gameWindow);
					Wall[playerTurn][wallIndex[playerTurn]].setPosition(mousePos.x - WallWidth / 2, mousePos.y - WallHeight / 2);
				}
				break;
			}
			default: {
				if (wallBeingPlaced == true)
				{
					int WallWidth = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().width;
					int WallHeight = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().height;
					Vector2i mousePos = Mouse::getPosition(gameWindow);
					Wall[playerTurn][wallIndex[playerTurn]].setPosition(mousePos.x - WallWidth / 2, mousePos.y - WallHeight / 2);
					wallPlacementPrediction(mousePos, WallPrediction);
					WallPlacement.setPosition(WallPrediction.x * 77 + 19 - WallWidth / 2, WallPrediction.y * 77 + 19 - WallHeight / 2);
				}
				else if (wallBeingPlaced == false && isSpriteClicked(Wall[playerTurn][wallIndex[playerTurn]], gameWindow))
				{
					wallBeingPlaced = true;
					WallFacing = true;
					if (playerTurn == false && wallIndex[0] + 1 < 10)
						Wall[0][wallIndex[0] + 1].setPosition(762, 591);
					else if (playerTurn == true && wallIndex[1] + 1 < 10)
						Wall[1][wallIndex[1] + 1].setPosition(762, 129);
				}
				break;
			}
			}
		}
		gameWindow.clear();
		gameWindow.draw(Board);
		gameWindow.draw(WallPlacement);
		if(gameMode==TWOPLAYERS)
		for (index = 9; index > 0; index--)
		{
			gameWindow.draw(Wall[1][index]);
			gameWindow.draw(Wall[0][index]);
		}
		else
		{
			for (index = 4; index > 0; index--)
			{
				gameWindow.draw(Wall[1][index]);
				gameWindow.draw(Wall[0][index]);
				gameWindow.draw(Wall[2][index]);
				gameWindow.draw(Wall[3][index]);
			}
		}
		gameWindow.draw(Wall[1][0]);
		gameWindow.draw(Wall[0][0]);
		gameWindow.draw(Player[0]);
		gameWindow.draw(Player[1]);
		if (gameMode == FOURPLAYERS)
		{
			gameWindow.draw(Player[2]);
			gameWindow.draw(Player[3]);
		}
		gameWindow.display();
	}
	return 0;
}

void changeGraphicalPos(Sprite& player, Vector2i playerPosMatrix)
{
	player.setPosition((playerPosMatrix.x / 2) * JUMP + 13, (playerPosMatrix.y / 2) * JUMP + 10);
}

bool changeMatrixPos(Vector2i &playerPosMatrix, Vector2i player2PosMatrix, bool boardMatrix[17][17], int direction)
{
	Vector2i nextPos = playerPosMatrix;
	if (isAbleToMove(boardMatrix, playerPosMatrix, direction))
	{
		switch (direction)
		{
		case LEFT: {nextPos.x -= 2; break; }
		case RIGHT: {nextPos.x += 2; break; }
		case UP: {nextPos.y -= 2; break; }
		case DOWN: {nextPos.y += 2; break; }
		}
		if (nextPos == player2PosMatrix && isAbleToMove(boardMatrix, nextPos, direction))
		{
			switch (direction)
			{
			case LEFT: {playerPosMatrix.x -= 4; break; }
			case RIGHT: {playerPosMatrix.x += 4; break; }
			case UP: {playerPosMatrix.y -= 4; break; }
			case DOWN: {playerPosMatrix.y += 4; break; }
			}
		}
		else if (nextPos != player2PosMatrix)
		{
			switch (direction)
			{
			case LEFT: {playerPosMatrix.x -= 2; break; }
			case RIGHT: {playerPosMatrix.x += 2; break; }
			case UP: {playerPosMatrix.y -= 2; break; }
			case DOWN: {playerPosMatrix.y += 2; break; }
			}
		}
		else return true;
	}
	return false;
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
	case LEFT: { return boardMatrix[playerPos.y][playerPos.x - 1] == false && playerPos.x - 2 >= 0; break; }
	case RIGHT: { return boardMatrix[playerPos.y][playerPos.x + 1] == false && playerPos.x + 2 <= 17; break; }
	case UP: { return boardMatrix[playerPos.y - 1][playerPos.x] == false && playerPos.y - 2 >= 0; break; }
	case DOWN: { return boardMatrix[playerPos.y + 1][playerPos.x] == false && playerPos.y + 2 <= 17; break; }
	default: { return false; break; }
	}
}

void wallPlacementPrediction(Vector2i mousePos, Vector2i &prediction)
{
	prediction.x = (mousePos.x + 19) / 77;
	prediction.y = (mousePos.y + 19) / 77;
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

bool pressedToMove(int direction, int playerTurn, Event event)
{
	switch (direction)
	{
	case LEFT:
	{
		return ((Event::KeyReleased && event.key.code == Keyboard::Left && playerTurn == 0)
			|| (Event::KeyReleased && event.key.code == Keyboard::A && playerTurn == 1));
		break;
	}
	case RIGHT:
	{
		return ((Event::KeyReleased && event.key.code == Keyboard::Right && playerTurn == 0)
			|| (Event::KeyReleased && event.key.code == Keyboard::D && playerTurn == 1));
		break;
	}
	case UP:
	{
		return ((Event::KeyReleased && event.key.code == Keyboard::Up && playerTurn == 0)
			|| (Event::KeyReleased && event.key.code == Keyboard::W && playerTurn == 1));
		break;
	}
	case DOWN:
	{
		return ((Event::KeyReleased && event.key.code == Keyboard::Down && playerTurn == 0)
			|| (Event::KeyReleased && event.key.code == Keyboard::S && playerTurn == 1));
		break;
	}
	default: { return false; break; }
	}
}

void searchRoute(bool boardMatrix[17][17], Vector2i player, int playerTurn, int &result, int visited[17][17])
{
	if (result == 0)
	{
		if (player.y == 16 && playerTurn == 0)
			result = 1;
		else if (player.y == 0 && playerTurn == 1)
			result = 1;
		else if (visited[player.y][player.x] == 0)
		{
			visited[player.y][player.x] = 1;
			if (boardMatrix[player.y + 1][player.x] == 0)
			{
				player.y += 2;
				searchRoute(boardMatrix, player, playerTurn, result, visited);
			}
			if (boardMatrix[player.y][player.x - 1] == 0)
			{
				player.x -= 2;
				searchRoute(boardMatrix, player, playerTurn, result, visited);
			}
			if (boardMatrix[player.y][player.x + 1] == 0)
			{
				player.x += 2;
				searchRoute(boardMatrix, player, playerTurn, result, visited);
			}
			if (boardMatrix[player.y - 1][player.x] == 0)
			{
				player.y -= 2;
				searchRoute(boardMatrix, player, playerTurn, result, visited);
			}
		}
	}
}
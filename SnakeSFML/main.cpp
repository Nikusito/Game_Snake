#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

chrono::milliseconds elapsedTime;
chrono::milliseconds timer;

void Init();
void InitTimer();
void SetupWindow();
void InitSprites();
void InitGameField();
void DrawSprite(sf::Sprite& sprite, int col, int row);
void Draw();
void DrawGameField();
void Update(int dt);
void MoveSnake();
void HandleKeyBoardEvent();
void SpawnApple();

const int SNAKE_MOVE_TIME = 500;
const int TILE_SIZE = 64;
// Horizontal amount of tiles and Vertical amount of tiles
const int GAME_FIELD_SIZE_H = 15;
const int GAME_FIELD_SIZE_V = 15;

// Array with tile codes
char gameField[GAME_FIELD_SIZE_H][GAME_FIELD_SIZE_V];

int snakeMoveTimer = SNAKE_MOVE_TIME;
int addMoveTimer = 0;

//Declaring textures
sf::RenderWindow window(sf::VideoMode(GAME_FIELD_SIZE_H * TILE_SIZE, GAME_FIELD_SIZE_V * TILE_SIZE), "Snake");
sf::Sprite treeSprite;
sf::Sprite grassSprite;
sf::Sprite snakeSprite;
sf::Sprite appleSprite;
sf::Sprite headSprite;
sf::Sprite tailSprite;
sf::Sprite UpDownSprite;
sf::Sprite LeftRightSprite;
sf::Sprite UpRightSprite;
sf::Sprite UpLeftSprite;
sf::Sprite DownLeftSprite;
sf::Sprite DownRightSprite;
sf::Texture textureTerrain;

//enumerate the movement of the snake
enum EDirection {
	Left,
	Right,
	Down,
	Up
};

//enumeration of snake sprite rendering
enum class ESegment {
  Head,
  Left_Right,
  Up_Down,
  Up_Right,
  Up_Left,
  Down_Left,
  Down_Right,
  Tail,
  SnakeSprite
};

struct SnakeSegment
{// TODO: Add constructor with "row, col, direction" arguments that set corresponding class members. Default value for direction should be EDirection::Left
  SnakeSegment(int _row = 0, int _col = 0, int _rotat = 0, EDirection _direction = EDirection::Left, ESegment _segment = ESegment::SnakeSprite) :
    row(_row),
    col(_col),
    rotation(_rotat),
    direction(_direction),
    segment(_segment)
  {}

  //Draw Sprite
	void Draw()
	{
    switch (segment)
    {
    case ESegment::Head:
      DrawSprite(headSprite, col, row);
      break;
    case ESegment::Left_Right:
      LeftRightSprite.setOrigin(0.f, 0.f);
      DrawSprite(LeftRightSprite, col, row);
      break;
    case ESegment::Up_Down:
      DrawSprite(UpDownSprite, col, row);
      break;
    case ESegment::Up_Right:
      DrawSprite(UpRightSprite, col, row);
      break;
    case ESegment::Up_Left:
      DrawSprite(UpLeftSprite, col, row);
      break;
    case ESegment::Down_Left:
      DrawSprite(DownLeftSprite, col, row);
      break;
    case ESegment::Down_Right:
      DrawSprite(DownRightSprite, col, row);
      break;
    case ESegment::Tail:
      DrawSprite(tailSprite, col, row);
      break;
    case ESegment::SnakeSprite:
      DrawSprite(snakeSprite, col, row);
      break;
    }
	}

  //Moved Sprite head
  void MoveDown(int& _deltacol, int& _deltarow, sf::Sprite& sprite) {
    _deltacol = 0;
    _deltarow = 1;
    sprite.setOrigin(0.f, 0.f);
    sprite.setRotation(0.f);
  }
  void MoveUp(int& _deltacol, int& _deltarow, sf::Sprite& sprite) {
    _deltacol = 0;
    _deltarow = -1;
    sprite.setOrigin(64.f, 64.f);
    sprite.setRotation(180.f);
  }
  void MoveLeft(int& _deltacol, int& _deltarow, sf::Sprite& sprite) {
    _deltacol = -1;
    _deltarow = 0;
    sprite.setOrigin(0.f, 64.f);
    sprite.setRotation(90.f);
  }
  void MoveRight(int& _deltacol, int& _deltarow, sf::Sprite& sprite) {
    _deltacol = 1;
    _deltarow = 0;
    sprite.setOrigin(64.f, 0.f);
    sprite.setRotation(-90.f);
  }

	void Move()
	{
		int deltaCol = -1;
		int deltaRow = 0;

		switch (direction)
		{
		case Down:
		{
      MoveDown(deltaCol, deltaRow, headSprite);
			break;
		}
		case Up:
		{
      MoveUp(deltaCol, deltaRow, headSprite);
			break;
		}
		case Left:
		{
      MoveLeft(deltaCol, deltaRow, headSprite);
			break;
		}
		case Right:
		{
      MoveRight(deltaCol, deltaRow, headSprite);
			break;
		}
		}

		col += deltaCol;
		row += deltaRow;
	}

  ESegment segment;
	EDirection direction;

	int col;
	int row;
  int rotation;
};

struct Snake {
  vector<SnakeSegment> segments = {
    {4, 5},
    {4, 6},
    {4, 7},
    {4, 8}
  };
  vector<SnakeSegment>::iterator tail = segments.end();
  vector<SnakeSegment>::iterator head = segments.begin();
  vector<SnakeSegment>::iterator seg = segments.begin();
	int size = segments.size();

  void RotaitTail(vector<SnakeSegment>::iterator& it, sf::Sprite& sprite) {
    it->segment = ESegment::Tail;
    if (it->direction == EDirection::Left) {
      sprite.setOrigin(0.f, 0.f);
      sprite.setRotation(0.f);
    }
    else if (it->direction == EDirection::Up) {
      sprite.setOrigin(0.f, 54.f);
      sprite.setRotation(90.f);
    }
    else if (it->direction == EDirection::Right) {
      sprite.setOrigin(64.f, 64.f);
      sprite.setRotation(180.f);
    }
    else if (it->direction == EDirection::Down) {
      sprite.setOrigin(64.f, -4.f);
      sprite.setRotation(-90.f);
    }
  }

  void CheckMove(vector<SnakeSegment>::iterator& it) {
    if ((snake.head->direction == EDirection::Up && snake.seg->direction == EDirection::Right) ||
       (snake.head->direction == EDirection::Left && snake.seg->direction == EDirection::Down)) {
      it->segment = ESegment::Down_Left;
    }
    else if ((snake.head->direction == EDirection::Down && snake.seg->direction == EDirection::Right) ||
            (snake.head->direction == EDirection::Left && snake.seg->direction == EDirection::Up)) {
      it->segment = ESegment::Up_Left;
    }
    else if ((snake.head->direction == EDirection::Up && snake.seg->direction == EDirection::Left) ||
            (snake.head->direction == EDirection::Right && snake.seg->direction == EDirection::Down)) {
      it->segment = ESegment::Down_Right;
    }
    else if ((snake.head->direction == EDirection::Down && snake.seg->direction == EDirection::Left) ||
            (snake.head->direction == EDirection::Right && snake.seg->direction == EDirection::Up)) {
      it->segment = ESegment::Up_Right;
    }
    else if (snake.head->direction == EDirection::Left && snake.seg->direction == EDirection::Left) {
      it->segment = ESegment::Left_Right;
    }
    else if (snake.head->direction == EDirection::Right && snake.seg->direction == EDirection::Right) {
      it->segment = ESegment::Left_Right;
    }
    else if ((snake.head->direction == EDirection::Up && snake.seg->direction == EDirection::Up) || (snake.head->direction == EDirection::Down && snake.seg->direction == EDirection::Down)) {
      it->segment = ESegment::Up_Down;
    }
  }

  void Draw()
  {
    tail = segments.end();
    head = segments.begin();
    seg = segments.begin();
    tail--;
    seg++;
    for (vector<SnakeSegment>::iterator it = segments.begin(); it != segments.end(); ++it) {
      if (it->col == head->col && it->row == head->row) {
        head->segment = ESegment::Head;
      }
      else if (it->col == seg->col && it->row == seg->row) {
        --it;
        snake.CheckMove(it);
        ++it;
      }
      snake.RotaitTail(tail, tailSprite);
      it->Draw();
    }

    /*true
    for (SnakeSegment& snakSegm : segments) {
      if (snakSegm.col == head->col && snakSegm.row == head->row) {
        snakSegm.segment = ESegment::Head;
      }
      else {
        snakSegm.segment = ESegment::SnakeSprite;
      }
      snakSegm.Draw();
    }*/
  }

	// TODO: Implement snake movement
	void Move() {
    for (int i = size - 1; i > 0; i--)
    {
      segments[i] = segments[i - 1];
    }
    head->Move();
	}

  void AddSize() {
    size++;
    --tail;
    segments.emplace(tail, *tail);
    tail = segments.end();
    head = segments.begin();
    cout << "Segments size " << segments.size() << endl;
  }

  bool CheckCollision() {
    for (int i = size - 1; i > 0; i--)
    {
      if (head->col == segments[i].col && head->row == segments[i].row) {
        return true;
      }
    }
    return false;
  }
} snake;

struct Fruit {
  Fruit(int _col, int _row) :
    col(_col),
    row(_row)
  {
  }

  void Draw() {
    DrawSprite(appleSprite, col, row);
  }

  int col;
  int row;
};

struct Apple {
  Fruit* new_fruit = new Fruit(3,2);

  void Draw() {
    apple.new_fruit->Draw();
  }

  void SpawnApple() {
    new_fruit = nullptr;
    new_fruit = new Fruit(rand() % (GAME_FIELD_SIZE_V - 1), rand() % (GAME_FIELD_SIZE_H - 1));
  }

  //Checking for eating an apple with a head and Check for the appearance of apples on the snake 
  bool Check(const Snake& snake)const {
  return snake.head->col == new_fruit->col && snake.head->row == new_fruit->row;
  }
  bool CheckSnake(const vector<SnakeSegment>& segment) const {
    for (auto seg : segment) {
      if (new_fruit->col == seg.col && new_fruit->row == seg.row) {
        return true;
      }
    }
    return false;
  }

  void AppleEat() {
  if (apple.Check(snake)) {
    cout << "eat apple" << endl;
    apple.SpawnApple();
    snake.AddSize();
    addMoveTimer += 1;
    }
  }
} apple;


int main()
{
	Init();

  srand(unsigned(time(NULL)));

	bool isRunning = true;

	while (isRunning)
	{
		std::chrono::milliseconds newTime = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());
		elapsedTime = newTime - timer;
		timer = newTime;

		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();

      HandleKeyBoardEvent();
		}

		Update(elapsedTime.count());

    SpawnApple();

		Draw();

		window.display();
	}

	return 0;
}

void SetupWindow()
{
  window.setFramerateLimit(60);
}

//We take the texture from PNG
void InitSprites()
{
  if (!textureTerrain.loadFromFile("snakeset.png")) {
    std::cerr << "Can't load texture" << endl;
    exit(1);
  }
  treeSprite.setTexture(textureTerrain);
  treeSprite.setTextureRect(sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));

  grassSprite.setTexture(textureTerrain);
  grassSprite.setTextureRect(sf::IntRect(0, TILE_SIZE, TILE_SIZE, TILE_SIZE));

  snakeSprite.setTexture(textureTerrain);
  snakeSprite.setTextureRect(sf::IntRect(2 * TILE_SIZE, 2 * TILE_SIZE, TILE_SIZE, TILE_SIZE));

  appleSprite.setTexture(textureTerrain);
  appleSprite.setTextureRect(sf::IntRect(3 * TILE_SIZE, 2 * TILE_SIZE, TILE_SIZE, TILE_SIZE));

  headSprite.setTexture(textureTerrain);
  headSprite.setTextureRect(sf::IntRect(1 * TILE_SIZE, 2 * TILE_SIZE, TILE_SIZE, TILE_SIZE));

  tailSprite.setTexture(textureTerrain);
  tailSprite.setTextureRect(sf::IntRect(0, 2 * TILE_SIZE, TILE_SIZE, TILE_SIZE));

  UpDownSprite.setTexture(textureTerrain);
  UpDownSprite.setTextureRect(sf::IntRect(1 * TILE_SIZE, 1 * TILE_SIZE, TILE_SIZE, TILE_SIZE));

  LeftRightSprite.setTexture(textureTerrain);
  LeftRightSprite.setTextureRect(sf::IntRect(2*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));

  UpRightSprite.setTexture(textureTerrain);
  UpRightSprite.setTextureRect(sf::IntRect(TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));

  UpLeftSprite.setTexture(textureTerrain);
  UpLeftSprite.setTextureRect(sf::IntRect(3 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));

  DownLeftSprite.setTexture(textureTerrain);
  DownLeftSprite.setTextureRect(sf::IntRect(3 * TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE));

  DownRightSprite.setTexture(textureTerrain);
  DownRightSprite.setTextureRect(sf::IntRect(2 * TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE));
}

//Drawing the playing field
void InitGameField()
{
  // Tile codes in gameField array:
  // '#' char means tree
  // 's' char means snake
  // ' ' empty tile, draws it as grass
  // 'a' char means apple

  for (int row = 0; row < GAME_FIELD_SIZE_V; row++)
  {
    for (int col = 0; col < GAME_FIELD_SIZE_H; col++)
    {
      if (col == 0 || col == GAME_FIELD_SIZE_H - 1
        || row == 0 || row == GAME_FIELD_SIZE_V - 1)
      {
        gameField[row][col] = '#';
      }
      else
      {// ' ' empty tile, draws it as grass
        gameField[row][col] = ' ';
      }
    }
  }
}

void InitTimer()
{
  timer = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());
}

void Init()
{
  SetupWindow();

  InitSprites();

  InitGameField();

  InitTimer();
}

//Keyboard controls for the snake
void HandleKeyBoardEvent() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
  {
    snake.head->direction = EDirection::Down;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
  {
    snake.head->direction = EDirection::Up;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
  {
    snake.head->direction = EDirection::Left;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
  {
    snake.head->direction = EDirection::Right;
  }
}

//snake speeding up when eating an apple
void Update(int dt)
{
  snakeMoveTimer -= addMoveTimer;
	snakeMoveTimer -= dt;

	if (snakeMoveTimer <= 0)
	{
		MoveSnake();
		snakeMoveTimer = SNAKE_MOVE_TIME;
	}
}

//what happens to a snake when interacting with an object
void MoveSnake()
{
	// Crash to the tree
	if (gameField[snake.head->row][snake.head->col] == '#')
	{
		cout << "You loose!" << endl;
		exit(1);
  }
  if (snake.CheckCollision())
  {
    cout << "You loose! The snake collided the body" << endl;
    exit(1);
  }
  
	snake.Move();
}

//Checking fruit formation in the field. fruit eating test
void SpawnApple() {
  if (gameField[apple.new_fruit->row][apple.new_fruit->col] == '#') {
    apple.SpawnApple();
  }

  apple.AppleEat();

  if (apple.CheckSnake(snake.segments)) {
    apple.SpawnApple();
  }
}

void Draw()
{
	DrawGameField();

	snake.Draw();

  apple.new_fruit->Draw();
}

//Assigning symbols to sprites
void DrawGameField()
{
	for (int row = 0; row < 15; row++)
	{
		for (int col = 0; col < 15; col++)
		{
			char tile = gameField[row][col];

			if (tile == ' ')
			{
				grassSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
				window.draw(grassSprite);
			}
			else if (tile == '#')
			{
				treeSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
				window.draw(treeSprite);
			}
			else if (tile == 's')
			{
				grassSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
				window.draw(grassSprite);

				snakeSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
				window.draw(snakeSprite);
			}
      else if(tile == 'a')
      {
        appleSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
        window.draw(appleSprite);
      }
		}
	}
}

void DrawSprite(sf::Sprite& sprite, int col, int row)
{
	sprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
	window.draw(sprite);
}

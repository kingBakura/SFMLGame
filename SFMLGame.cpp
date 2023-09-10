//  includes
#include "SFML-2.6.0\include\SFML\Graphics.hpp"
#include "SFML-2.6.0\include\SFML\Audio.hpp"
#include <vector>

//  defines
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

//  classes
class Hero {
    public:
        Hero();
        ~Hero();
        void init(std::string textureName, int frameCount, float animDuration, sf::Vector2f position, float mass);
        void update(float dt);
        void jump(float velocity);
        sf::Sprite getSprite(void);

    private:
        sf::Texture mTexture;
        sf::Sprite mSprite;
        sf::Vector2f mPosition;
        int jumpCount = 0;
        float mMass;
        float mVelocity;
        const float mGravity = 9.80f;
        bool mGrounded;
        int mFrameCount;
        float mAnimDuration;
        float mElapsedTime;
        sf::Vector2i mSpriteSize;
};

Hero::Hero() { }

Hero::~Hero() { }

void Hero::init(std::string textureName, int frameCount, float animDuration, sf::Vector2f position, float mass)
{
    mPosition = position;
    mMass = mass;
    mGrounded = false;
    mFrameCount = frameCount;
    mAnimDuration = animDuration;
    mTexture.loadFromFile(textureName.c_str());
    mSpriteSize = sf::Vector2i(92, 126);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, mSpriteSize.x, mSpriteSize.y));
    mSprite.setPosition(mPosition);
    mSprite.setOrigin(mTexture.getSize().x / 2, mTexture.getSize().y / 2);

}

void Hero::update(float dt)
{
    mElapsedTime += dt;
    int animFrame = static_cast<int> ((mElapsedTime / mAnimDuration) * mFrameCount) % mFrameCount;
    mSprite.setTextureRect(sf::IntRect(animFrame * mSpriteSize.x, 0, mSpriteSize.x, mSpriteSize.y));

    mVelocity -= mMass * mGravity * dt;
    mPosition.y -= mVelocity * dt;
    mSprite.setPosition(mPosition);

    if (mPosition.y >= WINDOW_HEIGHT * 0.75f) {
        mPosition.y = WINDOW_HEIGHT * 0.75f;
        mVelocity = 0.0f;
        mGrounded = true;
        jumpCount = 0;
    }
}

void Hero::jump(float velocity)
{
    if (jumpCount < 2) {
        jumpCount++;
        mVelocity = velocity;
        mGrounded = false;
    }
}

sf::Sprite Hero::getSprite(void)
{
    return mSprite;
}

class Enemy {
    public:
        Enemy();
        ~Enemy();
        void init(std::string textureName, sf::Vector2f position, float speed);
        void update(float dt);
        sf::Sprite getSprite(void);

    private:
        sf::Texture mTexture;
        sf::Sprite mSprite;
        sf::Vector2f mPosition;
        float mSpeed;
};

Enemy::Enemy() { }

Enemy::~Enemy() { }

void Enemy::init(std::string textureName, sf::Vector2f position, float speed)
{
    mSpeed = speed;
    mPosition = position;
    mTexture.loadFromFile(textureName.c_str());
    mSprite.setTexture(mTexture);
    mSprite.setPosition(mPosition);
    mSprite.setOrigin(mTexture.getSize().x / 2, mTexture.getSize().y / 2);
}

void Enemy::update(float dt)
{
    mSprite.move(mSpeed * dt, 0);
}

sf::Sprite Enemy::getSprite(void)
{
    return mSprite;
}

class Rocket {
    public:
        Rocket();
        ~Rocket();
        void init(std::string textureName, sf::Vector2f position, float speed);
        void update(float dt);
        sf::Sprite getSprite(void);

    private:
        sf::Texture mTexture;
        sf::Sprite mSprite;
        sf::Vector2f mPosition;
        float mSpeed;
};

Rocket::Rocket() { }

Rocket::~Rocket() { }

void Rocket::init(std::string textureName, sf::Vector2f position, float speed)
{
    mSpeed = speed;
    mPosition = position;
    mTexture.loadFromFile(textureName.c_str());
    mSprite.setTexture(mTexture);
    mSprite.setPosition(mPosition);
    mSprite.setOrigin(mTexture.getSize().x / 2, mTexture.getSize().y / 2);
}

void Rocket::update(float dt)
{
    mSprite.move(mSpeed * dt, 0);
}

sf::Sprite Rocket::getSprite(void)
{
    return mSprite;
}

//  globals
sf::Vector2f viewSize(WINDOW_WIDTH, WINDOW_HEIGHT);
sf::VideoMode videoMode(viewSize.x, viewSize.y);
sf::RenderWindow window(videoMode, "SFML", sf::Style::Default);
sf::Texture skyTexture;
sf::Sprite skySprite;
sf::Texture bgTexture;
sf::Sprite bgSprite;
sf::Font headingFont;
sf::Text headingText;
sf::Font scoreFont;
sf::Text scoreText;
sf::Text tutorialText;
sf::Music bgMusic;
sf::SoundBuffer fireBuffer;
sf::SoundBuffer hitBuffer;
sf::Sound fireSound(fireBuffer);
sf::Sound hitSound(hitBuffer);
std::vector<Enemy*> enemies;
std::vector<Rocket*> rockets;
Hero hero;
float currentTime;
float prevTime = 0.0f;
int score = 0;
bool gameOver = true;

//  prototypes
void init(void);
void draw(void);
void updateInput(void);
void update(float dt);
void spawnEnemy(void);
void shootRocket(void);
bool checkCollision(sf::Sprite sprite1, sf::Sprite sprite2);
void reset(void);

//  functions
void init(void)
{
    skyTexture.loadFromFile("Assets/graphics/sky.png");
    skySprite.setTexture(skyTexture);

    bgTexture.loadFromFile("Assets/graphics/bg.png");
    bgSprite.setTexture(bgTexture);

    hero.init("Assets/graphics/heroAnim.png", 4, 1.0f, sf::Vector2f(viewSize.x * 0.25f, viewSize.y * 0.5f), 200);

    srand((int) time(0));

    headingFont.loadFromFile("Assets/fonts/SnackerComic.ttf");
    headingText.setFont(headingFont);
    headingText.setString("Tiny Bazooka");
    headingText.setCharacterSize(84);
    headingText.setFillColor(sf::Color::Red);
    sf::FloatRect headingBounds = headingText.getLocalBounds();
    headingText.setOrigin(headingBounds.width/2, headingBounds.height / 2);
    headingText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));

    scoreFont.loadFromFile("Assets/fonts/arial.ttf");
    scoreText.setFont(scoreFont);
    scoreText.setString("Score: 0");
    scoreText.setCharacterSize(45);
    scoreText.setFillColor(sf::Color::Red);
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
    scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));

    tutorialText.setFont(scoreFont);
    tutorialText.setString("Press Space Key to Fire and Start Game, Up Arrow to Jump");
    tutorialText.setCharacterSize(35);
    tutorialText.setFillColor(sf::Color::Red);
    sf::FloatRect tutorialBounds = tutorialText.getLocalBounds();
    tutorialText.setOrigin(tutorialBounds.width / 2, tutorialBounds.height / 2);
    tutorialText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.20f));

    bgMusic.openFromFile("Assets/audio/bgMusic.ogg");
    bgMusic.play();

    hitBuffer.loadFromFile("Assets/audio/hit.ogg");
    fireBuffer.loadFromFile("Assets/audio/fire.ogg");
}

void draw(void) 
{
    window.draw(skySprite);
    window.draw(bgSprite);
    window.draw(hero.getSprite());

    for (Enemy *enemy : enemies) {
        window.draw(enemy->getSprite());
    }

    for (Rocket *rocket : rockets) {
        window.draw(rocket->getSprite());
    }

    if (gameOver) {
        window.draw(headingText);
        window.draw(tutorialText);
    } else {
        window.draw(scoreText);
    }
}

void updateInput(void)
{
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                hero.jump(750.0f);
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                if (gameOver) {
                    gameOver = false;
                    reset();
                } else {
                    shootRocket();
                }
            }
        }
    }
}

void update(float dt)
{
    hero.update(dt);
    currentTime += dt;

    if (currentTime >= prevTime + 1.125f) {
        spawnEnemy();
        prevTime = currentTime;
    }

    for (int i = 0; i < enemies.size(); i++) {
        Enemy *enemy = enemies[i];
        enemy->update(dt);

        if (enemy->getSprite().getPosition().x < 0) {
            enemies.erase(enemies.begin() + i);
            delete(enemy);
            gameOver = true;
        }
    }

    for (int i = 0; i < rockets.size(); i++) {
        Rocket *rocket = rockets[i];
        rocket->update(dt);

        if (rocket->getSprite().getPosition().x > viewSize.x) {
            rockets.erase(rockets.begin() + i);
            delete(rocket);
        }
    }

    for (int i = 0; i < rockets.size(); i++) {
        for (int j = 0; j < enemies.size(); j++) {
            Rocket *rocket = rockets[i];
            Enemy *enemy = enemies[j];

            if (checkCollision(rocket->getSprite(), enemy->getSprite())) {
                rockets.erase(rockets.begin() + i);
                enemies.erase(enemies.begin() + j);
                delete(rocket);
                delete(enemy);
                hitSound.play();

                score++;
                std::string finalScore = "Score: " + std::to_string(score);
                scoreText.setString(finalScore);
                sf::FloatRect scoreBounds = scoreText.getLocalBounds();
                scoreText.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
                scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));

                printf("rocket intersects enemy\n");
            }
        }

    }
}

void spawnEnemy(void)
{
    int randLoc = rand() % 3;
    sf::Vector2f enemyPos;
    float speed;

    switch (randLoc) {
        case 0 :
            enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.75f);
            speed = -400;
            break;

        case 1 :
            enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.60f);
            speed = -550;
            break;

        case 2 :
            enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.40f);
            speed = -650;
            break;

        default :
            fprintf(stderr, "Incorrect y value.\n");
            return;
    }

    Enemy *enemy = new Enemy();
    enemy->init("Assets/graphics/enemy.png", enemyPos, speed);
    enemies.push_back(enemy);
}

void shootRocket(void)
{
    Rocket *rocket = new Rocket();
    rocket->init("Assets/graphics/rocket.png", hero.getSprite().getPosition(), 400.0f);
    rockets.push_back(rocket);
    fireSound.play();
}

bool checkCollision(sf::Sprite sprite1, sf::Sprite sprite2)
{
    sf::FloatRect shape1 = sprite1.getGlobalBounds();
    sf::FloatRect shape2 = sprite2.getGlobalBounds();

    if (shape1.intersects(shape2)) {
        return true;
    } else {
        return false;
    }
}

void reset(void)
{
    score = 0;
    currentTime = 0.0f;
    prevTime = 0.0f;

    for (Enemy *enemy : enemies) {
        delete(enemy);
    }

    for (Rocket *rocket : rockets) {
        delete(rocket);
    }

    enemies.clear();
    rockets.clear();

    scoreText.setString("Score: 0");
}

int main(void)
{
    sf::Clock clock;
    window.setFramerateLimit(60);

    init();

    while (window.isOpen()) {
        updateInput();
        sf::Time dt = clock.restart();

        if (!gameOver) {
            update(dt.asSeconds());
        }

        window.clear(sf::Color::Red);
        draw();
        window.display();
    }
    
    return 0;
}
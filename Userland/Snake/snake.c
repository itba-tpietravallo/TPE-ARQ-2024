#include <sys.h>
#include <stdio.h>
#include <stdlib.h>


// <------------------------------ DEFINES ------------------------------>

#define INITIAL_BODY_SIZE 3
#define MAX_BODY_SIZE 100     // to check if the player won
#define SQUARE_DIM 32
#define MAX_PLAYERS 2
#define HEAD 0
#define DEFAULT_GAME_SLEEPING_TIME 333
#define DEFAULT_WINNERS_SLEEPING_TIME 5000

// hues
#define HUE_1 120
#define HUE_2 0

// initial positions
#define INITIAL_POS_1_X (SQUARE_DIM * 28)
#define INITIAL_POS_1_Y (SQUARE_DIM * 21)
#define INITIAL_POS_2_X (SQUARE_DIM * 3)
#define INITIAL_POS_2_Y (SQUARE_DIM * 2)

// initial directions
#define INITIAL_DIR_1_X -1
#define INITIAL_DIR_1_Y 0
#define INITIAL_DIR_2_X 1
#define INITIAL_DIR_2_Y 0

// drawing
#define DEFAULT_BACKGROUND_COLOR 0xE1F5FE
#define OFFSET 4


// <------------------------------ DATA TYPES ------------------------------>

typedef struct {
    int width;
    int height;
} Square;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int x;
    int y;
} Direction;

typedef struct {
    Position position;
} SnakeBodySquare;

typedef struct {
    int hue;
    Direction direction;
    int size;
    SnakeBodySquare body[MAX_BODY_SIZE];
    int crashed;
} SnakeHeadSquare;

typedef struct {
    int hue;
    Position position;
} Food;


// <-------------------------- FUNCTION PROTOTYPES -------------------------->

static void registerKeys(void);

static void movingTo(int snake, int dir_x, int dir_y);
static void setDirection(enum REGISTERABLE_KEYS scancode);
static void setSquareDimensions(void);
static void setDefaultFeatures(void);

static void drawBackground(void);
static void drawSnakes(void);

static void moveSnakes(void);
static void drawFood(void);
static void randomizeFoodPosition(void);
static void checkFoodEaten(void);
static int overSnakeBody(int x, int y);

static void checkCrash(void);

static void endGame(void);
static void showWinners(void);

static uint32_t hsv2rgb(uint8_t h, uint8_t s, uint8_t v);


// <------------------------------ STATIC VARS ------------------------------>

static SnakeHeadSquare snakes[MAX_PLAYERS] = {};
static Square square;
static Food food;

// hues
static int hues[] = {HUE_1, HUE_2};

// initial positions
static Position initial_positions[MAX_PLAYERS] = { { .x = INITIAL_POS_1_X, .y = INITIAL_POS_1_Y } , { .x = INITIAL_POS_2_X, .y = INITIAL_POS_2_Y } };

// initial directions
static Direction initial_directions[MAX_PLAYERS] = { { .x = INITIAL_DIR_1_X, .y = INITIAL_DIR_1_Y } , { .x = INITIAL_DIR_2_X, .y = INITIAL_DIR_2_Y } };

// screen dimensions
static int window_height, window_width;

static int end_of_game;
static int snakes_amount;
static int food_eaten;




int main(void) {
    end_of_game = 0;

    snakes_amount = 1; //TODO scanf should get this

    int hours, minutes, seconds;
    getDate(&hours, &minutes, &seconds);
    srand(seconds);

    window_height = getWindowHeight();
    window_width = getWindowWidth();
    food_eaten = 0;

    registerKeys();
    setSquareDimensions();
    setDefaultFeatures();

    
    while(!end_of_game) {

        drawBackground();
        drawSnakes();
        drawFood();

        
        
        //=====================================================================================================
        sleep(DEFAULT_GAME_SLEEPING_TIME);
        //=====================================================================================================

        moveSnakes();

        checkFoodEaten();

        checkCrash();
        
    }
    
    showWinners();

    return 0;
}

static void endGame(void) {
    end_of_game = 1;
}

static void registerKeys(void) {
    // player 1 keys
    registerKey(KP_UP_KEY, setDirection);
    registerKey(KP_DOWN_KEY, setDirection);
    registerKey(KP_LEFT_KEY, setDirection);
    registerKey(KP_RIGHT_KEY, setDirection);

    // player 2 keys
    registerKey(W_KEY, setDirection);
    registerKey(S_KEY, setDirection);
    registerKey(A_KEY, setDirection);
    registerKey(D_KEY, setDirection);

    registerKey(X_KEY, (void (*)(enum REGISTERABLE_KEYS scancode)) endGame);
}

static void movingTo(int snake, int dir_x, int dir_y) {
    // changes direction only if the new isn't the opposite of the current
    if((dir_x != (snakes[snake - 1].direction.x * (-1)) && snakes[snake - 1].direction.y == 0) || (snakes[snake - 1].direction.x == 0 && dir_y != (snakes[snake - 1].direction.y * (-1)))){
        snakes[snake - 1].direction.x = dir_x;
        snakes[snake - 1].direction.y = dir_y;
    }
}

static void setDirection(enum REGISTERABLE_KEYS scancode) {
    switch (scancode) {
        case KP_UP_KEY:
            movingTo(1, 0, -1);
            break;
        case KP_DOWN_KEY:
            movingTo(1, 0, 1);
            break;
        case KP_LEFT_KEY:
            movingTo(1, -1, 0);
            break;
        case KP_RIGHT_KEY:
            movingTo(1, 1, 0);
            break;
        
        case W_KEY:
            movingTo(2, 0, -1);
            break;
        case S_KEY:
            movingTo(2, 0, 1);
            break;
        case A_KEY:
            movingTo(2, -1, 0);
            break;
        case D_KEY:
            movingTo(2, 1, 0);
            break;
        default:
            break;
    }
}

static void setSquareDimensions(void) {
    square.width = SQUARE_DIM;
    square.height = SQUARE_DIM;
}

static void setDefaultFeatures(void) {
    for(int i = 0; i < snakes_amount; i++){
        snakes[i].hue = hues[i];
        snakes[i].direction.x = initial_directions[i].x;
        snakes[i].direction.y = initial_directions[i].y;
        snakes[i].size = INITIAL_BODY_SIZE;
        for(int k = 0; k < INITIAL_BODY_SIZE; k++){
            snakes[i].body[k].position.x = initial_positions[i].x - k * snakes[i].direction.x * SQUARE_DIM;
            snakes[i].body[k].position.y = initial_positions[i].y - k * snakes[i].direction.y * SQUARE_DIM;
        }
        snakes[i].crashed = 0;
    }
    food.hue = 0xA933DC;
    randomizeFoodPosition();

}

static void drawBackground(void) {
    for(int y = 0; y < window_height; y += square.height){
        for(int x = 0; x < window_width; x += square.width){
            drawRectangle(DEFAULT_BACKGROUND_COLOR, square.width - OFFSET, square.height - OFFSET, x + OFFSET, y + OFFSET);
        }
    }
}

static void drawSnakes(void) {
    for(int i = 0; i < snakes_amount; i++){
        for(int k = 0; k < snakes[i].size; k++){
            drawRectangle(hsv2rgb(k * 10 + snakes[i].hue, 255, 255), square.width - OFFSET, square.height - OFFSET, snakes[i].body[k].position.x + OFFSET, snakes[i].body[k].position.y + OFFSET);
        }
    }
}

static void moveSnakes(void) {
    for(int i = 0; i < snakes_amount; i++){
        // move each square, except head
        for(int k = snakes[i].size - 1; k > 0; k--){
            snakes[i].body[k].position.x = snakes[i].body[k - 1].position.x;
            snakes[i].body[k].position.y = snakes[i].body[k - 1].position.y;
        }

        // move head
        snakes[i].body[HEAD].position.x += snakes[i].direction.x * SQUARE_DIM;
        snakes[i].body[HEAD].position.y += snakes[i].direction.y * SQUARE_DIM;
    }
}

static void checkCrash(void) {
    for(int i = 0; i < snakes_amount; i++){
        // checks whether the snake crashed with a border
        if(snakes[i].body[0].position.x < 0 || snakes[i].body[0].position.x > (window_width - SQUARE_DIM) || snakes[i].body[0].position.y < 0 || snakes[i].body[0].position.y > (window_height - SQUARE_DIM)){
            snakes[i].crashed = 1;
            endGame();
        }

        // checks whether the snake crashed with itself
        for(int k = 1; k < snakes[i].size && !end_of_game; k++){
            if(snakes[i].body[0].position.x == snakes[i].body[k].position.x && snakes[i].body[0].position.y == snakes[i].body[k].position.y){
                snakes[i].crashed = 1;
                endGame();
            }
        }

        // checks whether snakes crashed with each other
        for(int k = i + 1; k < snakes_amount; k++){
            for(int j = 0; j < snakes[k].size; j++){
                if(snakes[i].body[0].position.x == snakes[k].body[j].position.x && snakes[i].body[0].position.y == snakes[k].body[j].position.y){
                    snakes[i].crashed = 1;
                    // if both heads crash, both players lose
                    if(j == 0){
                        snakes[k].crashed = 1;
                    }
                    endGame();
                }
            }
        }
    }
}

static void showWinners(void) {
    clearScreen();

    if(snakes_amount == 1){
        if(!snakes[0].crashed){
            printf("You won!\n");
        } else{
            printf("You lost.\n");
        }
    } else{
        for(int i = 0; i < snakes_amount; i++){
            if(!snakes[i].crashed){
                printf("Player %d won!\n", i + 1);
            } else{
                printf("Player %d lost.\n", i + 1);
            }
            printf("\n");
        }
    }

    sleep(DEFAULT_WINNERS_SLEEPING_TIME);
}

static int overSnakeBody(int x, int y) {

    int overSnake = 0;

    for (int i = 0 ; i < snakes_amount && !overSnake ; i++) {

         for(int k = 0; k < snakes[i].size && !overSnake ; k++){

           if (snakes[i].body[k].position.x == x && snakes[i].body[k].position.y == y) {
                overSnake = 1;
           }

        }
    }
    return overSnake;
}

static void randomizeFoodPosition(void){
    int x, y;

    do {

        x = (rand() % SQUARE_DIM) * SQUARE_DIM;
        x = x % (getWindowWidth());

        y = (rand() % SQUARE_DIM) * SQUARE_DIM;
        y = y % (getWindowHeight());

    } while (overSnakeBody(x, y));

    food.position.x = x;
    food.position.y = y;
}

static void drawFood() {

    drawCircle(food.hue, food.position.x + OFFSET, food.position.y + OFFSET, SQUARE_DIM - OFFSET);
}

static void checkFoodEaten(void) {

   for (int i = 0 ; i < snakes_amount; i++) {
        if (snakes[i].body[HEAD].position.x == food.position.x && snakes[i].body[HEAD].position.y == food.position.y) {

            snakes[i].size++;
            food_eaten = 1;
        }
   }

   if (food_eaten) {
        randomizeFoodPosition();
        food_eaten = 0;
    }
}


// <------------------------------ COLORED SNAKE LOGIC ------------------------------>

// Hand transpiled from https://github.com/hughsk/glsl-hsv2rgb/blob/master/index.glsl
#define clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define abs(a) ((a) < 0 ? -(a) : (a))
#define fract256(x) ((x) % 256)
// Hand transpiled from https://github.com/hughsk/glsl-hsv2rgb/blob/master/index.glsl
static uint32_t hsv2rgb(uint8_t h, uint8_t s, uint8_t v) {
//   vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//   vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//   return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
    uint16_t pr = abs(fract256( h ) * 6 - 255*3);
    uint16_t pg = abs(fract256( h + (85*2) ) * 6 - 255*3);
    uint16_t pb = abs(fract256( h + 85 ) * 6 - 255*3);
    return (((v * clamp(pr-256, 0, 255)) / 255) << 16) | (((v * clamp(pg-256, 0, 255)) / 255) << 8) | (v * clamp(pb-256, 0, 255)) / 255;
}
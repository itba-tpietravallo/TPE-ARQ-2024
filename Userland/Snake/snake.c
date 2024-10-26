#include <sys.h>
#include <stdio.h>

#define INITIAL_BODY_SIZE 3
#define MAX_BODY_SIZE 100     // to check if the player won
#define SQUARE_DIM 32
#define INITIAL_POS_X (SQUARE_DIM * INITIAL_BODY_SIZE)
#define INITIAL_POS_Y (SQUARE_DIM * 2)
#define OFFSET 4

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
    int color;
    Direction direction;
    int size;
    SnakeBodySquare body[INITIAL_BODY_SIZE];
} SnakeHeadSquare;

static void registerKeys(void);
static void movingTo(int dir_x, int dir_y);
static void setDirection(enum REGISTERABLE_KEYS scancode);
static void setSquareDimensions(void);
static void setDefaultFeatures(void);
static void drawBackground(void);
static void endGame(void);
static void checkCrash(void);

static uint32_t hsv2rgb(uint8_t h, uint8_t s, uint8_t v);

static SnakeHeadSquare head;
static Square square;

// screen dimensions
static int window_height, window_width;

static int crashed;

int main(void) {
    window_height = getWindowHeight();
    window_width = getWindowWidth();

    registerKeys();
    setSquareDimensions();
    setDefaultFeatures();
    
    crashed = 0;

    int rounds = 10;

    while(!crashed) {
        drawBackground();

        // print snake
        for(int k = 0; k < head.size; k++){
            drawRectangle(hsv2rgb(k * 10, 255, 255), square.width - OFFSET, square.height - OFFSET, head.body[k].position.x + OFFSET, head.body[k].position.y + OFFSET);
        }

        sleep(1000);

        // move each square, except head
        for(int i = head.size - 1; i > 0; i--){
            head.body[i].position.x = head.body[i - 1].position.x;
            head.body[i].position.y = head.body[i - 1].position.y;
        }

        // move head
        head.body[0].position.x += head.direction.x * SQUARE_DIM;
        head.body[0].position.y += head.direction.y * SQUARE_DIM;

        checkCrash();

        rounds--;
        if(rounds <= 0){
            endGame();
        }
    }

    return 0;
}

static void endGame(void) {
    crashed = 1;
}

static void registerKeys(void) {
    registerKey(KP_UP_KEY, setDirection);
    registerKey(KP_DOWN_KEY, setDirection);
    registerKey(KP_LEFT_KEY, setDirection);
    registerKey(KP_RIGHT_KEY, setDirection);
    registerKey(X_KEY, (void (*)(enum REGISTERABLE_KEYS scancode)) endGame);
}

static void movingTo(int dir_x, int dir_y) {
    if((dir_x != (head.direction.x * (-1)) && head.direction.y == 0) || (head.direction.x == 0 && dir_y != (head.direction.y * (-1)))){
        head.direction.x = dir_x;
        head.direction.y = dir_y;
    }
}

static void setDirection(enum REGISTERABLE_KEYS scancode) {
    switch (scancode) {
    case KP_UP_KEY:
        movingTo(0, -1);
        break;
    case KP_DOWN_KEY:
        movingTo(0, 1);
        break;
    case KP_LEFT_KEY:
        movingTo(-1, 0);
        break;
    case KP_RIGHT_KEY:
        movingTo(1, 0);
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
    head.color = 0xFFFFFFFF;
    head.direction.x = 1;
    head.direction.y = 0;
    head.size = INITIAL_BODY_SIZE;
    for(int i = 0; i < INITIAL_BODY_SIZE; i++){
        head.body[i].position.x = INITIAL_POS_X - i * head.direction.x * SQUARE_DIM;
        head.body[i].position.y = INITIAL_POS_Y - i * head.direction.y * SQUARE_DIM;
    }
}

static void drawBackground(void) {
    for(int y = 0; y < window_height; y += square.height){
        for(int x = 0; x < window_width; x += square.width){
            drawRectangle(0x4DB6AC, square.width - OFFSET, square.height - OFFSET, x + OFFSET, y + OFFSET);
        }
    }
}

static void checkCrash(void) {
    // checks whether the snake crashed with a border
    if(head.body[0].position.x < 0 || head.body[0].position.x > window_width || head.body[0].position.y < 0 || head.body[0].position.y > window_height){
        endGame();
    }

    // checks whether the snake crashed with itself
    for(int i = 1; i < head.size && !crashed; i++){
        if(head.body[0].position.x == head.body[i].position.x && head.body[0].position.y == head.body[i].position.y){
            endGame();
        }
    }
}

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
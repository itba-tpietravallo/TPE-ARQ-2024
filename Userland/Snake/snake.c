#include <sys.h>
#include <stdio.h>

#define INITIAL_BODY_SIZE 3
#define MAX_BODY_SIZE 100     // to check if the player won
#define INITIAL_POS_X 128
#define INITIAL_POS_Y 64
#define OFFSET 4
#define SQUARE_DIM 32

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
static void endGame(void);

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
        //TODO use fillVideoMemory() instead of painting the background
        // print backgruond
        for(int y = 0; y < window_height; y += square.height){
            for(int x = 0; x < window_width; x += square.width){
                drawRectangle(0x4DB6AC, square.width - OFFSET, square.height - OFFSET, x + OFFSET, y + OFFSET);
            }
        }

        // print snake
        for(int k = 0; k < head.size; k++){
            drawRectangle(head.color, square.width - OFFSET, square.height - OFFSET, head.body[k].position.x + OFFSET, head.body[k].position.y + OFFSET);
        }

        if (!crashed) sleep(2000);

        // move each square, except head
        for(int i = head.size - 1; i > 0; i--){
            head.body[i].position.x = head.body[i - 1].position.x;
            head.body[i].position.y = head.body[i - 1].position.y;
        }

        // move head
        head.body[0].position.x += head.direction.x * SQUARE_DIM;
        head.body[0].position.y += head.direction.y * SQUARE_DIM;

        // check crash
        for(int i = 1; i < head.size && !crashed; i++){
            if(head.body[0].position.x == head.body[i].position.x && head.body[0].position.y == head.body[i].position.y){
                crashed = 1;
            }
        }

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

// Hand transpiled from https://github.com/hughsk/glsl-hsv2rgb/blob/master/index.glsl
#define clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define abs(a) ((a) < 0.0 ? -(a) : (a))
#define fract(x) ((x) - ((int) (x)))
static void hsv2rgb(double h, double s, double v, uint8_t* r, uint8_t* g, uint8_t* b) {
//   vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//   vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//   return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
    double pr = abs(fract(h + 1) * 6.0 - 3.0);
    double pg = abs(fract(h + (2.0/3.0)) * 6.0 - 3.0);
    double pb = abs(fract(h + (1.0/3.0)) * 6.0 - 3.0);
    *r = v * clamp(pr-1., 0., 1.) * 255;
    *g = v * clamp(pg-1., 0., 1.) * 255;
    *b = v * clamp(pb-1., 0., 1.) * 255;
}
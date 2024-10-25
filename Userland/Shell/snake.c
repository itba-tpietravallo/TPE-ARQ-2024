#include <sys.h>

#define INITIAL_BODY_SIZE 3
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
    SnakeBodySquare body[INITIAL_BODY_SIZE];
} SnakeHeadSquare;

static void registerKeys(void);
static void movingTo(int dir_x, int dir_y);
static void setDirection(enum REGISTERABLE_KEYS scancode);
static void setSquareDimensions(void);
static void setDefaultFeatures(void);



static SnakeHeadSquare head;
static Square square;

// screen dimensions
static int window_height, window_width;

int snake(void){
    window_height = getWindowHeight();
    window_width = getWindowWidth();

    registerKeys();
    setSquareDimensions();
    setDefaultFeatures();

    for(int y = 0; y < window_height; y += square.height){
        for(int x = 0; x < window_width; x += square.width){
            drawRectangle(0x4DB6AC, square.width - OFFSET, square.height - OFFSET, x + OFFSET, y + OFFSET);
        }
    }

    for ( int k = 0 ; k < INITIAL_BODY_SIZE ; k++) {
        drawRectangle(head.color, square.width - OFFSET, square.height - OFFSET, head.body[k].position.x + OFFSET, head.body[k].position.y + OFFSET);
    }

    return 0;
}

static void registerKeys(void) {
    registerKey(KP_UP_KEY, setDirection);
    registerKey(KP_DOWN_KEY, setDirection);
    registerKey(KP_LEFT_KEY, setDirection);
    registerKey(KP_RIGHT_KEY, setDirection);
}

static void movingTo(int dir_x, int dir_y) {
    head.direction.x = dir_x;
    head.direction.y = dir_y;
}

static void setDirection(enum REGISTERABLE_KEYS scancode) {
    switch (scancode) {
    case KP_UP_KEY:
        movingTo(0, 1);
        break;
    case KP_DOWN_KEY:
        movingTo(0, -1);
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
    for(int i = 0; i < INITIAL_BODY_SIZE; i++){
        head.body[i].position.x = INITIAL_POS_X - i * head.direction.x * SQUARE_DIM;
        head.body[i].position.y = INITIAL_POS_Y - i * head.direction.y * SQUARE_DIM;
    }
}


// TODO
static void resizeBody(void){

}

#include <fonts.h>

/* 
    Note: An attempt was made to use the Linux kernel's Solarize.12x29.psf (https://wiki.osdev.org/PC_Screen_Font). Now only the pain remains.
    The Makefile recipe is left here in case an(other) insane attempt at porting it is made. 

    For the time being a simpler bitmapped font embedded into a header file is used.
    Note: Symbol table entries on Solarize are: 
    - _binary___font_assets_Solarize_12x29_psf_start
    - _binary___font_assets_Solarize_12x29_psf_end
    - _binary___font_assets_Solarize_12x29_psf_size

    NOT the same as the ones listed on https://wiki.osdev.org/PC_Screen_Font

    font.o:
        objcopy -O elf64-x86-64 -B i386 -I binary ./font_assets/Solarize.12x29.psf font.o
 */

#include "../font_assets/font_basic.c"

static uint16_t glyphSizeX = 8;
static uint16_t glyphSizeY = 8;
static uint16_t fontSize = 1;

static char * bitmap = (char *) font8x8_basic;

static uint16_t xBufferPosition;
static uint16_t yBufferPosition;

static char buffer[64] = { '0' };

static inline void renderFromBitmap(char * bitmap, uint64_t xBase, uint64_t yBase);
static inline void renderAscii(char ascii, uint64_t x, uint64_t y);

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
static void printBase(uint64_t value, uint32_t base);

// * Uses inline to avoid stack frames on hot paths *
static inline void renderFromBitmap(char * bitmap, uint64_t xBase, uint64_t yBase) {
    int xs, xo;
    for (int x = 0; x < glyphSizeX * fontSize; x++) {
        xs = xBase + x; 
        xo = x / fontSize;
        for (int y = 0; y < glyphSizeY * fontSize; y++) {
            // Read into char * slice and mask
            putPixel(*(bitmap + (y / fontSize)) & (1 << xo) ? 0x00FFFFFF : 0x0, xs, yBase + y);
        }
    }
}

// * Uses inline to avoid stack frames on hot paths *
// `x` and `y` are the TOP LEFT corner positions
static inline void renderAscii(char ascii, uint64_t x, uint64_t y) {
    if (ascii < 128) {
        // The function only takes in a slice of the whole matrix
        renderFromBitmap(bitmap + (ascii * glyphSizeY), x, y);
    }
}

// `ascii` ASCII character to print (0-127)
void putChar(char ascii) {
    if (xBufferPosition + glyphSizeX * fontSize > getWindowWidth()) {
        // @todo: Text may overflow at the bottom of the screen
        yBufferPosition += glyphSizeY * fontSize;
        xBufferPosition = 0;
    }

    renderAscii(ascii, xBufferPosition, yBufferPosition);
    xBufferPosition += glyphSizeX * fontSize;
}

// `string` Null terminated string
void print(char * string) {
    // Put char is technically writing arbitrary data (0-128) to memory... Should it be hardened, or is a null terminator check enough?
    for (int i = 0; string[i] != 0; i++) {
        putChar(string[i]);
    }
}

// Jumps to the next line, does not print an empty line
void newLine(void) {
    yBufferPosition += glyphSizeY * fontSize;
    xBufferPosition = 0;
}

void printDec(uint64_t value) {
    printBase(value, 10);
}

void printHex(uint64_t value) {
    printBase(value, 16);
}

void printBin(uint64_t value) {
    printBase(value, 2);
}

void clear(void) {
    uint64_t width = getWindowWidth(), height = getWindowHeight();
    for (uint64_t x = 0; x < width; x++) {
        for (uint64_t y = 0; y < height; y++) {
            putPixel(0x0, x, y);
        }
    }
}

void increaseFontSize(void) {
    fontSize *= 2;
}

void decreaseFontSize(void) {
    fontSize /= 2;
    fontSize = fontSize < 1 ? 1 : fontSize;
}

static void printBase(uint64_t value, uint32_t base) {
    uintToBase(value, buffer, base);
    print(buffer);
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base) {
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
    // @todo Could this overflow?
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

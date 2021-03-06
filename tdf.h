#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>



#define NO_ANSI_IN_RASTER


#define TDF_MAGIC_SIZE          20
#define TDF_FONTMARKER_SIZE     4
#define TDF_MAXCHAR             94
#define TDF_ASCII_LO            33
#define TDF_ASCII_HI            126

#define TYPE_OUTLINE            0
#define TYPE_BLOCK              1
#define TYPE_COLOR              2

#define MAX_NAMELEN             12
//#define MAX_LINES               12  /* we'll see */
#define MAX_LINES               13  /* this is more like it */ 

#define MAX_ANSI_SEQUENCE       32

struct tdf_raster;
struct tdf_canvas;
struct tdf_char;
struct tdf_font;
struct tdf;


typedef uint8_t color_t;
typedef color_t ibmcolor_t;
typedef color_t ansicolor_t;

struct tdf_raster {
    uint16_t bytes;
    unsigned char *chardata;
    ansicolor_t   *fgcolors;
    ansicolor_t   *bgcolors;
    struct tdf_raster *next_raster;     /* not required, but handy */
    uint16_t index;
};

struct tdf_canvas {
    uint64_t lines;
    struct tdf_raster *first_raster;
    int debug_level;
};

struct tdf_char {
    uint8_t ascii_value;
    uint16_t offset;
    unsigned char *fontdata;
    uint16_t datasize;
    struct tdf_font *parent_font;
    uint8_t width;
    uint8_t height;
    uint8_t discovered_height;
    uint8_t type;
    bool prerendered;
    bool undefined;
    struct tdf_raster *char_rasters[MAX_LINES];
};


struct tdf_font {
    char *name;
    unsigned char *data;
    uint8_t type;
    uint8_t spacing;
    uint32_t offset;
    uint16_t blocksize;
    uint16_t references;
    struct tdf_char characters[TDF_MAXCHAR];
    struct tdf_font *next_font;
    struct tdf *parent_tdf;
    uint8_t average_width;
    uint8_t average_height;
    uint8_t maximum_height;
    uint8_t defined_characters;
};

struct tdf {
    unsigned char tdfmagic[TDF_MAGIC_SIZE];
    uint8_t fontcount;
    struct tdf_font *first_font;
    FILE *fh;
    off_t limit;
    int debug_level;
};

typedef struct tdf_raster   TDFRaster;
typedef struct tdf_canvas   TDFCanvas;
typedef struct tdf_char     TDFCharacter;
typedef struct tdf_font     TDFFont;
typedef struct tdf          TDFHandle;

bool push_font(struct tdf *my_tdf, struct tdf_font *new_font);
const char *get_font_name(struct tdf *my_tdf, int id);
struct tdf_font* getfont_by_id(struct tdf *my_tdf, int id);
bool render_glyph(struct tdf_font *render_font, unsigned c);
bool prerender_glyph(TDFFont *font, unsigned char c);
const char *get_font_type(int type);
bool display_glyph(TDFFont *tdf, uint8_t c);

bool raster_append_bytes(TDFRaster *r, unsigned char *data, uint8_t bytes, ansicolor_t fg, ansicolor_t bg, bool debug);
bool raster_append_byte(TDFRaster *r, unsigned char data, ansicolor_t fg, ansicolor_t bg, bool debug);

TDFCanvas *new_canvas();
bool push_glyph(TDFCanvas *my_canvas, TDFFont *tdf, uint8_t c);
TDFRaster *canvas_get_raster(TDFCanvas *canvas, int line);
TDFRaster *canvas_add_raster(TDFCanvas *canvas);
bool canvas_output(TDFCanvas *canvas);
TDFRaster *create_new_raster();
TDFFont *create_new_font();
bool raster_output(TDFRaster *r, bool debug);

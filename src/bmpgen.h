/** bmpgen.h - Bitmap generator used with lgen (header)
 *  @author Enzo CRANCE
 *  @version 1.0
 *  @date 31/12/2017
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/** Sets the size of a cell in the labyrinth. */
#define SQUARESIZE 16

/** Defines a pixel with 3 bytes, one for each colour. */
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_t;

/** Defines a bitmap structure with width and height values,
 *  and a two-dimensional array of pixels. */
typedef struct {
    uint32_t width;
    uint32_t height;
    pixel_t** pixels;
} bitmap_t;

/** Initialises a bitmap structure.
 *  @param w The bitmap's width.
 *  @param h The bitmap's height.
 *  @param b A pointer to the bitmap structure.
 */
void init_bitmap(uint32_t w, uint32_t h, bitmap_t* b);

/** Divides a 32-bit unsigned integer into 4 bytes.
 *  @param u32 The 32-bit unsigned integer.
 *  @param t An 8-bit unsigned integer array of length 4.
 */
void uint32_to_bytes(uint32_t u32, uint8_t t[4]);

/** Writes the content of a byte array to the header.
 *  @param header The header.
 *  @param bytes The byte array.
 *  @param count The number of bytes to write.
 *  @param offset The index to start from when writing the data.
 */
void write_bytes_to_header(uint8_t* header, uint8_t* bytes, size_t count, size_t offset);

/** Writes a bitmap structure to a file.
 *  @param fp The file descriptor.
 *  @param b The bitmap structure.
 */
void write_bitmap_to_file(FILE* fp, bitmap_t b);

/** Fills a rectangle with a colour in the bitmap structure.
 *  The starting point must be closer to the origin than the ending point.
 *  @param b A pointer to the bitmap structure.
 *  @param x1 The starting point's x position.
 *  @param y1 The starting point's y position.
 *  @param x2 The ending point's x position.
 *  @param y2 The ending point's y position.
 *  @param colour The colour to fill the rectangle with.
 */
void fill_rect(bitmap_t* b, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, pixel_t colour);

/** Generates the BMP file from a labyrinth file generated by lgen.
 *  @param file_in The name of the labyrinth file.
 *  @param file_out The name of the BMP file.
 *  The function return codes are the following :
 *  - 0 if everything went well;
 *  - 1 if there was an error opening one of the files.
 */
int bmpgen(char* file_in, char* file_out);

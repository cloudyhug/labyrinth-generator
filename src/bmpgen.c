/** bmpgen.c - Bitmap generator used with lgen
 *  @author Enzo CRANCE
 *  @version 1.0
 *  @date 31/12/2017
 */

#include "bmpgen.h"

const pixel_t BLACK = {0x00, 0x00, 0x00};
const pixel_t WHITE = {0xFF, 0xFF, 0xFF};

void init_bitmap(uint32_t w, uint32_t h, bitmap_t* b) {
    int i, j;
    
    b->width = w;
    b->height = h;
    b->pixels = (pixel_t**) malloc(h * sizeof(pixel_t*));
    
    for (i = 0; i < h; i++) {
        b->pixels[i] = (pixel_t*) malloc(w * sizeof(pixel_t));
        for (j = 0; j < w; j++)
            b->pixels[i][j] = WHITE; /* Sets every pixel to white. */
    }
}

void uint32_to_bytes(uint32_t u32, uint8_t t[4]) {
    t[0] = (uint8_t)(u32 & 0x000000FF);
    t[1] = (uint8_t)((u32 & 0x0000FF00) >> 8);
    t[2] = (uint8_t)((u32 & 0x00FF0000) >> 16);
    t[3] = (uint8_t)((u32 & 0xFF000000) >> 24);
}

void write_bytes_to_header(uint8_t* header, uint8_t* bytes, size_t count, size_t offset) {
    int i;
    
    for (i = 0; i < count; i++)
        header[offset + i] = bytes[i];
}

void write_bitmap_to_file(FILE* fp, bitmap_t b) {
    int i, j;
    
    for (i = 0; i < b.height; i++) {
        for (j = 0; j < b.width; j++) {
            /* For the BMP file format we need to write the colours in this order. */
            fwrite(&b.pixels[i][j].blue, 1, 1, fp);
            fwrite(&b.pixels[i][j].green, 1, 1, fp);
            fwrite(&b.pixels[i][j].red, 1, 1, fp);
        }
    }
}

void fill_rect(bitmap_t* b, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, pixel_t colour) {
    uint32_t i, j;
    
    for (i = x1; i < x2; i++) {
        for (j = y1; j < y2; j++)
            b->pixels[i][j] = colour;
    }
}

int bmpgen(char* file_in, char* file_out) {
    FILE* fp_in; /* The file descriptor associated to the file_in file name. */
    FILE* fp_out; /* The file descriptor associated to the file_out file name. */
    bitmap_t bmp; /* The bitmap structure that will be used for the labyrinth. */
    uint32_t iwidth; /* The image's width. */
    uint32_t iheight; /* The image's height. */
    uint32_t x1, y1, x2, y2, i;
    
    /* Three 4-byte arrays that will contain the integer values of the BMP file's size,
     * and the image's width and height, as 32-bit integers converted to a 4-byte array.
     * Indeed, it is easier to copy bytes into the header, which is a byte array, than
     * to start from a 32-bit integer.
     */
    uint8_t size_tab[4];
    uint8_t width_tab[4];
    uint8_t height_tab[4];
    
    fp_in = fopen(file_in, "r");
    fp_out = fopen(file_out, "wb");
    
    if (fp_in == NULL || fp_out == NULL) return 1;
    
    /* Creates a header for the BMP file. */
    uint8_t header[54] = {
        0x42, 0x4D, /* "BM" */
        0x00, 0x00, 0x00, 0x00, /* total size of BMP file */
        0x00, 0x00, 0x00, 0x00, /* leave this to zero */
        0x36, 0x00, 0x00, 0x00, /* 54 bytes header */
        0x28, 0x00, 0x00, 0x00, /* 40 bytes BITMAPINFOHEADER */
        0x00, 0x00, 0x00, 0x00, /* width */
        0x00, 0x00, 0x00, 0x00, /* height */
        0x01, 0x00, /* 1 colour plane */
        0x18, 0x00, /* 24 bits per pixel */
        0x00, 0x00, 0x00, 0x00, /* disable compression */
        0x00, 0x00, 0x00, 0x00, /* sizeof raw pixel data */
        0x13, 0x0b, 0x00, 0x00, /* 2835 horizontal resolution */
        0x13, 0x0b, 0x00, 0x00, /* 2835 vertical resolution */
        0x00, 0x00, 0x00, 0x00, /* 0 colours = all colours */
        0x00, 0x00, 0x00, 0x00 /* 0 colours = all colours */
    };
    
    fscanf(fp_in, "%d %d\n", &iwidth, &iheight);
    
    iwidth *= SQUARESIZE;
    iheight *= SQUARESIZE;
    
    /* Initialises the bitmap structure with the width and height values that we got from the file. */
    init_bitmap(iwidth, iheight, &bmp);
    
    /* Converts the 3 integers into byte arrays, as explained before, then writes them to the header.
     * The size of the BMP file is the size of the header, added to the size of the two-dimensional
     * pixel array in the bitmap structure.
     */
    uint32_to_bytes(iwidth, width_tab);
    uint32_to_bytes(iheight, height_tab);
    uint32_to_bytes(54 + iwidth * iheight * 3, size_tab);
    
    write_bytes_to_header(header, size_tab, 4, 2);
    write_bytes_to_header(header, width_tab, 4, 18);
    write_bytes_to_header(header, height_tab, 4, 22);
    
    /* Creates the 4 outer walls of the labyrinth. */
    fill_rect(&bmp, 0, 0, 2, iwidth, BLACK);
    fill_rect(&bmp, 0, 0, iheight, 2, BLACK);
    fill_rect(&bmp, iheight-2, 0, iheight, iwidth, BLACK);
    fill_rect(&bmp, 0, iwidth-2, iheight, iwidth, BLACK);
    
    /* Reads each line of the file and graphically creates the walls in the bitmap structure. */
    while (fscanf(fp_in, "%d:%d->%d:%d\n", &x1, &y1, &x2, &y2) == 4) {
        if (x1 == x2) {
            /* Wall separating two cells on the same line */
            fill_rect(&bmp, x2 * SQUARESIZE, y2 * SQUARESIZE, (x2+1) * SQUARESIZE, y2 * SQUARESIZE + 2, BLACK);
        } else {
            /* Wall separating two cells on the same column */
            fill_rect(&bmp, x2 * SQUARESIZE, y2 * SQUARESIZE, x2 * SQUARESIZE + 2, (y2+1) * SQUARESIZE, BLACK);
        }
    }
    
    /* Writes the header then the bitmap structure to the file. */
    fwrite(header, 1, 54, fp_out);
    write_bitmap_to_file(fp_out, bmp);
    
    for (i = 0; i < iheight; i++)
        free(bmp.pixels[i]);
    free(bmp.pixels);
    fclose(fp_in);
    fclose(fp_out);
    return 0;
}

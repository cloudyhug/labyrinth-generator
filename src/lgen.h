/** lgen.h - Labyrinth generator (header)
 *  @author Enzo CRANCE
 *  @version 1.0
 *  @date 31/12/2017
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/** Defines a wall between 2 cells */
typedef struct {
    int x1; /**< First cell's x position. */
    int y1; /**< First cell's y position. */
    int x2; /**< Second cell's x position. */
    int y2; /**< Second cell's y position. */
    int exists; /**< Tells whether the wall exists or not. */
} wall_t;

/** Writes a labyrinth to a file.
 *  @param fp The file we need to write the labyrinth to.
 *  @param walls A wall array describing the labyrinth.
 *  @param nb_walls The number of walls in the previous array.
 *  @param lwidth The labyrinth's width.
 *  @param lheight The labyrinth's height.
 */
void write_labyrinth_to_file(FILE* fp, wall_t* walls, int nb_walls, int lwidth, int lheight);

/** Generates a labyrinth and exports it to a file.
 *  @param filename The name of the file where the labyrinth will be written.
 *  @param lwidth The labyrinth's width.
 *  @param lheight The labyrinth's height.
 *  The function return codes are the following :
 *  - 0 if everything went well;
 *  - 1 if the width or height parameters are wrong;
 *  - 2 if there was an error opening the file;
 *  - 3 if there was a memory allocation error.
 */
int lgen(char* filename, int lwidth, int lheight);

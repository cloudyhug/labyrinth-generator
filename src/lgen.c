/** lgen.c - Labyrinth generator
 *  @author Enzo CRANCE
 *  @version 1.0
 *  @date 31/12/2017
 */

#include "lgen.h"

void write_labyrinth_to_file(FILE* fp, wall_t* walls, int nb_walls, int lwidth, int lheight) {
    int i;
    wall_t w;
    
    /* Prints the width and height values to the file. */
    fprintf(fp, "%d %d\n", lwidth, lheight);
    
    /* Prints every wall to the file in a particular format. */
    for (i = 0; i < nb_walls; i++) {
        w = walls[i];
        if (w.exists)
            fprintf(fp, "%d:%d->%d:%d\n", w.x1, w.y1, w.x2, w.y2);
    }
}

int lgen(char* filename, int lwidth, int lheight) {
    FILE* fp; /* The file descriptor associated to the file name given as a parameter. */
    int* id; /* An array containing an identifier associated to every cell in the labyrinth. */
    wall_t* walls; /* An array containing all the labyrinth's walls. Some are randomly opened during the algorithm's execution. */
    int nb_walls; /* The number of walls in the labyrinth. */
    int opened_walls; /* The number of opened walls in the labyrinth. All the walls are closed at the beginning. */
    int i, j, k, index, id1, id2;
    
    if (lwidth < 1 || lheight < 1) return 1;
    
    fp = fopen(filename, "w");
    if (fp == NULL) return 2;
    
    nb_walls = 2 * lwidth * lheight - lwidth - lheight;
    walls = (wall_t*) malloc(nb_walls * sizeof(wall_t));
    
    id = (int*) malloc(lwidth * lheight * sizeof(int));
    
    if (walls == NULL || id == NULL) return 3;
    
    /* Sets different identifiers for every cell at the beginning. */
    for (i = 0; i < lheight * lwidth; i++) id[i] = i;
    
    /* Creates all the walls in the labyrinth. Every cell is surrounded by 4 walls. */
    k = 0;
    /* Vertical walls */
    for (i = 0; i < lheight; i++) {
        for (j = 0; j < lwidth - 1; j++) {
            walls[k++] = (wall_t) {i, j, i, j+1, 1};
        }
    }
    /* Horizontal walls */
    for (i = 0; i < lheight - 1; i++) {
        for (j = 0; j < lwidth; j++)
            walls[k++] = (wall_t) {i, j, i+1, j, 1};
    }
    
    /* Initialises the random functions, because we will need them later on to remove walls. */
    srand(time(NULL));
    
    /* Randomly removes MN-1 walls (with M the height and N the width). */
    opened_walls = 0;
    while (opened_walls < lwidth * lheight - 1) {
        /* Finds a wall that exists. */
        do {
            index = rand() % nb_walls;
        } while (!walls[index].exists);
        
        /* Gets the IDs of the cells in both sides of the wall. */
        id1 = id[walls[index].x1 * lwidth + walls[index].y1];
        id2 = id[walls[index].x2 * lwidth + walls[index].y2];
        
        /* If the IDs are different, opens the wall and copies the first cell's ID to every
         * cell having the second cell's ID in the labyrinth. It means these cells belong to
         * the same path. When we stop the algorithm, all the cells will have the same ID.
         * It means every cell of the labyrinth can be accessed from every other cell. */
        if (id1 != id2) {
            walls[index].exists = 0;
            for (i = 0; i < lwidth * lheight; i++)
                if (id[i] == id2) id[i] = id1;
            opened_walls++;
        }
    }
    
    write_labyrinth_to_file(fp, walls, nb_walls, lwidth, lheight);

    fclose(fp);
    free(id);
    free(walls);
    return 0;
}

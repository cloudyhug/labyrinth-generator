#include "lgen.h"
#include "bmpgen.h"

int main(int argc, char* argv[]) {
    int lwidth;
    int lheight;
    int res;
    
    if (argc != 4) {
        printf("Syntax : ./%s filename height width\n", argv[0]);
        return -1;
    }

    sscanf(argv[2], "%d", &lheight);
    sscanf(argv[3], "%d", &lwidth);

    res = lgen("out.lab", lwidth, lheight);
    switch(res) {
        case 0:
            printf("Labyrinth successfully written to the file!\n");
            break;
        case 1:
            printf("Width and height parameters are wrong.\n");
            return -1;
        case 2:
            printf("There was an error opening the file.\n");
            return -1;
        case 3:
            printf("There was a memory allocation error.\n");
            return -1;
    }

    res = bmpgen("out.lab", argv[1]);
    switch(res) {
        case 0:
            printf("BMP file created successfully!\n");
            break;
        case 1:
            printf("There was an error opening the files.\n");
            return -1;
    }

    remove("out.lab");
    
    return 0;
}

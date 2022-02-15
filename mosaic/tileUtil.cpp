
// File:        tileUtil.cpp
// Author:      Cinda
// Date:        2020-10-31
// Description: file handling functions that prepare data for the tiling algorithm


#include "tileUtil.h"

/**
 * Function tile:
 * @param PNG & target: an image to use as base for the mosaic. it's pixels will be
 *                      be replaced by thumbnail images whose average color is close
 *                      to the pixel.
 * @param rgbtree & ss: a kd-tree of RGBAPixels, used as a query structure for
 *                      nearest neighbor search. 
 * @param map<RGBAPixel, string> & photos: a map that takes a color key and returns the
 *                      filename of an image whose average color is that key.
 *
 * returns: a PNG whose dimensions are TILESIZE times that of the target. Each
 * pixel in the target is used as a query to ss.findNearestNeighbor, and the response
 * is used as a key in photos. 
 */

PNG tiler::tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos){
    PNG newTarget = PNG(TILESIZE*target.width(), TILESIZE*target.height());
    //go through every pixel in target 
    for(unsigned x=0; x<target.width(); x++){
        for(unsigned y=0; y<target.height(); y++){
            //find the nearest neighbor for current pixel
            RGBAPixel *currPixel = target.getPixel(x,y);
            RGBAPixel nearest_neighbor = ss.findNearestNeighbor(*currPixel);
            //obtain the nearest pic
            string path = photos.at(nearest_neighbor);
            PNG currNearest; currNearest.readFromFile(path);
            //render the pic to newTarget
            for(unsigned i=x*TILESIZE; i<(x+1)*TILESIZE; i++){
                for(unsigned j=y*TILESIZE; j<(y+1)*TILESIZE; j++){
                    RGBAPixel *currPixel = newTarget.getPixel(i,j);
                    *currPixel = *currNearest.getPixel(i-x*TILESIZE, j-y*TILESIZE);
                }
            }
        }
    }
    return newTarget;
}

/* buildMap: function for building the map of <key, value> pairs, where the key is an
 * RGBAPixel representing the average color over an image, and the value is 
 * a string representing the path/filename.png of the TILESIZExTILESIZE image
 * whose average color is the key.
 * 
 * We've provided a bit of the C++ code that allows you to iterate over the files
 * in a directory. It is up to you to figure out what it means, and how to use it.
 * 
 * @param path is the subdirectory in which the tiles can be found. In our examples
 * this is imlib.
 *
*/
map<RGBAPixel, string> tiler::buildMap(string path) {

    map < RGBAPixel, string> thumbs;
    for (const auto & entry : fs::directory_iterator(path)) {
        PNG curr; curr.readFromFile(entry.path());
        int rAvg = 0; int gAvg = 0; int bAvg = 0;
        for (unsigned x = 0; x < curr.width(); x++) {
           for (unsigned y = 0; y < curr.height(); y++) {
               RGBAPixel *pixel = curr.getPixel(x,y);
               rAvg += pixel->r; gAvg += pixel->g; bAvg += pixel->b;
            }
        } 
        int pixelNum = curr.width()*curr.height();
        rAvg = rAvg/pixelNum;
        gAvg = gAvg/pixelNum;
        bAvg = bAvg/pixelNum;
        RGBAPixel avgColor = RGBAPixel(rAvg, gAvg, bAvg);
        cout<< "insert"<<endl;
        thumbs[avgColor] = entry.path();
    }
    return thumbs;
}



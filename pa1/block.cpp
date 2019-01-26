#include "block.h"

    Block::Block(){
        w = 0;
        h = 0;
        vector< vector<HSLAPixel> > data;
    }

    /**
     * From im, grabs vertical strip of pixels starting
     * at (column, 0), with width width
     */
    void Block::build(PNG & im, int column, int width){
        data.resize(im.height(), vector<HSLAPixel>(width));
        for(int row = 0; row < int(im.height()); row++){
            for(int col = 0; col < width; col++){ 
                if(col+column >= int(im.width())){
                    cout << "ended build at: "; cout << col + column <<endl;
                    return;
                }
                data[row][col] = *(im.getPixel(col + column,row)); 
            }
        }
        w = width;
        h = im.height();
    }

    /**
     * Draws current block at position (column, 0) in im
     */
    void Block::render(PNG & im, int column) const {
        //grab stuff out of block and draw those pixels in im
        int im_height = im.height();
        int im_width = im.width();
        for(int row = 0; row < h; row++){
            if(row >= im_height) return;
            for(int col = 0; col < w; col++){ 
                if(col+column >= im_width) return;
                (im.getPixel(col+column,row))->h = (data[row][col]).h;
                (im.getPixel(col+column,row))->s = (data[row][col]).s;
                (im.getPixel(col+column,row))->l = (data[row][col]).l;
                (im.getPixel(col+column,row))->a = (data[row][col]).a;
                //cout << row; cout << ", "; cout << col << endl;
            }
        }
    }

    /**
     * Changes saturation of every pixel in the block to
     * 0 (changes all pixels to grey)
     */
    void Block::greyscale(){
        for(int i = 0; i < int(data.size()); i++){ 
            for(int j = 0; j < int(data[i].size()); j++){
                data[i][j].s = 0;
            }
        }
    }

    /**
     * Returns width of current block
     */
    int Block::width() const {
        return w;
    }

    /**
     * Returns height of current block
     */

    int Block::height() const {
        return h;
    }
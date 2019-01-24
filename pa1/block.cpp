#include "block.h"

    Block::Block(){
        w = 0;
        h = 0;
        vector< vector<HSLAPixel *> > data;
    }

    /**
     * From im, grabs vertical strip of pixels starting
     * at (column, 0), with width width
     */
    void Block::build(PNG & im, int column, int width){
        //vector<vector<int> > t(10, vector<int>(10));// = *(new vector<int>);
        int height = im.height();
        int wid = im.width();
        data.resize(height, vector<HSLAPixel *>(width));
        HSLAPixel * temp;
        for(int row = 1; row < height; row++){
            for(int col = 1; col < width; col++){ 
                if(col+column >= wid){
                    cout << "ended build at: "; cout << col + column <<endl;
                    return;
                }
                temp = im.getPixel(col + column,row);
                data[row-1][col-1] = new HSLAPixel(temp->h, temp->s, temp->l, temp->a);
                
            }
        }
        w = width;
        h = height;
    }

    /**
     * Draws current block at position (column, 0) in im
     */
    void Block::render(PNG & im, int column) const {
        //grab stuff out of block and draw those pixels in im
        int im_height = im.height();
        int im_width = im.width();
        for(int row = 1; row < h-1; row++){
            if(row >= im_height) return;
            for(int col = 1; col < w-1; col++){ 
                if(col+column >= im_width) return;
                (im.getPixel(col+column,row))->h = (data[row-1][col-1])->h;
                (im.getPixel(col+column,row))->s = (data[row-1][col-1])->s;
                (im.getPixel(col+column,row))->l = (data[row-1][col-1])->l;
                (im.getPixel(col+column,row))->a = (data[row-1][col-1])->a;
                //cout << row; cout << ", "; cout << col << endl;
            }
        }
    }

    /**
     * Changes saturation of every pixel in the block to
     * 0 (changes all pixels to grey)
     */
    void Block::greyscale(){
        cout << "in grey" << endl;
        for(int i = 0; i < int(data.size())-1; i++){ //afhoadfhoeihfoif
            for(int j = 0; j < int(data[i].size())-1; j++){
                //cout << i; cout << ", "; cout << j << endl;
                data[i][j]->s = 0;
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
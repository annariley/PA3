
#include "stats.h"
// #include <math.h>

stats::stats(PNG & im){

    for(int x = 0; x < im.width(); x++) {
        for(int y = 0; y < im.height(); y++) {

            sumHueX[x][y] = 0;
            sumHueY[x][y] = 0;
            sumSat[x][y] = 0;
            sumLum[x][y] = 0;

            for(int xSum = 0; xSum < x; xSum++) {
                for(int ySum = 0; ySum < y; ySum++) {

                    HSLAPixel *pixel = im.getPixel(xSum, ySum);

                    double hRad = pixel->h * PI / 180;
                    sumHueX[x][y] += cos(hRad) * 180 / PI;
                    sumHueY[x][y] += sin(hRad) * 180 / PI;
                    sumSat[x][y] += pixel->s;
                    sumLum[x][y] += pixel->l;
                }
            }
        }
    }

}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

    return ((lr.first-ul.first)+1)*((lr.second-ul.second)+1);

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){

    HSLAPixel avg = HSLAPixel();

    double xHue, yHue;
    
    xHue = sumHueX[ul.first-1][ul.second-1] - sumHueX[lr.first][lr.second] / rectArea(ul,lr);
    yHue = sumHueY[ul.first-1][ul.second-1] - sumHueY[lr.first][lr.second] / rectArea(ul,lr);
    
    avg.h = atan2(yHue, xHue);
    avg.s = sumSat[ul.first-1][ul.second-1] - sumSat[lr.first][lr.second] / rectArea(ul,lr);
    avg.l = sumLum[ul.first-1][ul.second-1] - sumLum[lr.first][lr.second] / rectArea(ul,lr);
    
}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

    vector<int> distn;

    /* using private member hist, assemble the distribution over the
    *  given rectangle defined by points ul, and lr into variable distn.
    *  You will use distn to compute the entropy over the rectangle.
    *  if any bin in the distn has frequency 0, then do not add that 
    *  term to the entropy total. see .h file for more details.
    */

    /* my code includes the following lines:
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) 
                                    * log2((double) distn[i]/(double) area);
    */
    
    return  -1 ;//* entropy;
}


#include "stats.h"

stats::stats(PNG & im){

    HSLAPixel *pixel;

    vector<double> colSumHueX;
    vector<double> colSumHueY;
    vector<double> colSumSat;
    vector<double> colSumLum;

    vector<int> tempHist;
    vector<vector<int>> colHist;

    for(int x = 0; x < (int)im.width(); x++) {

        colSumHueX.clear();
        colSumHueY.clear();
        colSumSat.clear();
        colSumLum.clear();

        for(int y = 0; y < (int)im.height(); y++) {

            double tempSumHueX = 0;
            double tempSumHueY = 0;
            double tempSumSat = 0;
            double tempSumLum = 0;

            // initialize an empty, 36-bin hist
            for(int i = 0; i < 36; i++) {
                tempHist.push_back(0);
            }

            for(int xSum = 0; xSum <= x; xSum++) {
                for(int ySum = 0; ySum <= y; ySum++) {

                    pixel = im.getPixel(xSum, ySum);

                    double hRad = pixel->h * PI / 180;
                    tempSumHueX += cos(hRad) * 180 / PI;
                    tempSumHueY += sin(hRad) * 180 / PI;
                    tempSumSat += pixel->s;
                    tempSumLum += pixel->l;

                    int bin = (int)(pixel->h / 10);
                    tempHist[bin] = tempHist[bin] + 1;
                }
            }

            colSumHueX.push_back(tempSumHueX);
            colSumHueY.push_back(tempSumHueY);
            colSumSat.push_back(tempSumSat);
            colSumLum.push_back(tempSumLum);

            colHist.push_back(tempHist);
        }
            
        this->sumHueX.push_back(colSumHueX);
        this->sumHueY.push_back(colSumHueY);
        this->sumSat.push_back(colSumSat);
        this->sumLum.push_back(colSumLum);

        this->hist.push_back(colHist);
    }

}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

    return ((lr.first-ul.first)+1)*((lr.second-ul.second)+1);

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){

    HSLAPixel avg;

    cout << sumSat[0][0] << endl;
    cout << sumSat[0][1] << endl;
    cout << sumSat[1][0] << endl;
    cout << sumSat[1][1] << endl;

    double xHue = this->sumHueX[lr.first][lr.second];
    double yHue = this->sumHueY[lr.first][lr.second];
    double sat = this->sumSat[lr.first][lr.second];
    double lum = this->sumLum[lr.first][lr.second];

    if(ul.first > 0 && ul.second == 0) {
        xHue -= this->sumHueX[lr.first-1][lr.second];
        yHue -= this->sumHueY[lr.first-1][lr.second];
        sat -= this->sumSat[lr.first-1][lr.second];
        lum -= this->sumLum[lr.first-1][lr.second];
    }
    else if(ul.first == 0 && ul.second > 0) {
        xHue -= this->sumHueX[lr.first-1][lr.second];
        yHue -= this->sumHueY[lr.first-1][lr.second];
        sat -= this->sumSat[lr.first-1][lr.second];
        lum -= this->sumLum[lr.first-1][lr.second];
    }
    else if(ul.first > 0 && ul.second > 0) {
        xHue -= this->sumHueX[lr.first-1][lr.second];
        xHue -= this->sumHueX[lr.first-1][lr.second];
        xHue += this->sumHueX[ul.first-1][ul.second-1];

        yHue -= this->sumHueY[lr.first-1][lr.second];
        yHue -= this->sumHueY[lr.first-1][lr.second];
        yHue += this->sumHueY[ul.first-1][ul.second-1];

        sat -= this->sumSat[lr.first-1][lr.second];
        sat -= this->sumSat[lr.first-1][lr.second];
        sat += this->sumSat[ul.first-1][ul.second-1];

        lum -= this->sumLum[lr.first-1][lr.second];
        lum -= this->sumLum[lr.first-1][lr.second];
        lum += this->sumLum[ul.first-1][ul.second-1];
    }

    double area = rectArea(ul,lr);
    
    avg.h = atan2(yHue/area, xHue/area) * 180 / PI;
    avg.s = sat / area;
    avg.l = lum / area;
    avg.a = 1.0;

    return avg;
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

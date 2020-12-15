#include "ImgClusters.h"

ImgClusters::ImgClusters(Image * img, int rows, int cols, int clstRows, int clstCols) {
    this->clstRows = clstRows;
    this->clstCols = clstCols;

    this->findClusters(img, rows, cols);
};

void ImgClusters::findClusters(Image * img, int imgRows, int imgCols) {
    // find amount of clusters to be created
    int clustersVertical = imgRows / this->clstRows;
    int clustersHorizontal = imgCols / this->clstCols;

    // used for normalization
    int totalBrightness = std::accumulate(img->getPixels()->begin(),img->getPixels()->end(),0);

    vector<int> weights;
    // for each cluster find weight and representative
    for (int x = 0; x < clustersVertical; ++x) {
        for (int y = 0; y < clustersHorizontal; ++y) {
            int weight = this->findClustBrightness(img, imgCols, x, y);
            weights.push_back(weight);
            tuple<int,int> representative = this->findClustRepresentative(x,y);
            this->clusters.push_back(make_tuple(representative, (double)weight/totalBrightness));
        }
    }

    assert(accumulate(weights.begin(),weights.end(),0)==totalBrightness);
}

int ImgClusters::findClustBrightness(Image * img, int imgCols, int x, int y) {
    // x and y are cluster coordinates
    int xOffset = this->clstRows * x;
    int yOffset = this->clstCols * y;

    int brightness = 0;
    for (int x_i = xOffset; x_i < xOffset + this->clstRows; ++x_i) {
        for (int y_i = yOffset; y_i < yOffset + this->clstCols; ++y_i) {
            int pixel_index = x_i * imgCols + y_i;
            brightness += img->getPixels()->at(pixel_index);
        }
    }

    return brightness;
}


tuple<int, int> ImgClusters::findClustRepresentative(int x, int y) {
    // x and y are cluster coordinates
    int startRow = this->clstRows * x;
    int startCol = this->clstCols * y;

    int endRow = this->clstRows * (x+1);
    int endCol = this->clstCols * (y+1);

    int reprRow = (startRow + endRow) / 2;
    int reprCol = (startCol + endCol) / 2;

    return make_tuple(reprRow, reprCol);
}
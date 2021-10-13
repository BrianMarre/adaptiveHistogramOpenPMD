#include <openPMD/openPMD.hpp>

// example: data handling
#include <numeric>  // std::iota
#include <vector>   // std::vector

/**@file test example for writing histogram woth openPMD-api from c++ code
  *
  */

int main(){
    /// open
    auto series = openPMD::Series(
        "myOutput/data_%05T.bp",
        openPMD::Access::CREATE);

    /// open iteration
    auto iteration = series.iterations[0];

    constexpr int xExtent = 2;
    constexpr int yExtent = 3;
    constexpr int maxNumBins = 5;

    openPMD::Extent extent = {xExtent, yExtent * maxNumBins};

    typedef float T_Value;
    typedef float T_Argument;

    /// create test data
    // set up is xExtent x yExtent grid points and for each grid point a histogram with maxNumBins bins
    std::vector<T_Value> weightBins_data(xExtent * yExtent * maxNumBins);
    std::vector<T_Argument> leftBoundaryBins_data(xExtent * yExtent *
                                                  maxNumBins);
    std::vector<T_Argument> widthBins_data(xExtent * yExtent * maxNumBins);

    std::vector<int> numBins(xExtent * yExtent);

    // fill x_data with ascending index
    std::iota(weightBins_data.begin(), weightBins_data.end(), 1.);
    std::iota(widthBins_data.begin(), widthBins_data.end(), 1.);
    std::iota(leftBoundaryBins_data.begin(), leftBoundaryBins_data.end(), 1.);

    // create a new record for mesh
    auto histogramMesh = iteration.meshes["adaptiveHistogram"];

    histogramMesh.setAttribute("maxNumBins", maxNumBins);

    // create 1 record component each corresponding to one single entry in histogram
    auto entryLeftBoundary = histogramMesh["leftBoundaryBins"];
    auto entryWidth = histogramMesh["widthBins"];
    auto entryWeight = histogramMesh["weightBins"];

    ///create new dataset, description of how data is to be stored
    openPMD::Datatype dataTypeValue =
        openPMD::determineDatatype(openPMD::shareRaw(weightBins_data));
    openPMD::Datatype dataTypeArgument =
        openPMD::determineDatatype(openPMD::shareRaw(widthBins_data));

    openPMD::Dataset dataSetValue = openPMD::Dataset(dataTypeValue, extent);
    openPMD::Dataset dataSetArgument =
        openPMD::Dataset(dataTypeArgument, extent);

    /// set what records actually store(what record components actually are)
    entryWeight.resetDataset(dataSetArgument);
    entryWidth.resetDataset(dataSetArgument);
    entryLeftBoundary.resetDataset(dataSetValue);

    /// actual data is passed
    entryLeftBoundary.storeChunk(
        openPMD::shareRaw(leftBoundaryBins_data), // returns raw pointer to data
        {0, 0},                                   // offset
        extent); // extent, artificially inflated
    entryWidth.storeChunk(
        openPMD::shareRaw(widthBins_data), // returns raw pointer to data
        {0, 0},                            // offset
        extent);                           // extent, artificially inflated
    entryWeight.storeChunk(
        openPMD::shareRaw(weightBins_data), // returns raw pointer to data
        {0, 0},                             // offset
        extent);                            // extent, artificially inflated

    /// actual write happens
    series.flush();

    // destruct series object,
    // e.g. when out-of-scope

    return 0;
}
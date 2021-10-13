#include <openPMD/openPMD.hpp>

// example: data handling
#include <numeric>  // std::iota
#include <vector>   // std::vector

// print to console
#include <iostream>

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

    openPMD::Extent extentBins = {xExtent, yExtent * maxNumBins};
    openPMD::Extent extentNumBins = {xExtent, yExtent};

    typedef float T_Value;
    typedef float T_Argument;

    /// create test data
    // set up is xExtent x yExtent grid points and for each grid point a histogram with maxNumBins bins
    std::vector<T_Value> weightBins_data(xExtent * yExtent * maxNumBins);
    std::vector<T_Argument> leftBoundaryBins_data(xExtent * yExtent *
                                                  maxNumBins);
    std::vector<T_Argument> widthBins_data(xExtent * yExtent * maxNumBins);

    std::vector<int> numBins_data(xExtent * yExtent, 1.);

    // fill x_data with ascending index
    std::iota(weightBins_data.begin(), weightBins_data.end(), 1.);
    std::iota(widthBins_data.begin(), widthBins_data.end(), 1.);
    std::iota(leftBoundaryBins_data.begin(), leftBoundaryBins_data.end(), 1.);

    // create new meshes
    auto histogramBinsMesh = iteration.meshes["adaptiveHistogramBins"];
    auto histogramNumBinMesh = iteration.meshes["adaptiveHistogramNumBins"];

    /// sotre maxNumBins
    histogramBinsMesh.setAttribute("maxNumBins", maxNumBins);

    // create 1 record component for each entry in bin
    auto entryLeftBoundary = histogramBinsMesh["leftBoundaryBins"];
    auto entryWidth = histogramBinsMesh["widthBins"];
    auto entryWeight = histogramBinsMesh["weightBins"];

    // create one numBin per super cell
    auto entryNumBins =
        histogramNumBinMesh[openPMD::MeshRecordComponent::SCALAR];

    ///create new dataset, description of how data is to be stored
    openPMD::Datatype dataTypeValue =
        openPMD::determineDatatype(openPMD::shareRaw(weightBins_data));
    openPMD::Datatype dataTypeArgument =
        openPMD::determineDatatype(openPMD::shareRaw(widthBins_data));
    openPMD::Datatype dataTypeNumber =
        openPMD::determineDatatype(openPMD::shareRaw(numBins_data));

    openPMD::Dataset dataSetValue = openPMD::Dataset(dataTypeValue, extentBins);
    openPMD::Dataset dataSetArgument =
        openPMD::Dataset(dataTypeArgument, extentBins);
    openPMD::Dataset dataSetNumber =
        openPMD::Dataset(dataTypeNumber, extentNumBins);

    /// set what records actually store(what record components actually are)
    entryWeight.resetDataset(dataSetArgument);
    entryWidth.resetDataset(dataSetArgument);
    entryLeftBoundary.resetDataset(dataSetValue);
    entryNumBins.resetDataset(dataSetNumber);

    /// actual data is passed
    entryLeftBoundary.storeChunk(
        openPMD::shareRaw(leftBoundaryBins_data), // returns raw pointer to data
        {0, 0},                                   // offset
        extentBins); // extent, artificially inflated
    entryWidth.storeChunk(
        openPMD::shareRaw(widthBins_data), // returns raw pointer to data
        {0, 0},                            // offset
        extentBins);                       // extent, artificially inflated
    entryWeight.storeChunk(
        openPMD::shareRaw(weightBins_data), // returns raw pointer to data
        {0, 0},                             // offset
        extentBins);                        // extent, artificially inflated
    entryNumBins.storeChunk(
        openPMD::shareRaw(numBins_data), // returns raw pointer to data
        {0, 0},                          // offset
        extentNumBins);                  // extent, actual

    // NOTE: openPMD seems to use the last dimension index as the fastest
    // changing index, probably due to their list of list implementation and
    // filling, while piconpgu gridbuffers do use the first simulation dimension
    // as the fastest changing index. I did not find documentation for both
    // describing this behaviour, therefore it might change in the future.
    // Acceptable I guess since only a plugin relies on it. should leave a note
    // in the source code nevertheless

    /// actual write happens
    series.flush();

    // destruct series object,
    // e.g. when out-of-scope

    return 0;
}
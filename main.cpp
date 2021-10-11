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
    auto iteration = series.iterations[42];

    constexpr int xExtent = 2;
    constexpr int yExtent = 3;
    constexpr int maxNumBins = 5;

    openPMD::Extent extent = {xExtent, yExtent, maxNumBins};

    int numBins = 3;

    /// create test data
    // set up is xExtent x yExtent grid points and for each grid point a histogram with maxNumBins bins
    std::vector<float> x_data(
        xExtent * yExtent * maxNumBins);

    // fill x_data with ascending index
    std::iota(
        x_data.begin(),
        x_data.end(),
        1.);

    // create a new record for mesh
    auto histogramMesh = iteration.meshes["adaptiveHistogram"];

    // create 1 record component each corresponding to one single entry in histogram
    auto entry = histogramMesh[openPMD::MeshRecordComponent::SCALAR];

    ///create new dataset, description of how data is to be stored
    openPMD::Datatype dataType = openPMD::determineDatatype(openPMD::shareRaw(x_data));
    openPMD::Dataset dataSet = openPMD::Dataset(dataType, extent);

    /// set what records actually store(what record components actually are)
    entry.resetDataset(dataSet);

    /// actual data is passed
    entry.storeChunk(
        openPMD::shareRaw(x_data), // returns raw pointer to data
        {0, 0, 0}, // offset
        extent); // extent, artificially inflated

    /// actual write happens
    series.flush();

    // destruct series object,
    // e.g. when out-of-scope

    return 0;
}
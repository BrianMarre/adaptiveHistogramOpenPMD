import openpmd_api as io
import numpy as np
import math

filename = "/home/marre55/testOpenPMD/build/myOutput/data_%T.bp"

# open a series of adios [.bp] files, in read only mode
series = io.Series(filename, io.Access.read_only)

#debug output
step = series.iterations[42]

print("meshes for iteration 42")
for mesh in step.meshes:
    print("\t {0}".format(mesh))
    print("with records:")
    for record in step.meshes[mesh]:
        print("\t {0}".format(record))

adaptiveHistogram = step.meshes["adaptiveHistogram"]
leftBoundaryBins = adaptiveHistogram["leftBoundaryBins"]
widthBins = adaptiveHistogram["widthBins"]
weightBins = adaptiveHistogram["weightBins"]


print(adaptiveHistogram.get_attribute("maxNumBins"))

leftBoundaryBins_data = leftBoundaryBins.load_chunk()
widthBins_data = widthBins.load_chunk()
weightBins_data = weightBins.load_chunk()

series.flush()

print(leftBoundaryBins_data)

del series
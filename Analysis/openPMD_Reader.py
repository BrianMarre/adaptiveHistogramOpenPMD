import openpmd_api as io
import numpy as np
import math

filename = "/home/marre55/testOpenPMD/build/myOutput/data_%T.bp"

# open a series of adios [.bp] files, in read only mode
series = io.Series(filename, io.Access.read_only)

#debug output
step = series.iterations[0]

print("iteration 0")
for mesh in step.meshes:
    print("with mesh")
    print("\t {0}".format(mesh))
    print("with records:")
    for record in step.meshes[mesh]:
        print("\t {0}".format(record))

adaptiveHistogramBins = step.meshes["adaptiveHistogramBins"]
leftBoundaryBins = adaptiveHistogramBins["leftBoundaryBins"]
widthBins = adaptiveHistogramBins["widthBins"]
weightBins = adaptiveHistogramBins["weightBins"]

adaptiveHistogramNumBins = step.meshes["adaptiveHistogramNumBins"][io.Mesh_Record_Component.SCALAR]

print(adaptiveHistogramBins.get_attribute("maxNumBins"))

leftBoundaryBins_data = leftBoundaryBins.load_chunk()
widthBins_data = widthBins.load_chunk()
weightBins_data = weightBins.load_chunk()
numBins_data = adaptiveHistogramNumBins.load_chunk()

series.flush()

print(leftBoundaryBins_data)
print(widthBins_data)
print(weightBins_data)
print(numBins_data)

del series
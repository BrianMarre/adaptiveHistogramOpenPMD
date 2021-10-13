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

print(series.get_attribute("maxNumBins"))

adaptiveHistogram = step.meshes["adaptiveHistogram"][io.Mesh_Record_Component.SCALAR]
x_data = adaptiveHistogram.load_chunk()
series.flush()

print(x_data)

del series
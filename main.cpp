// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>
#include <iostream>
#include <vector>
#include <cstdint>

#include "gdal_priv.h"

using namespace std;

class MasterIndex
{
public:
    pair<uint64_t, uint64_t> index;
    vector<pair<uint64_t, uint64_t>> index_array;

    void show_indices()
    {
        for (auto iter : index_array) {
            cout << iter.first << " " << iter.second << endl;
        }
    }
};

MasterIndex* get_master_index(GDALRasterBand *band, float value = 0.0f)
{
    MasterIndex *master_index = new MasterIndex;
    int nrows, ncols;
    double nodata;

    nrows = band->GetYSize();
    ncols = band->GetXSize();
    nodata = band->GetNoDataValue();

    float *input_row = (float *)CPLMalloc( sizeof(float) * ncols );

    for (int i = 0; i < nrows; i++) {
        band->RasterIO(GF_Read, 0, i, ncols, 1, input_row, ncols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < ncols; j++) {
            if (input_row[j] != nodata && input_row[j] > value) {   
                master_index->index.first = i;
                master_index->index.second = j;
                master_index->index_array.push_back(master_index->index);
            } else {
                continue;
            }
        }
    }

    CPLFree(input_row);
    return master_index;
}

int main(int argc, char **argv)
{
    GDALDataset *in_ds;
    GDALAllRegister();
    in_ds = (GDALDataset *)GDALOpen(argv[1], GA_ReadOnly);

    MasterIndex *master_index = get_master_index(in_ds->GetRasterBand(1));
    master_index->show_indices();

    GDALClose(in_ds);
    GDALDestroyDriverManager();
    delete master_index;

    cin.get();
    // _CrtDumpMemoryLeaks();
    return 0;
}
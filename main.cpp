#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include "gdal_priv.h"

using namespace std;

template<typename T1, typename T2>
void get_master_index(const char *input_filepath, vector<pair<T1, T2>> &master_index, float value = 0) {

    GDALDataset *in_ds;
    GDALAllRegister ();

    int nrows, ncols, nbands;
    double nodata;
    double transform[6];

    in_ds = (GDALDataset *) GDALOpen (input_filepath, GA_ReadOnly);

    nrows = in_ds->GetRasterBand (1)->GetYSize ();
    ncols = in_ds->GetRasterBand (1)->GetXSize ();
    nbands = in_ds->GetRasterCount ();
    nodata = in_ds->GetRasterBand (1)->GetNoDataValue ();
    in_ds->GetGeoTransform (transform);

    float *input_Row = (float *) CPLMalloc (sizeof (float) * ncols);

    cout << "Getting master index from file: " << *input_filepath << endl;

    for (int i = 0; i < nrows; i++) {
        in_ds->GetRasterBand (1)->RasterIO (GF_Read, 0, i, ncols, 1, input_Row, ncols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < ncols; j++) {
            if (input_Row[j] != nodata && input_Row[j] > value) {
                pair<size_t, size_t> index;
                index.first = i;
                index.second = j;
                master_index.push_back(index);
            } else {
                continue;
            }
        }
    }

    CPLFree (input_Row);
    GDALClose (in_ds);
    GDALDestroyDriverManager ();
}

template<typename T1, typename T2>
GDALDataset subset_raster(GDALDataset &dataset, vector<pair<T1, T2>> &master_index) {
    // TODO
}

template<typename T1, typename T2>
void save_master_index(const char *output_filepath, vector<pair<T1, T2>> &master_index) {
    ofstream outfile;
    outfile.open(output_filepath);

    for (auto iter : master_index) {
        outfile << iter.first << " " << iter.second << endl;
    }

    outfile.close();
}

template<typename T1, typename T2>
void load_master_index(const char *input_filepath, vector<pair<T1, T2>> &master_index) {
    // TODO
}

template<typename T1, typename T2, typename T3>
void get_values(vector<pair<T1, T2>> &master_index, vector<T3> &values) {
    // TODO
}

int main(int argc, char **argv) {
    const char *input_filename = "/home/iis_backup/Documents/data/max_ndvi_ls_ts_rj_2018/max_ndvi_ls_ts_rj_2018_clip.tif";
    const char *output_filename = "/home/iis_backup/Documents/data/max_ndvi_ls_ts_rj_2018/master_index";
    vector<pair<size_t, size_t>> master_index;
    get_master_index(input_filename, master_index);
    // save_master_index(output_filename, master_index);
    return 0;
}

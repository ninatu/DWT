# Dynamic Time Warping 

This module contains tools for calculating alignment between time-series using DTW. 
There are implementations of different algorithms for calculation DTW or
approximation of DTW: classical DTW, 
[PrunedDTW](http://www.producao.usp.br/bitstream/handle/BDPI/51065/2775697.pdf?sequence=1),
[SparseDTW](https://arxiv.org/abs/1201.2969), 
[FastDTW](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.432.4253&rep=rep1&type=pdf#page=64)
 and [MultiscaleDTW](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.205.9304&rep=rep1&type=pdf).
 
 
 ## Instalation 
 
 ```
 mkdir build 
 cd build
 cmake ../dtw_computation/
 make
 cd ..
 ``` 
 
 ## Usage
 
 For computation dtw path:
 ```
 ./build/dtw_computation data_type(vector|double) input_path output_path \
                  alg_name(dtw|pruned_dtw|fast_dtw|sparse_dtw|ms_dtw)

```
 For algorithms pruned_dtw, fast_dtw, sparse_dtw there optionals paraments, usage
 ```
 ./build/dtw_computation data_type(vector|double) input_path output_path pruned_dtw window
 
 ./build/dtw_computation data_type(vector|double) input_path output_path fast_dtw radius scale
 
 ./build/dtw_computation data_type(vector|double) input_path output_path sparse_dtw res
 ```
 
 
 For computation chroma features for audio groups use script `compute_chroma.py`, usage:
 
 ```angular2html
python compute_chroma.py [-h] [--n_fft N_FFT] [--hop_lenght HOP_LENGHT] [--sr SR]
       config_path output_dir

positional arguments:
  config_path           The path to the dataset config file, containing dict
                        with composition and paths of its recordings in "json"
                        format
  output_dir            Path to dir for saving results

optional arguments:
  -h, --help            show this help message and exit
  --n_fft N_FFT         FFT window size(ms)
  --hop_lenght HOP_LENGHT
                        Hop length(ms)
  --sr SR               Sampling rate
```
 


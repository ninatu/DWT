data_dir="../data/SMD_Western-Music/vectors/"
files="Brahms_Op034b-03.txt Brahms_Op108-01.txt Brahms_Op108-02.txt Brahms_Op108-03.txt Brahms_Op108-04.txt Chopin_Op060.txt Debussy_L140-01.txt Debussy_L140-02.txt Debussy_L140-03.txt Martin_Trio-02.txt Mozart_KV448-01.txt Rachmaninoff_Op005-01.txt Rachmaninoff_Op005-04.txt Ravel_PianoTrio-01.txt Ravel_PianoTrio-02.txt Ravel_PianoTrio-03.txt Ravel_PianoTrio-04.txt"

mkdir -p results/

dtw_sum=0
pruned_dtw_sum=0
sparce_dtw_sum=0

for dataset in $files; do
    echo $dataset
    cmake-build-debug/srс "dtw" "vector" ${data_dir}/${dataset} ./results/dtw_${dataset}

    cmake-build-debug/src pruned_dtw vector ${data_dir}/${dataset} ./results/pruned_dtw_${dataset}

    cmake-build-debug/src sparce_dtw vector ${data_dir}/${dataset} ./results/sparce2_dtw_${dataset}

    cmake-build-debug/src fast_dtw vector ${data_dir}/${dataset} ./results/fast_dtw_${dataset}

done



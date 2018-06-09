data_dir="../data/UCR_TS_Archive_2015/"
files="Car CinC_ECG_torso Haptics InlineSkate MALLAT OliveOil"

features_dir=${data_dir}/${features_dirname}

files=$(ls ${features_dir})

algorithms="dtw ms_dtw pruned_dtw sparse_dtw"

for alg in $algorithms; do
    echo "ALGORITHM: " ${alg}

    output_dir=${data_dir}/${maps_dirname}/${alg}
    mkdir -p ${output_dir}

    for dataset in $files; do
        ./build/dtw_computation "vector" ${features_dir}/${dataset} ${output_dir}/${dataset} ${alg}
done
done



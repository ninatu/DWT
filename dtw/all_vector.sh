data_dir="../data/SMD_Western-Music/"

features_dirname='chroma'
maps_dirname='maps'
features_dir=${data_dir}/${features_dirname}

files=$(ls ${features_dir})

algorithms="pruned_dtw" #dtw ms_dtw pruned_dtw sparse_dtw"

for alg in $algorithms; do
    echo "ALGORITHM: " ${alg}

    output_dir=${data_dir}/${maps_dirname}/${alg}
    mkdir -p ${output_dir}

    for dataset in $files; do
#        echo ${dataset}
#        echo ./build/dtw_computation "vector" ${features_dir}/${dataset} ${output_dir}/${dataset} ${alg}
        ./build/dtw_computation "vector" ${features_dir}/${dataset} ${output_dir}/${dataset} ${alg}
done
done

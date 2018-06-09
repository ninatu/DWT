data_dir="../data/UCR_TS_Archive_2015/"
files="Car CinC_ECG_torso Haptics InlineSkate MALLAT OliveOil"

mkdir -p results/

for dataset in $files; do
    echo $dataset
    cmake-build-debug/src "dtw" "double" ${data_dir}/${dataset}/${dataset}_TRAIN ./results/dtw_res_${dataset}.txt

    cmake-build-debug/src pruned_dtw double ${data_dir}/${dataset}/${dataset}_TRAIN ./results/pruned_dtw_res_${dataset}.txt

    cmake-build-debug/src sparce_dtw double ${data_dir}/${dataset}/${dataset}_TRAIN ./results/sparce_dtw_res_${dataset}.txt

    cmake-build-debug/src fast_dtw double ${data_dir}/${dataset}/${dataset}_TRAIN ./results/fast_dtw_res_${dataset}.txt

done


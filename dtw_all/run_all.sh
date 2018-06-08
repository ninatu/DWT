data_dir="../data/UCR_TS_Archive_2015/"
files="Car CinC_ECG_torso Haptics InlineSkate MALLAT OliveOil"

mkdir -p results/
outfile=results/UCR_TS_Archive_2015.txt
echo "dataset | dtw | pruned_dtw | acceleration" > ${outfile}

dtw_sum=0
pruned_dtw_sum=0
sparce_dtw_sum=0

for dataset in $files; do
    echo $dataset
    dtw=$(cmake-build-debug/src dtw ${data_dir}/${dataset}/${dataset}_TRAIN)
    dtw_sum=$(echo $dtw_sum + $dtw | bc)

    pruned_dtw=$(cmake-build-debug/src pruned_dtw ${data_dir}/${dataset}/${dataset}_TRAIN)
    pruned_dtw_sum=$(echo $pruned_dtw_sum + $pruned_dtw | bc)

    sparce_dtw=$(cmake-build-debug/src pruned_dtw ${data_dir}/${dataset}/${dataset}_TRAIN)
    sparce_dtw_sum=$(echo $sparce_dtw_sum + $sparce_dtw | bc)

    echo "${dataset} | ${dtw} | ${pruned_dtw} | $(echo $dtw / $pruned_dtw | bc -l) | ${sparce_dtw} | $(echo $dtw / $sparce_dtw | bc -l)" >> ${outfile}
    echo "$dtw_sum  $pruned_dtw_sum $sparce_dtw_sum"
done

echo ============================== >> ${outfile}
echo "SUM | $dtw_sum | $pruned_dtw_sum | $(echo $dtw_sum / $pruned_dtw_sum | bc -l) | $sparce_dtw_sum | $(echo $dtw_sum / $sparce_dtw_sum | bc -l)" >> ${outfile}


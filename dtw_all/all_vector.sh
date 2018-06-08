data_dir="../data/SMD_Western-Music/vectors/"
files="Brahms_Op034b-03.txt Brahms_Op108-01.txt Brahms_Op108-02.txt Brahms_Op108-03.txt Brahms_Op108-04.txt Chopin_Op060.txt Debussy_L140-01.txt Debussy_L140-02.txt Debussy_L140-03.txt Martin_Trio-02.txt Mozart_KV448-01.txt Rachmaninoff_Op005-01.txt Rachmaninoff_Op005-04.txt Ravel_PianoTrio-01.txt Ravel_PianoTrio-02.txt Ravel_PianoTrio-03.txt Ravel_PianoTrio-04.txt"

mkdir -p results/
outfile=results/UCR_TS_Archive_2015.txt
#echo "dataset | dtw | pruned_dtw | acceleration | sparce_dtw | acceleration" > ${outfile}

dtw_sum=0
pruned_dtw_sum=0
sparce_dtw_sum=0

for dataset in $files; do
    echo $dataset
   # cmake-build-debug/src_vector "dtw" "vector" ${data_dir}/${dataset} ./results/dtw_${dataset}
    #dtw_sum=$(echo $dtw_sum + $dtw | bc)

   # cmake-build-debug/src_vector pruned_dtw vector ${data_dir}/${dataset} ./results/pruned_dtw_${dataset}
   # pruned_dtw_sum=$(echo $pruned_dtw_sum + $pruned_dtw | bc)

    cmake-build-debug/src_vector sparce_dtw vector ${data_dir}/${dataset} ./results/sparce2_dtw_${dataset}

   # cmake-build-debug/src_vector fast_dtw vector ${data_dir}/${dataset} ./results/fast_dtw_${dataset}
   # sparce_dtw_sum=$(echo $sparce_dtw_sum + $sparce_dtw | bc)

   # echo "${dataset} | ${dtw} | ${pruned_dtw} | $(echo $dtw / $pruned_dtw | bc -l) | ${sparce_dtw} | $(echo $dtw / $sparce_dtw | bc -l)" >> ${outfile}
   # echo "$dtw_sum  $pruned_dtw_sum $sparce_dtw_sum"
done

#echo ============================== >> ${outfile}
#echo "SUM | $dtw_sum | $pruned_dtw_sum | $(echo $dtw_sum / $pruned_dtw_sum | bc -l) | $sparce_dtw_sum | $(echo $dtw_sum / $sparce_dtw_sum | bc -l)" >> ${outfile}

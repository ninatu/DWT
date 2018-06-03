INPUT_DIR=data/SMD_Western-Music/mp3/
OUTPUT_DIR=data/SMD_Western-Music/wav/

mkdir -p ${OUTPUT_DIR}

files=$(ls ${INPUT_DIR})
for file_ in ${files}
do
  base_file=$(basename ${file_} .mp3)
  ffmpeg -i ${INPUT_DIR}/${file_} ${OUTPUT_DIR}/${base_file}.wav
done
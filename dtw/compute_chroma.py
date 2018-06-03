import argparse
import json
import os

import tqdm
import librosa
import numpy as np


DEFAULT_N_FFT = 200
DEFAULT_HOP_LENGTH = 100
DEFAULT_SAMPLING_RATE = 16000


def main():
    parser = argparse.ArgumentParser('The computation chroma features for all recordings in the dataset')
    parser.add_argument("config_path", type=str,
                        help='The path to the dataset config file, '
                             'containing dict with composition and paths of its recordings in "json" format')
    parser.add_argument('output_dir', type=str,
                        help='Path to dir for saving results')

    parser.add_argument('--n_fft', type=int, default=DEFAULT_N_FFT,
                        help='FFT window size(ms)')
    parser.add_argument('--hop_lenght', type=int, default=DEFAULT_HOP_LENGTH,
                        help='Hop length(ms)')
    parser.add_argument('--sr', type=int, default=DEFAULT_SAMPLING_RATE,
                        help='Sampling rate')

    args = parser.parse_args()

    config_path = args.config_path
    output_dir = args.output_dir
    os.makedirs(output_dir, exist_ok=True)

    sr = args.sr
    hop_length = int(sr * args.hop_lenght / 1000.0)
    n_fft = int(sr * args.n_fft / 1000.0)

    with open(config_path) as f:
        composition_groups = json.load(f)

    np.set_printoptions(threshold=np.nan, linewidth=1000)
    dataset_dir = os.path.dirname(config_path)
    for group_name, rel_record_paths in tqdm.tqdm(composition_groups.items()):
        output_group_path = os.path.join(output_dir, '{}.txt'.format(group_name))
        with open(output_group_path, 'w') as f:
            for rel_record_path in rel_record_paths:
                full_path = os.path.join(dataset_dir, rel_record_path)
                y, _ = librosa.load(full_path, sr=sr)
                features = librosa.feature.chroma_stft(y, sr=sr, n_fft=n_fft, hop_length=hop_length)
                print(features.T, file=f)


if __name__ == '__main__':
    main()
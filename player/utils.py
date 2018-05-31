import os
import json


GROUP_COMPOSITION_FILENAME = 'group_composition.json'
DTW_MAPS_DIRNAME = 'dtw_maps'


class Recording:
    def __init__(self, name, number, path):
        self.name = name
        self.number = number
        self.path = path
        self.dtw_maps = {}


class Composition:
    def __init__(self, name, recordings):
        self.name = name
        self.recordings = recordings


def load_dataset(dataset_dir):
    group_composition_dict = find_and_parse_group_composition(dataset_dir)
    find_and_parse_dtw_algs(dataset_dir, group_composition_dict)
    return group_composition_dict

    
def find_and_parse_group_composition(dataset_dir):
    group_composition_path = os.path.join(dataset_dir, GROUP_COMPOSITION_FILENAME)
    
    if not (os.path.exists(group_composition_path) and os.path.isfile(group_composition_path)):
        raise ValueError("File '{}' not found in dataset directory.".format(GROUP_COMPOSITION_FILENAME))

    try:
        with open(group_composition_path) as f:
            data = json.load(f)
    except:
        raise ValueError("Error reading {} file".format(GROUP_COMPOSITION_FILENAME))

    group_composition_dict = {}
    for composition, recording_paths in data.items():
        if len(recording_paths) == 0:
            raise ValueError("There is no recordings for composition {} in {}.".format(composition, GROUP_COMPOSITION_FILENAME))
        recordings = []
        for i, recording_path in enumerate(recording_paths):
            full_path = os.path.join(dataset_dir, recording_path)
            if not(os.path.exists(full_path) and os.path.isfile(full_path)):
                raise ValueError("File {}, mentioned in {}, is not found.".format(full_path, GROUP_COMPOSITION_FILENAME))

            name = os.path.basename(full_path)
            recordings.append(Recording(name, i, full_path))
        group_composition_dict[composition] = Composition(composition, recordings)

    return group_composition_dict


def find_and_parse_dtw_algs(dataset_dir, group_composition_dict):
    dtw_dir = os.path.join(dataset_dir, DTW_MAPS_DIRNAME)
    if not (os.path.exists(dtw_dir) and os.path.isdir(dtw_dir)):
        raise ValueError("Directory {} not found in dataset directory.".format(DTW_MAPS_DIRNAME))

    for alg_name in os.listdir(dtw_dir):
        dtf_alg_path = os.path.join(dtw_dir, alg_name)
        if not os.path.isdir(dtf_alg_path):
            raise ValueError("{} is not a directory".format(dtf_alg_path))

        for dwt_compose_filename in dtf_alg_path:
            composition, _ = os.path.splitext(dwt_compose_filename)
            dtw_compose_fullpath = os.path.join(dtf_alg_path, dwt_compose_filename)
            if composition in group_composition_dict.keys():
                parse_dtw_file_for_compose(alg_name, dtw_compose_fullpath, group_composition_dict[composition].recordings)


def parse_dtw_file_for_compose(alg_name, filepath, recordings):
    try:
        with open(filepath) as f:
            dwt_maps = json.load(f)
    except:
        raise ValueError("Error reading {}.".format(filepath))

    for rec in recordings:
        rec.dtw_maps[alg_name] = {}

    for pair, dtw_path in dwt_maps.item():
        for rec in recordings:
            if rec.number == pair[0]:
                rec.dtw_maps[alg_name][pair[1]] = create_map_from_dtw_path(dtw_path)
            if rec.number == pair[1]:
                rec.dtw_maps[alg_name][pair[0]] = create_inverse_map_from_dtw_path(dtw_path)

    for rec in recordings:
        if len(rec.dtw_maps[alg_name]) != len(recordings) - 1:
            raise ValueError("Not found all dtw paths for algorithm {} and composition {}".format(alg_name, rec.name))


def create_map_from_dtw_path(dtw_path):
    return dtw_path


def create_inverse_map_from_dtw_path(dtw_path):
    return dtw_path
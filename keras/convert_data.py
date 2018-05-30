import os
import random
import cv2
import numpy as np

NUM_VALIDATION = 3000;
#NUM_VALIDATION = 1;
RANDOM_SEED = 0;

def get_filenames_and_classes(dataset_dir):
    path_root = os.path.join(dataset_dir);
    directories = [];
    class_names = [];
    for filename in os.listdir(path_root):
        path = os.path.join(path_root, filename);
        if os.path.isdir(path):
            directories.append(path);
            class_names.append(filename);

    photo_filenames = [];
    for directory in directories:
        for filename in os.listdir(directory):
            path = os.path.join(directory, filename);
            photo_filenames.append(path)

    return photo_filenames, sorted(class_names);

def convert_imagefile(split_name, filename, class_names_to_ids, dataset_dir):
    assert split_name in ['train', 'validation']
    imageArray = [];
    labelsArray = [];
    for imagename in filename:
        #print(imagename);
        Image = cv2.imread(imagename);
        #print(Image.shape)          # shape: row col channel
 #       if Image.shape[0] != 50:
 #           print(imagename)
        imageArray.append(Image);
        
        class_name = os.path.basename(os.path.dirname(imagename));
        class_id = class_names_to_ids[class_name];
        #print(class_id);
        labelsArray.append(class_id);
    return imageArray, labelsArray;


def run(dataset_dir):
    photo_filenames, class_names = get_filenames_and_classes(dataset_dir);
    #print(class_names);
    class_names_to_ids = dict(zip(class_names, range(len(class_names))));
    #print(class_names_to_ids);

    random.seed(RANDOM_SEED);
    random.shuffle(photo_filenames);

    training_filenames = photo_filenames[NUM_VALIDATION: ];
    validation_filenames = photo_filenames[ :NUM_VALIDATION];
    ##print(validation_filenames);
    
    train_image, train_label = convert_imagefile('train', training_filenames, class_names_to_ids, dataset_dir);
    validation_image, validation_label = convert_imagefile('validation', validation_filenames, class_names_to_ids, dataset_dir);
    labels_to_class_names = dict(zip(range(len(class_names)), class_names));

    train_image_resize = np.array(train_image);
    validation_image_resize = np.array(validation_image);

    train_label_resize = np.array(train_label);
    validation_label_resize = np.array(validation_label);
    #print(labels_to_class_names);

    return train_image_resize,train_label_resize, validation_image_resize, validation_label_resize;



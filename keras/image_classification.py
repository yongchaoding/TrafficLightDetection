#!/usr/bin/env python
# coding=utf-8

import yaml
import os
import numpy as np
import cv2
import datetime
import time
from keras.models import model_from_yaml
import msgImagePath
import msgImageMsg


def model_load(model_file, weight):
    f = open(model_file);
    yaml_f = yaml.load(f);
    model = model_from_yaml(yaml_f);

    model.load_weights(weight, by_name=True);
    return model

def image_load(imagePath):
    images = [];
    starttime = time.time()
    for filename in os.listdir(imagePath):
        path = imagePath + filename;
        print(path);
        image = cv2.imread(path);
        images.append(image);
    image_np = np.array(images);
    endtime = time.time();
    print(image_np.shape);
    print("Read image Using Time: ", endtime - starttime)
    return image_np;

def model_predict(model, images):
    batch_size = images.shape[0];
    assert batch_size != 0
    starttime = time.time()
    result = model.predict(images, batch_size=batch_size);
    endtime = time.time();
    print("Using Model Time: ", endtime - starttime)
    res = np.argmax(result, axis=1)
    return res;


def run():
    model = model_load("model.yaml", "model.weight");
    image = image_load("Image/");
    starttime = time.time()
    res = model_predict(model, image);
    res = model_predict(model, image);
    res = model_predict(model, image);
    res = model_predict(model, image);
    res = model_predict(model, image);
    endtime = time.time();
    print(res);
    print("Using Total Time: ", endtime - starttime)

if __name__ == "__main__":
    run();

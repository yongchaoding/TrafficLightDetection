#!/usr/bin/env python
# coding=utf-8

import yaml
import os
import numpy as np
import cv2
import time
import thread
from keras.models import model_from_yaml
import msgImagePath
import msgImageMsg

CPP_PYTHON_MSG = [];
RECEIVER_FLAG = 0;

def CPP_PYTHON(channel, data):
    global CPP_PYTHON_MSG
    global RECEIVER_FLAG
    CPP_PYTHON_MSG = msgImagePath.decode(data);
    RECEIVER_FLAG = 1;

def PYTHON_CPP_PUB(LCM, ImageMsg):
    Num = len(ImageMsg)
    msg = msgImageMsg();
    msg.ImageNum = Num;
    msg.ImageMsg = ImageMsg;
    LCM.publish("PYTHON_CPP", msg.encode());

def LCMRUN(LCM):
    while True:
        LCM.handle();

def LCM_define():
    LCM = lcm.LCM();
    subscription = LCM.subscribe("CPP_PYTHON", CPP_PYTHON)
    try:
        thread.start_new_thread(LCMRUN, (LCM, ))
    except:
        print("THREAD ERROR!");
    return LCM

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
    LCM = LCM_define();
    model = model_load("model.yaml", "model.weight");
    while True:
        if RECEIVER_FLAG == 1:
            starttime = time.time()
            image = image_load("Image/");
            res = model_predict(model, image);
            endtime = time.time();
            print("Using Total Time: ", endtime - starttime)
            RECEIVER_FLAG = 0;
            PYTHON_CPP_PUB(LCM, res);

if __name__ == "__main__":
    run();

#!/usr/bin/env python
# coding=utf-8

import convert_data
import model_define
import numpy as np

train_image,train_labels, validation_image,validation_labels = convert_data.run("/home/yongchao/Dataset/Results");
print("Train Data Info:")
print(train_image.shape);
print(train_labels.shape);

print("Validation Data Info:")
print(validation_image.shape);
print(validation_labels.shape);

model_define.run(train_image, train_labels, validation_image, validation_labels, 9);

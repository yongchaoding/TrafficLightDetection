#!/usr/bin/env python
# coding=utf-8

import keras
from keras.models import model_from_yaml
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D
import yaml

batch_size = 128
epochs = 20

def run(x_train, y_train, x_test, y_test, class_nums):
    x_train = x_train.astype('float32');
    x_test = x_test.astype('float32');
    x_train /= 255;
    x_test /= 255;

    y_train = keras.utils.to_categorical(y_train, class_nums);
    y_test = keras.utils.to_categorical(y_test, class_nums);

    input_shape = (50, 50, 3);

    model = Sequential();
    model.add(Conv2D(32, kernel_size=(3,3),
                    activation='relu',
                    input_shape = input_shape));
    model.add(Conv2D(64, kernel_size=(3,3),
                    activation='relu'));
    model.add(MaxPooling2D(pool_size=(2,2)));
    model.add(Dropout(0.25));
    model.add(Flatten());
    model.add(Dense(128, activation='relu'));
    model.add(Dropout(0.5));
    model.add(Dense(class_nums, activation='softmax'));

    model.compile(loss = keras.losses.categorical_crossentropy,
                 optimizer = keras.optimizers.Adadelta(),
                 metrics = ['accuracy']);

    model.fit(x_train, y_train,
             batch_size = batch_size,
             epochs = epochs,
             verbose = 1,
             validation_data=(x_test, y_test));
    score = model.evaluate(x_test, y_test, verbose=0)

    yaml_string = model.to_yaml();
    yaml_f = open('model.yaml', 'w');
    yaml.dump(yaml_string, yaml_f);

    model.save_weights("model.weight");

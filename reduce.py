import sys

import tensorflow as tf
from tensorflow import keras
from keras import Model, models
from keras.utils import normalize

from Project_2.input import readImages
from Project_2.classification import encoder


# gpu fix
physical_devices = tf.config.experimental.list_physical_devices('GPU')
assert len(physical_devices) > 0, "Not enough GPU hardware devices available"
config = tf.config.experimental.set_memory_growth(physical_devices[0], True)


def readArgs():
    datasetGiven = False
    outFileGiven = False

    for i,arg in enumerate(sys.argv):
        if arg == '-d':
            dataset = sys.argv[i+1]
        elif arg == '-o':
            out_file = sys.argv[i+1]

    if not datasetGiven:
        dataset = input('Enter dataset path: ')
    if not outFileGiven:
        out_file = input('Enter output file path: ')

    return dataset, out_file


def minMaxNormalize(vec):
    vec_normd = []

    for elem in vec:
        norm = ((elem - vec.min())/(vec.max()-vec.min())) * 255
        vec_normd.append(int(round(norm)))

    return vec_normd

def normVecs(vecs):
    toRet = []

    for vec in vecs:
        toRet.append(minMaxNormalize(vec))

    return toRet


def saveLatentVectors(out, latent_dim, img_num, vectors):
    magic_num = 7
    rows = 1
    with open(out, 'wb') as fl:
        fl.write(magic_num.to_bytes(4, byteorder='big')) # magic num
        fl.write(img_num.to_bytes(4, byteorder='big')) # number of images
        fl.write(rows.to_bytes(4, byteorder='big')) # number of rows
        fl.write(latent_dim.to_bytes(4, byteorder='big')) # number of columns

        for vec in vectors:
            for elem in vec:
                fl.write(elem.to_bytes(1, byteorder='big'))


if __name__=='__main__':
    # read command line arguments or get them from the user
    dataset_fname, output = readArgs()

    images, imgNum, rows, cols = readImages(dataset_fname)

    # scaling i.e. normalization
    images = normalize(images)

    NN_path = input('Path to autoencoder: ')
    # load autoencoder model
    autoencoder = models.load_model(NN_path, compile=False)

    # 28 x 28 x 1 array per image, i.e. one channel for gray scale
    input_img = keras.Input(shape=(rows, cols, 1))

    # process encoder part of autoencoder
    NN, _ = encoder(autoencoder, input_img, False, None)

    # create reduction model
    neural_net = Model(input_img, NN, name='Reducer')

    neural_net.summary()


    # get latent vectors
    latent_imgs = neural_net.predict(images)

    # normalize to range 0-255
    latent_imgs_normed = normVecs(latent_imgs)

    latent_dim = neural_net.get_layer(name='latent_layer').get_config()['units']
    # save to binary file
    saveLatentVectors(output, latent_dim, imgNum, latent_imgs_normed)

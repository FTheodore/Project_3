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


# get arguments from user
def readArgs():
    datasetGiven = False
    outDatasetGiven = False
    queryGiven = False
    outQueryGiven = False

    for i,arg in enumerate(sys.argv):
        if arg == '-d':
            dataset = sys.argv[i+1]
            datasetGiven = True
        elif arg == '-od':
            outDataset = sys.argv[i+1]
            outDatasetGiven = True
        elif arg == '-q':
            query = sys.argv[i+1]
            queryGiven = True
        elif arg == '-oq':
            outQuery = sys.argv[i+1]
            outQueryGiven = True

    if not datasetGiven:
        dataset = input('Enter dataset file path: ')
    if not outDatasetGiven:
        outDataset = input('Enter output dataset file path: ')
    if not queryGiven:
        query = input('Enter query file path: ')
    if not outQueryGiven:
        outQuery = input('Enter output query file path: ')

    return dataset, outDataset, query, outQuery

# normalize vector to [0, 25500]
def minMaxNormalize(vec):
    vec_normd = []

    for elem in vec:
        norm = ((elem - vec.min())/(vec.max()-vec.min())) * 25500
        vec_normd.append(int(round(norm)))

    return vec_normd

# normalize all vectors
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
                fl.write(elem.to_bytes(2, byteorder='big')) # write pixel value


if __name__=='__main__':

    # read command line arguments or get them from the user
    dataset, outDataset, query, outQuery = readArgs()

    dImages, dImgNum, rows, cols = readImages(dataset)
    qImages, qImgNum, _, _ = readImages(query)

    # scaling i.e. normalization
    dImages = normalize(dImages)
    qImages = normalize(qImages)

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
    latent_imgs_dataset = neural_net.predict(dImages)
    latent_imgs_query = neural_net.predict(qImages)

    # normalize to range 0-25500
    latent_imgs_dataset_normed = normVecs(latent_imgs_dataset)
    latent_imgs_query_normed = normVecs(latent_imgs_query)

    latent_dim = neural_net.get_layer(name='latent_layer').get_config()['units']
    # save to binary file
    saveLatentVectors(outDataset, latent_dim, dImgNum, latent_imgs_dataset_normed)
    saveLatentVectors(outQuery, latent_dim, qImgNum, latent_imgs_query_normed)


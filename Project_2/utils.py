from tensorflow import keras
from keras.layers import Conv2D, BatchNormalization, UpSampling2D, MaxPooling2D
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from pylab import *
from mpl_toolkits.mplot3d import Axes3D

import os
if os.getcwd().split('/')[-1] == 'Project_2':
    from input import readVal
else:
    from .input import readVal

import seaborn as sns

# fancy graphics
plt.style.use('seaborn')
plt.tight_layout()

# ask user for next layer type and layer parameters
def nextLayer(convLayers, poolingLayers, forbidPool):
    layer = {}
    prompt ="""
Please specify next layer,
Convolutional layers left: {}
Pooling layers left: {}
Enter [conv|pool]: """.format(convLayers, poolingLayers)

    layer['type'] = input(prompt)
    # don't allow inefficient architectures, e.g. all convolutions first and then all pools
    if poolingLayers == 0 or forbidPool: # no pools left or previous layer is pool or this is first layer
        while layer['type'] != 'conv':
            layer['type'] = input('You can only add a Convolutional layer, re-enter: ')
    elif convLayers == 0 or (poolingLayers == 2 and convLayers == 1): # second operand prevents dead-end
        while layer['type'] != 'pool':
            layer['type'] = input('You can only add a Pooling layer, re-enter: ')
    else: # can add any of the two, i.e. convolution or pool
        while layer['type'] not in ('conv','pool'):
            layer['type'] = input('Wrong input, re-enter: ')

    if layer['type'] == 'conv':
        convLayers -= 1
        layer['filter_num'] = readVal(1, 'Specify number of filters: ')
        layer['filter_size'] = readVal(1, 'Specify kernel size: ')
    else:
        poolingLayers -= 1

    return layer, convLayers, poolingLayers

# add layer of neurons specified by user
def addLayer(layer_params, input): # layer_params is a dict with layer-specific information
    if layer_params['type'] == "conv": # layer that extracts features from subset of input
        NN= Conv2D(layer_params['filter_num'], layer_params['filter_size'],\
            activation= 'relu', padding= 'same')(input)
        return BatchNormalization()(NN) # scaling
    elif layer_params['type'] == "pool":
        return MaxPooling2D((2,2))(input) # reduce number of parameters and keep important learned features
    elif layer_params['type'] == "upsample":
        return UpSampling2D((2,2))(input) # get back to initial dimensions

def plotLoss(losses, loss_fn, ep_first_phase=None):
    #Plotting the validation and training errors
    x_axis = range(1,len(losses['loss'])+1)

    if ep_first_phase is not None:
        plt.axvline(ep_first_phase, c='teal', linestyle='--', label='End of first training phase')
    plt.plot(x_axis, losses['loss'], label='Training loss', c='orange')
    plt.plot(x_axis, losses['val_loss'], label='Validation loss', linestyle='-.', c='brown', linewidth=2)

    plt.xlabel('Epochs')
    plt.ylabel(loss_fn)
    plt.title('Training and validation loss')
    plt.legend()
    plt.show()

def nextAction():
    prompt = """
Enter one of 1, 2, 3, 4, 5, 6:
\t1) repeat process
\t2) plot training and validation loss over epochs
\t3) plot hyperparameters over losses until now
\t4) save model
\t5) save losses during training (for research)
\t6) exit
"""
    action = input(prompt)
    while action not in {'1','2','3','4','5','6'}:
        action = input("Wrong input, enter again: ")

    return int(action)


# save hyperparameters and losses for plotting
def saveInfo(convLayersNum, batchSize, epochs, train_loss, val_loss, saveInto):
    sv = \
    {
        'cnv': convLayersNum,
        'btchSz': batchSize,
        'ep': epochs,
        'tr_l': train_loss,
        'val_l': val_loss
    }
    saveInto.append(sv)

def plotAll(saves):
    convNums, batchSizes, epochs, train_l, val_l = [],[],[],[],[]
    for save in saves:
        convNums.append(save['cnv'])
        batchSizes.append(save['btchSz'])
        epochs.append(save['ep'])
        train_l.append(save['tr_l'])
        val_l.append(save['val_l'])

    # make two 3d heatmaps, one for training loss and one for validation loss
    fig = plt.figure(figsize=(24,18))
    for i,loss in enumerate((train_l,val_l)):
        ax = fig.add_subplot(121+i, projection='3d')

        colmap = cm.ScalarMappable(cmap=cm.YlOrRd)
        colmap.set_array(train_l)

        ax.scatter(convNums,batchSizes,epochs,marker='s',s=140,c=loss,cmap='YlOrRd')
        fig.colorbar(colmap)
        ax.set_xlabel('Convolution layers')
        ax.set_ylabel('Batch Size')
        ax.set_zlabel('Epochs')
        ax.grid(True)
        ax.set_title('Training Loss') if i == 0 else ax.set_title('Validation Loss')

    plt.show()

'''
in case model is pre-trained, helper method for finding number
of convolutional layers in neural network
'''
def getConvLayersHndl():
    convNum = [0] # use wrapper list

    def lookup_summary(line):
        if 'Conv2D' in line:
            convNum[0] += 1

    return lookup_summary, convNum






import sys
import matplotlib.pyplot as plt
import math

# fancy graphics
plt.style.use('seaborn')
plt.tight_layout()

class FilePaths:

    def __init__(self):
        self.train_set_fname = None
        self.train_labels_fname = None
        self.test_set_fname = None
        self.test_labels_fname = None
        self.autoenc_model_fname = None


    # read command line arguments
    def setPaths(self):
        for i,arg in enumerate(sys.argv):
            if arg == '-d':
                self.train_set_fname = sys.argv[i+1]
            elif arg == '-dl':
                self.train_labels_fname = sys.argv[i+1]
            elif arg == '-t':
                self.test_set_fname = sys.argv[i+1]
            elif arg == '-tl':
                self.test_labels_fname = sys.argv[i+1]
            elif arg == '-model':
                self.autoenc_model_fname = sys.argv[i+1]

        self.askUser()

    def isNone(self, val):
        if val == None:
            return True

        return False

    # get values for unspecified args on exec time
    def askUser(self):
        if self.isNone(self.train_set_fname):
            self.train_set_fname = input('Training set path: ')

        if self.isNone(self.train_labels_fname):
            self.train_labels_fname = input('Training labels path: ')

        if self.isNone(self.test_set_fname):
            self.test_set_fname = input('Test set path: ')

        if self.isNone(self.test_labels_fname):
            self.test_labels_fname = input('Test labels path: ')

        if self.isNone(self.autoenc_model_fname):
            self.autoenc_model_fname = input('Autoencoder model path: ')

'''
~~~~~~~~~ END OF CLASS
'''

# used for dropout, i.e. locate convolutional layers
def getConvLayersIdx(autoencoder):
    convLayers = []

    for layer in autoencoder.layers:
        if 'conv2d' in layer.name:
            convLayers.append(True)
        else:
            convLayers.append(False)

    return convLayers

'''
ask user if he wants dropout layers to be added
in order to prevent overfitting
'''
def askDropout():
    prompt = """
Enter one of 1, 2, 3, 4:
\t1) no dropout layers
\t2) dropout layers after convolution layers
\t3) dropout layer after fully connected layer
\t4) both 2 and 3 (RECOMMENDED)
"""
    action = input(prompt)
    while action not in {'1','2','3','4'}:
        action = input("Wrong input, enter again: ")

    return int(action)

'''
if dropout layers to be added, ask for the probability
of a neuron getting dropped out
'''
def readProb(mssg):
    while True:
        try:
            prob = float(input(mssg))
            if prob >= 1.0 or prob <= 0.0:
                raise Exception
            break
        except:
            print('Input error!')

    return prob

'''
convert user input into method arguments
'''
def actionToBool(dropoutAction):
    # returns two bools (dropout after convolution, >> after full conn layer)
    if dropoutAction == 1:
        return False, False
    elif dropoutAction == 2:
        return True, False
    elif dropoutAction == 3:
        return False, True
    else:
        return True, True


'''
load weights of encoder from pre-trained autoencoder(first task)
and make them non trainable
'''
def get_setUntrainable(classifier, autoencoder, encLayers):
    counter = 0
    for i in range(encLayers):
        class_idx = i + counter
        if 'dropout' in classifier.layers[class_idx].name: # skip dropout layer
            class_idx += 1
            counter += 1

        # get trained weights from autoencoder
        classifier.layers[class_idx].set_weights(autoencoder.layers[i].get_weights())
        # don't train the weights in first training phase
        classifier.layers[class_idx].trainable = False


'''
make weights of encoder trainable
'''
def setTrainable(classifier, encLayers):
    counter = 0
    for i in range(encLayers):
        class_idx = i + counter
        if 'dropout' in classifier.layers[class_idx].name: # skip dropout layer
            class_idx += 1
            counter += 1

        # train the weights in second training phase
        classifier.layers[class_idx].trainable = True


def nextAction():
    prompt = """
Enter one of 1, 2, 3, 4, 5:
\t1) repeat
\t2) plot training and validation loss over epochs (current experiment)
\t3) predict classes of images in test set
\t4) plot training and validation loss over hyperparameters (all experiments)
\t5) save model
\t6) save losses during training (for research)
\t7) exit
"""
    action = input(prompt)
    while action not in {'1','2','3','4','5','6','7'}:
        action = input("Wrong input, enter again: ")

    return int(action)

def concatErrorHistory(phaseOne, phaseTwo): # concatenate lists with losses from two training phases
    toRet = {}

    toRet['loss'] = phaseOne.history['loss'] + phaseTwo.history['loss']
    toRet['val_loss'] = phaseOne.history['val_loss'] + phaseTwo.history['val_loss']

    return toRet

'''
loss over epochs plot for all experiments
'''
def plotAll(experiments): # argument is list with hyperparams and train/val losses
    fig, axes = plt.subplots(1,len(experiments), sharey=True)
    axes = axes.ravel()

    for ax,exp in zip(axes,experiments):
        ax.yaxis.set_tick_params(labelbottom=True)
        x_axis = range(1,exp['ep']+1)
        ax.plot(x_axis, exp['loss'], label='Training loss', c='orange')
        ax.plot(x_axis, exp['val_loss'], label='Validation loss', linestyle='-.', c='brown', linewidth=2)
        ax.axvline(exp['secPhase'], c='teal', linestyle='--', label='End of first training phase')
        ax.set_xlabel('Epochs')
        ax.set_ylabel('Cross Entropy')
        ax.set_title(f'Epochs: {exp["ep"]}\nBatch Size: {exp["bSz"]}\nFully Conn Layer Nodes: {exp["fc"]}')


    plt.legend()
    plt.show()


# save hyperparameters and losses for plotting
def saveInfo(batchSize, fcNodes, epochs, train_loss, val_loss, phaseEnd, saveInto):
    sv = \
    {
        'bSz': batchSize,
        'fc': fcNodes,
        'ep': epochs,
        'loss': train_loss,
        'val_loss': val_loss,
        'secPhase': phaseEnd
    }
    saveInto.append(sv)


# show images and their predicted values
def showImgs(count, indexes, allImages, preds, labels):
    subplt_dim = math.ceil(math.sqrt(count))

    for i,idx in enumerate(indexes):
        if i >= count:
            break

        plt.subplot(subplt_dim,subplt_dim,i+1)
        plt.imshow(allImages[idx].reshape(28,28), cmap='gray', interpolation='none')
        plt.grid(False)
        plt.xticks([])
        plt.yticks([])
        plt.title(f'Predicted:{preds[idx]}, Label:{labels[idx]}')
        plt.tight_layout()

    plt.show()







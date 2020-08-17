# classifierAgents.py
# parsons/07-oct-2017
#
# Version 1.0
#
# Some simple agents to work with the PacMan AI projects from:
#
# http://ai.berkeley.edu/
#
# These use a simple API that allow us to control Pacman's interaction with
# the environment adding a layer on top of the AI Berkeley code.
#
# As required by the licensing agreement for the PacMan AI we have:
#
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from pacman import Directions
from game import Agent
import api
import random
import game
import util
import sys
import os
import csv
import numpy as np
from sklearn import tree
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import chi2
from sklearn.model_selection import train_test_split
from collections import Counter
from collections import namedtuple

# ClassifierAgent
#
# An agent that runs a classifier to decide what to do.

SplitInfo = namedtuple("SplitInfo", "split_data gain feature value")
SplitData = namedtuple("SplitData", "left right left_indices right_indices")

def prune_features(features, target, k=10):
    """Function that selects the k best features from a dataset and returns the pruned dataset

    """
    SelectKBest(chi2, k).fit(features, target)
    #First remove all features that have the same value
    return SelectKBest(chi2, k).fit_transform(features, target)


def convertToArray(numberString):
        numberArray = []
        for i in range(len(numberString) - 1):
            if numberString[i] == '0':
                numberArray.append(0)
            elif numberString[i] == '1':
                numberArray.append(1)
            elif numberString[i] == '2':
                numberArray.append(2)
            elif numberString[i] == '3':
                numberArray.append(3)

        return numberArray

def get_data_from_file(file):
        """Parameterized version of the registerInitialState function

        Gets move data from a named file, and returns them as a tuple of data and target
        """
        datafile = open(file, 'r')
        content = datafile.readlines()
        datafile.close()

        # Now extract data, which is in the form of strings, into an
        # array of numbers, and separate into matched data and target
        # variables.
        data = []
        target = []
        # Turn content into nested lists
        for i in range(len(content)):
            lineAsArray = convertToArray(content[i])
            dataline = []
            for j in range(len(lineAsArray) - 1):
                dataline.append(lineAsArray[j])

            data.append(dataline)
            targetIndex = len(lineAsArray) - 1
            target.append(lineAsArray[targetIndex])

        return data, target

def run(test_size1=0.1, test_size2=0.2, k=10):
    trees = []
    trees2 = []
    scores = []
    scores2 = []
    data, target = get_data_from_file('good-moves.txt')
    data = prune_features(data, target, k).tolist()
    for i in range(10):
        X_train, X_test, y_train, y_test = train_test_split(data, target, test_size=test_size1)
        tmp_tree = DTClassifier(X_train, y_train).fit()
        scores.append(tmp_tree.score(X_test, y_test))
        trees.append(tmp_tree)

        X_train, X_test, y_train, y_test = train_test_split(data, target, test_size=test_size2)
        tmp_tree = DTClassifier(X_train, y_train).fit()
        scores2.append(tmp_tree.score(X_test, y_test))
        trees2.append(tmp_tree)

    print np.array(scores)
    print np.array(scores2)
    print np.array(scores).mean()
    print np.array(scores2).mean()


    return trees, trees2, scores, scores2

def do_all(k=10, t1=0.1, t2=0.2):
    D, Y = get_data_from_file('moves1.txt')
    D = prune_features(D, Y, k).tolist()

    trees, trees2, scores, scores2 = run(t1, t2, k)

    A = map(lambda t: t.score(D, Y), trees)
    B = map(lambda t: t.score(D, Y), trees2)

    print np.array(A).mean()
    print np.array(B).mean()

    return A, B

class DTClassifier():
    """My own decision tree classifier class.

    Loosely based on code from https://github.com/random-forests/tutorials/blob/master/decision_tree.py, however I have
    made my own modifications and improvements
    """
    def __init__(self, data=None, target=None):
        """DTClassifier constructor

        The data to be classified can be passed to the constructor, otherwise the default initialization
        is from good-moves.txt
        """
        # print "DTClassifier made"
        if data == None or target is None:
            self.registerInitialState()
        else:
            # print "Data passed"
            self.data = data
            self.target = target

    def predict(self, data):
        """Returns the prediction/classification for one row of features
        """
        return self.root.predict(data)

    def score(self, data, target):
        """Returns the percentage of correct classifications made from a dataset

        It is assumed data and target are the same length
        """
        correct = 0

        for i in range(0, len(target)):
            correct += self.predict(data[i]) == target[i]

        return float(correct) / len(target)

    def registerInitialState(self):
        """Initializes data and target from good-moves.txt
        """
        # print "Registering state"
        self.data, self.target = get_data_from_file('good-moves.txt')
        

    def gini(self, labels):
        """Calculates the gini impurity of a set/list of classes
        """

        impurity = 0

        #Counter is a dictionary subclass that counts each
        #occurrence of an item in a list.
        #If labels is not a Counter, it is presumed to be a list, and is
        #Converted to a counter
        if not isinstance(labels, Counter):
            labels = Counter(labels)

        #total number of classes/labels in the dataset
        total = sum(labels.values())
        
        #Loops over the classes and calculates the gini impurity
        for lbl in labels:
            p = (labels[lbl]/float(total))
            impurity += p*(1-p)

        return impurity



    def information_gain(self, current_gini, left, right):
        """Calculates the information gained by splitting the dataset"""
        left_gini, right_gini = self.gini(left), self.gini(right)
        total = sum(left.values()) + sum(right.values())
        p = sum(left.values()) / float(total)
        return current_gini - p*left_gini - (1-p)*right_gini



    def split(self, feature_list, labels, value):
        """Funtion that splits a dataset into two halves 

        feature_list and labels are both the same length
        feature_list[i] is the value of one particular feature of labels[i]

        The function will separate the labels into two halves, ones where
        feature[i] == value and ones where it doesn't"""
        left = Counter()
        right = Counter()
        #List of indices used to keep track of what classes are in each branch
        left_indices, right_indices = [], []
        for i in range(len(feature_list)):
            if feature_list[i] == value:
                left[labels[i]] += 1
                left_indices.append(i)
            else:
                right[labels[i]] += 1
                right_indices.append(i)

        return SplitData(left, right, left_indices, right_indices)




    def best_split(self, data, labels):
        """Finds the best split of data set that yields the most information gain

        For each feature, it will try all the values of the feature that show up in the
        dataset, and split the dataset based on labels that have that value for that feature
        and labels that do not.

        Howbeit, as we know already that the only values in this dataset and 0 and 1, it is a bit
        redundant to find all the values for each feature; nevertheless I implemented the general case
        for completeness.

        Returns:
            left and right branches of the split data
            """

        # print "Splitting dataset"
        # print np.array(labels)

        num_features = len(data[0])
        data_array = np.array(data)
        #Impurity of the current root to be split
        current_gini = self.gini(Counter(labels))
        best_split_data, best_feature, best_value, best_gain  = (None,)* 4

        for i in range(num_features):
            #Just want the ith feature for all rows in the dataset
            feature_list = data_array[ :, i]
            unique_vals = set(feature_list)
            for v in unique_vals:
                #Splits the data based on the value
                split_data = self.split(feature_list, labels, v)
                
                #If the given feature and value don't split the data set (e.g. all rows have the same value for that feature)
                #Then ignore this split and continue
                #However, this case can be dealt with beforehand by doing feature selection to remove useless features,
                #So this part is kind of redundant, but I have left it here
                if len(split_data.left_indices) == 0 or len(split_data.right_indices) == 0:
                    split_data = None
                    continue
                gain = self.information_gain(current_gini, split_data.left, split_data.right)
                if gain > best_gain:
                    # print "{} > {} == {}".format(gain, best_gain, gain > best_gain)
                    # print "left", split_data.left
                    # print "right", split_data.right
                    best_gain = gain
                    best_feature = i
                    best_value = v
                    best_split_data = split_data

        #if best_gain is None then it means that no split was found, so None will be returned
        #Otherwise it returns the SplitInfo about the split
        return SplitInfo(best_split_data, best_gain, best_feature, best_value) if best_gain else None



    def fit(self, data=None, target=None, depth=0, threshold=0):
        """Fits/trains the tree from the given data.

        data and target assumed to be the same length.
        If none are given, the tree is trained with its stored data

        depth is a variable used to track the current depth of the tree at each recursion.
        Used for debugging and printing purposes, as well as ensuring tree doesn't exceed a certain depth.

        threshold the minimum amount of gain needed to qualify for a split. 
        E.g. if the information gain for a split does not exceed threshold, the data is not split
        """
        if data or target is None:
            self.root = self.train(self.data, self.target)
        else:
            self.root = self.train(data, target)

        return self

    def train(self, features, labels, parent_examples=None, depth=0, threshold=0):
        """Refer to docstring above

        parent_examples used for plurality classification if labels is empty
        """
        
        if len(Counter(labels)) == 1:
            return AnswerNode(labels)
        elif len(features) == 0:
            return AnswerNode(labels)
        elif len(labels) == 0:
            return AnswerNode(parent_examples)


        #Finds the best split for the dataset
        new_split = self.best_split(features, labels)
        # print new_split
        # new_split is None if no split is found
        if new_split is None or new_split.gain <= threshold:
            # print new_split.gain
            # print "making AnswerNode"
            return AnswerNode(labels)
        
        # This removes the feature that was split on from the list of features, so that it is not considered again for a split
        # Returns a view of the data; doesn't modify it
        features = np.delete(features, new_split.feature, axis=1)

        split_data = new_split.split_data

        #Setup arguments for recursion
        left_features, left_labels, right_features, right_labels = [], [], [], []
        
        #Uses the indices to reconstruct the data structures needed, ensuring that the indices of each list match up
        for i in split_data.left_indices:
            left_features.append(features[i])
            left_labels.append(labels[i])

        for i in split_data.right_indices:
            right_features.append(features[i])
            right_labels.append(labels[i])

        # print "Left", depth
        left_branch = self.train(left_features, left_labels, labels, depth + 1)
        # print "Right", depth
        right_branch = self.train(right_features, right_labels, labels, depth + 1)
        
        # print new_split
        return DTNode(left_branch, right_branch, new_split.feature, new_split.value)



class AnswerNode():
    """AnswerNode class used to given a classification

    AnswerNode is a leaf of the tree and has no children.
    """
    def __init__(self, labels):
        """Constructor
        
        Node is initialized with a list of labels. The node represents a branch of a split
        It will return a classification based on the classes in the branch"""
        self.labels = labels
        # print "AnswerNode made"

    def predict(self):
        """Returns a classification based on the labels
        
        Picks a random classification from the labels and returns it.
        Labels with higher frequency more likely to be returned"""
        return random.choice(self.labels)




class DTNode():
    """(D)ecision (T)ree Node used to branch based on a decision"
    
    DTNode always has children"""
    def __init__(self, left=None, right=None, index=None, value=None):
        """Constructor
        
        left and right are the branches.
        The index is the index of the feature in the feature list that was used to split the dataset
        at this point.
        value is the value of the corresponding feature"""
        self.left = left
        self.right = right
        self.index = index
        self.value = value

    def predict(self, features):
        """Returns the classification of the given list of features.
        
        features is a one dimensional list of features, equivalent to one line of good-moves.txt"""
        # print np.array(features)
        # print "deciding on", self.index
        if features[self.index] == self.value:
            branch = self.left
        else:
            branch = self.right
        if (isinstance(branch, AnswerNode)):
            return branch.predict()
        else:
            # To maintain consistency with the method that was used to construct the tree,
            # The feature used to split is remove from the list.
            return branch.predict(np.delete(features, self.index, axis=0))







class ClassifierAgent(Agent):

    # Constructor. This gets run when the agent starts up.
    def __init__(self):
        print "Initialising"

    # Take a string of digits and convert to an array of
    # numbers. Exploits the fact that we know the digits are in the
    # range 0-4.
    #
    # There are undoubtedly more elegant and general ways to do this,
    # exploiting ASCII codes.

    def convertNumberToMove(self, number):
        if number == 0:
            return Directions.NORTH
        elif number == 1:
            return Directions.EAST
        elif number == 2:
            return Directions.SOUTH
        elif number == 3:
            return Directions.WEST

    def convertToArray(self, numberString):
        numberArray = []
        for i in range(len(numberString) - 1):
            if numberString[i] == '0':
                numberArray.append(0)
            elif numberString[i] == '1':
                numberArray.append(1)
            elif numberString[i] == '2':
                numberArray.append(2)
            elif numberString[i] == '3':
                numberArray.append(3)

        return numberArray
                
    # This gets run on startup. Has access to state information.
    #
    # Here we use it to load the training data.
    def registerInitialState(self, state):

        # open datafile, extract content into an array, and close.
        self.datafile = open('moves1.txt', 'r')
        content = self.datafile.readlines()
        self.datafile.close()

        # Now extract data, which is in the form of strings, into an
        # array of numbers, and separate into matched data and target
        # variables.
        self.data = []
        self.target = []
        # Turn content into nested lists
        for i in range(len(content)):
            lineAsArray = self.convertToArray(content[i])
            dataline = []
            for j in range(len(lineAsArray) - 1):
                dataline.append(lineAsArray[j])

            self.data.append(dataline)
            targetIndex = len(lineAsArray) - 1
            self.target.append(lineAsArray[targetIndex])

        
        self.__init_classifier__()


    def gen_trees(self, num_trees=50, test_size=0.3):
        """Generate trees used for classification """
        trees = []
        for i in range(num_trees):
            X_train, X_test, y_train, y_test = train_test_split(self.data, self.target, test_size=test_size)
            tmp_tree = DTClassifier(X_train, y_train).fit()
            trees.append(tmp_tree)
        return trees

    def __init_classifier__(self):
        #This trains the classifier on the dataset.
        # Basic outline: train 20 trees, then do majority vote to determine the classification
        self.pruner = SelectKBest(chi2, 'all').fit(self.data, self.target)
        self.data = self.pruner.transform(self.data).tolist()
        self.trees = self.gen_trees()

    def move_from_trees(self, features):
        """ Gets classification for the row of features from the decision trees"""
        print "move_from_trees"
        moves = map(lambda t: t.predict(features), self.trees)
        return max(set(moves), key=moves.count)



    def classify(self, features, legal):
        features = self.pruner.transform([features])[0].tolist()
        """Get classification from each tree, then do majority vote.
        If the classification is not a legal move, then this is repeated a number of times
        to try and get a legal move, otherwise a random legal move is returned
        otherwise just return a legal move."""
        print features
        move = self.move_from_trees(features)
        print "move is {}".format(move)
        i = 0
        while self.convertNumberToMove(move) not in legal:
            if i > 20:
                return random.choice(legal)
            move = self.move_from_trees(features)
            i+=1
        return self.convertNumberToMove(move)




    # Tidy up when Pacman dies
    def final(self, state):

        print "I'm done!"
        
        # *********************************************
        #
        # Any code you want to run at the end goes here.
        #
        # *********************************************

    def getAction(self, state):

        # How we access the features.
        features = api.getFeatureVector(state)
        

        # Get the legal moves we can try.
        legal = api.legalActions(state)

        move = self.classify(features, legal)
        
        return api.makeMove(move, legal)

    

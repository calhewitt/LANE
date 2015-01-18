#!/usr/bin/env python2
import os, sys, glob

class Frame:
    clusters = []
    meta = {}

def splitLine(l):
    key = ''
    value = ''
    finishedKey = False
    for c in l:
        if c == ' ':
            finishedKey = True
        elif finishedKey:
            value += c
        else:
            key += c
    return key, value.strip()

def getPaths(path):
    paths = glob.glob(os.path.join(path, 'BCA_*.txt'))
    return paths    
    
def readFile(path):
    frameMeta = ('TimeStamp','ShutterTime')
    f = open(path)
    lines = f.readlines()
    f.close()
    frames = []
    channel = -1
    cluster = {}
    clusterValues = [] #the values read in for the current cluster 
    currentFrame = Frame()
    for l in lines:
        if l == '\n':
            continue
        key, value = splitLine(l)
        if key == 'Frame':
            currentFrame.clusters.append(cluster)
            cluster = {}
            currentFrame = Frame()
            frames.append(currentFrame)
            currentFrame.meta = {}
            currentFrame.clusters = []
            currentFrame.meta['ID'] = int(value)
            currentFrame.meta['Channel'] = channel
            clusterValues = []
        elif key == 'Channel':
            channel = int(value)
        elif key in frameMeta:
            currentFrame.meta[key] = float(value)
        elif key in clusterValues:
            currentFrame.clusters.append(cluster)
            cluster = {}
        else:
            cluster[key] = value
    return frames
    
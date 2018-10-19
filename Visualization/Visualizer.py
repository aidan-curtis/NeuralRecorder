#!/usr/bin/python

from pyqtgraph.Qt import QtGui, QtCore
import numpy as np
import pyqtgraph as pg
import random
from pyqtgraph.ptime import time
import sys


class Visualizer:
    def __init__(self, generator):
        self.app = QtGui.QApplication([])

        if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
            QtGui.QApplication.instance().exec_()

        self.generator = generator
        self.num_channels = generator.num_channels
        self.streams = [[] for _ in range(self.num_channels)]

        self.x_range = 5000
        self.update_speed = 64
        # Colors = [yellow, purple, blue, green, red, pink, grey, orange]
        self.colors = [(255, 235, 59), (156, 39, 176), (3, 169, 244), (76, 175, 80), (244, 67, 54), (233, 30, 99), (158, 158, 158), (255, 87, 54)]

        assert self.generator.load_chunk_size/float(self.update_speed)==int(self.generator.load_chunk_size/self.update_speed)
        self.setup()
        self.plot()

    def setup(self):
        self.w = QtGui.QMainWindow()
        self.cw = pg.GraphicsLayoutWidget()

        self.w.show()
        self.w.resize(400,600)
        self.w.setCentralWidget(self.cw)

        self.plots = []
        self.curves = []
        self.scatter_plots = []

        for i in range(self.num_channels):
            self.plots.append(self.cw.addPlot(row=i, col=0))
            self.plots[i].hideAxis('bottom')
            self.plots[i].setRange(QtCore.QRectF(0, -1000, self.x_range, 2000))
            self.curves.append(self.plots[i].plot())
            self.curves[i].setPen(pg.mkPen(color=self.colors[i]))

        for i in range(self.num_channels):
            self.scatter_plots.append(self.cw.addPlot(row=i, col=1))
            s1 = pg.ScatterPlotItem(size=4, pen=pg.mkPen(None), brush=pg.mkBrush(self.colors[i][0], self.colors[i][1], self.colors[i][2], 120))
            pos = np.random.normal(size=(2,100), scale=1e-5)
            spots = [{'pos': pos[:,i], 'data': 1} for i in range(100)] + [{'pos': [0,0], 'data': 1}]
            s1.addPoints(spots)
            self.scatter_plots[-1].addItem(s1)

        self.app.processEvents()  # force complete redraw for every plot



    def add_data_to_stream(self, chunk, ch_index):
        self.streams[ch_index]+=list(chunk)
        if(len(self.streams[ch_index])>self.x_range):
            self.streams[ch_index] = list(np.array(self.streams[ch_index])[len(self.streams[ch_index])-self.x_range:len(self.streams[ch_index])])

    def plot(self):
        print(self.curves)
        while(not self.generator.done):
            generated_data = self.generator.get_data_chunk()
            chunk_index=0
            while(chunk_index*self.update_speed<self.generator.load_chunk_size):
                for ch in range(self.num_channels):
                    self.add_data_to_stream(generated_data[chunk_index*self.update_speed:(chunk_index+1)*self.update_speed,ch], ch)
                    self.curves[ch].setData(self.streams[ch])
                chunk_index+=1
                self.app.processEvents()  # force complete redraw for every plot






        
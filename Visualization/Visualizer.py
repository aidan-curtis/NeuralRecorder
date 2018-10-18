#!/usr/bin/python

from pyqtgraph.Qt import QtGui, QtCore
import numpy as np
import pyqtgraph as pg
import random
from pyqtgraph.ptime import time


class Visualizer:
    def __init__(self):
        import sys
        if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
            QtGui.QApplication.instance().exec_()


        self.setup()

    def give_data_chunk(self):

        # for i in range(num_channels):
        #     plots.append(cw.addPlot(row=i, col=0))

        #     plots[-1].hideAxis('bottom')

        #     plots[-1].setRange(QtCore.QRectF(0, -10, 1000, 40))
        #     curves.append(plots[-1].plot())

        pass 


    def setup(self):
        num_channels = 8
        app = QtGui.QApplication([])
        w = QtGui.QMainWindow()
        cw = pg.GraphicsLayoutWidget()

        w.show()
        w.resize(400,600)
        w.setCentralWidget(cw)


        plots = []
        curves = []


    def plot(self, x_value, x_filt, pos_thresh, neg_thresh)
        # What should we do if there is no data in the queue ... NOTHING
        scatter_plots = []

        for i in range(num_channels):
            scatter_plots.append(cw.addPlot(row=i, col=1))
            s1 = pg.ScatterPlotItem(size=4, pen=pg.mkPen(None), brush=pg.mkBrush(255, 255, 255, 120))
            pos = np.random.normal(size=(2,100), scale=1e-5)
            spots = [{'pos': pos[:,i], 'data': 1} for i in range(100)] + [{'pos': [0,0], 'data': 1}]
            s1.addPoints(spots)
            scatter_plots[-1].addItem(s1)
            

        #Gernating synthetic data streams
        datas = [[0] for _ in range(num_channels)]
        for j in range(num_channels):
            for i in range(10000):
                    datas[j].append(datas[j][-1]+random.uniform(-1, 1))


        fps = None
        total = 0

    def update(self):

        for ch in range(num_channels):
            curves[ch].setData(np.array(datas[ch][0:total]))


        total+=1

        app.processEvents()  # force complete redraw for every plot

    timer = QtCore.QTimer()
    timer.timeout.connect(update)
    timer.start(0)

        
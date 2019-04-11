import csv
import numpy as np
import matplotlib.pyplot as plt
from bitstring import BitArray
import binascii

y = []
i = 0
with open('./intanresult/result0.txt') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    time = []
    for row in csv_reader:
        if i > 1:
            t = float(row[0])
            # if (t > 2):
            MISO_str = (row[3].split("  "))[1] + (row[3].split("  "))[2] + (row[3].split("  "))[3]+(row[3].split("  "))[4]
            MISO = BitArray(bin=MISO_str)
            MISO = int.from_bytes(MISO, byteorder="big", signed=True)
            y.append(MISO)
            time.append(t)
        i += 1
        # if(i==):
        #     break
nptime = np.array(time)
nptime = nptime - nptime[0]
plt.plot(nptime, y)
plt.show()
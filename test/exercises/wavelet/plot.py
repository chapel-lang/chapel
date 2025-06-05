import sys
import numpy as np
import matplotlib.pyplot as plt
import math

# get data
num_levels = int(sys.argv[1])
signal = np.loadtxt("results/signal.txt")
coeffs = np.loadtxt("results/coeffs.txt")

fig, ax = plt.subplots(num_levels + 1, 1)

# plot signal
ax[0].plot(signal)
ax[0].set_title("ecg signal")

# plot detail layers
fmin = 0
fmax = math.floor(len(coeffs) / 2**num_levels)
for i in range(0, num_levels):
    idx = num_levels - i
    ax[idx].plot(coeffs[fmin:fmax])
    fmin += fmax
    fmax *= 2
    ax[idx].set_title("detail level {}".format(idx))

fig.tight_layout()
fig.savefig("results/ecg_dwt.png", dpi=200)

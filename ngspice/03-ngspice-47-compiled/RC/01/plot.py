import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("rc.dat", skiprows=1)

t = data[:, 0]
vin = data[:, 1]
vout = data[:, 2]

plt.plot(t * 1e3, vin, label="v(in)")
plt.plot(t * 1e3, vout, label="v(out)")

plt.xlabel("Tempo [ms]")
plt.ylabel("Tensão [V]")
plt.grid()
plt.legend()
plt.show()

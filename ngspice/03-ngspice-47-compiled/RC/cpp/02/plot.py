import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("rc.dat", skiprows=1)

t = data[:, 0]
vin = data[:, 1]
vout = data[:, 2]

Ain = (vin.max() - vin.min()) / 2
Aout = (vout.max() - vout.min()) / 2

plt.plot(
    t * 1e3,
    vin,
    label=f"v(in)  A={Ain:.3f} V"
)

plt.plot(
    t * 1e3,
    vout,
    label=f"v(out) A={Aout:.3f} V"
)

plt.xlabel("Tempo [ms]")
plt.ylabel("Tensão [V]")
plt.grid()
plt.legend()
plt.show()

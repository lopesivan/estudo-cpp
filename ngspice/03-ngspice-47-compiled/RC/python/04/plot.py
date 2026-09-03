import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("rc.dat", skiprows=1)

t = data[:, 0]
vin = data[:, 1]
vout = data[:, 2]

# ------------------------------------------------------------
# Amplitudes
# ------------------------------------------------------------

Ain = (vin.max() - vin.min()) / 2
Aout = (vout.max() - vout.min()) / 2

# ------------------------------------------------------------
# Interseções: vin(t) = vout(t)
# ------------------------------------------------------------

d = vin - vout

# Procura intervalos onde a diferença muda de sinal
indices = np.where(d[:-1] * d[1:] < 0)[0]

tcross = []
vcross = []

for i in indices:
    # Instante do cruzamento por interpolação linear
    tc = t[i] - d[i] * (t[i + 1] - t[i]) / (d[i + 1] - d[i])

    # Tensão correspondente ao cruzamento
    vc = vin[i] + (
        (vin[i + 1] - vin[i])
        * (tc - t[i])
        / (t[i + 1] - t[i])
    )

    tcross.append(tc)
    vcross.append(vc)

tcross = np.array(tcross)
vcross = np.array(vcross)

# ------------------------------------------------------------
# Informações
# ------------------------------------------------------------

print(f"Amplitude v(in)  = {Ain:.6f} V")
print(f"Amplitude v(out) = {Aout:.6f} V")
print(f"Número de interseções = {len(tcross)}")

for i, (tc, vc) in enumerate(zip(tcross, vcross), start=1):
    print(
        f"Interseção {i}: "
        f"t = {tc * 1e3:.6f} ms, "
        f"V = {vc:.6f} V"
    )

# ------------------------------------------------------------
# Gráfico
# ------------------------------------------------------------

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

plt.scatter(
    tcross * 1e3,
    vcross,
    zorder=5,
    label="Interseções"
)

plt.xlabel("Tempo [ms]")
plt.ylabel("Tensão [V]")
plt.grid()
plt.legend()

plt.show()

import numpy as np
import matplotlib.pyplot as plt

# ------------------------------------------------------------
# Dados da simulação
# ------------------------------------------------------------

data = np.loadtxt("rc.dat", skiprows=1)

t = data[:, 0]
vin = data[:, 1]
vout = data[:, 2]

# ------------------------------------------------------------
# Medidas calculadas pelo ngspice
# ------------------------------------------------------------

m = np.loadtxt(
    "measures.dat",
    skiprows=1,
    max_rows=1
)

_, tcross1, vcross1, tcross2, vcross2 = m

# ------------------------------------------------------------
# Amplitudes
# ------------------------------------------------------------

Ain = (vin.max() - vin.min()) / 2
Aout = (vout.max() - vout.min()) / 2

# ------------------------------------------------------------
# Informações
# ------------------------------------------------------------

print(f"Amplitude v(in)  = {Ain:.6f} V")
print(f"Amplitude v(out) = {Aout:.6f} V")

print()
print(
    f"Cruzamento 1: "
    f"t = {tcross1 * 1e3:.6f} ms, "
    f"V = {vcross1:.6f} V"
)

print(
    f"Cruzamento 2: "
    f"t = {tcross2 * 1e3:.6f} ms, "
    f"V = {vcross2:.6f} V"
)

# ------------------------------------------------------------
# Vetores dos cruzamentos
# ------------------------------------------------------------

tcross = np.array([
    tcross1,
    tcross2
])

vcross = np.array([
    vcross1,
    vcross2
])

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
    label="Interseções",
    zorder=5
)

# Marca os valores ao lado dos pontos
for tc, vc in zip(tcross, vcross):
    plt.annotate(
        f"({tc * 1e3:.3f} ms, {vc:.3f} V)",
        xy=(tc * 1e3, vc),
        xytext=(8, 8),
        textcoords="offset points"
    )

plt.xlabel("Tempo [ms]")
plt.ylabel("Tensão [V]")

plt.grid()
plt.legend()
plt.tight_layout()

plt.show()

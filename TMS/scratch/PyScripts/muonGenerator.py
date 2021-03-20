import numpy as np
from matplotlib import pyplot as plt
  
E_0 = 4.28 #GeV
eps = 854 #GeV

support_E = np.linspace(0.,1000.,100000)
dE = support_E[1] - support_E[0]

pdf_E = 1./(E_0 + support_E)**2/(1 + support_E/eps)
pdf_E = pdf_E/(np.sum(pdf_E)*dE)

cdf_E = np.zeros(len(support_E))

for i in range(0,len(support_E)):
  cdf_E[i] = np.sum(pdf_E[0:i])*dE

plt.figure(1)
plt.plot(support_E,pdf_E)
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Energy (GeV)')
plt.ylabel('Normalized flux')

plt.figure(2)
plt.plot(support_E,cdf_E)
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Energy (GeV)')
plt.ylabel('Cumulative probability')

support_Th = np.linspace(0.,3.1415927/2.,10000)
dTh = support_Th[1] - support_Th[0]

pdf_Th = np.cos(support_Th)**2
pdf_Th = pdf_Th/(np.sum(pdf_Th*np.sin(support_Th))*dTh)

cdf_Th = np.zeros(len(support_Th))

for i in range(0,len(cdf_Th)):
  cdf_Th[i] = np.sum(pdf_Th[0:i]*np.sin(support_Th[0:i]))*dTh

plt.figure(3)
plt.plot(support_Th*180./3.1415927,pdf_Th)
plt.yscale('log')
plt.xscale('linear')
plt.show()


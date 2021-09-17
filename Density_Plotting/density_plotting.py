import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

#1. Load csv file containing rod position data of all samples into NumPy array


df = pd.read_csv("T0.01_200Samples_4050rods_RodLength_0.001.csv", header = None)
#df = df.dropna(axis=1)
positions_arr = df.to_numpy()

#%%

#2. Calculate histograms of rod position data for all samples and return an averaged histogram

sys_length = 30
cell_length = 0.1 #length of fluid cell over which we average, i.e. bin_size

bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))]) #partition sys_length into bins of width cell_length


no_samples = positions_arr.shape[0]
no_rods = positions_arr.shape[1]

hist_container = np.array([]) #initalise empty array to store hist on each for loop iteration

for i in range(no_samples):
    hist, bin_edges = np.histogram(positions_arr[i], bins = bin_edges)
    hist_container = np.append(hist_container, hist)
    
arr_container = hist_container.reshape(no_samples,len(bin_edges)-1)  

averaged_density = arr_container.mean(axis=0)/cell_length
    
#%%

#3. Plot averaged density pdf

#Plot histogram for single array of position data
#rho_scaling = np.ones(len(bin_edges)-1)/cell_length Define 'weights' so that histogram plots density rather than bin count
#n,bins,patches = plt.hist(averaged_bincounts,bins = bin_edges, histtype = 'step', weights = rho_scaling) 

bin_midpoints = bin_edges + cell_length/2

x = bin_midpoints[:-1]
y = averaged_density

fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(x,y)

plot_title = 't = 0.01, '+ str(no_samples) + ' Samples, '+ str(no_rods) + ' Rods'

ax.set(title = plot_title, ylabel= r'$\rho (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,30], ylim = [75,200])
plt.savefig(plot_title + '.png')
plt.show()


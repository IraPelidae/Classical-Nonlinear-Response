import os, glob
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

#1. Load csv file containing rod position data of all samples into NumPy array

#TODO write for loop using glob to load all .csv simulation data into separate dataframes

#path = "./Sim_Data"
#csv_files = glob.glob(os.path.join(path, "*.csv"))

#df_collection = {} # dictionary to store all dataframes
#for f in csv_files:
    

#%%

df = pd.read_csv("Sim_Data/T1_2000Samples_2437rods_10.000000Sys_Length0.001000RodLength.csv", header = None) #load simulation data
#df = df.dropna(axis=1) check df.shape, drop any NaN columns if necessary
positions_arr = df.to_numpy()

doyon_df = pd.read_csv("Doyon_Digitised/Doyon 3b) red.csv", header = None) 
doyon_arr = doyon_df.to_numpy()

#%%

#2. Calculate histograms of rod position data for all samples and return an averaged histogram

sys_length = 10
cell_length = 0.1 #length of fluid cell over which we average, i.e. bin_size

bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))]) #partition sys_length into bins of width cell_length

no_samples = positions_arr.shape[0]
no_rods = positions_arr.shape[1]

hist_container = np.array([]) #initalise empty array to store hist on each for loop iteration

for i in range(no_samples):
    hist, bin_edges = np.histogram(positions_arr[i], bins = bin_edges)
    hist_container = np.append(hist_container, hist)
    
arr_container = hist_container.reshape(no_samples,len(bin_edges)-1)  

averaged_density_dist = arr_container.mean(axis=0)/cell_length
    
#%%

#3. Plot averaged density distribution

#Plot histogram for single array of position data
#rho_scaling = np.ones(len(bin_edges)-1)/cell_length Define 'weights' so that histogram plots density rather than bin count
#n,bins,patches = plt.hist(averaged_bincounts,bins = bin_edges, histtype = 'step', weights = rho_scaling) 

#TODO - Write for loop to produce multiple plots at once

bin_midpoints = bin_edges + cell_length/2

x1 = bin_midpoints[:-1]
y1 = averaged_density_dist

fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(x1,y1)

x2 = doyon_arr[:,0] + sys_length/2 #Doyon has x_lims of [-L/2,L/2] while I chose [0,L]
y2 = doyon_arr[:,1]

ax.plot(x2,y2, color = "red")


plot_title = 't = 1, Temp = 1, '+ str(no_samples) + ' Samples, '+ str(no_rods) + ' Rods vs Doyon 3b)'

ax.set(title = plot_title, ylabel= r'$\rho (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,10], ylim = [125,400])
ax.tick_params(labelright = True)
plt.savefig('Figs/' + plot_title + '.png')
plt.show()


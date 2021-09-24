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
# Load position simulation data
df1 = pd.read_csv("Sim_Data/P_Time3.000000_Temp0.707107_Samples200_Rods3988_Sys_Length30.000000_RodLength0.001000.csv", header = None) 
positions_arr = df1.to_numpy()

#%%

#Load velocity simulation data

df2 = pd.read_csv("Sim_Data/V_Time3.000000_Temp0.707107_Samples200_Rods3988_Sys_Length30.000000_RodLength0.001000.csv", header = None)
velocities_arr = df2.to_numpy()

#%%

#Load Doyon benchmark data

doyon_df = pd.read_csv("Doyon_Digitised/Doyon 1e) red 2.csv", header = None) 
doyon_arr = doyon_df.to_numpy()

#%%

#Calculate histograms of rod position data for all samples and return an averaged histogram

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

averaged_density_dist = arr_container.mean(axis=0)/cell_length
#%%
#Calculate current histogram
    
#%%

# Plot averaged density distribution

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


plot_title = 'UPDATE 1e) t = 3, Temp = 0.707, '+ str(no_samples) + ' Samples, '+ str(no_rods) + ' Rods'

ax.set(title = plot_title, ylabel= r'$\rho (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length], ylim = [75,200])
ax.tick_params(labelright = True)
plt.savefig('Figs/' + plot_title + '.png')
plt.show()

#%%

#4. Plot current distribution





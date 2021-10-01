import os, glob
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from plot_functions import average_density_dist, average_densityandcurrent_dist

#Create filepath lists for position and velocity arrays

path = "./Sim_Data/24_09_VelocityKicks"
P_csv_filepaths = glob.glob(os.path.join(path, "*P_*.csv")) #create list of all position csv filepaths
V_csv_filepaths = glob.glob(os.path.join(path, "*V_*.csv")) #create list of all velocity csv filepaths


#%%
# Load position simulation data for each sampling time

P_array_list = [] # list to store all position np_arrays
for i in range(len(P_csv_filepaths)):
    df = pd.read_csv(P_csv_filepaths[i], header = None)
    arr = df.to_numpy()
    P_array_list.append(arr)

no_samples = P_array_list[0].shape[0]
no_rods = P_array_list[0].shape[1]
#%%

#Load velocity simulation data for each sampling time

V_array_list = [] # list to store all velocity np_arrays

for i in range(len(V_csv_filepaths)):
    df = pd.read_csv(V_csv_filepaths[i], header = None)
    arr = df.to_numpy()
    V_array_list.append(arr)

#%%
#Load Doyon benchmark data

#doyon_df = pd.read_csv("Doyon_Digitised/Doyon 1e) red 2.csv", header = None) 
#doyon_arr = doyon_df.to_numpy()

#doyon_v_df = pd.read_csv("Doyon_Digitised/Doyon 2c).csv", header = None)
#doyon_v_arr = doyon_v_df.to_numpy()

#%%

#Set system parameters

sys_length = 30
cell_length = 0.1 #length of fluid cell over which we average, i.e. bin_size

V_times_width = 1 # N.B. response is currently measure in terms of density/cell length, so width is in units of cell_length
rod_length = 0.001
temp = 0.5
rho_0 = no_rods/(sys_length/cell_length) # assumes uniform distribution where rho_0 is constant along rod
chi_0 = no_rods/(sys_length*V_times_width)

#%%

#Call average_density_dist in for loop
density_dist_list = []

for i in range(len(P_csv_filepaths)):
    rho_dist = average_density_dist(sys_length, cell_length, P_array_list[i])
    chi_dist = rho_dist/V_times_width - chi_0
    density_dist_list.append(chi_dist) 

#%%

#Including velocity as well

density_dist_list = []
velocity_dist_list = []

for i in range(len(P_csv_filepaths)):
    rho_dist, j_dist = average_densityandcurrent_dist(sys_length, cell_length, P_array_list[i], V_array_list[i])
    rho_dist = rho_dist/V_times_width - chi_0
    j_dist = j_dist/V_times_width
    density_dist_list.append(rho_dist) 
    velocity_dist_list.append(j_dist)

#%%

# Compare numerical averaged density distribution with analytic

from plot_functions import chi_rho

#Plot histogram for single array of position data
#rho_scaling = np.ones(len(bin_edges)-1)/cell_length Define 'weights' so that histogram plots density rather than bin count
#n,bins,patches = plt.hist(averaged_bincounts,bins = bin_edges, histtype = 'step', weights = rho_scaling) 

#TODO - Write for loop to produce multiple plots at once
bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))])
bin_midpoints = bin_edges + cell_length/2

x = bin_midpoints[:-1]

fig = plt.figure()
ax = fig.add_subplot(111)

#Plot all the density graphs
for i in range(len(density_dist_list)):
    line_label = "t = " + str(i)
    ax.plot(x,density_dist_list[i], label = line_label)

plot_title = "600samples, density time series"


#Benchmarking Doyon
#x2 = doyon_arr[:,0] + sys_length/2 #Doyon has x_lims of [-L/2,L/2] while I chose [0,L]
#y2 = doyon_arr[:,1]
#ax.plot(x2,y2, color = "red")

#ax.plot(x,density_dist_list[1], label = "Numerical")

#plot_title = 'Numerical vs Analytic chi_rho, t = 1, V = 0.05, kicked_width = 0.2, ' + str(no_samples) + ' Samples, '+ str(no_rods) + ' Rods'

ax.set(title = plot_title, ylabel= r'$\chi_\rho (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length])
ax.tick_params(labelright = True)

#plt.savefig('Figs/' + plot_title + '.png')


# Plot analytic chi_rho

chi_rho = np.array([chi_rho(xi,1,rod_length,rho_0,temp) for xi in (x-sys_length/2)])

#ax2 = fig.add_subplot(111)
#ax.plot(x,chi_rho, label = "Analytic", color = "red")

#plot_title = 'Analytic chi_rho'

#ax.set(title = plot_title, ylabel= r'$\chi_\rho (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length])
ax.tick_params(labelright = True)
plt.legend()
#plt.savefig('Figs/' + plot_title + '.png')
plt.show()

plt.close()
#%%

# Plot analytic chi_rho time series

from plot_functions import chi_rho

bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))])
bin_midpoints = bin_edges + cell_length/2
x = bin_midpoints[:-1]

fig = plt.figure()
ax = fig.add_subplot(111)

times = [0.025,0.05,1,0.5,2,5]

for i in times:
    current = np.array([chi_rho(xi,i,rod_length,rho_0,temp) for xi in (x-sys_length/2)])
    line_label = "t = " + str(i)
    ax.plot(x,current, label = line_label)

plot_title = 'Analytic chi_rho'

ax.set(title = plot_title, ylabel= r'$\chi_\rho(x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length])
ax.tick_params(labelright = True)
plt.legend()
#plt.savefig('Figs/' + plot_title + '.png')
plt.show()

plt.close()
#%%

#Plot numerical chi_j time series

from plot_functions import chi_j

bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))])
bin_midpoints = bin_edges + cell_length/2
x = bin_midpoints[:-1]

fig = plt.figure()
ax = fig.add_subplot(111)

#Plot all the numerical current graphs
for i in range(len(velocity_dist_list)):
    line_label = "t = " + str(i)
    ax.plot(x,velocity_dist_list[i], label = line_label)

#ax.plot(x,velocity_dist_list[1], label = "Numerical")

plot_title = 'Current, V = 0.5, kicked_width = 1, ' + str(no_samples) + ' Samples, '+ str(no_rods) + ' Rods'

ax.set(title = plot_title, ylabel= r'$\rho(x)u(x)$',  xlabel= r'$x$')
    
ax.set(xlim = [0,sys_length])
ax.tick_params(labelright = True)
plt.legend()    
plt.show()

plt.close()



#%%

# Plot analytic chi_j time series

fig = plt.figure()
ax = fig.add_subplot(111)

# current_start = np.array([chi_j(xi,0.05,rod_length,rho_0,temp) for xi in (x-sys_length/2)])
# ax.plot(x,current_start, label = "t = 0.05")

times = [0.025,0.05,0.5,2,5]

for i in times:
    current = np.array([chi_j(xi,i,rod_length,rho_0,temp) for xi in (x-sys_length/2)])
    line_label = "t = " + str(i)
    ax.plot(x,current, label = line_label)



plot_title = 'Analytic chi_j'



ax.set(title = plot_title, ylabel= r'$\chi_j(x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length])
ax.tick_params(labelright = True)
plt.legend()
#plt.savefig('Figs/' + plot_title + '.png')
plt.show()

plt.close()

#%%

# Benchmark Doyon current distribution

x3 = bin_midpoints[:-1]
y3 = averaged_current_dist

fig2 = plt.figure()
ax2 = fig2.add_subplot(111)
ax2.plot(x3,y3)

x4 = doyon_v_arr[:,0] + sys_length/2
y4 = doyon_v_arr[:,1]

ax2.plot(x4,y4, color = "red")

plot_title2 = 'Current 2c) t = 3, Temp = 0.707, '+ str(no_samples) + ' Samples, '+ str(no_rods) + ' Rods'

ax2.set(title = plot_title2, ylabel= r'$\rho (x)u(x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax2.set(xlim = [0,sys_length], ylim = [0,40])
ax2.tick_params(labelright = True)
plt.savefig('Figs/' + plot_title2 + '.png')
plt.show()
plt.close()



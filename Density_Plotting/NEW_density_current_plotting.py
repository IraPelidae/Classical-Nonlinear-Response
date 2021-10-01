# -*- coding: utf-8 -*-
"""
Created on Wed Sep 29 22:41:49 2021

@author: Jack
"""
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import glob, os

from plot_functions import chi_rho, chi_j

#%%

path = "Sim_Data/01_10_high_interaction/"

no_samples = 10000000

D_csv_filepaths = glob.glob(os.path.join(path, "D_*" + "*" + str(no_samples) + "*.csv")) #create list of all position csv filepaths
J_csv_filepaths = glob.glob(os.path.join(path, "J_*" + "*" + str(no_samples) + "*.csv")) #create list of all velocity csv filepaths

df1 = pd.read_csv(D_csv_filepaths[0], header = None)
density_arr = df1.to_numpy()

df2 = pd.read_csv(J_csv_filepaths[0], header = None)
current_arr = df2.to_numpy()

#%%

#Set system parameters

sys_length = 30
cell_length = 0.1 #length of fluid cell over which we average, i.e. bin_size


V_times_width = 0.2 # N.B. response is currently measure in terms of density/cell length, so width is in units of cell_length
rod_length = 0.125
temp = 0.5
no_rods = 128
rho_0 = no_rods/(sys_length/cell_length) # assumes uniform distribution where rho_0 is constant along rods
arho = no_rods*rod_length/sys_length
sampling_time = 0.5
max_time = 2


#%%

#Plot density time series

bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))])
bin_midpoints = bin_edges + cell_length/2

x = bin_midpoints[:-1]

fig = plt.figure()
ax = fig.add_subplot(111)

#Plot all the density graphs
for i in range(density_arr.shape[0]):
    line_label = "t = " + str(i*sampling_time)
    chi_rho_i = (density_arr[i] - rho_0)/V_times_width
    ax.plot(x,chi_rho_i, label = line_label)

plot_title = r"$\rho a$ = 0.53," + " Samples #" + str(no_samples) + ", Density time series"

ax.set(title = plot_title, ylabel= r'$\chi_\rho (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length], ylim = [-0.1,0.1])
ax.tick_params(labelright = True)

plt.legend()
#plt.savefig('Figs/' + plot_title + '.png')
plt.show()

plt.close()
#%%

#Numerical vs analytic density

time_samples_elapsed = 2
compare_time = sampling_time * time_samples_elapsed

bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))])
bin_midpoints = bin_edges + cell_length/2

x = bin_midpoints[:-1]

fig = plt.figure()
ax = fig.add_subplot(111)

chi_rho_comp = (density_arr[time_samples_elapsed] - rho_0)/V_times_width
ax.plot(x,chi_rho_comp, label = "Numerical")

chi_rho_analytic = np.array([chi_rho(xi,compare_time,rod_length,rho_0,temp) for xi in (x-sys_length/2)])

ax.plot(x,chi_rho_analytic, label = "Analytic", color = "red")

plot_title = "t = " + str(compare_time) +", " + str(no_samples) + " samples, Numerical vs Analytic chi_rho"

ax.set(title = plot_title, ylabel= r'$\chi_\rho (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length], ylim = [-0.2,0.2])
ax.tick_params(labelright = True)


plt.legend()
plt.show()
plt.close()


#%%

bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))])
bin_midpoints = bin_edges + cell_length/2

x = bin_midpoints[:-1]

fig = plt.figure()
ax = fig.add_subplot(111)

#Plot all the current graphs
for i in range(current_arr.shape[0]):
    line_label = "t = " + str(i)
    chi_j_i = current_arr[i]/V_times_width
    ax.plot(x,chi_j_i, label = line_label)

plot_title = "Cell Length " + str(cell_length) + ", Sample #" + str(no_samples) + ", Current time series"

ax.set(title = plot_title, ylabel= r'$\chi_\rho (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length])
ax.tick_params(labelright = True)

plt.legend()
#plt.savefig('Figs/' + plot_title + '.png')
plt.show()

plt.close()

#%%

#Numerical vs analytic current

time_samples_elapsed = 2
compare_time = sampling_time * time_samples_elapsed

bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))])
bin_midpoints = bin_edges + cell_length/2

x = bin_midpoints[:-1]

fig = plt.figure()
ax = fig.add_subplot(111)

chi_j_comp = current_arr[time_samples_elapsed]/(V_times_width)
ax.plot(x,chi_j_comp, label = "Numerical")

chi_j_analytic = np.array([chi_j(xi,compare_time,rod_length,rho_0,temp) for xi in (x-sys_length/2)])

ax.plot(x,chi_j_analytic, label = "Analytic", color = "red")

plot_title = "t = " + str(compare_time) +", " + str(no_samples) + " samples, Numerical vs Analytic chi_j"

ax.set(title = plot_title, ylabel= r'$\chi_j (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length])
ax.tick_params(labelright = True)


plt.legend()
plt.show()
plt.close()


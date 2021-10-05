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

path = "Sim_Data/04_10_errors_saga"

no_samples = 1000000

D_csv_filepaths = glob.glob(os.path.join(path, "D_Val*" + "*" + str(no_samples) + "*.csv")) #create list of all position csv filepaths
J_csv_filepaths = glob.glob(os.path.join(path, "J_Val*" + "*" + str(no_samples) + "*.csv")) #create list of all velocity csv filepaths

D_err_csv_filepaths = glob.glob(os.path.join(path, "D_Err*" + "*" + str(no_samples) + "*.csv"))
J_err_csv_filepaths = glob.glob(os.path.join(path, "J_Err*" + "*" + str(no_samples) + "*.csv"))

df1 = pd.read_csv(D_csv_filepaths[0], header = None)
density_arr = df1.to_numpy()

df2 = pd.read_csv(J_csv_filepaths[0], header = None)
current_arr = df2.to_numpy()

df3 = pd.read_csv(D_err_csv_filepaths[0], header = None)
density_err_arr = df3.to_numpy()
#density_err_arr = density_err_arr/np.sqrt(no_samples)

df4 = pd.read_csv(J_err_csv_filepaths[0], header = None)
current_err_arr = df4.to_numpy()
#current_err_arr = current_err_arr/np.sqrt(no_samples)
#%%

#Set system parameters

sys_length = 30
cell_length = 0.2 #length of fluid cell over which we average, i.e. bin_size


V_times_width = 0.02 # N.B. response is currently measure in terms of density/cell length, so width is in units of cell_length
rod_length = 0.001
temp = 0.5
no_rods = 4000
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
    chi_err_i = (density_err_arr[i])/V_times_width
    ax.plot(x,chi_rho_i, label = line_label)
    ax.fill_between(x, chi_rho_i - chi_err_i, chi_rho_i + chi_err_i)

plot_title = " Samples #" + str(no_samples) + ", Density time series"

ax.set(title = plot_title, ylabel= r'$\chi_\rho (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length])
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
chi_rho_err = (density_err_arr[time_samples_elapsed])/V_times_width
ax.plot(x,chi_rho_comp, label = "Numerical")

ax.fill_between(x,chi_rho_comp - chi_rho_err, chi_rho_comp + chi_rho_err, alpha = 0.5)

chi_rho_analytic = np.array([chi_rho(xi,compare_time,rod_length,rho_0,temp) for xi in (x-sys_length/2)])

ax.plot(x,chi_rho_analytic, label = "Analytic", color = "red")



plot_title = "t = " + str(compare_time) +", " + str(no_samples) + " samples, Numerical vs Analytic chi_rho"

ax.set(title = plot_title, ylabel= r'$\chi_\rho (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length])
ax.tick_params(labelright = True)


plt.legend()
plt.show()
plt.close()


#%%
#Plot all the current graphs

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

time_samples_elapsed = 1
compare_time = sampling_time * time_samples_elapsed

bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))])
bin_midpoints = bin_edges + cell_length/2

x = bin_midpoints[:-1]

fig = plt.figure()
ax = fig.add_subplot(111)

chi_j_comp = current_arr[time_samples_elapsed]/(V_times_width)
chi_j_err = (current_err_arr[time_samples_elapsed])/V_times_width

ax.plot(x,chi_j_comp, label = "Numerical")
ax.fill_between(x,chi_j_comp - chi_j_err, chi_j_comp + chi_j_err)

chi_j_analytic = np.array([chi_j(xi,compare_time,rod_length,rho_0,temp) for xi in (x-sys_length/2)])

#ax.plot(x,chi_j_analytic, label = "Analytic", color = "red")

plot_title = "t = " + str(compare_time) +", " + str(no_samples) + " samples, Numerical vs Analytic chi_j"

ax.set(title = plot_title, ylabel= r'$\chi_j (x)$',  xlabel= r'$x$') #rho(x) = bin_count/bin size (cell_length)
ax.set(xlim = [0,sys_length])
ax.tick_params(labelright = True)


plt.legend()
plt.show()
plt.close()


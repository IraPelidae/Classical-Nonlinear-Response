# -*- coding: utf-8 -*-
"""
Created on Fri Sep 24 14:33:34 2021

@author: Jack
"""
import numpy as np

def cell_velocity_dist(vel_sample, hist):
    """  

    Parameters
    ----------
    vel_sample : NUMPY ARRAY
        array containing velocities of all particles, shape 1 x no_rods
    hist : NumPy array
        array containing the rod bincounts, shape 1 x no_bins (bin_edges.size -1)

    Returns
    -------
    vel_hist : NUMPY ARRAY
        array containing the cell averaged velocity, shape 1 x no_bins
    """
    vel_hist = np.array([])
    cumulative_bincount = 0    
    
    #calculate all the mean cell velocities bar the last cell
    for i in range(hist.size): 
        n = hist[i]
        if n == 0:
            vel_hist = np.append(vel_hist,0)
            
        else:
            mean_cell_velocity = np.mean(vel_sample[cumulative_bincount: (cumulative_bincount + n)])        
            vel_hist = np.append(vel_hist, mean_cell_velocity)
            cumulative_bincount += n  
    
    #calculate the mean cell velocity for the last cell
    #vel_hist = np.append(vel_hist, np.mean(vel_sample[cumulative_bincount:]))
        
    return vel_hist

def average_density_dist(sys_length, cell_length, positions_arr):
    """
    
    Parameters
    ----------
    sys_length : FLOAT
        Length of system
    cell_length : FLOAT
        histogram bin size
    positions_arr: NUMPY ARRAY
        array containing all rod sample positions, size (no.samples x no.rods). 
        each sample's (row's) position values are sorted from smallest to largest.

    Returns
    -------
    averaged_density_dist : NUMPY ARRAY
        array containing the sample averaged density, shape 1 x no_bins
    """
    
    bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))]) #partition sys_length into bins of width cell_length
    no_samples = positions_arr.shape[0]
    
    #initalise empty arrays to store hist from each for loop run
    position_hist_container = np.array([]) 
    
    for i in range(no_samples):
        hist, bin_edges = np.histogram(positions_arr[i], bins = bin_edges) #compute rod histogram
        position_hist_container = np.append(position_hist_container, hist) #append hist to position_hist_container 
    
    arr_container = position_hist_container.reshape(no_samples,len(bin_edges)-1) 
    averaged_density_dist = arr_container.mean(axis=0)/cell_length
    
    return averaged_density_dist

def average_densityandcurrent_dist(sys_length, cell_length, positions_arr, velocities_arr):
    """
    

    Parameters
    ----------
    sys_length : FLOAT
        Length of system
    cell_length : FLOAT
        histogram bin size
    positions_arr: NUMPY ARRAY
        array containing all rod sample positions, size (no.samples x no.rods). 
        each sample's (row's) position values are sorted from smallest to largest.
    velocities_arr : NUMPY ARRAY
        array containing all rod sample velocities, size (no.samples x no.rods).

    Returns
    -------
    averaged_density_dist : NUMPY ARRAY
        array containing the sample averaged density, shape 1 x no_bins
    averaged_current_dist : NUMPY ARRAY
        array containing the sample averaged current, shape 1 x no_bins

    """
   
    bin_edges = np.array([x*cell_length for x in range(0,int(sys_length/cell_length + 1))]) #partition sys_length into bins of width cell_length

    no_samples = positions_arr.shape[0]
        
    #initalise empty arrays to store hist and mean bin velocities from each for loop run
    position_hist_container = np.array([]) 
    velocity_dist_container = np.array([])
    
    for i in range(no_samples):
        hist, bin_edges = np.histogram(positions_arr[i], bins = bin_edges) #compute rod histogram
        vel_dist = cell_velocity_dist(velocities_arr[i],hist) # compute rod velocity distribution
        
        
        velocity_dist_container = np.append(velocity_dist_container, vel_dist) #append vel_dist to velocity_dist_container
        position_hist_container = np.append(position_hist_container, hist) #append hist to position_hist_container 
        
    arr_container = position_hist_container.reshape(no_samples,len(bin_edges)-1)  
    vel_container = velocity_dist_container.reshape(no_samples, len(bin_edges)-1)
    
    averaged_density_dist = arr_container.mean(axis=0)/cell_length
    averaged_vel_dist = vel_container.mean(axis=0)
    
    averaged_current_dist = np.multiply(averaged_density_dist, averaged_vel_dist)
    
    return averaged_density_dist, averaged_current_dist


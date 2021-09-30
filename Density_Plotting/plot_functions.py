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
    current_dist_container = np.array([])
    
    for i in range(no_samples):
        hist, bin_edges = np.histogram(positions_arr[i], bins = bin_edges) #compute rod histogram
        vel_dist = cell_velocity_dist(velocities_arr[i],hist) # compute rod velocity distribution
        
        
        cur_dist = np.multiply(hist,vel_dist)
        current_dist_container = np.append(current_dist_container, cur_dist) #append vel_dist to velocity_dist_container
        position_hist_container = np.append(position_hist_container, hist) #append hist to position_hist_container 
        
    arr_container = position_hist_container.reshape(no_samples,len(bin_edges)-1)  
    cur_container = current_dist_container.reshape(no_samples, len(bin_edges)-1)
    
    averaged_density_dist = arr_container.mean(axis=0)/cell_length
    averaged_cur_dist = cur_container.mean(axis=0)   
    
    
    return averaged_density_dist, averaged_cur_dist

def chi_rho(x,t,a,rho_0,T):
    """
    See Michcle pdf eqn 27
    
    Parameters
    ----------
    x : FLOAT
        Position coordinate along system
    t : FLOAT
        Time since t = 0
    a : FLOAT
        rod length
    rho_0 : FLOAT
        Initial rod density (assumes density is initally uniformly distributed)
    T : FLOAT
        temperature

    Returns
    -------
    chi_rho_1 : FLOAT
        normalised density linear response

    """
    
    
    #Define variables
    m = 1 - a*rho_0
    v = m*x/t
    dn_by_dv = (-rho_0/m)*(v/T)*(1/np.sqrt(2*np.pi*T))*np.e**(-v**2/(2*T))
    
    chi_rho_1 = -m**3*dn_by_dv/t
    
    return chi_rho_1

def chi_j(xi,t,a,rho_0,T):
    
    """
    See Michele pdf eqn 28
    
    Parameters
    ----------
    xi : FLOAT
        Position coordinate along system
    t : FLOAT
        Time since t = 0
    a : FLOAT
        rod length
    rho_0 : FLOAT
        Initial rod density (assumes density is initally uniformly distributed)
    T : FLOAT
        temperature

    Returns
    -------
    chi_j_1 : FLOAT
        normalised current linear response

    """
    
    #Define variables
    m = 1 - a*rho_0
    v = m*xi/t
    dn_by_dv = (-rho_0/m)*(v/T)*(1/np.sqrt(2*np.pi*T))*np.e**(-v**2/(2*T))
    
    chi_j_1 = -m**2*v*dn_by_dv/t
        
    return chi_j_1



    
    
    


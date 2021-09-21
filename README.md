# Classical Nonlinear Response
Numerical simulations of classical nonlinear response in a 1d hard rod gas at University of Oxford.

## Progress update 21/09

Implemented new particle evolution idea which doesn't track collisions. Also reproduced Doyon's results in Fig 1. e) and Fig 3. b) exactly!

<p float="left">
  <img src="https://github.com/IraPelidae/Classical-Nonlinear-Response/blob/main/Density_Plotting/Figs/1e)%20t%20%3D%203%2C%20Temp%20%3D%200.707%2C%202000%20Samples%2C%203988%20Rods.png" width="400" />
  <img src="https://github.com/IraPelidae/Classical-Nonlinear-Response/blob/main/Density_Plotting/Figs/3b)%20t%20%3D%201%2C%20Temp%20%3D%200.707%2C%202000%20Samples%2C%202437%20Rods.png" width="400" /> 
</p>

## Progress update 17/09
Reproduced picture 1e) (t=3s, 200 samples) from Doyon paper! Computer restarted after 51 samples overnight at t=4s.

## Notes
1. Have been using Visual Studio 19 to complete the project, didn't edit the MakeFile
2. Using c++ -17 standard (used std::filesystem in main.cpp)

## Major research goals

## Minor to-do

1. Look into multithreading/registering for physics department computing
2. Code a progress bar during each simulation run

## Literature

Sid's journal club discussing the general idea and references to previous work
(https://www.condmatjclub.org/uploads/2020/06/JCCM_June_2020_01.pdf)

Recent papers in non-linear response from our group :  
(https://arxiv.org/pdf/2007.04323.pdf)
(https://arxiv.org/pdf/2103.06899.pdf)

Papers which outline approaches to calculation of non-linear response for classical systems.
(https://mukamel.ps.uci.edu/publications/pdfs/458.pdf)
(https://mukamel.ps.uci.edu/publications/pdfs/455.pdf)

Closely related papers with fluctuation dissipation relations, etc:
(https://mukamel.ps.uci.edu/publications/pdfs/288.pdf)
(https://mukamel.ps.uci.edu/publications/pdfs/475.pdf)

Hydrodynamics of the hard-rod gas (probably useful up to page 6):
(https://arxiv.org/abs/1703.05971)

For curiosity/bibliography:

Lecture notes on Kinetic Theory (with a chapter on linear response):
(https://www.damtp.cam.ac.uk/user/tong/kinetic.html)

Lecture notes on generalized hydrodynamics:
(https://arxiv.org/abs/1912.08496)

An introduction to integrability (although focusing mainly on quantum models):
https://people.sissa.it/~ffranchi/IntegraIntro.pdf

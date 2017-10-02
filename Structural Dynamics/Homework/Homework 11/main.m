%   *------------------------------------------------------------*
%   |  Two or Three Dimensional Static Linear Truss Analysis     |
%   |  Program. Includes prescribed forces and motions at DOF    |
%   |                                                            |                                                  
%   |  Author:  K. D. Hjelmstad                                  |
%   |  Date:    March 28, 2013                                   |
%   |                                                            |
%   |  Associated Files:                                         |
%   |     inputs.m   (data input)                                |
%   |     bound.m    (process boundary conditions)               |
%   |     loads.m    (build applied nodal load vector)           |
%   |     localize.m (compute element local quantities)          |
%   |     assemble.m (assemble stiffness matrix )                |
%   |     solve.m    (solve global equations)                    |
%   |     elem.m     (form element stiffness matrix)             |
%   |     stress.m   (compute element stresses)                  |
%   |     plotgeo.m  (plot problem geometry)                     |
%   |     plotdef.m  (plot deflected shape)                      |
%   |     outputs.m  (print results)                             |
%   *------------------------------------------------------------*

      clear; clc;
      
%.... Print Banner
      fprintf('%s\n','*---------------------------------------*')
      fprintf('%s\n','|             TRUSS ANALYSIS PROGRAM    |')
      fprintf('%s\n','|                    K. D. Hjelmstad    |')
      fprintf('%s\n','|                                       |')
      fprintf('%s\n','|       Original Version: 03.28.2013    |')
      fprintf('%s\n','|          Latest Update: 03.28.2013    |')
      fprintf('%s\n','*---------------------------------------*')
      
%.... Input problem data (nodal coordinates, element connectivity, 
%     boundary cond, material sets, forces, etc.)
      [numdim,x,ix,id,d,f,numnod,numelm,nummat] = inputs3D;
      ndof = numnod*numdim;                  % Total number of DOF
      
%.... Change id array to be equations numbers for use in assembly routine
      [id,jpos] = bound(id,numdim,numnod);   % jpos is number of free DOF
      
%.... Echo input values to screen
      fprintf('\r\r%s\n', '   MODEL PROPERTIES              ')
      fprintf('%s%5i\n',  '   Dimension of space            ',numdim)
      fprintf('%s%5i\n',  '   Number of nodes               ',numnod)
      fprintf('%s%5i\n',  '   Number of members             ',numelm)
      fprintf('%s%5i\n',  '   Number of material sets       ',nummat)
      fprintf('%s%5i\n',  '   Number of degrees of freedom  ',ndof)
      fprintf('%s%5i\n',  '   Free DOF (jpos)               ',jpos)
      
%.... Form applied load vector
      [F] = loads(f,id,numnod,numdim,jpos);  % Retrieve loads F from  f
      
%... Initialize newton iteration parameters      
      to = 0;
      t = to; its = 0; itmax = 10; tol = 10*10^-8; err = 1.0;
      out  = 0; iout  = 0;
      
%... Create 'new' displacement vector and recalculate prescribed disp.      
      xnew = zeros(jpos,1);
      up = F(jpos+1:ndof);
      
%.... Assemble stiffness matrix
      while (err > tol) && (its < itmax)
        its = its + 1;
        u = [xnew; up];    
        [A,g,r,N] = assemble(x,ix,id,d,numdim,ndof,numelm,jpos,u);
        g = g - F(1:jpos);
        xnew = xnew - A\g;
        err = norm(g);
      end
      
%.... Compute element forces from displacements 
      u = [xnew; up];  
      [stresses] = stress(x,ix,id,d,u,numdim,numelm,jpos);   
      
%.... Output results
      plotdef(x,ix,id,u,numnod,numelm,numdim,jpos);
      
%.... Report itmax error
      if its == itmax
         fprintf('\r\r%s\n','ERROR: Maximum iterations performed!')
      end
          
%.... End of program Truss Analysis
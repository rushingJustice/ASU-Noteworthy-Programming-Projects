%---------------------------------------------------------------loads
      function [F] = loads(f,id,numnod,numdim,jpos)
%   *--------------------------------------------------------------*
%   |                                                              |
%   |   LOADS: Initialize nodal load vector F from loads stored    |
%   |   in the array f (loads organized by node number)            |
%   |                                                              |
%   |   NOTE: If a DOF is constrained then the entry in f is the   |
%   |         prescribed displacement at that location.            |
%   |                                                              |
%   |   Incoming values:                                           |
%   |      f  = applied loads (organized by node number)           |
%   |      id = array of boundary conditions (1=fixed, 0=free)     |
%   |      numnod = number of nodes in structure                   |
%   |      numdim = dimension of geometric space (i.e., 2D or 3D)  |
%   |      jpos   = number of free DOF (a pointer)                 |
%   |                                                              |
%   |   Outgoing values:                                           |
%   |      F = Global load vector                                  |
%   |                                                              |
%   *--------------------------------------------------------------*

%.... Loop over all nodes and put applied loads into F
      for n=1:numnod
          for j=1:numdim
              ndof = id(n,j);
              if(ndof > 0)
                  F(ndof,1) = f(n,j);        % Applied loads
              else
                  F(jpos-ndof,1) = f(n,j);   % Prescribed displacements
              end
          end
      end          
                            
      return
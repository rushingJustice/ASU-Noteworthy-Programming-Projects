%----------------------------------------------------------------bound
      function [id,jpos] = bound(id,numdim,numnod)
%   *-----------------------------------------------------------------*
%   |                                                                 |
%   |   BOUND: Replace the boundary condition array with equation     |
%   |   numbers. Unconstrained DOF are positive, constrained are      |
%   |   negative. The id array that gets passed back has positive     |
%   |   sequential numbers for the free DOF and negative sequential   |
%   |   numbers for the fixed DOF. This information is used in        |
%   |   assembly.                                                     |
%   |                                                                 |
%   |   Incoming values:                                              |
%   |      id = array of boundary conditions (1=fixed, 0=free)        |
%   |      numdim = dimension of geometric space (i.e., 2D or 3D)     |
%   |      numnod = number of nodes in structure                      |
%   |                                                                 |
%   |   Outgoing values:                                              |
%   |      id = equation numbers (free DOF positive, fixed neg.)      |
%   |      jpos = number of free DOF (a pointer)                      |
%   |                                                                 |
%   *-----------------------------------------------------------------*

      jpos = 0;
      jneg = 0;
      
%.... Loop over all nodes in the mesh
      for i=1:numnod;
        for j=1:numdim          
%........ If the id entry is 0 set to next postive equation number
          if(id(i,j)==0);
            jpos = jpos + 1;
            id(i,j) = jpos;
%........ If the id entry is 1 set to next negative equation number
          else
            jneg = jneg - 1;
            id(i,j) = jneg;
          end               
        end  % loop on j    
      end % loop on i        

      return
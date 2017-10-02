%---------------------------------------------------------------outputs
      function [] = outputs(u,r,stresses,numdim,numnod,id,jpos)
%   *-----------------------------------------------------------------*
%   |                                                                 |
%   |   OUTPUTS: Output arrays in more convenient form                |
%   |                                                                 |
%   |   Incoming values:                                              |
%   |      u = global displacement vector                             |
%   |      r = global reaction (net force) vector                     |
%   |      stresses = element forces                                  |
%   |      numdim = dimension of geometric space (i.e., 2D or 3D)     |
%   |      numnod = number of nodes in structure                      |
%   |      id = equation numbers (free DOF positive, fixed neg.)      |
%   |      jpos = number of free DOF (a pointer)                      |
%   |                                                                 |
%   |   Outgoing values:                                              |
%   |      Nothing is return. This routine is only to print stuff.    |
%   |                                                                 |
%   *-----------------------------------------------------------------*

      displacements = zeros(numnod,numdim+1);
      reactions     = zeros(numnod,numdim+1);
      nodalforcesum = zeros(1,numdim);
      nst = 2*numdim;
           
%.... Loop over all nodes to output displacements and reactions
      for inode = 1:numnod
      ii = zeros(numdim,1);
                            
%...... Construct the assembly pointer array
        for j=1:numdim
          ii(j) = id(inode,j);
        end  % loop on j
        
%...... If ii is negative then set the pointer to appropriate place 
%       for u array
        for i=1:numdim
            if(ii(i)<0) 
                ii(i) = jpos - ii(i);
            end
        end  % loop on i

%...... Localize the displacement from the global array
        for i=1:numdim
            ul(i) = u(ii(i));
            rl(i) = r(ii(i));
        end  % loop on i
        nodalforcesum = nodalforcesum + rl;
          
%...... Print node, x-displ, y-displ, z-displ, similar for reactions
        displacements(inode,:) = [inode, ul];
        reactions(inode,:) = [inode, rl];
      end
    
%.... Write results
      disp('  ')
      disp('RESULTS')
      format short
      displacements
      reactions
      nodalforcesum
      stresses
      
      return
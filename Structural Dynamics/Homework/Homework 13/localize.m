%--------------------------------------------------------------localize
      function [xe,mat,ii] = localize(n,x,ix,id,jpos,numdim,nst)
%   *-----------------------------------------------------------------*
%   |                                                                 |
%   |   LOCALIZE: Extract local element quanties from global          |
%   |   arrays (coordinates and assembly pointers)                    |
%   |                                                                 |
%   |   Incoming values:                                              |
%   |      n = current element number                                 |
%   |      x = array nodal coordinates for ends of the element        |
%   |      ix = array of element connectivities                       |
%   |      id = array of equation numbers                             |
%   |      jpos   = number of unrestrained degrees of freedom         |
%   |      numdim = dimension of geometric space (i.e., 2D or 3D)     |
%   |      nst = dimension of element stiffness matrix                |
%   |                                                                 |
%   |   Outgoing values:                                              |
%   |      xe   = nodal coordinates associated with element           |
%   |      mat  = element material set                                |
%   |      ii   = pointer indices for assembly of global matrices     |
%   |                                                                 |
%   *-----------------------------------------------------------------*

%...... Find the i-node, j-node, and material set for element n          
        inode = ix(n,1);
        jnode = ix(n,2);
        mat   = ix(n,3);

%...... Find coordinates of nodes connected to element n
        xe(1,:) = x(inode,:);
        xe(2,:) = x(jnode,:);
                                  
%...... Construct the assembly pointer array
        ii = zeros(nst,1);
        for j=1:numdim
          ii(j)        = id(inode,j);
          ii(j+numdim) = id(jnode,j);
        end  % loop on j
        
%...... If ii is negative then set the pointer to appropriate place for u array
        for i=1:nst
            if(ii(i)<0) 
                ii(i) = jpos - ii(i);
            end
        end  % loop on i      
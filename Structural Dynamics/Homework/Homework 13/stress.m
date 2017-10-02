%-----------------------------------------------------------------stress
     function [stresses] = stress(x,ix,id,d,u,numdim,numelm,jpos)
%   *-----------------------------------------------------------------*
%   |                                                                 |
%   |   STRESSES: Recover element internal stresses from              |
%   |   displacements.                                                |
%   |                                                                 |
%   |   Incoming values:                                              |
%   |      x = array nodal coordinates for ends of the element        |
%   |      ix = array of element connectivities                       |
%   |      id = array of equation numbers                             |
%   |      d = array of material properties, indexed to element       |
%   |      u = global displacement vector                             |
%   |      numdim = dimension of geometric space (i.e., 2D or 3D)     |
%   |      numelm = number of elements in the structure               |
%   |      jpos   = number of unrestrained degrees of freedom         |
%   |                                                                 |
%   |   Outgoing values:                                              |
%   |      stresses  = internal forces in truss elements              |
%   |                                                                 |
%   *-----------------------------------------------------------------*

      nst = 2*numdim;
      stresses = zeros(numelm,2);
      
%.... Loop over all elements recover element stresses
      for n = 1:numelm

%...... Retrieve element local quantities for element n
        [xe,mat,ii] = localize(n,x,ix,id,jpos,numdim,nst);
       
%...... Localize the displacement from the global array
        for i=1:numdim
            ue(1,i) = u(ii(i));
            ue(2,i) = u(ii(i+numdim));
        end  % loop on i
        
%...... Compute element length (h) and direction cosines (nvec)
        nvec = xe(2,:)-xe(1,:);  % vector pointing from i-end t0 j-end
        h = norm(nvec);          % length of element
        nvec = nvec/h;           % unit vector in direction of element
          
%...... Compute element stresses (elmstress) for element n
        ea = d(mat,1);
        e = (ue(2,:)-ue(1,:))/h;
        elmstrain = nvec*e';
        elmstress = ea*elmstrain;
        stresses(n,:) = [n,elmstress];

      end  %loop on n
      
      return                          

%--------------------------------------------------------------assemble
      function [A,g,r] = assemble(x,ix,id,d,numdim,ndof,numelm,jpos,u)
%   *-----------------------------------------------------------------*
%   |                                                                 |
%   |   ASSEMBLE: Assemble global stiffness matrix A from element     |
%   |   contributions computed in subroutine ELEM.                    |
%   |                                                                 |
%   |   Incoming values:                                              |
%   |      x  = array nodal coordinates for ends of the element       |
%   |      ix = array of element connectivities                       |
%   |      id = array of equation numbers                             |
%   |      d  = array of material properties, indexed to element      |
%   |      numdim = dimension of geometric space (i.e., 2D or 3D)     |
%   |      ndof   = total number of degrees of freedom                |
%   |      numelm = number of elements in the structure               |
%   |      jpos   = number of unrestrained degrees of freedom         |
%   |                                                                 |
%   |   Outgoing values:                                              |
%   |      A = global stiffness matrix                                |
%   |                                                                 |
%   |   Internal alues used in this routine:                          |
%   |      xe  = nodal coordinates associated with element            |
%   |     mat  = element material set                                 |
%   |      ii  = pointer indices for assembly of global matrices      |
%   |      s   = element tangent stiffness matrix (from elem)         |
%   |                                                                 |
%   *-----------------------------------------------------------------*

      nst = 2*numdim;  % Dimension of element stiffness matrix
      
%.... Zero global stiffness matrix A and residual matrix g
      A = zeros(ndof,ndof);
      g = zeros(ndof,1);
      
%.... Loop over all elements in the structure to assemble A
      for n = 1:numelm

%...... Retrieve element local quantities for element n
        [xe,mat,ii] = localize(n,x,ix,id,jpos,numdim,nst);
        
%...... Calculate vector pointing from start node to end node
        ue = zeros(numdim,1);
        for i = 1:numdim
            ue(i) = u(ii(i+numdim)) - u(ii(i));
        end     
        
%...... Retrieve element stiffness matrix for element n
        [s,ge] = elem(d,xe,mat,ue,numdim);
        
%...... Assemble element stiffness 
        for i=1:nst
             for j=1:nst
                 A(ii(i),ii(j)) = A(ii(i),ii(j)) + s(i,j);
             end  % loop on j
        end  % loop on i
        
%...... Assemble residual matrix 
        for i = 1:numdim
            g(ii(i)) = g(ii(i)) - ge(i);
            g(ii(i+numdim)) = g(ii(i+numdim)) + ge(i);
        end
        
      end    % loop on n   
      
      A = A(1:jpos,1:jpos);
      r = g(jpos+1:ndof);
      g = g(1:jpos);
      
      
      return
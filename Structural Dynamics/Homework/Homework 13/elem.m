  %-------------------------------------------------------------------elem
      function [s,ge] = elem(d,x,mat,ue,numdim)
%   *-----------------------------------------------------------------*
%   |                                                                 |
%   |   ELEM: Compute the element stiffness matrix s for a            |
%   |   two or three dimensional linear truss element.                |
%   |                                                                 |
%   |   Incoming values:                                              |
%   |      d = array of material properties, indexed to element       |
%   |      x = array nodal coordinates for ends of the element        |
%   |      mat = material set number for current element              |
%   |                                                                 |
%   |   Outgoing values:                                              |
%   |      s = element stiffness matrix (in global coordinates)       |
%   |                                                                 |
%   *-----------------------------------------------------------------*
%.... Preallocate element stiffness matrix and element residual for speed
      s = zeros(2*numdim,2*numdim);
      ge = zeros(numdim,1);

%.... Retrieve the elastic stiffness from the "d" array
      ea = d(mat,1);
      yield = d(mat,2);
      
%.... Compute element length (h) and direction cosines (nvec)
      nvec = x(2,:)-x(1,:);    % vector pointing from i-end to j-end
      h = norm(nvec);          % length of element
      nvec = nvec/h;           % unit vector in direction of element

%.... Compute special E
      SE = nvec' + (1/h)*ue;
      
%.... Compute Lagrangian strain
      Ee = dot(ue/h,(nvec' + ue/(2*h)));
      
%.... Compute element force
      Ne = ea*Ee;
      
%.... Create element stiffness in global coordinates
      kg = (SE*ea*SE'/h + Ne*eye(numdim)/h);
      
%.... Check if element has yielded
      if abs(Ne) >= yield
          Ne = yield*sign(Ne);
          kg = zeros(numdim);
      end

%.... Find residual for each element
      ge = SE*Ne;
      
%.... Create full element stiffness, ready for assembly into global K
      s = [kg, -kg;
          -kg,  kg ];

      return